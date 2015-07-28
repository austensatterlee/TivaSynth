/*
 * includes.h
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */

#ifndef OSCILLATOR_H_
#define OSCILLATOR_H_
#include <stdint.h>
#include <stdbool.h>

#define NUM_OSC_MOD_PORTS 3
typedef struct Osc_st {
	float gain;
	uint16_t targetFreq;
	float freqMods[NUM_OSC_MOD_PORTS];
	uint32_t period;
	uint32_t phase;
	uint32_t fs;
} Osc;

void initOsc(Osc*, uint32_t);
float getOscSample(Osc*);
void setOscNote(Osc*, uint16_t);
void setOscFreq(Osc*, uint16_t);
void incrementOscPhase(Osc* osc);
void modifyOscGain(void*, float, uint8_t);
void modifyOscFreq(void*, float, uint8_t);
void applyMods(Osc*);

#endif /* OSCILLATOR_H_ */
