/*
 * inputs.c
 *
 *  Created on: Jul 21, 2015
 *      Author: austen
 */

#include "input.h"

#include <stdbool.h>
#include <driverlib/rom_map.h>
#include <driverlib/gpio.h>
#include <driverlib/adc.h>
#include <inc/hw_memmap.h>

#include "oscillator.h"
#include "systeminit.h"

extern Osc mainOsc1;
extern Env volEnv;
extern Env pitchAmpEnv;
extern void setMainOscNote(uint16_t);

void triggerGate() {
	/*
	 * Trigger envelopes here
	 */
	triggerEnv(&volEnv);
	triggerEnv(&pitchAmpEnv);
	return;
}

void releaseGate() {
	/*
	 * Release envelopes here
	 */
	releaseEnv(&volEnv);
	releaseEnv(&pitchAmpEnv);
	return;
}

uint32_t buttons[] = { E_BUTTON_1, E_BUTTON_2, E_BUTTON_3, E_BUTTON_4,
		E_BUTTON_5 };
uint16_t buttonNotes[] = { 12, 15, 17, 19, 20 };
uint8_t currButtonStates;
uint8_t prevButtonStates = 0;
uint8_t activeButton = 0;

void initKnob(Knob* knob, void* target, uint8_t port, float gain) {
	knob->out_port = port;
	knob->send_target = target;
	knob->gain = gain;
	knob->lastValue = 0;
	knob->currValue = 0;
}

void handleDigitalInputs() {
	currButtonStates = ~MAP_GPIOPinRead(GPIO_PORTE_BASE, ALL_E_BUTTONS);
	uint8_t buttonChanges = currButtonStates ^ prevButtonStates;
	uint8_t i;
	for (i = 0; i < NUM_BUTTONS; i++) {
		if ((buttonChanges & buttons[i]) && (currButtonStates & buttons[i])) {
			// if button was just pressed
			if (activeButton != (i + 1)) {
				if (!activeButton) {
					triggerGate();
				}
				setMainOscNote(buttonNotes[i]);
				activeButton = (i + 1);
			}
		} else if ((buttonChanges & buttons[i])
				&& !(currButtonStates & buttons[i])) {
			// if button was just released
			if (activeButton == (i + 1)) {
				releaseGate();
				activeButton = 0;
			}
		}
	}

	prevButtonStates = currButtonStates;
	return;
}

uint32_t currADCSeq1Values[NUM_KNOBS];
void handleAnalogInputs(Knob knobs[]) {
	////
	// Trigger the ADC conversion.
	MAP_ADCSequenceDataGet(ADC0_BASE, 1, currADCSeq1Values);
	///
	// Process analog samples
	uint8_t i = NUM_KNOBS;
	Knob* knob;
	int32_t diff;
	while (i--) {
		knob = (knobs + i);
		diff = ((int32_t) currADCSeq1Values[i]) - knob->lastValue;
		knob->currValue = knob->lastValue + diff>>2;
		if (knob->currValue != knob->lastValue && knob->send_fn) {
			knob->send_fn(knob->send_target,
					((float) knob->currValue)* knob->gain / 1080.0 ,
					knob->out_port);
		}
		knob->lastValue = knob->currValue;
	}
}
