/*
 * oscillators.c
 *
 *  Created on: Jul 20, 2015
 *      Author: austen
 */
#include <stdint.h>
#include "oscillator.h"
#include "tables.h"
#include "arm_math.h"
/*****************************************************************
 *
 * Oscillator methods
 *
 *****************************************************************/
void initOsc(Osc* osc) {
	osc->targetGain = 0x7FFFFFFF;
	osc->phase = 0;
	osc->step  = 0;
	osc->freqMod = 0;
	osc->noteMod = 0;
	osc->gainMod = 0x7FFFFFFF;
	setOscType(osc, SOUND_OSC, TriTable);
	setOscNote(osc, 0);
	applyMods(osc);
}
void incrOscPhase(Osc* osc) {
	osc->phase+=osc->step;
	osc->output=arm_linear_interp_q31(osc->wavetable,osc->phase>>4,256);
	arm_mult_q31(&osc->output,&osc->gain,&osc->output,1);
}
void setOscType(Osc* osc, OscType osctype, const q31_t *wavetable){
	osc->wavetable = wavetable;
	switch(osctype){
	case SOUND_OSC:
		osc->getFreq_fn = getNoteFreq;
		break;
	case LFO_OSC:
		osc->getFreq_fn = getLFOFreq;
		break;
	default:
		break;
	}
}
void setOscNote(Osc* osc, q31_t noteNum) {
	osc->targetNote = noteNum<<20;
	osc->noteMod = 0;
}
void setOscGain(Osc* osc, q31_t gain) {
	osc->targetGain = gain;
}
void applyMods(Osc* osc) {
	osc->step = osc->getFreq_fn(osc->targetNote+osc->noteMod+osc->freqMod);
	arm_mult_q31(&osc->targetGain,&osc->gainMod,&osc->gain,1);
	osc->freqMod = 0;
	osc->gainMod = 0x7FFFFFFF;
}
void modifyOscFreq(Osc* osc, q31_t modAmount) {
	modAmount<<=10;
	arm_add_q31(&modAmount,&osc->freqMod,&osc->freqMod,1);
}
void modifyOscGain(Osc* osc, q31_t modAmount) {
	arm_mult_q31(&modAmount,&osc->gainMod,&osc->gainMod,1);
}
/*****************************************************************
 *
 * Envelope methods
 *
 *****************************************************************/
void initEnv(Env* env, uint32_t fs){
	env->atkrate 	= 0;
	env->relrate	= 0;
	env->phase 		= 0x7FFFFFFF;
	env->output 	= 0;
	env->rate 		= 0;
	env->gate		= 0;
	setEnvHold(env,1.0);
}

void setEnvAtkTime(Env* env, float atktime){
	env->atkrate = envDecayTable[(uint16_t)(atktime/0.020)];
}
void setEnvRelTime(Env* env, float reltime){
	env->relrate = envDecayTable[(uint16_t)(reltime/0.020)];
}
void setEnvHold(Env* env, float hold){
	arm_float_to_q31(&hold,&env->hold,1);
}
void triggerEnv(Env* env){
	env->gate = 1;
	env->rate = env->atkrate;
}
void releaseEnv(Env* env){
	env->gate = 0;
	env->rate = env->relrate;
	env->phase = 0x7FFFFFFF - env->phase;
}
void incrEnvPhase(Env* env){
	arm_mult_q31(&env->phase,&env->rate,&env->phase,1);
	if( env->gate ){
		env->output = 0x7FFFFFFF - env->phase;
	}else{
		env->output = env->phase;
	}
	arm_mult_q31(&env->output,&env->hold,&env->output,1);
}
