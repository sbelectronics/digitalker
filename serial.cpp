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

    if (data >= 240) {
      LastSerialBank = data - 240;    
    } else if (data >= 220) {
      Serial.write(SlaveReadRegister(data-220));
    } else if (data == 219) {
      // do nothing -- next byte will be volume
    } else if (LastSerialBank == NO_BANK) {
      // ESP8266 spews out nonsense when it powers up,
      // so ignore any commands until we have first received
      // a bank select.
      return;
    } else {
      uint16_t word;
      LastSerialWord = data;
      word = (LastSerialBank << 8) | data;
      LastSerialInsertResult = SpeechBufInsert(word);
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