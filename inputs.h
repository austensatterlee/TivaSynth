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
void assignNextOscToButton(uint8_t);
void handleInputs(void);

#define NUM_BUTTONS 5
#define BUTTON_1 GPIO_PIN_4
#define BUTTON_2 GPIO_PIN_0
#define BUTTON_3 GPIO_PIN_1
#define BUTTON_4 GPIO_PIN_2
#define BUTTON_5 GPIO_PIN_5
#define ALL_BUTTONS (BUTTON_1|BUTTON_2|BUTTON_3|BUTTON_4|BUTTON_5)

#endif /* INPUTS_H_ */
