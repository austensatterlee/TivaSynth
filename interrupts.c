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
#include "inputs.h"
#include "inc/hw_memmap.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/rom_map.h"

uint32_t timerCount = 0;
uint32_t pollPeriod = 1;
void Timer0AIntHandler(void){
	MAP_TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
	timerCount+=1;
	if (timerCount>=pollPeriod){
		handleInputs();
		timerCount=0;
	}
}

void PWMGen2IntHandler(void){
	MAP_PWMGenIntClear(PWM0_BASE, PWM_GEN_2, PWM_INT_CNT_ZERO);
	tickOscillators();
}

void PortEIntHandler(void){

}
