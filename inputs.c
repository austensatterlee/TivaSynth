/*
 * inputs.c
 *
 *  Created on: Jul 20, 2015
 *      Author: austen
 */

#include "inputs.h"
#include "pwm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/rom.h"

void initDigitalInputs(){
	/*
	 * Configure buttons
	 * Peripheral: 	GPIO
	 * Pins: 		Pin E0 (in,pullup), E1 (in,pullup)
	 * Interrupts:
	 *
	 */
	// Init input parameters
	pollPeriod = INIT_POLL_PERIOD;
	// Configure hardware
	ROM_GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1,
				     GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	//ROM_GPIOIntTypeSet(GPIO_PORTE_BASE,GPIO_PIN_0 | GPIO_PIN_1, GPIO_LOW_LEVEL);
	//ROM_IntEnable(INT_GPIOE);
	//ROM_GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
}

void initAnalogInputs(){

}

void pollInputs(){
	uint8_t buttons = ~ROM_GPIOPinRead(GPIO_PORTE_BASE,GPIO_INT_PIN_0 | GPIO_INT_PIN_1);
	float *parameterAddr = &BaseTickStep;
	uint32_t max_value = 2;
	uint32_t min_value = 1;
	float step = .005;
	if (buttons & GPIO_INT_PIN_0){
		if (*(parameterAddr)+step > max_value){
			*parameterAddr = min_value;
		}else{
			*parameterAddr += step;
		}

	}else if(buttons & GPIO_INT_PIN_1){
		if (*(parameterAddr)-step <= min_value){
			*parameterAddr = max_value;
		}else{
			*parameterAddr-=step;
		}
	}
	return;
}
