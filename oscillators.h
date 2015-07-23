/*
 * includes.h
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */

#ifndef OSCILLATORS_H_
#define OSCILLATORS_H_
#include <stdint.h>
#include <stdbool.h>
#define VOICE 0
#define AMPLITUDE 1
struct Oscillator {
	float TickStep;
	uint16_t TickPeriod;
	float tick;
	float gate;
	uint8_t *wavetable;
};

void OscillatorsInit(void);
void initOscillator(struct Oscillator*,uint8_t*);
void triggerNote(uint8_t,uint16_t);
void releaseOscillator(uint8_t);
void tickOscillators(void);

float getNextSample(uint8_t,uint8_t oscType);
// Math constants
#define APP_PI      3.1415926535897932384626433832795f
// System parameters
#define INIT_TICKSTEP 0
#define INIT_PWMPERIOD 500
#define INIT_TICKPERIOD 512
#define NUM_OSCILLATORS 2
// System states
uint32_t _ui32SysClock; // system clock speed
#endif /* OSCILLATORS_H_ */
