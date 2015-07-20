/*
 * interrupts.c
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */
#include "interrupts.h"
uint32_t timcount = 0;
uint32_t pollPeriod = 1;
uint32_t sequencePeriod = 180;
uint16_t sequenceInd = 0;
float tickLengthSequence[] = {1,2,3,4,5,4,3,2};
void Timer0AIntHandler(void){
	MAP_TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
	if (!(timcount%sequencePeriod)){
		TickLength = tickLengthSequence[sequenceInd];
		sequenceInd+=1;
		if (sequenceInd>=8){
			sequenceInd=0;
		}
	}
	if (!(timcount%pollPeriod)){
		handleInputs();
	}
	timcount+=1;
	if (timcount==0xFFFFFFFE){
		timcount=0;
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
	uint8_t buttons = ~MAP_GPIOPinRead(GPIO_PORTE_BASE,GPIO_INT_PIN_0 | GPIO_INT_PIN_1);
	uint32_t *parameterAddr = &PwmPeriod;
	uint32_t max_value = 4096;
	uint32_t min_value = 128;
	uint32_t step = 1;
	if (buttons & GPIO_INT_PIN_0){
		if (*(parameterAddr)+step > max_value){
			*parameterAddr = min_value;
		}else{
			*parameterAddr += step;
		}
		PWMGenPeriodSet(PWM0_BASE,PWM_GEN_2,PwmPeriod);

	}else if(buttons & GPIO_INT_PIN_1){
		if (*(parameterAddr)-step <= min_value){
			*parameterAddr = max_value;
		}else{
			*parameterAddr-=step;
		}
		PWMGenPeriodSet(PWM0_BASE,PWM_GEN_2,PwmPeriod);
	}
	return;
}

void PortEIntHandler(void){

}
