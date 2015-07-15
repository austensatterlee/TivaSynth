/*
 * interrupts.c
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */
#include "interrupts.h"

void Timer0AIntHandler(void){
	TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
}

void PWMGen2IntHandler(void){
    PWMGenIntClear(PWM0_BASE, PWM_GEN_0, PWM_INT_CNT_ZERO);
	_ui32SysTick++;
	if (_ui32SysTick == TICK_PERIOD-1)
	{
		_ui32SysTick = 0;
	}
}
