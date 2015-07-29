/*
 * systeminit.c
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */
#include "systeminit.h"
#include <driverlib/adc.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/pwm.h>
#include <driverlib/rom_map.h>
#include <driverlib/ssi.h>
#include <driverlib/sysctl.h>
#include <driverlib/timer.h>
#include <driverlib/interrupt.h>
#include <inc/hw_memmap.h>
#include <inc/tm4c1294ncpdt.h>


//*****************************************************************************
void setupPWM() {
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
	MAP_PWMClockSet(PWM0_BASE, PWM_SYSCLK_DIV_1);    // 120MHz
	MAP_PWMGenConfigure(PWM0_BASE, PWM_GEN_2,
			PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_GEN_SYNC_LOCAL);
	MAP_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, PWMPERIOD);

	MAP_PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);
	MAP_PWMGenEnable(PWM0_BASE, PWM_GEN_2);
}

void setupSSI(void) {
	/*
	 * Configure pins for the TLC5615 DAC
	 * Peripheral: 	SSI3
	 * Pins: 		Pins Q0 (CLK), Q1 (CS), Q2 (Tx)
	 * Interrupts: 	N/A
	 *
	 */
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);

	MAP_GPIOPinConfigure(GPIO_PQ0_SSI3CLK); // Enable pin PQ0 for SSI3 SSI3CLK
	MAP_GPIOPinConfigure(GPIO_PQ1_SSI3FSS); // Enable pin PQ1 for SSI3 SSI3FSS
	MAP_GPIOPinConfigure(GPIO_PQ2_SSI3XDAT0); // Enable pin PQ2 for SSI3 SSI3XDAT0 (data in)
	MAP_GPIOPinTypeSSI(GPIO_PORTQ_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);
	/*
	 * Configure SSI module
	 */
	MAP_SSIConfigSetExpClk(SSI3_BASE, g_ui32SysClock, SSI_FRF_MOTO_MODE_0,
			SSI_MODE_MASTER, SSI_BAUDRATE, 12); // SPI freq = 1e6 Hz, 12 bits
	MAP_SSIEnable(SSI3_BASE);
}

void setupDigitalInputs(void) {
	/*
	 * Configure GPIO pins (no peripherals)
	 * Peripheral: 	N/A
	 * Pins: 		Pin E0, E1, E2, E4, E5
	 * Interrupts: 	N/A
	 *
	 */
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

	////
	// Enable GPIOInput pins for buttons
	MAP_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, ALL_E_BUTTONS);
	MAP_GPIOPadConfigSet(GPIO_PORTE_BASE, ALL_E_BUTTONS, GPIO_STRENGTH_2MA,
			GPIO_PIN_TYPE_STD_WPU);
	////
	// Enable output to the onboard LEDs
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
}

void setupAnalogInputs() {
	/*
	 * Configure ADC
	 * Peripheral: 	ADC0
	 * Pins: 		Pin K0, K1
	 * Interrupts: 	N/A
	 *
	 */
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	////
	// Enable K pins for ADC
	MAP_GPIOPinTypeADC(GPIO_PORTK_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	//ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PLL | ADC_CLOCK_RATE_FULL, 1);
	MAP_ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_TIMER, 0);
	MAP_ADCHardwareOversampleConfigure(ADC0_BASE, 8);
	/* Sample Sequence 1 */
	// sample from pin K0
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH16);
	// sample from pin K1
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH17 | ADC_CTL_IE | ADC_CTL_END);

	MAP_ADCSequenceEnable(ADC0_BASE, 1);
	MAP_ADCIntEnable(ADC0_BASE, 1);
	MAP_IntEnable(INT_ADC0SS1);
	MAP_ADCIntClear(ADC0_BASE, 1);
}

void setupTimers() {
	/*
	 * Configure Timers
	 * Peripheral: 	Timers 0A, 1A, 2A
	 * Pins: 		N/A
	 * Interrupts: 	Timer0AIntHandler, Timer1AIntHandler, Timer2AIntHandler (all TIMER_TIMA_TIMEOUT)
	 *
	 */
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	MAP_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	MAP_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
	MAP_TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
	MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, g_ui32SysClock / FS);
	MAP_TimerLoadSet(TIMER1_BASE, TIMER_A, g_ui32SysClock / MOD_FS);
	MAP_TimerLoadSet(TIMER2_BASE, TIMER_A, g_ui32SysClock / INPUT_FS);
	MAP_TimerControlTrigger(TIMER2_BASE, TIMER_A, true);

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

