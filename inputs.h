/*
 * inputs.h
 *
 *  Created on: Jul 20, 2015
 *      Author: austen
 */

#ifndef INPUTS_H_
#define INPUTS_H_
#include <stdint.h>
#include <stdbool.h>
void initDigitalInputs(void);
void initAnalogInputs(void);
void pollInputs(void);

#define INIT_POLL_PERIOD 60
uint32_t pollPeriod;

#endif /* INPUTS_H_ */
