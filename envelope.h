/*
 * envelope.h
 *
 *  Created on: Jul 23, 2015
 *      Author: austen
 */

#ifndef ENVELOPE_H_
#define ENVELOPE_H_

#include <stdint.h>
#include <stdbool.h>
#include "wavetables.h"

typedef struct Envelope_st {
	Wavetable *wavetable;
	uint16_t sustainIndex;
	uint32_t timerLoadAtk;
	uint32_t timerLoadRel;
	uint32_t *currTimerLoad;
	float currAmplitude;
	float gain;
	float relRate;
	float tick;
	float gate;
} Env;

void initEnvelope(Env*, Wavetable*, uint16_t , uint32_t, uint32_t, float);
bool tickEnvelope(Env*);
void releaseEnvelope(Env*);
void triggerEnvelope(Env*);
float getEnvelopeSample(Env*);
#endif /* ENVELOPE_H_ */
