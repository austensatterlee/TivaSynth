/*
 * inputs.c
 *
 *  Created on: Jul 21, 2015
 *      Author: austen
 */
#include "input.h"
#include "systeminit.h"

#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/rom_map.h"



void triggerGate(){
	triggerEnvelope(&ampEnv);
	triggerEnvelope(&ampEnvLFO);
}

void releaseGate(){
	releaseEnvelope(&ampEnv);
	releaseEnvelope(&ampEnvLFO);
}

uint32_t buttons[] = {E_BUTTON_1,E_BUTTON_2,E_BUTTON_3,E_BUTTON_4,E_BUTTON_5};
uint16_t buttonNotes[] = {0,5,7,9,10};
uint8_t currButtonStates;
uint8_t prevButtonStates = 0;
uint8_t activeButton = 0;

void handleDigitalInputs(){
	currButtonStates = ~MAP_GPIOPinRead(GPIO_PORTE_BASE,ALL_E_BUTTONS);
	uint8_t buttonChanges = currButtonStates^prevButtonStates;
	uint8_t i;
	for(i=0;i<NUM_BUTTONS;i++){
		if((buttonChanges & buttons[i]) && (currButtonStates & buttons[i])){
			// if button was just pressed
			if(activeButton!=(i+1)){
				if (!activeButton){
					triggerGate();
				}
				setOscFreq(&mainOsc,buttonNotes[i]);
				activeButton=(i+1);
			}
		}else if ((buttonChanges & buttons[i]) && !(currButtonStates & buttons[i])){
			// if button was just released
			if (activeButton==(i+1)){
				releaseGate();
				activeButton=0;
			}
		}
	}

	prevButtonStates = currButtonStates;
	return;
}

uint32_t currADCSeq1Values[NUM_KNOBS];
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
    int32_t diff;
    while(i--){
    	knob=(knobs+i);
    	currADCSeq1Values[i]>>=2;
    	diff = ((int32_t)(currADCSeq1Values[i])-knob->lastValue)>>4;
    	knob->currValue = knob->lastValue+diff;
		if(knob->currValue != knob->lastValue){
			knob->send_fn(knob->send_target,knob->currValue,knob->out_port);
		}
    	knob->lastValue = knob->currValue;
    }
}
