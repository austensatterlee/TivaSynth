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

void initEnvelope(Env* env, Wavetable* wavetable,uint16_t sustainIndex, uint32_t timerLoad, float gain, float relTimeConstant){
	 env->wavetable 		= wavetable;
	 env->gain 				= gain;
	 env->tick 				= 0;
	 env->gate 				= 0;
	 env->currAmplitude		= 0;
	 env->relRate			= expf(-5.2983174/relTimeConstant);
	 env->sustainIndex 		= sustainIndex;
	 env->timerLoad 		= timerLoad;
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
	env->tick = 0;
	if(!env->gate){
		env->gate=1;
	}
}

void releaseEnvelope(Env* env){
	env->gate=0;
}

float getEnvelopeSample(Env* env){
	return env->currAmplitude;
}
