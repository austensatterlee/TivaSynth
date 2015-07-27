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

typedef struct Osc_st {
	float gain;
	uint16_t targetNote;
	float freqMods[2];
	uint32_t period;
	uint32_t phase;
	uint32_t fs;
} Osc;

void initOsc(Osc*,uint32_t);
uint32_t getOscSample(Osc*);
void setOscFreq(Osc*,uint16_t*);
void incrementOscPhase(Osc* osc);
void modifyOscGain(void*,int32_t,uint8_t);
void modifyOscFreq(void*,int32_t,uint8_t);

#endif /* OSCILLATOR_H_ */
