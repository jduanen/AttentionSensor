/********************************************************************************
 * 
 * Attention Sensor
 * 
 *******************************************************************************/

#include <Wire.h>

#include "attention.h"


////#define TESTING


#ifdef TESTING
#define     print(val)      Serial.print(val);
#define     println(val)    Serial.println(val);
#else /* TESTING */
#define     print(val) ;
#define     println(val) ;
#endif /* TESTING */


void (* resetFunc)(void) = 0;


bool active = false;
uint8_t detects = 0;
unsigned long activeTime;

USPS *usps;


void setup() {
#ifdef TESTING
    Serial.begin(115200);
    while (!Serial) { ; };
    delay(500);
#endif

    pinMode(ACTIVATE_PIN, OUTPUT);
    digitalWrite(ACTIVATE_PIN, LOW);

    pinMode(PIN_LED_R, OUTPUT);
    digitalWrite(PIN_LED_R, HIGH);
    pinMode(PIN_LED_G, OUTPUT);
    digitalWrite(PIN_LED_G, HIGH);
    pinMode(PIN_LED_B, OUTPUT);
    digitalWrite(PIN_LED_B, HIGH);

    Wire.begin();

    int8_t n = -1;
    while (n < 0) {
        USPSface_t faces[1];
        usps = new USPS();
        n = usps->getFaces(faces, 1);
    }

    println("BEGIN");
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
*/
    if (numFaces < 0) {
        println("ERROR: failed to read, resetting...");
        // turn User LED red to indicate failure of peripheral read
        digitalWrite(PIN_LED_R, LOW);
        digitalWrite(PIN_LED_G, HIGH);
        digitalWrite(PIN_LED_B, HIGH);
        usps = new USPS();
        return;
    }
    if (numFaces) {
        detects++;
        if (!active) {
            if (detects >= DETECT_COUNT) {
                active = true;
                activeTime = millis();
                digitalWrite(ACTIVATE_PIN, HIGH);
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
                digitalWrite(PIN_LED_R, HIGH);
                digitalWrite(PIN_LED_G, HIGH);
                digitalWrite(PIN_LED_B, HIGH);
            }
        }
    }

    delay(LOOP_DELAY);
};
