/********************************************************************************
 * 
 * Attention Sensor
 * 
 * Xiao RP2040 has a power LED (Red), a user-programmable RGB LED, and a NEOPixel.
 *   LED_BUILTIN = 17
 *   PIN_LED_R = 17
 *   PIN_LED_G = 16
 *   PIN_LED_B = 25
 *
 *******************************************************************************/

#ifndef ATTENTION_H
#define ATTENTION_H

#include <stdint.h>
#include <Arduino.h>
#include <USPS.h>


#define ACTIVATE_PIN    D2

// minimum confidence level required to qualify as a match [0-100]
#define MIN_CONFIDENCE  85

// number of consecutive detections required to trigger
#define DETECT_COUNT    2

// minimum active time (msec)
#define MIN_ACTIVE_MS   10000

// amount to delay between sensor samples
#define LOOP_DELAY      100  //// TODO tune this

#endif  // ATTENTION_H
