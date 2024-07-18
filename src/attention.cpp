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
unsigned long activeTime;

void (* resetFunc)(void) = 0;


void setup() {
//    Serial.begin(115200);
//    while (!Serial) { ; };
    delay(500);

    pinMode(ACTIVATE_PIN, OUTPUT);
    digitalWrite(ACTIVATE_PIN, LOW);

    pinMode(PIN_LED_R, OUTPUT);
    digitalWrite(PIN_LED_R, HIGH);
    pinMode(PIN_LED_G, OUTPUT);
    digitalWrite(PIN_LED_G, HIGH);
    pinMode(PIN_LED_B, OUTPUT);
    digitalWrite(PIN_LED_B, HIGH);

    Wire.begin();
    usps = new USPS();
//    Serial.println("BEGIN");  //// TMP TMP TMP
};

void loop() {
    USPSface_t faces[USPS_MAX_FACES];
    int8_t numFaces;

    int8_t n = usps->getFaces(faces, USPS_MAX_FACES);
    numFaces = n;
/*
    for (int i = 0; (i < n); i++) {
//        usps->printFace(faces[i]);
        if (!faces[i].isFacing || (faces[i].boxConfidence < MIN_CONFIDENCE)) {
            numFaces--;
//            Serial.print("F: ");Serial.print(faces[i].isFacing); Serial.print(", ");
//            Serial.println(faces[i].boxConfidence);
        }
    }
//    Serial.print("N: ");Serial.print(n); Serial.print(", "); Serial.println(numFaces);
*/
    if (numFaces < 0) {
//        Serial.println("ERROR: failed to read, resetting...");
        digitalWrite(PIN_LED_R, LOW);
        digitalWrite(PIN_LED_G, HIGH);
        digitalWrite(PIN_LED_B, HIGH);
        resetFunc();
        return;
    }
    if (numFaces) {
        detects++;
        if (!active) {
            if (detects >= DETECT_COUNT) {
                active = true;
                activeTime = millis();
                digitalWrite(ACTIVATE_PIN, HIGH);
//                Serial.println("ACTIVE");
                digitalWrite(PIN_LED_R, LOW);
                digitalWrite(PIN_LED_G, LOW);
                digitalWrite(PIN_LED_B, LOW);
            }
        }
    } else {
        detects = 0;
        if (active) {
            if ((millis() - activeTime) > MIN_ACTIVE_MS) {
                active = false;
                digitalWrite(ACTIVATE_PIN, LOW);
//                Serial.println("INACTIVE");
                digitalWrite(PIN_LED_R, HIGH);
                digitalWrite(PIN_LED_G, HIGH);
                digitalWrite(PIN_LED_B, HIGH);
            }
        }
    }
    /*
    Serial.print(numFaces); Serial.print(" ");
    Serial.print(active); Serial.print(" ");
    Serial.print(detects); Serial.print(" ");
    Serial.print(activeTime); Serial.print(" ");
    Serial.println(millis());
    */

    delay(LOOP_DELAY);
};
