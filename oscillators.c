/*
 * oscillators.c
 *
 *  Created on: Jul 20, 2015
 *      Author: austen
 */
#include <math.h>
#include "oscillators.h"
#include "driverlib/pwm.h"
#include "inc/hw_memmap.h"
#include "wavetables.h"

/* Tick steps to generate notes from A3 to A5 with (120e6/256) Hz PWM
 * Generated with the formula:
 * 		tick_step = note_freq/(cpu_freq/(pwm_period*wavetable_length))
 */
float tickStepTable[] = {0.46933333,  0.49724135,  0.52680885,  0.55813454,  0.59132295,
        0.62648484,  0.66373757,  0.70320545,  0.74502023,  0.78932144,
        0.83625693,  0.88598335,  0.93866667,  0.99448269,  1.05361771,
        1.11626908,  1.18264589,  1.25296968,  1.32747513,  1.40641091,
        1.49004045,  1.57864287,  1.67251386,  1.77196671,  1.87733333,
        1.98896538,  2.10723542,  2.23253816,  2.36529178,  2.50593935,
        2.65495026,  2.81282182,  2.98008091,  3.15728574,  3.34502772,
        3.54393342,  3.75466667};

struct Oscillator oscillators[NUM_OSCILLATORS];
uint8_t activeVoiceCount;

void OscillatorsInit(){
	setOscillator(0,SINE512);
	setOscillator(1,TRI512);
	setOscillator(2,SINOCT512);
	setOscillator(3,COS512);
	activeVoiceCount=0;
}

void setOscillator(uint8_t oscNum,uint8_t *wavetableAddr){
	struct Oscillator *osc = oscillators+oscNum;
	osc->TickPeriod = INIT_TICKPERIOD;
	osc->TickStep 	= 0;
	osc->tick 		= 0;
	osc->gate 		= 0;
	osc->wavetable 	= wavetableAddr;
}

void setNote(uint8_t oscNum,uint16_t freqIndex){
	struct Oscillator *osc = oscillators+oscNum;
	osc->TickStep = tickStepTable[freqIndex];
	osc->gate = 1;
	activeVoiceCount++;
}

void releaseOscillator(uint8_t oscNum){
	struct Oscillator *osc = oscillators+oscNum;
	osc->TickStep = 0;
	osc->tick = 0;
	osc->gate = 0;
	activeVoiceCount--;
}

uint16_t getNextSample(){
	float nextSample = 0;
	float oscSample = 0;
	struct Oscillator *osc = oscillators+NUM_OSCILLATORS;
	uint16_t bNeighbor;
	uint16_t fNeighbor;
	while(osc--!=oscillators){
		if(osc->TickStep){
			bNeighbor = (osc->wavetable)[(int)(osc->tick)];
			fNeighbor = (osc->wavetable)[(int)(1+osc->tick)];
			oscSample = (fNeighbor-bNeighbor)*(osc->tick-(int)(osc->tick))+bNeighbor;
			nextSample += ((oscSample-1)/(NUM_OSCILLATORS*255.0f)*INIT_PWMPERIOD)*(osc->gate);
		}
	}
	return (uint16_t)(nextSample);
}

void tickOscillators(){
	struct Oscillator *osc = oscillators+NUM_OSCILLATORS;
	while(osc--!=oscillators){
		if (osc->TickStep){
			osc->tick+=osc->TickStep;
			if (osc->tick >= osc->TickPeriod)
			{
				osc->tick -= osc->TickPeriod;
			}
		}
	}
}
