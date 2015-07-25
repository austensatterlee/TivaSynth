/*
 * inputs.c
 *
 *  Created on: Jul 21, 2015
 *      Author: austen
 */
#include "inputs.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/rom_map.h"

uint32_t buttons[] = {E_BUTTON_1,E_BUTTON_2,E_BUTTON_3,E_BUTTON_4,E_BUTTON_5};
uint8_t buttonNotes[] = {0,5,7,9,10};
uint8_t prevButtonStates,currButtonStates;
uint8_t activeButton;

void InputQueueInit(){
	prevButtonStates = 0;
	activeButton = 0;
}

void triggerGate(uint8_t buttonNum){
	triggerEnvelope(&ampEnv);
	triggerEnvelope(&ampEnvLFO);
}

void handleDigitalInputs(){
	currButtonStates = ~MAP_GPIOPinRead(GPIO_PORTE_BASE,ALL_E_BUTTONS);
	uint8_t buttonChanges = currButtonStates^prevButtonStates;
	uint8_t i;
	for(i=0;i<NUM_BUTTONS;i++){
		if((buttonChanges & buttons[i]) && (currButtonStates & buttons[i])){
			// if button was just pressed
			if(activeButton!=(i+1)){
				if (!activeButton){
					triggerGate(i);
				}
				setMainOscNote(buttonNotes[i]);
				activeButton=(i+1);
			}
		}else if ((buttonChanges & buttons[i]) && !(currButtonStates & buttons[i])){
			// if button was just released
			if (activeButton==(i+1)){
				releaseMainOsc();
				activeButton=0;
			}
		}
	}

	prevButtonStates = currButtonStates;
	return;
}

uint32_t currADCSeq1Values[NUM_KNOBS];
uint32_t prevADCSeq1Values[NUM_KNOBS] = {{0}};
void handleAnalogInputs(){
    ////
    // Trigger the ADC conversion.
	MAP_ADCProcessorTrigger(ADC0_BASE, 1);
    while(!MAP_ADCIntStatus(ADC0_BASE, 1, false))
    {
    }
    MAP_ADCIntClear(ADC0_BASE, 1);
    MAP_ADCSequenceDataGet(ADC0_BASE, 1, currADCSeq1Values);
    ///
    // Process analog samples
    uint8_t i = NUM_KNOBS;
    Knob* knob;
    while(i--){
    	knob=(knobs+i);
    	knob->value = prevADCSeq1Values[i]+(currADCSeq1Values[i]-prevADCSeq1Values[i])>>2;
    	if(knob->value!=prevADCSeq1Values[i]){
    		knob->send_fn(&knob->value);
    	}
    	prevADCSeq1Values[i] = knob->value;
    }
}
