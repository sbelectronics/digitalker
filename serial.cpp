#include <Arduino.h>
#include <Wire.h>
#include <util/crc16.h>
#include "speech.h"
#include "serial.h"
#include "amplifier.h"
#include "slave.h"

#define NO_BANK 0xFF

bool LastSerialInsertResult;
uint8_t LastSerialBank;
uint8_t LastSerialWord;
uint8_t LastSerialByte;

void SerialUpdate()
{
  if (Serial.available() > 0) {
    uint8_t data = Serial.read();
    uint8_t lastLastSerialByte;

    lastLastSerialByte = LastSerialByte;
    LastSerialByte = data;

    if (lastLastSerialByte == 219) {
      AmpSetVolume(data, true);
      return;
    }
    switch (lastLastSerialByte) {
      case 250:
        // set bank
        LastSerialBank = data;
        return;
      case 251:
        // set volume
        AmpSetVolume(data, true);
        return;
      case 252:
        // read register
        Serial.write(SlaveReadRegister(data));
        return;
      case 253:
        // unused
        return;
      case 254:
        // NOP
        return;
    }

    switch (data) {
        case 250: // set bank escape sequence
        case 251: // set volume escape sequence
        case 252: // read register escape sequence
        case 253: // unused escape sequence
        case 254: // NOP
            break;
        
        default:
          uint16_t word;
          LastSerialWord = data;
          word = (LastSerialBank << 8) | data;
          LastSerialInsertResult = SpeechBufInsert(word);
          break;
    }
  }
}

void SerialInit() {
    LastSerialByte = 0xFF;
    LastSerialBank = NO_BANK;
    LastSerialWord = 0;
    LastSerialInsertResult = false;
    Serial.begin(9600);
}