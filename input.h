/*
 * inputs.h
 *
 *  Created on: Jul 21, 2015
 *      Author: austen
 */

#ifndef INPUT_H_
#define INPUT_H_
#include <stdint.h>
#include "arm_math.h"

typedef struct {
	q31_t gain;
	q31_t output;
	int32_t currValue;
	int32_t lastValue;
	uint8_t shift;
} Knob;

void initKnob(Knob*, float);
void handleDigitalInputs(void);
void handleAnalogInputs(Knob[]);
void readFromExpansion(void);

#endif /* INPUT_H_ */
