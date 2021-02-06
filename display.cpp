#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

#include "defs.h"
#include "vocab.h"
#include "display.h"
#include "globals.h"

/*
 * If Alpha4 is defined, then we will act as an I2C master
 * and support output to the Adafruit Alphanumeric (14-segment)
 * backpack.
 */

#ifdef ALPHA4

Adafruit_AlphaNum4 alpha4hi, alpha4lo;

void DisplayInit()
{
    alpha4hi = Adafruit_AlphaNum4();
    alpha4hi.begin(0x70);

    alpha4lo = Adafruit_AlphaNum4();
    alpha4lo.begin(0x71);
}

void DisplayUpdate(uint8_t bank, uint8_t index)
{
    char word[9];

    if (!FindWord(bank, index, word, 8)) {
        DisplayWriteString((char*) "UNKNOWN!");
        return;
    }

    DisplayWriteString(word);
}

void DisplayWriteString(char *word)
{
    uint8_t len;
    uint8_t i;
    len = strlen(word);

    alpha4hi.clear();
    alpha4lo.clear();

    for (i=0; i<4; i++) {
        if (len>i) {
            alpha4hi.writeDigitAscii(i, toupper(word[i]));
        } else {
            //alpha4hi.writeDigitAscii(i, 0);
        }
    }
    alpha4hi.writeDisplay();

    for (i=0; i<4; i++) {
        if (len>i+4) {
            alpha4lo.writeDigitAscii(i, toupper(word[i+4]));
        } else {
            //alpha4lo.writeDigitAscii(i, 0);
        }
    }
    alpha4lo.writeDisplay();
}

#else

void DisplayInit()
{
}

void DisplayUpdate(uint8_t, uint8_t)
{
}

void DisplayWriteString(char *)
{
}

#endif