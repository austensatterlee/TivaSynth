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
	osc->output=osc->wavetable[(osc->phase&0x7F800000)>>23]<<16;
	arm_mult_q31(&osc->output,&osc->gain,&osc->output,1);
}
void setOscType(Osc* osc, OscType osctype, const q15_t *wavetable){
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
	env->fs 		= fs;
	env->atkstep 	= 0;
	env->relstep	= 0;
	env->phase 		= 0;
	env->output 	= 0;
	env->step 		= 0;
	env->gate		= 0;
	setEnvHold(env,1.0);
}

void setEnvAtkTime(Env* env, float atktime){
	env->atkstep = (0x7FFFFFFF/(env->fs*atktime));
}
void setEnvRelTime(Env* env, float reltime){
	env->relstep = (0x7FFFFFFF/(env->fs*reltime));
}
void setEnvHold(Env* env, float hold){
	arm_float_to_q31(&hold,&env->hold,1);
}
void triggerEnv(Env* env){
	env->gate = 1;
	env->step = env->atkstep;
}
void releaseEnv(Env* env){
	env->gate = 0;
	env->step = env->relstep;
}
void incrEnvPhase(Env* env){
	if( env->gate ){
		if (env->phase < (0x7FFFFFFF-env->atkstep) ){
			env->phase 	+= env->step;
		}else{
			env->phase = 0x7FFFFFFF;
		}
	}else{
		if( env->phase > env->relstep ){
			env->phase -= env->step;
		}else{
			env->phase = 0;
		}
	}
	arm_mult_q31((q31_t*)&env->phase,(q31_t*)&env->hold,(q31_t*)&env->output,1);
}
