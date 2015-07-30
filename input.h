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
	void (*send_fn)(void*, float, uint8_t);
	void* send_target;
	uint8_t out_port;
	float gain;
	float output;
	int32_t currValue;
	int32_t lastValue;
} Knob;

void initKnob(Knob*, void*, uint8_t, float);
void handleDigitalInputs(void);
void handleAnalogInputs(Knob[]);

#endif /* INPUT_H_ */
