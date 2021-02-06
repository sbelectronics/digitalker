#include <Arduino.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

#include "vocab.h"
#include "vocab/vocab.bin.inc"

bool FindWord(uint8_t bank, uint8_t index, char *dest, uint8_t maxLen) {
    uint16_t offset = (uint16_t) &vocab[0];
    uint8_t curBank = 0xFF;
    uint8_t curIndex = 0xFF;
    uint8_t len;

    while (true) {
        uint8_t b = pgm_read_byte_near(offset);
        switch (b) {
            case 0xFF:
                return false;

            case 0xFE:
                offset ++;
                curBank = pgm_read_byte_near(offset);
                offset ++;
                break;
            
            default:
                curIndex = b;
                offset ++;
                len = pgm_read_byte_near(offset);
                if ((curBank == bank) && (curIndex == index)) {
                    if (dest != NULL) {
                        if (len > maxLen) {
                            len = maxLen;
                        }
                        while (len > 0) {
                            offset ++;
                            *dest = pgm_read_byte_near(offset);
                            dest++;
                            len --;
                        }
                        *dest = '\0';
                    }
                    return true;
                } else {
                    offset ++;
                    offset += len;
                }
        }
    }
}