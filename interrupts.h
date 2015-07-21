/*
 * interrupts.h
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_
#include <stdbool.h>

void Timer0AIntHandler(void);
void PWMGen2IntHandler(void);
void PortEIntHandler(void);

void handleInputs(void);

#endif /* INTERRUPTS_H_ */
