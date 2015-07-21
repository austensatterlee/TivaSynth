/*
 * interrupts.c
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */
#include <stdint.h>
#include <stdbool.h>
#include "interrupts.h"
#include "systeminit.h"
#include "oscillators.h"
#include "inc/hw_memmap.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/rom_map.h"

uint32_t timerCount = 0;
uint32_t pollPeriod = 1;
uint32_t sequencePeriod = 180;
uint16_t sequenceInd = 0;
float tickLengthSequence[] = {1,2,3,4,5,4,3,2};
void Timer0AIntHandler(void){
	MAP_TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
	if (!(timerCount%sequencePeriod)){
		TickLength = tickLengthSequence[sequenceInd];
		sequenceInd+=1;
		if (sequenceInd>=1){
			sequenceInd=0;
		}
	}
	if (!(timerCount%pollPeriod)){
		handleInputs();
	}
	timerCount+=1;
	if (timerCount==0xFFFFFFFE){
		timerCount=0;
	}
}

void PWMGen2IntHandler(void){
	MAP_PWMGenIntClear(PWM0_BASE, PWM_GEN_2, PWM_INT_CNT_ZERO);
	_ui32SysMiniTick++;
	if (_ui32SysMiniTick >= TickLength){
		_ui32SysMiniTick = 0;
		_ui32SysTick++;
		if (_ui32SysTick >= TickPeriod)
		{
			_ui32SysTick = 0;
		}
	}
}

void handleInputs(){
	uint8_t buttons = ~MAP_GPIOPinRead(GPIO_PORTE_BASE,ALL_BUTTONS);
	bool anyButtonOn = false;
	if (buttons & BUTTON_1){
		setPWMFrequencyIndex(0);
		anyButtonOn = true;
	}else if(buttons & BUTTON_2){
		setPWMFrequencyIndex(4);
		anyButtonOn = true;
	}else if(buttons & BUTTON_3){
		setPWMFrequencyIndex(5);
		anyButtonOn = true;
	}else if(buttons & BUTTON_4){
		setPWMFrequencyIndex(7);
		anyButtonOn = true;
	}else if(buttons & BUTTON_5){
		setPWMFrequencyIndex(12);
		anyButtonOn = true;
	}
	gate=anyButtonOn;
	return;
}

void PortEIntHandler(void){

}
