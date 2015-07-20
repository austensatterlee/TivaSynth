/*
 * systeminit.h
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */

#ifndef SYSTEMINIT_H_
#define SYSTEMINIT_H_
#include "includes.h"

void setupClocks(void);
void setupAudioOutput(void);
void setupDigitalOutputs(void);
void setupDigitalInputs(void);
void setupAnalogInputs(void);
void setupTimers(void);

#endif /* SYSTEMINIT_H_ */
