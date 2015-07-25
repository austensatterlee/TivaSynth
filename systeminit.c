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
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pwm.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"

//*****************************************************************************
void PWMInit(){
	/*
	 * Configure PWM
	 * Peripheral: 	PWM
	 * Pins: 		Pin G0 (out)
	 * Interrupts: 	PWMGen2IntHandler (PWM_INT_CNT_ZERO)
	 *
	 */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	MAP_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
    ////
    // Enable pin PG0 for PWM0 M0PWM4
    MAP_GPIOPinConfigure(GPIO_PG0_M0PWM4);
    MAP_GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_0);
    ////
    // Configure PWM waveform and frequency
	MAP_PWMClockSet(PWM0_BASE,PWM_SYSCLK_DIV_1);// 120MHz
	MAP_PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_GEN_SYNC_LOCAL);
	MAP_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, INIT_PWMPERIOD);
	////
	// Enable PWM interrupt
	MAP_PWMIntEnable(PWM0_BASE,PWM_INT_GEN_2);
	MAP_PWMGenIntTrigEnable(PWM0_BASE,PWM_GEN_2,PWM_INT_CNT_LOAD);
	MAP_IntEnable(INT_PWM0_2);

    MAP_PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);
	MAP_PWMGenEnable(PWM0_BASE, PWM_GEN_2);
}

void DigitalInputInit(void)
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    ////
    // Enable pin PE0,PE1,PE2,PE3 for GPIOInput
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, ALL_E_BUTTONS);
    MAP_GPIOPadConfigSet(GPIO_PORTE_BASE, ALL_E_BUTTONS, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    ////
    // Enable output to the onboard LEDs
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
}

void AnalogInputInit(){
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	////
	// Enable K pins for ADC
	MAP_GPIOPinTypeADC(GPIO_PORTK_BASE, GPIO_PIN_0);
	MAP_ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
	/* Sample Sequence 1 */
	// sample from pin K0
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 1, 0,
    		ADC_CTL_CH16);
	// sample from pin K1
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 1, 1,
			ADC_CTL_CH17 | ADC_CTL_IE | ADC_CTL_END);
	MAP_ADCSequenceEnable(ADC0_BASE, 1);
	MAP_ADCIntClear(ADC0_BASE, 1);
}

void TimerInit(){
	/*
	 * Configure Timers
	 * Peripheral: 	Timer 0A
	 * Pins: 		N/A
	 * Interrupts: 	Timer0AIntHandler (TIMER_TIMA_TIMEOUT), Timer
	 *
	 */
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	MAP_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	MAP_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
	MAP_TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
	MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, 0x2ee00);
	MAP_TimerLoadSet(TIMER1_BASE, TIMER_A, 1500);
	MAP_TimerLoadSet(TIMER2_BASE, TIMER_A, 15000);

	MAP_IntEnable(INT_TIMER0A);
	MAP_IntEnable(INT_TIMER1A);
	MAP_IntEnable(INT_TIMER2A);
	MAP_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	MAP_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	MAP_TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

	MAP_TimerEnable(TIMER0_BASE, TIMER_A);
	MAP_TimerEnable(TIMER1_BASE, TIMER_A);
	MAP_TimerEnable(TIMER2_BASE, TIMER_A);
}


