/********************************************************************************
 * 
 * Attention Sensor
 * 
 *******************************************************************************/

#include <Wire.h>

#include "attention.h"


#define ACTIVATE_PIN    D3


USPS *usps;


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

bool toggle = LOW;
int cnt = 0;

void loop() {
    if (usps->printFaces() > 0) {
        Serial.println("--------");
    }
    digitalWrite(ACTIVATE_PIN, toggle);
    if ((cnt % 100) == 0) {
        toggle = (toggle == HIGH) ? LOW : HIGH;
    }
    cnt++;
};
