/*
 * inputs.h
 *
 *  Created on: Jul 21, 2015
 *      Author: austen
 */

#ifndef INPUT_H_
#define INPUT_H_
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/gpio.h"
#include "oscillator.h"

typedef struct {
	void 	(*send_fn)(void*,float,uint8_t);
	void* 	send_target;
	uint8_t out_port;
	float	gain;
	int32_t currValue;
	int32_t lastValue;
} Knob;

void initKnob(Knob*,void*,uint8_t, float);
void handleDigitalInputs(void);
void handleAnalogInputs(Knob[]);
void setSequenceFreq(float);

#define NUM_SEQUENCES 1
#define SEQUENCE_LENGTH 4
uint32_t seqChangePeriod;
extern uint16_t sequenceTable[NUM_SEQUENCES][SEQUENCE_LENGTH];
extern uint8_t currSequence;
extern uint8_t currSequenceNote;

extern Osc mainOsc1;
extern Osc mainOsc2;
#endif /* INPUT_H_ */
