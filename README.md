# The Princess Ophelias Sister Alarm

Do you have a sneaky snooping little sister or brother? We know you
always want to keep them out of your room with the most high-tech
solution available.

The following is a tutorial project for building a motion sensor based
alarm that is activated/deactivated with an RFID card / tag. It should
be suitably loud and annoying to your snooping little friend.

![PSB labels](https://github.com/innovationgarage/SisterAlarm/raw/master/images/overview.jpg)

# Usage

The SisterAlarm has two states, engaged and disengaged. In its engaged
state, any input from the motion sensor will trigger the buzzer to
make loud, obnoxious and annoying noises to scare away your sibling,
and to alert you to come to the scene to save what can be saved of
your belongings before they are stolen, whacked or eaten (depending on
the age of your... problem).

In its disengaged state, it allows you to reside in your room without
triggering the alarm.

To switch between the two states, you swipe your RFID card or tag over
the reader. The display will announce the new state.

If an invalid card (e.g. a random bus card your annoyance have found)
is swiped while in the engaged state, the alarm will immediately
activate, screaming for you to come handle attempt to hack your
security system.

Finally, we would like to point out that it's obviously trivial to
change the texts displayed on the display in the different states and
for the various events. Actually, we encourage you to do so to
properly scare off your sibling, or just plainly state their
unwelcomedness!

# Ingredients
## Main ingredients
* Arduino Nano
* ProtoSwitchBoard

![Arduino](https://github.com/innovationgarage/SisterAlarm/raw/master/images/Arduino.jpg)

## Peripheral ingredients
(all from Luxorparts)
* RFID reader RC522
* LCD screen SH1106
* PIR (motion) sensor
* Active piezo buzzer module

![RFID](https://github.com/innovationgarage/SisterAlarm/raw/master/images/RFID.jpg)
![Screen](https://github.com/innovationgarage/SisterAlarm/raw/master/images/Screen.jpg)
![PIR](https://github.com/innovationgarage/SisterAlarm/raw/master/images/PIR.jpg)
![BUZZ](https://github.com/innovationgarage/SisterAlarm/raw/master/images/BUZZ.jpg)

# Instructions

Start by labeling your PSB this way:

![PSB labels](https://github.com/innovationgarage/SisterAlarm/raw/master/images/PSBLabels.jpg)

Okay, so this might sound quite techinical at first, but hang on, it's
actually pretty simple to get it working:

The PIR (motion) sensor connects to a single digital input pin, and
the piezo buzzer to a single PWM digital output pin.

The screen connects via I2C (SDA & SCL signals), and the RFID reader
via SPI (MOSI, MISO, SCK, SS) and RST.

All the modules are sold with labeled pins, so all you need to do is
actually to wire them up to the correspondingly labeled pins on the
PSB! Then do the same with the Arduino. The PSB has separate labels
for the arduino pins. Don't forget Ground and 5V (VCC).

Some notes though: The RFID reader we got wants 3.3V, not 5V. You'll
have to run an extra cable just for this, directly to the arduino:s
3V3 pin. Be carefull!

# Software

You need to program your Arduino to turn it into a SisterAlarm Main Unit! To do so, first download and install the prerequisite Arduino libraries listed below, then download the SisterAlaram.ino from above and load it in your Arduino IDE, connect the Arduino USB cable and press "play".

# Comments

It's a bit unfortunate that the screen and RFID reader connects via
different busses, but these are the most common components available.
If you can find ones that share bus you could simplify the cabling a
bit!
