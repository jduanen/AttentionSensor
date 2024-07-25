/********************************************************************************
 * 
 * Attention Sensor
 * 
 *******************************************************************************/

#pragma once


#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>

#include <OnBoardLED.h>
#include <USPS.h>


#define ACTIVATE_PIN    D3

// minimum confidence level required to qualify as a match [0-100]
#define MIN_CONFIDENCE  85

// number of consecutive detections required to trigger
#define DETECT_COUNT    1

// minimum active time (msec)
#define MIN_ACTIVE_MS   10000

// amount to delay between sensor samples
#define LOOP_DELAY      100  //// TODO tune this
