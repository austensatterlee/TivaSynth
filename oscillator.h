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
typedef enum Waveforms {SAW_WV,TRI_WV,SQUARE_WV} WvfmType;
typedef enum OscTypes {SOUND_OSC,LFO_OSC} OscType;
typedef struct Osc_st {
	WvfmType	wvfmType;
	uint8_t		oscType;
	float 		output;
	float 		freqMod;
	float*		freqTable;
	float 		gainMod;
	float 		targetGain;
	float		gain;
	uint16_t 	targetNote;
	uint32_t 	period;
	uint32_t 	phase;
	uint32_t 	fs;
} Osc;
void initOsc(Osc*, uint32_t);
float getOscSample(Osc*);
void setOscType(Osc*, WvfmType, OscType);
void setOscNote(Osc*, uint16_t);
void setOscFreq(Osc*, uint16_t);
void setOscGain(Osc*, float);
void modifyOscFreq(void*, float);
void modifyOscGain(void*, float);
void applyMods(Osc*);
void incrOscPhase(Osc* osc);

typedef struct Env_st {
	float	 	sample;
	float 	 	hold;
	uint32_t 	atkstep;
	uint32_t 	relstep;
	uint32_t 	step;
	uint32_t 	phase;
	uint32_t 	fs;
	uint8_t		gate;
} Env;
void initEnv(Env*, uint32_t);
float getEnvSample(Env*);
void setEnvAtkTime(Env*, float);
void setEnvHold(Env*, float);
void setEnvRelTime(Env*, float);
void triggerEnv(Env*);
void releaseEnv(Env*);
void incrEnvPhase(Env*);
#endif /* OSCILLATOR_H_ */
