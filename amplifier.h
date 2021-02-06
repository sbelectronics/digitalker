#ifndef _AMPLIFIER_H_
#define _AMPLIFIER_H_

#define AMP_GAIN_ZERO 28
#define AMP_GAIN_MIN 0
#define AMP_GAIN_MAX 57

void AmpInit();
void AmpSetup();
void AmpSetVolume(uint8_t volume, bool updateDisplay);

#endif
