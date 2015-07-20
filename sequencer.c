/*
 * sequencer.c
 *
 *  Created on: Jul 20, 2015
 *      Author: austen
 */

#include "sequencer.h"
#include "pwm.h"

uint16_t sequenceInd;
uint8_t sequenceLength;
float semitones[] = {1.0,
				1.05946309,  1.12246205,  1.18920712,  1.25992105,
				1.33483985,  1.41421356,  1.49830708,  1.58740105,
				1.68179283,  1.78179744,  1.88774863,  2.};
uint16_t sequence[] = {
		0,2,3,7,12,7,3,2
};

void initSequencer(){
	sequencerPeriod = INIT_SEQUENCER_PERIOD;
	sequenceInd = 0;
	sequenceLength = 8;
}

void pokeSequencer(){
	TickStep_0 = semitones[sequence[sequenceInd]];
	sequenceInd+=1;
	if (sequenceInd>=sequenceLength){
		sequenceInd=0;
	}
}
