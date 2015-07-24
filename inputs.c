/*
 * inputs.c
 *
 *  Created on: Jul 21, 2015
 *      Author: austen
 */
#include "inputs.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/rom_map.h"

uint32_t buttons[] = {E_BUTTON_1,E_BUTTON_2,E_BUTTON_3,E_BUTTON_4,E_BUTTON_5};
uint8_t buttonNotes[] = {0,3,2,5,9};
uint8_t prevButtonStates;
uint8_t activeButton;

void InputQueueInit(){
	prevButtonStates = 0;
	activeButton = 0;
}

void triggerGate(uint8_t buttonNum){
	setMainOscNote(buttonNotes[buttonNum]);
}

void handleInputs(){
	uint8_t buttonStates = ~MAP_GPIOPinRead(GPIO_PORTE_BASE,ALL_E_BUTTONS);
	uint8_t buttonChanges = buttonStates^prevButtonStates;
	uint8_t i;
	for(i=0;i<NUM_E_BUTTONS;i++){
		if((buttonChanges & buttons[i]) && (buttonStates & buttons[i])){
			// if button was just pressed
			if(activeButton!=(i+1)){
				triggerGate(i);
				activeButton=(i+1);
			}
		}else if ((buttonChanges & buttons[i]) && !(buttonStates & buttons[i])){
			// if button was just released
			if (activeButton==(i+1)){
				releaseMainOsc();
				activeButton=0;
			}
		}
	}

	prevButtonStates = buttonStates;
	return;
}
