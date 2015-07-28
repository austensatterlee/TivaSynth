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
const float freqNoteTable[] = { 55., 58.27046967, 61.7354126, 65.40639496,
		69.2956543, 73.4161911, 77.78174591, 82.40689087, 87.30706024,
		92.49860382, 97.99885559, 103.82617188, 110., 116.54093933, 123.4708252,
		130.81278992, 138.59130859, 146.8323822, 155.56349182, 164.81378174,
		174.61412048, 184.99720764, 195.99771118, 207.65234375, 220.,
		233.08187866, 246.94165039, 261.62557983, 277.18261719, 293.6647644,
		311.12698364, 329.62756348, 349.22824097, 369.99441528, 391.99542236,
		415.3046875, 440., 466.16375732, 493.88330078, 523.25115967,
		554.36523438, 587.32952881, 622.25396729, 659.25512695, 698.45648193,
		739.98883057, 783.99084473, 830.609375, 880., 932.32751465,
		987.76660156, 1046.50231934, 1108.73046875, 1174.65905762,
		1244.50793457, 1318.51025391, 1396.91296387, 1479.97766113,
		1567.98168945, 1661.21875, 1760., 1864.6550293, 1975.53320312,
		2093.00463867, 2217.4609375, 2349.31811523, 2489.01586914,
		2637.02050781, 2793.82592773, 2959.95532227, 3135.96337891 };

void initOsc(Osc* osc, uint32_t fs) {
	osc->fs = fs;
	osc->gain = 1.0;
	osc->phase = 0;
	setOscNote(osc, 0);
	uint8_t i = NUM_OSC_MOD_PORTS;
	while (i--) {
		osc->freqMods[i] = 0;
	}
}

float getOscSample(Osc* osc) {
	return (int32_t) ((osc->phase << 1) - osc->period)
			* (osc->gain / osc->period);
}

void setOscNote(Osc* osc, uint16_t noteNum) {
	osc->targetFreq = freqNoteTable[noteNum];
	osc->period = osc->fs / (osc->targetFreq);
}

void setOscFreq(Osc* osc, uint16_t freq) {
	osc->targetFreq = freq;
	osc->period = osc->fs / (osc->targetFreq);
}

void incrementOscPhase(Osc* osc) {
	osc->phase = (osc->phase + 1) % osc->period;
}

void applyMods(Osc* osc) {
	float slideAmt;
	int16_t freqSlope;
	uint16_t newFreq = osc->targetFreq;
	uint8_t index = NUM_OSC_MOD_PORTS;
	while (index--) {
		slideAmt = osc->freqMods[index];
		if (slideAmt > 3) {
			freqSlope = osc->targetFreq * 8;
			slideAmt -= 3;
		} else if (slideAmt > 2) {
			freqSlope = osc->targetFreq * 4;
			slideAmt -= 2;
		} else if (slideAmt > 1) {
			freqSlope = osc->targetFreq * 2;
			slideAmt -= 1;
		} else if (slideAmt > 0) {
			freqSlope = osc->targetFreq;
		} else if (slideAmt < 0) {
			freqSlope = osc->targetFreq / 2;
		}
		newFreq += (slideAmt) * freqSlope;
	}
	osc->period = (uint32_t) osc->fs / newFreq;
}

void modifyOscFreq(void* oscptr, float modAmount, uint8_t port) {
	Osc* osc = (Osc*) oscptr;
	osc->freqMods[port] = modAmount;
}
