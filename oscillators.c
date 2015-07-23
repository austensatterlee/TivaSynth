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

struct Oscillator voiceOscillators[NUM_OSCILLATORS];
struct Oscillator amplitudeOscillators[NUM_OSCILLATORS];
uint8_t activeVoiceCount;

void OscillatorsInit(){
	initOscillator(&voiceOscillators[0],TRI512);
	initOscillator(&voiceOscillators[1],TRI512);
	initOscillator(&amplitudeOscillators[0],TRI512);
	amplitudeOscillators[0].TickStep = 0.00109;
	initOscillator(&amplitudeOscillators[1],TRI512);
	amplitudeOscillators[1].TickStep = 0.00109;
	activeVoiceCount=0;
}

void initOscillator(struct Oscillator* osc, uint8_t* wavetableAddr){
	osc->TickPeriod = INIT_TICKPERIOD;
	osc->TickStep 	= 0;
	osc->tick 		= 0;
	osc->gate 		= 0;
	osc->wavetable 	= wavetableAddr;
}

void triggerNote(uint8_t oscNum,uint16_t freqIndex){
	(voiceOscillators+oscNum)->TickStep = tickStepTable[freqIndex];
	(amplitudeOscillators+oscNum)->gate = 1;
	(amplitudeOscillators+oscNum)->tick = 0;
	activeVoiceCount++;
}

void releaseOscillator(uint8_t oscNum){
	(voiceOscillators+oscNum)->TickStep = 0;
	(amplitudeOscillators+oscNum)->gate = 0;
	activeVoiceCount--;
}

float getNextSample(uint8_t oscNum, uint8_t oscType){
	float nextSample = 0;
	float oscSample = 0;
	struct Oscillator *osc;
	if(oscType==VOICE){
		osc=voiceOscillators+oscNum;
		osc->gate = getNextSample(oscNum,AMPLITUDE);
	}else if(oscType==AMPLITUDE){
		osc=amplitudeOscillators+oscNum;
	}
	uint16_t bNeighbor;
	uint16_t fNeighbor;
	if(osc->TickStep){
		bNeighbor = (osc->wavetable)[(int)(osc->tick)];
		fNeighbor = (osc->wavetable)[(int)(1+osc->tick)];
		oscSample = (fNeighbor-bNeighbor)*(osc->tick-(int)(osc->tick))+bNeighbor;
		nextSample += ((oscSample-1)/255.0f)*(osc->gate);
	}
	return nextSample;
}

void tickOscillators(){
	struct Oscillator* osc;
	uint8_t i = NUM_OSCILLATORS;
	while(i--){
		osc = (voiceOscillators+i);
		if (osc->TickStep){
			osc->tick+=osc->TickStep;
			if (osc->tick >= osc->TickPeriod)
			{
				osc->tick -= osc->TickPeriod;
			}
		}
		osc = (amplitudeOscillators+i);
		if (osc->TickStep){
			osc->tick+=osc->TickStep;
			if (osc->tick >= osc->TickPeriod)
			{
				osc->tick -= osc->TickPeriod;
			}
		}
	}
}
