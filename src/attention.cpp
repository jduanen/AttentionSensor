/********************************************************************************
 * 
 * Attention Sensor
 * 
 *******************************************************************************/

#include <Wire.h>

#include "attention.h"


USPS *usps;

bool active = false;
uint8_t detects = 0;
uint16_t activeTime;


void setup() {
    Serial.begin(115200);
    while (!Serial) { ; };
    delay(500);

    pinMode(ACTIVATE_PIN, OUTPUT);
    digitalWrite(ACTIVATE_PIN, LOW);

    Wire.begin();
    usps = new USPS();
    Serial.println("BEGIN");  //// TMP TMP TMP
};

//// FIXME make this work for multiple faces
void loop() {
    USPSface_t faces;
    int8_t numFaces;

    numFaces = usps->getFaces(&faces, 1);
    if (numFaces && (faces.boxConfidence >= MIN_CONFIDENCE)) {
        detects++;
        if (!active) {
            if (detects >= DETECT_COUNT) {
                active = true;
                activeTime = millis();
                digitalWrite(ACTIVATE_PIN, HIGH);
            }
        }
    } else {
        detects = 0;
        if (active) {
            if ((millis() - activeTime) > MIN_ACTIVE_MS) {
                active = false;
                digitalWrite(ACTIVATE_PIN, LOW);
            }
        }
    }

    delay(LOOP_DELAY);
};
