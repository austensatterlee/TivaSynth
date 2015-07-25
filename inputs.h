/*
 * inputs.h
 *
 *  Created on: Jul 21, 2015
 *      Author: austen
 */

#ifndef INPUTS_H_
#define INPUTS_H_
#include "oscillators.h"
#include "driverlib/gpio.h"

void InputQueueInit(void);
void triggerGate(uint8_t);
void handleDigitalInputs(void);
void handleAnalogInputs(void);

#define NUM_BUTTONS 5
#define E_BUTTON_1 GPIO_PIN_4
#define E_BUTTON_2 GPIO_PIN_0
#define E_BUTTON_3 GPIO_PIN_1
#define E_BUTTON_4 GPIO_PIN_2
#define E_BUTTON_5 GPIO_PIN_5
#define NUM_KNOBS 2
#define ALL_E_BUTTONS (E_BUTTON_1|E_BUTTON_2|E_BUTTON_3|E_BUTTON_4|E_BUTTON_5)

typedef struct {
	uint16_t value;
	void (*send_fn)(uint16_t*);
} Knob;

static Knob knobs[NUM_KNOBS] = {{0}};

#endif /* INPUTS_H_ */
