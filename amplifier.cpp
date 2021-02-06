#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_TPA2016.h>

#include "defs.h"
#include "amplifier.h"
#include "display.h"
#include "globals.h"

/*
 * If Alpha4 is defined, then we will act as an I2C master
 * and support output to the Adafruit Alphanumeric (14-segment)
 * backpack.
 */

#ifdef AMPLIFIER

Adafruit_TPA2016 audioamp;

uint8_t AmplifierVolume;

void AmpInit()
{
    audioamp = Adafruit_TPA2016();
    audioamp.begin();

    AmplifierVolume = AMP_GAIN_ZERO;
}

void AmpSetup()
{
    // Note: These do _NOTHING_ if the shutdown pin is
    // low while the commands are sent.
    
    audioamp.setAGCCompression(TPA2016_AGC_OFF);
    audioamp.setReleaseControl(0);
    audioamp.setAttackControl(0);
    audioamp.setHoldControl(0);
    audioamp.setLimitLevelOff();
    audioamp.setNoiseGateThreshold(0);
    audioamp.enableNoiseGate(false);    

    AmpSetVolume(AmplifierVolume, false);
}

void AmpSetVolume(uint8_t volume, bool updateDisplay)
{
    char tmp[16];
    int iVolume = volume;

    // range for the amplifier is -28 to +30
    iVolume -= 28;
    if (iVolume < -28) {
        iVolume = -28;
    }
    if (iVolume > 30) {
        iVolume = 30;
    }

    audioamp.setGain(iVolume);

    if (updateDisplay) {
        sprintf(tmp, "Gain %d", iVolume);
        DisplayWriteString(tmp);
    }
}

#else

void AmpInit()
{
}

void AmpSetup()
{
}

void AmpSetVolume(uint8_t)
{
}

#endif