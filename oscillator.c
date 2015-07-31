/*
 * oscillators.c
 *
 *  Created on: Jul 20, 2015
 *      Author: austen
 */
#include <math.h>
#include <stdint.h>
#include "oscillator.h"
#include "tables.h"
/*****************************************************************
 *
 * Oscillator methods
 *
 *****************************************************************/
void initOsc(Osc* osc, uint32_t fs) {
	osc->fs = fs;
	osc->oscType  = SOUND_OSC;
	osc->freqTable = soundFreqTable;
	osc->wvfmType = SAW_WV;
	osc->targetGain = 1.0;
	osc->phase = 0;
	setOscNote(osc, 0);
	uint8_t i = NUM_OSC_MOD_PORTS;
	while (i--) {
		osc->freqMods[i] = 0;
		osc->gainMods[i] = 1.0;
	}
}
void incrOscPhase(Osc* osc) {
	osc->phase = (osc->phase + 1) % osc->period;
}
float getOscSample(Osc* osc) {
	switch(osc->wvfmType){
	case SAW_WV:
		osc->output = (osc->phase * osc->gain / osc->period);
		break;
	case TRI_WV:
		osc->output = (osc->phase>(osc->period>>1) ? osc->period-osc->phase : osc->phase);
		osc->output = (osc->output * osc->gain / osc->period);
		break;
	default:
		osc->output = 0.5;
		break;
	}
	return osc->output;
}
void setOscType(Osc* osc, WvfmType wvfmtype, OscType osctype){
	osc->wvfmType = wvfmtype;
	osc->oscType = osctype;
	switch(osc->oscType){
	case SOUND_OSC:
		osc->freqTable = soundFreqTable;
		break;
	case LFO_OSC:
		osc->freqTable = lfoFreqTable;
		break;
	default:
		break;
	}
}
void setOscNote(Osc* osc, uint16_t noteNum) {
	osc->targetNote = noteNum;
	osc->period = osc->fs / (osc->freqTable[osc->targetNote]);
}
void setOscGain(Osc* osc, float gain) {
	osc->targetGain = gain;
}
void applyMods(Osc* osc) {
	float freqSlideAmt = 0.0;
	float gainSlideAmt = 1.0;
	uint8_t index = NUM_OSC_MOD_PORTS;
	while (index--) {
		freqSlideAmt += osc->freqMods[index];
		gainSlideAmt *= osc->gainMods[index];
	}
	freqSlideAmt = 1+getFreqSlideAmt(12*freqSlideAmt);
	osc->period = (uint32_t)( osc->fs /( freqSlideAmt*osc->freqTable[osc->targetNote] ));
	osc->gain	= osc->targetGain*gainSlideAmt;
}
void modifyOscFreq(void* oscptr, float modAmount, uint8_t port) {
	Osc* osc = (Osc*) oscptr;
	osc->freqMods[port] = modAmount;
}
void modifyOscGain(void* oscptr, float modAmount, uint8_t port) {
	Osc* osc = (Osc*) oscptr;
	osc->gainMods[port] = modAmount;
}
/*****************************************************************
 *
 * Envelope methods
 *
 *****************************************************************/
void initEnv(Env* env, uint32_t fs){
	env->fs 		= fs;
	env->atkstep 	= 0;
	env->hold 		= 0;
	env->relstep	= 0;
	env->phase 		= 0;
	env->sample 	= 0;
	env->step 		= 0;
	env->gate		= 0;
}
float getEnvSample(Env* env){
	env->sample = env->phase*env->hold/0xFFFFFFF;
	env->sample *= env->sample;
	return env->sample;
}
void setEnvAtkTime(Env* env, float atktime){
	env->atkstep = (0xFFFFFFF/(env->fs*atktime));
}
void setEnvRelTime(Env* env, float reltime){
	env->relstep = (0xFFFFFFF/(env->fs*reltime));
}
void setEnvHold(Env* env, float hold){
	env->hold = hold;
}
void triggerEnv(Env* env){
	env->gate = 1.0;
	env->step = env->atkstep;
}
void releaseEnv(Env* env){
	env->gate = 0.0;
	env->step = env->relstep;
}
void incrEnvPhase(Env* env){
	if( env->gate ){
		if (env->phase < (0xFFFFFFF-env->atkstep) ){
			env->phase 	+= env->step;
		}else{
			env->phase = 0xFFFFFFF;
		}
	}else{
		if( env->phase > env->relstep ){
			env->phase -= env->step;
		}else{
			env->phase = 0;
		}
	}
}
