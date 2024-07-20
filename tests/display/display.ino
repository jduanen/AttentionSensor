/********************************************************************************
 * 
 * Attention Sensor Display Test
 * 
 * Dynamic display bounding box
 *   - Waveshare 1.5in RGB OLED Display
 *     * 1351 controller, SPI interface
 *   - 
 * 
 *******************************************************************************/

#include "display.h"


Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);

USPS *usps;

uint16_t colors[] = { WHITE, BLUE, RED, GREEN, CYAN, MAGENTA, YELLOW };


void setup() {
    Serial.begin(115200);
    while (!Serial) { ; };
    delay(500);

    pinMode(ACTIVATE_PIN, OUTPUT);
    digitalWrite(ACTIVATE_PIN, LOW);

    tft.begin();
    tft.fillScreen(WHITE);
    delay(1000);
    tft.fillScreen(BLACK);

    Wire.begin();
    usps = new USPS();
    Serial.println("BEGIN");  //// TMP TMP TMP
};

void loop() {
    USPSface_t faces[USPS_MAX_FACES];
    int8_t numFaces;

    numFaces = usps->getFaces(faces, USPS_MAX_FACES);
    digitalWrite(ACTIVATE_PIN, LOW);
    tft.fillScreen(BLACK);
    uint8_t i = 0;
    while (numFaces > 0) {
//        Serial.print("Face #: "); Serial.println(i);
        tft.drawRect(faces[i].boxLeft / 2, faces[i].boxTop / 2,
                     faces[i].boxWidth / 2, faces[i].boxHeight / 2,
                     colors[i]);
        digitalWrite(ACTIVATE_PIN, HIGH);
        i++;
        numFaces--;
    }

    delay(67);  // V11_MIN_DELAY
};
