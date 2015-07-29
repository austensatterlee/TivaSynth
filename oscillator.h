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
#define NUM_OSC_MOD_PORTS 2
typedef enum Waveforms {SAW_WV,TRI_WV} WvfmType;
typedef enum OscTypes {SOUND_OSC,LFO_OSC} OscType;
typedef struct Osc_st {
	WvfmType	wvfmType;
	uint8_t		oscType;
	float 		output;
	float 		freqMods[NUM_OSC_MOD_PORTS];
	float*		freqTable;
	float 		gainMods[NUM_OSC_MOD_PORTS];
	float 		targetGain;
	float		gain;
	uint16_t 	targetNote;
	uint32_t 	period;
	uint32_t 	phase;
	uint32_t 	fs;
} Osc;
typedef struct Env_st {
	float	 	sample;
	float 	 	hold;
	uint32_t 	atkstep;
	uint32_t 	relstep;
	uint32_t* 	step;
	uint32_t 	phase;
	uint32_t 	fs;
	uint8_t		gate;
} Env;
void initOsc(Osc*, uint32_t);
float getOscSample(Osc*);
void setOscType(Osc*, WvfmType, OscType);
void setOscNote(Osc*, uint16_t);
void setOscFreq(Osc*, uint16_t);
void setOscGain(Osc*, float);
void modifyOscFreq(void*, float, uint8_t);
void modifyOscGain(void*, float, uint8_t);
void applyMods(Osc*);
void incrOscPhase(Osc* osc);

void initEnv(Env*, uint32_t);
float getEnvSample(Env*);
void setEnvAtkTime(Env*, float);
void setEnvHold(Env*, float);
void setEnvRelTime(Env*, float);
void triggerEnv(Env*);
void releaseEnv(Env*);
void incrEnvPhase(Env*);
#endif /* OSCILLATOR_H_ */
