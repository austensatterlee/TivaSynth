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
// Possible button status'
#define READY 0
#define INACTIVE 1
#define ACTIVE 2

uint32_t buttons[] = {BUTTON_1,BUTTON_2,BUTTON_3,BUTTON_4,BUTTON_5};
uint8_t buttonNotes[] = {12,24,27,29,31};
int osc2button[NUM_OSCILLATORS];
int button2osc[NUM_BUTTONS];
int buttonStatus[NUM_BUTTONS];
int currOsc;

void InputQueueInit(){
	uint8_t i;
	for (i=0;i<NUM_OSCILLATORS;i++){
		osc2button[i] = 0;
	}
	for (i=0;i<NUM_BUTTONS;i++){
		button2osc[i] = 0;
		buttonStatus[i] = READY;
	}
	currOsc = 0;
}

void assignNextOscToButton(uint8_t buttonNum){
	if (osc2button[currOsc]!=0){
		buttonStatus[osc2button[currOsc]-1]=INACTIVE;
		releaseOscillator(currOsc);
	}
	setNote(currOsc,buttonNotes[buttonNum]);
	osc2button[currOsc] = buttonNum+1;
	button2osc[buttonNum] = currOsc;
	buttonStatus[buttonNum] = ACTIVE;
	currOsc++;
	if (currOsc==NUM_OSCILLATORS){
		currOsc=0;
	}
}

void handleInputs(){
	uint8_t buttonStates = ~MAP_GPIOPinRead(GPIO_PORTE_BASE,ALL_BUTTONS);
	uint8_t i;
	for(i=0;i<NUM_BUTTONS;i++){
		if(buttonStates & buttons[i]){
			if(buttonStatus[i]==READY){
				// if button is pressed but not already assigned to an osc
				assignNextOscToButton(i);
			}
		}else if (buttonStatus[i]!=READY){
			// if button is not pressed
			if (buttonStatus[i]==ACTIVE){
				// if button has been released since it was last pressed
				releaseOscillator(button2osc[i]);
				osc2button[button2osc[i]]=0;
				currOsc = button2osc[i];
			}
			buttonStatus[i]=READY;
		}
	}
	return;
}
