/*
 * systeminit.h
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */

#ifndef SYSTEMINIT_H_
#define SYSTEMINIT_H_
#include "driverlib/gpio.h"

#define BUTTON_1 GPIO_PIN_4
#define BUTTON_2 GPIO_PIN_0
#define BUTTON_3 GPIO_PIN_1
#define BUTTON_4 GPIO_PIN_2
#define BUTTON_5 GPIO_PIN_5
#define ALL_BUTTONS (BUTTON_1|BUTTON_2|BUTTON_3|BUTTON_4|BUTTON_5)
#define TA0_PERIOD	10000

extern void PortFunctionInit(void);
extern void PWMInit(void);
extern void AnalogInputInit(void);
extern void TimerInit(void);

#endif /* SYSTEMINIT_H_ */
