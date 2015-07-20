/*
 * systeminit.c
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */
#include "systeminit.h"

void setupClocks(){
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG); // PWM output pin
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); // button inputs
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
}

void setupAudioOutput(){
	/*
	 * Configure PWM
	 * Peripheral: 	PWM
	 * Pins: 		Pin G0 (out)
	 * Interrupts: 	PWMGen2IntHandler (PWM_INT_CNT_ZERO)
	 *
	 */
	MAP_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
	MAP_PWMClockSet(PWM0_BASE,PWM_SYSCLK_DIV_1);// 120MHz
	MAP_GPIOPinConfigure(GPIO_PG0_M0PWM4);
	MAP_GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_0);
	MAP_PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_GEN_SYNC_LOCAL);
	MAP_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, PwmPeriod-1);
	MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, 1);

	MAP_PWMIntEnable(PWM0_BASE,PWM_INT_GEN_2);
	MAP_PWMGenIntTrigEnable(PWM0_BASE,PWM_GEN_2,PWM_INT_CNT_ZERO);
	MAP_IntEnable(INT_PWM0_2);
	MAP_PWMSyncTimeBase(PWM0_BASE,PWM_GEN_2_BIT);

    MAP_PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);
	MAP_PWMGenEnable(PWM0_BASE, PWM_GEN_2);
}

void setupDigitalOutputs(){
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_1);
	MAP_GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0);
}

void setupDigitalInputs(){
	/*
	 * Configure buttons
	 * Peripheral: 	GPIO
	 * Pins: 		Pin E0 (in,pullup), E1 (in,pullup)
	 * Interrupts:
	 *
	 */
	MAP_GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_DIR_MODE_IN);
	MAP_GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1,
						 GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	//MAP_GPIOIntTypeSet(GPIO_PORTE_BASE,GPIO_PIN_0 | GPIO_PIN_1, GPIO_LOW_LEVEL);
	//MAP_IntEnable(INT_GPIOE);
	//MAP_GPIOIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
}

void setupAnalogInputs(){

}

void setupTimers(){
	/*
	 * Configure Timers
	 * Peripheral: 	Timer 0A
	 * Pins: 		N/A
	 * Interrupts: 	Timer0AIntHandler (TIMER_TIMA_TIMEOUT)
	 *
	 */
	MAP_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, TA0_PERIOD);

	MAP_IntEnable(INT_TIMER0A);
	MAP_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	MAP_IntMasterEnable();

	MAP_TimerEnable(TIMER0_BASE, TIMER_A);
}


