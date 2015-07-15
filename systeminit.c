/*
 * systeminit.c
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */
#include "systeminit.h"

uint32_t initSystem(){
	uint32_t sysClkFreq;
	sysClkFreq = setupClocks();
	setupAudioOutput();
	setupTimers();
	return sysClkFreq;
}

uint32_t setupClocks(){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG); // PWM output pin
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	PWMClockSet(PWM0_BASE,PWM_SYSCLK_DIV_1);// 120MHz
	return sysClkFreq;
}

void setupAudioOutput(){
	/*
	 * Configure PWM
	 */
	GPIOPinConfigure(GPIO_PG0_M0PWM4);
	GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_0);
	PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN );
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, PWM_PERIOD-1);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, 0);
	PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);

	IntEnable(INT_PWM0_2);
	PWMIntEnable(PWM0_BASE,PWM_INT_GEN_2);
	PWMGenIntTrigEnable(PWM0_BASE,PWM_GEN_2,PWM_INT_CNT_ZERO);

	PWMGenEnable(PWM0_BASE, PWM_GEN_2);
}

void setupDigitalInputs(){

}

void setupAnalogInputs(){

}

void setupTimers(){
	/*
	 * Configure Timer 0A
	 */
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER0_BASE, TIMER_A, TICK_PERIOD);

	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();

	TimerEnable(TIMER0_BASE, TIMER_A);
}


