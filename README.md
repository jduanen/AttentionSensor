# Attention Sensor
This is a sensor that detects human attention based on Useful Sensors' SEN21231
person sensor.

This is a standalone device that generates a logic level output signal that is
active when a face is detected.

In both cases, the SEN21231 can be polled via its I2C interface, or its
interrupt signal can be used to determine if a face is detected.

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

![Attention Sensor](AttentionSensor.png)


# Links
* rp2040-zero
  - https://www.waveshare.com/rp2040-zero.htm
* Xiao RP2040
  - https://wiki.seeedstudio.com/XIAO-RP2040/
* Nixie QTC Plus clock (includes schematics)
  - http://www.pvelectronics.co.uk/kits/QTC4/QtcFourV1.pdf
* Useful Sensors SEN21231
  - https://github.com/usefulsensors/person_sensor_docs
