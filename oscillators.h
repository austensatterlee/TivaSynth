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
#include "wavetables.h"
#include "envelope.h"

typedef struct Osc_st {
	float dt;
	float tick;
	float gate;
	uint32_t timerLoad;
	Wavetable *wavetable;
	uint16_t wavetableSize;
	uint16_t amplitudeDivisor;
	int16_t amplitudeBias;
	Env *env;
} Osc;

void OscillatorsInit(void);
void initOscillator(Osc*,Wavetable*,Env*,uint16_t,int32_t,uint32_t);
void setMainOscNote(uint16_t);
void releaseMainOsc(void);
void tickOscillator(Osc*);

float getNextSample(Osc*);
// System parameters
#define INIT_PWMPERIOD 1000
// System states
uint32_t _ui32SysClock; // system clock speed
extern Osc mainOsc;
extern Osc pitchLFO;
extern Env ampEnv;
extern Env ampEnvLFO;
#endif /* OSCILLATORS_H_ */
