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
#include <driverlib/ssi.h>
#include <driverlib/adc.h>
#include <inc/hw_memmap.h>

#include "oscillator.h"
#include "systeminit.h"

extern Osc mainOsc1;
extern Env volEnv;
extern Env pitchAmpEnv;
extern void setMainOscNote(uint16_t);
extern void triggerGate();
extern void releaseGate();

uint32_t buttons[] = { E_BUTTON_1, E_BUTTON_2, E_BUTTON_3, E_BUTTON_4,
		E_BUTTON_5 };
uint16_t buttonNotes[] = { 0, 2, 3, 7, 8 };
uint8_t currButtonStates;
uint8_t prevButtonStates = 0;
uint8_t activeButton = 0;

void initKnob(Knob* knob, float gain) {
	arm_float_to_q31(&gain,&knob->gain,1);
	knob->lastValue = 0;
	knob->currValue = 0;
	knob->shift		= 4;
}

void handleDigitalInputs() {
	currButtonStates = ~MAP_GPIOPinRead(GPIO_PORTE_BASE, ALL_E_BUTTONS);
	uint8_t buttonChanges = currButtonStates ^ prevButtonStates;
	uint8_t i;
	for (i = 0; i < NUM_BUTTONS; i++) {
		if ((buttonChanges & buttons[i]) && (currButtonStates & buttons[i])) {
			// if button was just pressed
			if (activeButton != (i + 1)) {
				triggerGate();
				setMainOscNote(buttonNotes[i]);
				activeButton = (i + 1);
			}
		}
	}
	/* If all buttons have been released, release envelopes */
	if (!(currButtonStates&ALL_E_BUTTONS)&(buttonChanges&ALL_E_BUTTONS)){
		releaseGate();
		activeButton = 0;
	}

	prevButtonStates = currButtonStates;
	return;
}

uint32_t expanderData;
void readFromExpansion(){
	SSIDataPut(SSI3_BASE,0x41);
	SSIDataPut(SSI3_BASE,0x09);
	while(SSIBusy(SSI3_BASE));
	SSIDataGet(SSI3_BASE,&expanderData);
	if(!(expanderData&0x01)){
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 1);
	}else{
		GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
	}
}

uint32_t currADCSeq0Values[NUM_KNOBS];
void handleAnalogInputs(Knob knobs[]) {
	MAP_ADCSequenceDataGet(ADC0_BASE, 0, currADCSeq0Values);
	///
	// Process analog samples
	uint8_t i = NUM_KNOBS;
	Knob* knob;
	int32_t diff;
	while (i--) {
		knob = (knobs + i);
		diff = ((int32_t) (currADCSeq0Values[i])) - knob->lastValue;
		knob->currValue = knob->lastValue + diff>>knob->shift;

		if (knob->currValue != knob->lastValue) {
			knob->output = knob->currValue<<(31-12+knob->shift);
			arm_mult_q31(&knob->output,&knob->gain,&knob->output,1);
		}

		knob->lastValue = knob->currValue;
	}
}
