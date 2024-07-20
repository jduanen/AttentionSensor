/********************************************************************************
 * 
 * Attention Sensor Display Test
 * 
 *******************************************************************************/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
#include <Wire.h>

#include <USPS.h>


#define SENSOR_VERSION  10

#define ACTIVATE_PIN    D3

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128 // Change this to 96 for 1.27" OLED.

#define SCLK_PIN 2
#define MOSI_PIN 3
#define DC_PIN   4
#define CS_PIN   1
#define RST_PIN  28

#define	BLACK    0x0000
#define	BLUE     0x001F
#define	RED      0xF800
#define	GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0  
#define WHITE    0xFFFF


#endif  // DISPLAY_H
