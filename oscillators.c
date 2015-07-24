/*
 * oscillators.c
 *
 *  Created on: Jul 20, 2015
 *      Author: austen
 */
#include <math.h>
#include "oscillators.h"
#include "driverlib/pwm.h"
#include "driverlib/timer.h"
#include "driverlib/rom_map.h"
#include "inc/hw_memmap.h"
#include "wavetables.h"
#include "envelope.h"

/* Tick steps to generate notes from A3 to A5 with (120e6/256) Hz PWM
 * Generated with the formula:
 * 		tick_step = note_freq/(cpu_freq/(pwm_period*wavetable_length))
 */
float tickStepTable[] = {0.2816    ,  0.29834481,  0.31608531,  0.33488072,  0.35479377,
        0.3758909 ,  0.39824254,  0.42192327,  0.44701214,  0.47359286,
        0.50175416,  0.53159001,  0.5632    ,  0.59668961,  0.63217063,
        0.66976145,  0.70958754,  0.75178181,  0.79648508,  0.84384655,
        0.89402427,  0.94718572,  1.00350832,  1.06318003,  1.1264    ,
        1.19337923,  1.26434125,  1.33952289,  1.41917507,  1.50356361,
        1.59297016,  1.68769309,  1.78804854,  1.89437144,  2.00701663,
        2.12636005,  2.2528};

/* Equivalent to above, but for a hardware timer (w/CPU freq @ 120MHz and 256-length wavetable) */
uint32_t timerLoadTable[] = {
		       1065, 1005,  949,  895,  845,  798,  753,  711,  671,  633,  597,
		        564,  532,  502,  474,  447,  422,  399,  376,  355,  335,  316,
		        298,  282,  266,  251,  237,  223,  211,  199,  188,  177,  167,
		        158,  149,  141,  133
};
Osc mainOsc;
Env ampEnv;
Osc pitchLFO;
Env ampEnvLFO;
void OscillatorsInit(){
	initEnvelope(&ampEnv,&SAW256,200,1,1.0/256.0,3500);
	initEnvelope(&ampEnvLFO,&EXPTRI256,200,2000,1.0/256.0,350);
	initOscillator(&mainOsc,&COS256,&ampEnv,256,0,1);
	initOscillator(&pitchLFO,&COS256,&ampEnvLFO,0xFF,-127,5);
}

void initOscillator(Osc* osc, Wavetable* wavetable, Env* env, uint16_t aDivisor, int32_t aBias, uint32_t timerLoad){
	osc->wavetable 			= wavetable;
	osc->wavetableSize 		= wavetable->size;
	osc->env				= env;
	osc->amplitudeDivisor 	= aDivisor;
	osc->amplitudeBias 		= aBias;
	osc->timerLoad			= timerLoad;
	osc->dt					= 1;
	osc->tick 				= 0;
	osc->gate 				= 0;
}

void setMainOscNote(uint16_t freqIndex){
	triggerEnvelope(&ampEnv);
	triggerEnvelope(&ampEnvLFO);
	mainOsc.gate=1.0;
	MAP_TimerLoadSet(TIMER1_BASE,TIMER_A,timerLoadTable[freqIndex]);
}

void releaseMainOsc(){
	mainOsc.gate=0.0;
	releaseEnvelope(&ampEnv);
	releaseEnvelope(&ampEnvLFO);
}

float getNextSample(Osc* osc){
	float oscSample = 0;
	uint16_t bNeighbor;
	uint16_t fNeighbor;
	osc->gate = getEnvelopeSample(osc->env);
	if(osc->gate){
		bNeighbor = (osc->wavetable->wave)[(int)(osc->tick)];
		fNeighbor = (osc->wavetable->wave)[(int)(1+osc->tick)];
		oscSample = (fNeighbor-bNeighbor)*(osc->tick-(int)(osc->tick))+bNeighbor+osc->amplitudeBias;
		oscSample = (oscSample/osc->amplitudeDivisor)*(osc->gate);
	}
	return oscSample;
}

void tickOscillator(Osc* osc){
	osc->tick+=osc->dt;
	if (osc->tick >= osc->wavetableSize)
	{
		osc->tick = 0;
	}
}
