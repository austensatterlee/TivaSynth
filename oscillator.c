/*
 * oscillators.c
 *
 *  Created on: Jul 20, 2015
 *      Author: austen
 */
#include <stdint.h>
#include "oscillator.h"
#include "systeminit.h"
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
	osc->syncedPhase	 = 0;
	setOscType(osc, SOUND_OSC, TriTable);
	setOscNote(osc, 0);
	applyMods(osc);
}
void incrOscPhase(Osc* osc) {
	osc->phase+=osc->step;
	if (osc->syncedPhase && osc->phase<osc->step){
		*osc->syncedPhase=osc->phase;
	}
	/*
	 * The interpolation function takes a Q12.20 number. Our phase is a Q8.24 number, since our wavetables are 256 elements in size.
	 * To prevent the phase from being misinterpreted, we must first divide by 2^4.
	 */
	osc->output=arm_linear_interp_q31(osc->wavetable,osc->phase>>4,256);
	osc->output=((q63_t)osc->gain*osc->output)>>31;
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
	osc->gain = ((q63_t)osc->targetGain*osc->gainMod)>>31;
	osc->noteMod = 0;
	osc->freqMod = 0;
	osc->gainMod = 0x7FFFFFFF;
}
void modifyOscFreq(Osc* osc, q31_t modAmount) {
	modAmount=modAmount>>6;
	arm_add_q31(&modAmount,&osc->freqMod,&osc->freqMod,1);
}
void modifyOscGain(Osc* osc, q31_t modAmount) {
	arm_mult_q31(&modAmount,&osc->gainMod,&osc->gainMod,1);
}
void modifyOscNote(Osc* osc, q31_t semitones){
	osc->noteMod = semitones<<20;
}
/*****************************************************************
 *
 * Envelope methods
 *
 *****************************************************************/
void initEnv(Env* env, const q31_t* atkwavetable, const q31_t* relwavetable){
	env->atkstep 	= 0;
	env->relstep	= 0;
	env->phase 		= 0;
	env->output 	= 0;
	env->step 		= 0;
	env->gate		= 0;
	env->hold		= 0x7FFFFFFF;
	env->atkwavetable  = atkwavetable;
	env->relwavetable  = relwavetable;
}

void setEnvAtkTime(Env* env, float atktime){
	atktime = 1.0/(atktime*MOD_FS);
	arm_float_to_q31(&atktime,&env->atkstep,1);
}
void setEnvRelTime(Env* env, float reltime){
	reltime = 1.0/(reltime*MOD_FS);
	arm_float_to_q31(&reltime,&env->relstep,1);
}
void triggerEnv(Env* env){
	env->gate = 1;
	env->step = env->atkstep;
	env->hold = 0x7FFFFFFF;
}
void releaseEnv(Env* env){
	env->gate = 0;
	env->step = env->relstep;
	env->phase = 0;
	env->hold = env->output;
}
void incrEnvPhase(Env* env){
	arm_add_q31(&env->phase,&env->step,&env->phase,1);
	if( env->gate ){
		env->output = arm_linear_interp_q31(&env->atkwavetable[0],env->phase>>3,256);
	}else{
		env->output = env->relwavetable[255] - arm_linear_interp_q31(&env->relwavetable[0],env->phase>>3,256);
	}
	arm_mult_q31(&env->output,&env->hold,&env->output,1);
}
