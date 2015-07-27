/*
 * oscillators.c
 *
 *  Created on: Jul 20, 2015
 *      Author: austen
 */
#include <math.h>
#include <stdint.h>
#include "oscillator.h"
/*
 * Generate phase steps from freqs with the formula:
 * 		phase_step = note_freq/sample_freq*wavetable_length
 */
const float freqNoteTable[] = {
		110.        ,  116.54094038,  123.47082531,  130.81278265,
       138.59131549,  146.83238396,  155.56349186,  164.81377846,
       174.61411572,  184.99721136,  195.99771799,  207.65234879,
       220.        ,  233.08188076,  246.94165063,  261.6255653 ,
       277.18263098,  293.66476792,  311.12698372,  329.62755691,
       349.22823143,  369.99442271,  391.99543598,  415.30469758,
       440.        ,  466.16376152,  493.88330126,  523.2511306 ,
       554.36526195,  587.32953583,  622.25396744,  659.25511383,
       698.45646287,  739.98884542,  783.99087196,  830.60939516,  880.};

void initOsc(Osc* osc,uint32_t fs){
	*osc={{0}};
	osc->fs = fs;
}

uint32_t getOscSample(Osc* osc){
	return (uint32_t)((osc->phase*0x3FF*osc->gain)/osc->period);
}

void setOscFreq(Osc* osc,uint16_t noteNum){
	osc->targetNote = noteNum;
	osc->period = osc->fs/(freqNoteTable[osc->targetNote]);
}

void incrementOscPhase(Osc* osc){
	osc->phase = (osc->phase+1)%osc->period;
}

void applyMods(Osc* osc){
	float slideAmt = osc->freqMods[0]+osc->freqMods[1];
	int16_t freqSlope;
	if (slideAmt>0){
		freqSlope = freqNoteTable[osc->targetNote+(int16_t)ceil(slideAmt)]-freqNoteTable[osc->targetNote];
	}else if(slideAmt<0){
		freqSlope = freqNoteTable[osc->targetNote]-freqNoteTable[osc->targetNote+(int16_t)floor(slideAmt)];
	}
	osc->period = (uint32_t)osc->fs/(freqNoteTable[osc->targetNote]+slideAmt*freqSlope);
}

void modifyOscFreq(void* oscptr, int32_t modAmount, uint8_t port){
	Osc* osc = (Osc*)oscptr;
	osc->freqMods[port] = (modAmount-512)/512.0;
}
