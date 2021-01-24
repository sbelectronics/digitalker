#include <Arduino.h>
#include <Wire.h>
#include <util/crc16.h>
#include "speech.h"
#include "serial.h"

#define NO_BANK 0xFF

bool LastSerialInsertResult;
uint8_t LastSerialBank;
uint8_t LastSerialWord;

void SerialUpdate()
{
  if (Serial.available() > 0) {
    uint8_t data = Serial.read();

    if (data >= 240) {
      LastSerialBank = data - 240;    
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
    LastSerialBank = NO_BANK;
    LastSerialWord = 0;
    LastSerialInsertResult = false;
    Serial.begin(9600);
}