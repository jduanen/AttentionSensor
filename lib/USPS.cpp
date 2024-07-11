/********************************************************************************
 * 
 * Useful Sensors Person Sensor (USPS) SEN21231 Library
 * 
 * Specs:
 *   - I2C <= 400K baud
 *   - 3.3V, ~150mW
 *   - ~110 deg FOV
 *   - no IR filter (works with near-IR LEDs)
 *   - separate INT signal (one pulse per detection)
 *   - three versions
 *     * V1.0:
 *       - connect via Quiic connector
 *       - ~7 FPS (no face recognizer), ~5 FPS (face recognizer)
 *       - HX6537 CPU, HM0360 image sensor
 *       - ?
 *     * V1.1:
 *       - released Aug23, replaces V1.0
 *       - two mounting holes
 *       - has break-off board with .1" center holes for interface
 *         * also has pads for Quiic connector
 *       - ~7 FPS (no face recognizer), ~5 FPS (face recognizer)
 *       - ?
 *     * V2.0:
 *       - release Dec23, replaces V1.1
 *       - ESP32S3 CPU, GC032A image sensor
 *       - smaller PCB
 *       - >7 FPS (no face recognizer), >5 FPS (face recognizer)
 *       - ?
 *
 *******************************************************************************/

#include <Wire.h>

#include "USPS.h"


#if (SENSOR_VERSION == 1.0)
const int32_t SAMPLE_DELAY_MS = V10_DELAY
#elif (SENSOR_VERSION == 1.1)
const int32_t SAMPLE_DELAY_MS = V11_DELAY
#elif (SENSOR_VERSION == 2.0)
const int32_t SAMPLE_DELAY_MS = V20_DELAY
#endif


USPS::USPS(float sampleRate = 5.0, uint8_t thresh = DEF_CONFIDENCE,
           persistFaces = false, eraseFaces = false, ledEnable = true) {
    _sampleRate = sampleRate;
    _confidence = thresh;
    _persist = persistFaces;

    enableLED(ledEnable);

    if (eraseFaces) {
        eraseRegisteredFaces();
    }
}

bool USPS::setMode(uint8_t mode = USPS_MODE_CONT) {
    if ((mode != USPS_MODE_CONT) && (mode != USPS_MODE_STBY)) {
        //// TODO log error
        Serial.println("ERROR: Invalid mode");
        return true;
    }

    if (_write(USPS_MODE, mode)) {
        //// TODO log error
        Serial.println("ERROR: Failed to write mode");
        return true;
    }
    _mode = mode;
    return false;
}

uint8_t USPS::getMode() {
    return _mode;
}

bool USPS::enableFaceRec(bool enable = false) {
    if (_write(USPS_ENABLE_ID, enable)) {
        //// TODO log error
        return true;
    }
    _faceRecEnable = enable;
    return false;
}

bool USPS::isFaceRecEnabled() {
    return _faceRecEnable;
}

bool USPS::registerFace(uint8_t faceId = 0) {
    if (faceId >= USPS_MAX_FACES) {
        //// TODO log error
        Serial.println("ERROR: Invalid face ID");
        return true;
    }

    if (_write(USPS_CALIBRATE_ID, enable)) {
        //// TODO log error
        Serial.println("ERROR: Failed to enable registering of face")
        return true;
    }

    //// N.B. if two frames pass with no face detection, this label is discarded
    //// FIXME figure out how to tell if two frames passed without registering

    _faceBitmap |= (1 << faceId);
    return false;
}

uint8_t USPS::getRegisteredFaceBitmap() {
    return _facesBitmap;
}

bool USPS::persistRegisteredFaces(bool enable) {
    if (_write(USPS_PERSIST_IDS, enable)) {
        //// TODO log error
        Serial.println("ERROR: Failed to persist registered faces");
        return true;
    }

    _persist = enable;
    return false;
}

bool USPS::isRegisteredFacesPersistant() {
    return _persist;
}

bool USPS::eraseRegisteredFaces() {
    if (_write(USPS_ERASE_IDS, enable)) {
        //// TODO log error
        Serial.println("ERROR: Failed to erase registered faces");
        return true;
    }

    _facesBitmap = 0x00;
    return false;
}

bool USPS::enableLED(bool enable) {
    if (_write(USPS_DEBUG_MODE, enable)) {
        //// TODO log error
        Serial.println("ERROR: Failed to enable LED");
        return true;
    }

    _ledEnable = enable;
    return false;
}

bool USPS::isLEDEnabled() {
    return _ledEnable;
}

bool USPS::setConfidenceThreshold(uint8_t thresh = DEF_CONFIDENCE) {
    if (thresh > 100) {
        //// TODO log error
        Serial.println("ERROR: Confidence threshold must be < 100");
        return true;
    }

    _confidence = thresh;
    return false;
}

uint8_t USPS::getConfidenceThreshold() {
    return _confidence;
}

int8_t USPS::singleShot(USPSface_t faces[], uint8_t maxFaces) {
    if (_mode != USPS_MODE_STBY) {
        //// TODO log error
        Serial.println("ERROR: Device must be in Standby mode");
        return true;
    }
    if (_write(USPS_SINGLE_SHOT, enable)) {
        //// TODO log error
        Serial.println("ERROR: Failed to enable LED");
        return true;
    }
    getFaces(faces, maxFaces);
}

int8_t USPS::getFaces(USPSface_t faces[], uint8_t maxFaces) {
    uint8_t numFaces;
    USPSresults_t results;

    if (maxFaces > USPS_MAX_FACES) {
        //// TODO log warning
        Serial.println("WARNING: maxFaces larger than HW limit");
    }

    if (_read(&results)) {
        //// TODO log error
        Serial.println("ERROR: Read device failure");
        return -1;
    }
    numFaces = min(results.numFaces, maxFaces);
    for (int i = 0; (i < numFaces); i++) {
        faces[i] = results.faces[i];
    }
    return numFaces;
}

//// TODO decide if this should be inline
bool USPS::_read(USPSresults_t* results) {
    //// TODO determine if this is necessary now
    // https://www.arduino.cc/reference/en/language/functions/communication/wire/
    // "The Wire library implementation uses a 32 byte buffer, therefore any
    // communication should be within this limit. Exceeding bytes in a single
    // transmission will just be dropped."
    const int maxBytesPerChunk = 32;
    const int totalBytes = sizeof(USPSresults_t);
    int8_t* resultsBytes = (int8_t *)(results);
    int index = 0;

    while (index < totalBytes) {
        const int bytesRemaining = totalBytes - index;
        const int bytesThisChunk = min(bytesRemaining, maxBytesPerChunk);
        const int endIndex = index + bytesThisChunk;
        const bool isLastChunk = (bytesRemaining <= maxBytesPerChunk);
        Wire.requestFrom(USPS_I2C_ADDRESS, bytesThisChunk, isLastChunk);
        for (; index < endIndex; ++index) {
            if (Wire.available() < 1) {
                Serial.print("Only ");
                Serial.print(index);
                Serial.print(" bytes available on I2C, but need ");
                Serial.println(bytesThisChunk);
                return true;
            }
            resultsBytes[index] = Wire.read();
        }
    }
    return false;
}

bool USPS::_write(uint8_t addr, uint8_t value) {
    Wire.beginTransmission(USPS_I2C_ADDRESS);

    if (Wire.write(addr) != 1) {
        //// TODO log error
        Serial.println("ERROR: Failed to write register address");
        return true;
    }
    if (Wire.write(value) != 1) {
        //// TODO log error
        Serial.println("ERROR: Failed to write register value");
        return true;
    }

    Wire.endTransmission();
    return false;
}