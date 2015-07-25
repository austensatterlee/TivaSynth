/*
 * envelope.c
 *
 *  Created on: Jul 23, 2015
 *      Author: austen
 */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "envelope.h"
#include "wavetables.h"

void initEnvelope(Env* env, Wavetable* wavetable,uint16_t sustainIndex,
				  uint32_t timerLoadAtk, uint32_t timerLoadRel, float gain){
	 env->wavetable 		= wavetable;
	 env->gain 				= gain;
	 env->tick 				= 0;
	 env->gate 				= 0;
	 env->currAmplitude		= 0;
	 env->relRate			= 0.7;
	 env->sustainIndex 		= sustainIndex;
	 env->timerLoadAtk 		= timerLoadAtk;
	 env->timerLoadRel 		= timerLoadRel;
	 env->currTimerLoad 	= &env->timerLoadAtk;
}

bool tickEnvelope(Env* env){
	if(env->gate){
		if(env->tick < env->sustainIndex){
			env->currAmplitude = *(env->wavetable->wave+(uint16_t)(env->tick))*(env->gain);
			env->tick += 1;
		}
	}else if(env->currAmplitude > 0.005){
		env->currAmplitude*=env->relRate;
	}else{
		return false;
	}
	return true;
}

void triggerEnvelope(Env* env){
	if(!env->gate){
		env->gate=1;
		env->tick = 0.0;
		env->currTimerLoad = &env->timerLoadAtk;
	}
}

void releaseEnvelope(Env* env){
	env->gate=0;
	env->currTimerLoad = &env->timerLoadRel;
}

float getEnvelopeSample(Env* env){
	return env->currAmplitude;
}
