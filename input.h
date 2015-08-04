/*
 * inputs.h
 *
 *  Created on: Jul 21, 2015
 *      Author: austen
 */

#ifndef INPUT_H_
#define INPUT_H_
#include <stdint.h>

typedef struct {
	float gain;
	float output;
	int32_t currValue;
	int32_t lastValue;
} Knob;

void initKnob(Knob*, float);
void handleDigitalInputs(void);
void handleAnalogInputs(Knob[]);
void readFromExpansion(void);

#endif /* INPUT_H_ */
