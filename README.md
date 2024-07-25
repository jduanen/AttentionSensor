# AttentionSensor
Attention Sensor based on Useful Sensors' SEN21231 person sensor.

There are two types of devices using this sensor.  The first is a standalone
device that generates a logic level output signal that is active when a face
is detected. The second is a standalone device that allows (up to 7) faces to
be registered and, whenever a face is detected, an indication of which of the
registered faces is detected (or an indication of an unknown face).

In both cases, the SEN21231 can be polled via its I2C interface, or its
interrupt signal can be used to determine if a face is detected.

Both of these types of devices can be used in conjunction with a WiFi-enabled
controller that can be integrated with Home Assistant. [see examples in SensorNet]

# Attention Sensor

An example use case of this type of device is to create a module that can be
used to enable a display only when a person is facing the display. This can be
useful for displays that have limited lifetimes or consume substantial amounts
of energy and need not be active when no one is looking at them.

## QTC+ Nixie Tube Clock with Attention Sensor
The specific example given here is used in conjunction with a QTC+ nixie-tube
clock. The QTC+ is modified to enable and disable the clock's high-voltage
power supply. The PWM output pin is lifted and brought out to the Attention
Sensor and gated (via a 74HC08 AND gate) with the output from the Attention
Sensor and returned to the clock's HV PSU.

This example uses a Xiao RP2040 controller to interface to the SEN21231 and to
generate the output signal that gates the clock's HV PSU PWM signal with a
74HC08.

# Face Detection and Recognition Sensor

A variant of the Attention Sensor device supports the binding of faces to
identifiers that are to be emitted when the registered faces are detected,
as well as indications of the detection of unregistered faces.

This uses an RP2040-based microcontroller connected to the SEN21231, a set of
switches used to select the IDs for registered faces, and an RGB LED to
indicate the state of the registration and detection/recognition functions.

This design is modularized so that it can be used in different applications.
The attention sensor controller manages face registration and emits face
detection indications over a serial port. A separate controller can be used
that takes this serial input and interfaces the face detection/recognition
subsystem to Home Assistant.

* Features
  - Integrated with Home Assistant via independent microcontroller.
    * generate events when faces are detected and provide Face ID
  - Register Face
    * select face number with number switch
    * get Green LED indication of face detected
    * press momentary button to register face
    * colored LED (NeoPixel) on Xiao RP2040 indicates status
      - off: scanning for faces
      - Red: error
      - Green: ?
      - Blue: ?
      - Cyan: ?
      - Magenta: ?
      - Yellow: ?
  - Erase Registered Faces
    * hold momentary button while turning on power
    * NeoPixel indicates status
      - ?: ?
      - ?: ?
      - ?: ?
* Add HW to controller
  - [0-7] switch
  - register (momentary) button

# Links
* rp2040-zero
  - https://www.waveshare.com/rp2040-zero.htm
* Xiao RP2040
  - https://wiki.seeedstudio.com/XIAO-RP2040/
* Nixie QTC Plus clock (includes schematics)
  - http://www.pvelectronics.co.uk/kits/QTC4/QtcFourV1.pdf
* Useful Sensors SEN21231
  - https://github.com/usefulsensors/person_sensor_docs
