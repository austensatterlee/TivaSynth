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
#include "arm_math.h"

typedef enum OscTypes {SOUND_OSC,LFO_OSC} OscType;
typedef struct Osc_st {
	const q15_t	*wavetable;
	q31_t	 	output;
	q31_t	 	freqMod;
	q31_t		(*getFreq_fn)(q31_t);
	q31_t	 	gainMod;
	q31_t	 	targetGain;
	q31_t		gain;
	q31_t 	targetNote;
	q31_t 	noteMod;
	uint32_t 	phase;
	q31_t	 	step;
} Osc;
void initOsc(Osc*);
void setOscType(Osc*, OscType, const q15_t*);
void setOscNote(Osc*, q31_t);
void setOscFreq(Osc*, q31_t);
void setOscGain(Osc*, q31_t);
void modifyOscFreq(Osc*, q31_t);
void modifyOscGain(Osc*, q31_t);
void applyMods(Osc*);
void incrOscPhase(Osc*);

typedef struct Env_st {
	q31_t	 	output;
	q31_t 	 	hold;
	uint32_t 	atkstep;
	uint32_t 	relstep;
	uint32_t 	step;
	uint32_t 	phase;
	uint32_t 	fs;
	uint8_t		gate;
} Env;
void initEnv(Env*, uint32_t);
void setEnvAtkTime(Env*, float);
void setEnvHold(Env*, float);
void setEnvRelTime(Env*, float);
void triggerEnv(Env*);
void releaseEnv(Env*);
void incrEnvPhase(Env*);
#endif /* OSCILLATOR_H_ */
