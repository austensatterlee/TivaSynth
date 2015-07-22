/*
 * includes.h
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */

#ifndef OSCILLATORS_H_
#define OSCILLATORS_H_
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#include <stdint.h>
#include <stdbool.h>
void OscillatorsInit(void);
void setOscillator(uint8_t,uint8_t*);
void setNote(uint8_t,uint16_t);
void releaseOscillator(uint8_t);
void tickOscillators(void);
uint16_t getNextSample(void);
// Math constants
#define APP_PI      3.1415926535897932384626433832795f
// System parameters
#define INIT_TICKSTEP 1
#define INIT_PWMPERIOD 1000
#define INIT_TICKPERIOD 512
#define NUM_OSCILLATORS 4
struct Oscillator {
	float TickStep;
	uint16_t TickPeriod;
	float tick;
	bool gate;
	uint8_t *wavetable;
};
// System states
uint32_t _ui32SysClock; // system clock speed
#endif /* OSCILLATORS_H_ */
