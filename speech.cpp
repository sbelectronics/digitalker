#include <Arduino.h>
#include <Wire.h>
#include <util/crc16.h>
#include "speech.h"

// high when ready for new word
#define PIN_INTR (1<<PC0)

// lower 6 bits
#define WORD_LO_MASK ((1<<PD2) | (1<<PD3) | (1<<PD4) | (1<<PD5) | (1<<PD6) | (1<<PD7))
#define WORD_LO_SHIFT 2

// upper 2 bits
#define WORD_HI_MASK ((1<<PB0) | (1<<PB1))

#define PIN_CS (1<<PB2)
#define PIN_WR (1<<PB6)
#define PIN_CMS (1<<PB7)

// bank selection bits, using the SPI lines
#define PIN_BANK0 (1<<PB3)
#define PIN_BANK1 (1<<PB5)
#define PIN_BANK2 (1<<PB4)

#define PIN_BANK_MASK (PIN_BANK0 | PIN_BANK1 | PIN_BANK2)

// HIGH on SHDN enables the amplifier
#define SHDN_PIN (1<<PC1)

#define SPEECHBUF_SIZE 255

#define SPEECH_INIT             0x00
#define SPEECH_BUSY             0x01
#define SPEECH_IDLE             0x02
#define SPEECH_BUSYWAIT         0x03
#define SPEECH_ACKED            0x04

#define SPEECH_ERR_MASK         0x10  // high bit indicates halted due to error
#define SPEECH_ERR_BUSY_TIMEOUT 0x15

uint16_t SpeechBuf[SPEECHBUF_SIZE];
uint16_t *SpeechBufHead;
uint16_t *SpeechBufTail;
uint16_t *SpeechBufEnd = &SpeechBuf[SPEECHBUF_SIZE-1];  // the last valid position in the buffer
uint8_t SpeechBufCount;
uint8_t SpeechState;
uint8_t SpeechReady;

uint8_t SpeechLastWordOut;
uint8_t SpeechLastBankOut;
                 

bool SpeechBufIsFull()
{
    bool retVal;

    cli();
    retVal = (SpeechBufCount == SPEECHBUF_SIZE);

    sei();
    return retVal;
}

bool SpeechBufIsEmpty()
{
    bool retVal;

    cli();
    retVal = (SpeechBufCount == 0);

    sei();
    return retVal;
}

bool SpeechBufInsert(uint16_t data)
{
    bool retVal;
    cli();

    if (SpeechBufCount == SPEECHBUF_SIZE) {
        retVal = false;
    } else {
        *SpeechBufHead = data;

        if (SpeechBufHead == SpeechBufEnd) {
            SpeechBufHead = SpeechBuf;
        } else {
            SpeechBufHead++;
        }

        SpeechBufCount++;
        retVal = true;
    }

    sei();
    return retVal;
}

uint16_t SpeechBufRemove()
{
    uint16_t data;

    cli();

    if (SpeechBufCount==0) {
        data = 0;
    } else {
        data = *SpeechBufTail;
        if (SpeechBufTail == SpeechBufEnd) {
            SpeechBufTail = SpeechBuf;
        } else {
            SpeechBufTail++;
        }

        SpeechBufCount--;
    }

    sei();
    return data;
}

void SpeechAmpEnable(bool enable) 
{
    if (enable) {
        PORTC |= SHDN_PIN;
    } else {
        PORTC &= (~SHDN_PIN);
    }
}

void CSLow()
{
    PORTB &= (~PIN_CS);
}

void CSHigh()
{
    PORTB |= PIN_CS;
}

void WRLow()
{
    PORTB &= (~PIN_WR);
}

void WRHigh()
{
    PORTB |= PIN_WR;
}

void CMSLow()
{
    PORTB &= (~PIN_CMS);
}

void CMSHigh()
{
    PORTB |= PIN_CMS;
}

void OutputWord(uint8_t word)
{
    uint8_t word_lo = word & 0x3F;
    uint8_t word_hi = word >> 6;

    PORTD = (PORTD & ~WORD_LO_MASK) | (word_lo<<WORD_LO_SHIFT);
    PORTB = (PORTB & ~WORD_HI_MASK) | (word_hi);
}

void OutputBank(uint8_t bank)
{
    PORTB = (PORTB & ~PIN_BANK0) | ((bank & 1) ? PIN_BANK0 : 0);
    PORTB = (PORTB & ~PIN_BANK1) | ((bank & 2) ? PIN_BANK1 : 0);
    PORTB = (PORTB & ~PIN_BANK2) | ((bank & 4) ? PIN_BANK2 : 0);
}

void SpeechUpdate()
{
    uint8_t busyWaitCounter;

    if ((PINC & PIN_INTR) != 0) {
        // An interrupt occurred -- we're ready for the next word
        SpeechReady = true;
    }

    if ((SpeechState & SPEECH_ERR_MASK) == SPEECH_ERR_MASK) {
        // error state, do not update
        return;
    }

    if (!SpeechReady) {
        // We're not ready
        return;
    }

    if (SpeechBufIsEmpty()) {
        SpeechState = SPEECH_IDLE;
        return;
    }

    // votrax is ready for data, and there are phonemes to speak
    uint16_t data = SpeechBufRemove();

    SpeechLastWordOut = data & 0xFF;
    SpeechLastBankOut = data >> 8;

    CMSLow();
    CSLow();
    OutputBank(data >> 8);
    OutputWord(data & 0xFF);
    WRLow();
    WRHigh();
    CSHigh();

    SpeechReady = false;

    busyWaitCounter = 255;
    while (((PINC & PIN_INTR) != 0) && (busyWaitCounter>0)) {
        SpeechState = SPEECH_BUSYWAIT;
        busyWaitCounter--;
        delayMicroseconds(1);
    }
    
    if (busyWaitCounter == 0) {
        SpeechState = SPEECH_ERR_BUSY_TIMEOUT;
    } else {
        SpeechState = SPEECH_ACKED;
    }
}

void SpeechInit()
{
    SpeechBufHead = SpeechBuf;
    SpeechBufTail = SpeechBuf;
    SpeechBufCount = 0;
    SpeechLastWordOut = 0;

    SpeechReady = 1;

    // shdn is output, default is low (amp is shutdown)
    PORTC &= (~SHDN_PIN);
    DDRC |= SHDN_PIN;

    CSHigh();
    DDRB |= PIN_CS;

    WRHigh();
    DDRB |= PIN_WR;

    CMSLow();
    DDRB |= PIN_CMS;

    OutputWord(0x00);
    DDRD |= WORD_LO_MASK;
    DDRB |= WORD_HI_MASK;

    OutputBank(0x00);
    DDRB |= PIN_BANK_MASK;

    SpeechState = SPEECH_INIT;
}

void SpeechTest()
{
    SpeechBufInsert(0); // this is digitalker
}

