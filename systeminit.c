/*
 * systeminit.c
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */
#include <stdint.h>
#include <stdbool.h>
#include "systeminit.h"
#include "oscillators.h"
#include "inputs.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/tm4c1294ncpdt.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pwm.h"
#include "driverlib/interrupt.h"

//*****************************************************************************
void
PortFunctionInit(void)
{
    ////
    // Enable Peripheral Clocks
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    ////
    // Enable pin PE0,PE1,PE2,PE3 for GPIOInput
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, ALL_BUTTONS);
    MAP_GPIOPadConfigSet(GPIO_PORTE_BASE, ALL_BUTTONS, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    ////
    // Enable pin PG0 for PWM0 M0PWM4
    MAP_GPIOPinConfigure(GPIO_PG0_M0PWM4);
    MAP_GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_0);
    // Enable pin PG1 for PWM0 M0PWM5
    MAP_GPIOPinConfigure(GPIO_PG1_M0PWM5);
    MAP_GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_1);
}

void PWMInit(){
	/*
	 * Configure PWM
	 * Peripheral: 	PWM
	 * Pins: 		Pin G0 (out)
	 * Interrupts: 	PWMGen2IntHandler (PWM_INT_CNT_ZERO)
	 *
	 */
	MAP_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
	MAP_PWMClockSet(PWM0_BASE,PWM_SYSCLK_DIV_1);// 120MHz
	MAP_PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_GEN_SYNC_LOCAL);
	MAP_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, INIT_PWMPERIOD);
	MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, 1);

	MAP_PWMIntEnable(PWM0_BASE,PWM_INT_GEN_2);
	MAP_PWMGenIntTrigEnable(PWM0_BASE,PWM_GEN_2,PWM_INT_CNT_ZERO);
	MAP_IntEnable(INT_PWM0_2);
	MAP_PWMSyncTimeBase(PWM0_BASE,PWM_GEN_2_BIT);

    MAP_PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);
	MAP_PWMGenEnable(PWM0_BASE, PWM_GEN_2);
}

void AnalogInputInit(){

}

void TimerInit(){
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


