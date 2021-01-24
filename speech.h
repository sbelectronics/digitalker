#ifndef __SPEECH_H
#define __SPEECH_H

extern uint8_t SpeechLastWordOut;
extern uint8_t SpeechLastBankOut;
extern uint8_t SpeechBufCount;
extern uint8_t SpeechState;
extern uint8_t SpeechReady;

void SpeechUpdate();
void SpeechInit();
void SpeechAmpEnable(bool);
void SpeechTest();
bool SpeechBufInsert(uint16_t data);

#endif
