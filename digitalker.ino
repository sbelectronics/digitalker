#include <Arduino.h>
#include <Wire.h>
#include "slave.h"
#include "speech.h"
#include "serial.h"
#include "globals.h"

unsigned long tLoopTop;

void setup()
{
    SpeechInit();
    SlaveInit();
    SerialInit();

    SpeechAmpEnable(true);
    SpeechTest();
}

void loop()
{
    tLoopTop = micros();

    SpeechUpdate();
    SerialUpdate();
}