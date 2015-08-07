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
	osc->step  = 0;
	osc->freqMod = 0;
	osc->noteMod = 0;
	osc->gainMod = 1.0;
	osc->syncOsc = 0;
	setOscNote(osc, 0);
	applyMods(osc);
}
void incrOscPhase(Osc* osc) {
	osc->phase+=osc->step;//(osc->phase + 1) % osc->period;
	if(osc->syncOsc && osc->phase < osc->step){
		osc->syncOsc->phase = 0;
	}
	switch(osc->wvfmType){
	case SAW_WV:
		osc->output = (osc->phase * osc->gain / 0xFFFFFFFF);
		break;
	case TRI_WV:
		osc->output = (osc->phase>(0xFFFFFFFF>>1) ? 0xFFFFFFFF-osc->phase : osc->phase);
		osc->output = (osc->output * osc->gain / 0xFFFFFFFF);
		break;
	case SQUARE_WV:
		osc->output = (osc->phase>(0xFFFFFFFF>>1) ? 0xFFFFFFFF : 0);
		osc->output = (osc->output * osc->gain / 0xFFFFFFFF);
		break;
	default:
		osc->output = 0.5;
		break;
	}
	osc->output-=0.5;
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
	osc->noteMod = 0;
	//osc->period = osc->fs / (osc->freqTable[osc->targetNote]);
}
void setOscGain(Osc* osc, float gain) {
	osc->targetGain = gain;
}
void applyMods(Osc* osc) {
	float notefreq = getNoteFreq(osc->targetNote+osc->noteMod)*getFreqSlideAmt(osc->freqMod);
	osc->step = (uint32_t)( 0xFFFFFFFF*notefreq/osc->fs );
	osc->gain	= osc->targetGain*osc->gainMod;
	osc->freqMod = 0.0;
	osc->gainMod = 1.0;
}
void modifyOscFreq(void* oscptr, float modAmount) {
	Osc* osc = (Osc*) oscptr;
	osc->freqMod += (modAmount);
}
void modifyOscGain(void* oscptr, float modAmount) {
	Osc* osc = (Osc*) oscptr;
	osc->gainMod *= modAmount;
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
