/********************************************************************************
 * 
 * Useful Sensors Person Sensor (USPS) SEN21231 Library
 *
 * N.B. This was tested on an RP2040
 *
 * Derived from developer guide at https://usfl.ink/ps_dev
 * 
 *******************************************************************************/

#ifndef USPS_H
#define USPS_H

#include <stdint.h>


// N.B. must define SENSOR_VERSION  1.0 // 1.1 // 2.0

#define V10_MIN_DELAY  200  // 5 FPS
#define V11_MIN_DELAY  200  // 5 FPS
#define V20_MIN_DELAY  200  //// FIXME tune this to find actual value

#define USPS_I2C_ADDRESS    0x62

// Configuration commands: <cmdByte> <valueByte>
#define USPS_MODE           0x01  // 0x01 (continuous)
#define USPS_ENABLE_ID      0x02  // 0x00 (False)
#define USPS_SINGLE_SHOT    0x03  // 0x00
#define USPS_CALIBRATE_ID   0x04  // 0x00
#define USPS_PERSIST_IDS    0x05  // 0x01 (True)
#define USPS_ERASE_IDS      0x06  // 0x00 (False)
#define USPS_DEBUG_MODE     0x07  // 0x01 (True)

// Modes
#define USPS_MODE_STBY  0x00  // lowest power mode, sensor not capturing
#define USPS_MODE_CONT  0x01  // capture continuously, set INT pin if face detected

// settings
#define DEF_CONFIDENCE  85  //// FIXME tune this

// useful constants
#define USPS_MAX_FACES  7

// ref:
// https://github.com/esphome/esphome/tree/dev/esphome/components/sen21231/sen21231.h

using USPSresultsHdr_t = struct {
    uint8_t reserved[2];
    uint16_t dataSize;  // length of packet (header and checksum)
                        // V1.0: dataSize should be 40 (0x28)
};

using USPSface_t = struct __attribute__((__packed__)) {
    uint8_t boxConfidence;  // [0-255] confidence in face detection
    uint8_t boxLeft;        // [0-255] X coordinate of bounding box left side
    uint8_t boxTop;         // [0-255] Y coordinate of bounding box top side
    uint8_t boxRight;       // [1-255] width of bounding box
    uint8_t boxBottom;      // [1-255] height of bounding box
    int8_t idConfidence;    // [0-255] confidence in face recognition
    int8_t id;              // [0-7] number assigned to this (recognized) face
    uint8_t isFacing;       // [0,1]
};

using USPSresults_t = struct __attribute__((__packed__)) {
  USPSresultsHdr_t header;           // Bytes 0-4
  int8_t numFaces;                   // Byte 5
  USPSface_t faces[USPS_MAX_FACES];  // Bytes 6-37
  uint16_t checksum;                 // Bytes 38-39 CRC16 of Bytes 0-37
};


class USPS {
public:
    USPS(float sampleRate = 5.0, uint8_t thresh = DEF_CONFIDENCE,
         persistFaces = False, eraseFaces = False, ledEnable = True);

    bool setMode(uint8_t mode = USPS_MODE_CONT);
    uint8_t getMode();

    bool enableFaceRec(bool enable = False);
    bool isFaceRecEnabled();

    bool registerFace(uint8_t faceId = 0);
    uint8_t getRegisteredFaceBitmap();

    bool persistRegisteredFaces(bool enable);
    bool isRegisteredFacesPersistant();

    bool eraseRegisteredFaces();

    void enableLED(bool enable);
    bool isLEDEnabled();

    bool setConfidenceThreshold(uint8_t thresh = DEF_CONFIDENCE);
    uint8_t getConfidenceThreshold();

    //// TODO consider adding set/getMaxNumFaces() methods

    bool singleShot(USPSface_t faces[], uint8_t maxFaces);
    int8_t getFaces(USPSface_t faces[], uint8_t maxFaces);
protected:
    bool _persist;
    uint8_t _mode;
    uint8_t _facesBitmap;
    bool _faceRecEnable;
    uint8_t _confidence;
    bool _ledEnable;
    float _sampleRate;

    bool _read(USPSresults_t* results) {
    bool _write(uint8_t addr, uint8_t value);
}

#endif  // USPS_H
