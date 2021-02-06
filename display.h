#ifndef _DISPLAY_H_
#define _DISPLAY_H_

void DisplayInit();
void DisplayUpdate(uint8_t bank, uint8_t index); 
void DisplayWriteString(char *s);

#endif
