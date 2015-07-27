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

void handleDigitalInputs(void);
void handleAnalogInputs(void);

typedef struct {
	void (*send_fn)(void*,int32_t,uint8_t);
	void* send_target;
	uint8_t out_port;
	int32_t currValue;
	int32_t lastValue;
} Knob;


#endif /* INPUT_H_ */
