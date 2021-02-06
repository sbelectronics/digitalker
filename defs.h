#ifndef _DEFS_H_
#define _DEFS_H_

// uncomment to operate as I2C slave. Disables any display or
// amplifier control capability.
//#define SLAVE

// if ALPHA4 is defined, and SLAVE is not defined, will output
// words to ALPHA4 14-segment led boards.

#define ALPHA4

// if ALPHA4 is defined, and SLAVE is not defined, then serial
// can control the amplifier.

#define AMPLIFIER

#ifdef SLAVE
#undef ALPHA4
#undef AMPLIFIER
#endif

#endif
