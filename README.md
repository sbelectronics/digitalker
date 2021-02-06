# Scott's Digitalker Board

This project is a board to allow easy use of the Digitalker vintage
speech synthesis chipset. The board allows the digitalker to be accessed
by I2C, RS-232 Serial, TTL Serial, or Wifi (via esp8266).

## Configurations

Editing defs.h is essential to configuring the features in the Arduino
code. Do this before you compile and build your ROM.

SLAVE ... if this is defined, then the board will be able to function
as an I2C slave connected to a host microcontroller. Even if this is
defined, then serial mode is still supported. Defining SLAVE eliminates
the ability of the board to function as an I2C master -- effectively
preventing the AMPLIFIER and ALPHA4 defines.

ALPHA4 ... if this is defined (and if SLAVE is not defined), then the
board will function as an I2C master and control Adafruit I2C 14-segment
display backpacks to display the word spoken. It's a purely aesthetic
feature. If you define this but don't have the LED backpacks connected, then
the board may hang while trying to talk to the displays.

AMPLIFIER ... if this is defined (and if SLAVE is not defined), then the
board will function as an I2C master and control an attached TPA2016
amplifier board to have software control of volume. Volume may then
be controlled via serial. If you define this but don't have the amplifier
connected, then the board may hang while trying to talk to the
amplifier.

