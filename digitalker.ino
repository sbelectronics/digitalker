#include <Arduino.h>
#include <Wire.h>
#include "defs.h"
#include "display.h"
#include "amplifier.h"
#include "slave.h"
#include "speech.h"
#include "serial.h"
#include "vocab.h"
#include "globals.h"

unsigned long tLoopTop;

void setup()
{
    SpeechInit();
    SerialInit();

#ifdef SLAVE
    SlaveInit();
#endif

    DisplayInit();
    AmpInit();

    SpeechAmpEnable(true);
    SpeechTest();
}

void loop()
{
    tLoopTop = micros();

    SpeechUpdate();
    SerialUpdate();
}