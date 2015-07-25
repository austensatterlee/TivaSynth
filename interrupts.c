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
#include "envelope.h"
#include "inputs.h"
#include "inc/hw_memmap.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/rom_map.h"

uint32_t timer0ACount 		= 0;
uint32_t pollPeriod 		= 1;
uint16_t ledPeriod			= 1000;
uint8_t ui8PortNLEDStates 	= GPIO_PIN_0;
void Timer0AIntHandler(void){
	MAP_TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
	timer0ACount++;
	if (timer0ACount>=pollPeriod){
		handleDigitalInputs();
		handleAnalogInputs();
		timer0ACount=0;
	}
	if (timer0ACount>=ledPeriod){
		ui8PortNLEDStates ^= (GPIO_PIN_0|GPIO_PIN_1);
		GPIOPinWrite(GPIO_PORTN_BASE,GPIO_PIN_0|GPIO_PIN_1,ui8PortNLEDStates);
	}
}

uint32_t timer1ACounts[] = {0};
void Timer1AIntHandler(void){
	MAP_TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
	timer1ACounts[0]++;
	if (timer1ACounts[0]>=mainOsc.timerLoad){
		tickOscillator(&mainOsc);
		timer1ACounts[0]=0;
	}
}

uint32_t timer2ACounts[] = {0,0,0};
void Timer2AIntHandler(void){
	MAP_TimerIntClear(TIMER2_BASE,TIMER_TIMA_TIMEOUT);
	timer2ACounts[0]++;
	if (timer2ACounts[0]>=(pitchLFO.timerLoad)){
		tickOscillator(&pitchLFO);
		timer2ACounts[0]=0;
	}
	timer2ACounts[1]++;
	if (timer2ACounts[1]>=*ampEnvLFO.currTimerLoad){
		tickEnvelope(&ampEnvLFO);
		timer2ACounts[1]=0;
	}
	timer2ACounts[2]++;
	if (timer2ACounts[2]>=*ampEnv.currTimerLoad){
		tickEnvelope(&ampEnv);
		timer2ACounts[2]=0;
	}
}

void PWMGen2IntHandler(void){
	MAP_PWMGenIntClear(PWM0_BASE, PWM_GEN_2, PWM_INT_CNT_LOAD);
}

void PortEIntHandler(void){

}
