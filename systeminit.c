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
	 * Configure PWM for audio output
	 *
	 * Peripheral: 	PWM
	 * Pins: 		Pin G0,G1 (out)
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
	MAP_GPIOPinConfigure(GPIO_PG1_M0PWM5);
	MAP_GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_1);
	////
	// Configure PWM waveform and frequency
	MAP_PWMClockSet(PWM0_BASE, PWM_SYSCLK_DIV_1);    // 120MHz
	MAP_PWMGenConfigure(PWM0_BASE, PWM_GEN_2,
			PWM_GEN_MODE_UP_DOWN);// | PWM_GEN_MODE_GEN_SYNC_LOCAL);
	MAP_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, PWM_PERIOD);

	MAP_PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);
	MAP_PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, true);
	MAP_PWMGenEnable(PWM0_BASE, PWM_GEN_2);
}

void setupDAC(void) {
	/*
	 * Configure pins for the TLC5615 DAC
	 *
	 * Peripheral: 	SSI3
	 * Pins: 		Pins Q0 (CLK), Q1 (CS), Q2 (Tx)
	 * Interrupts: 	N/A
	 *
	 */
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);

	MAP_GPIOPinConfigure(GPIO_PQ0_SSI3CLK);
	MAP_GPIOPinConfigure(GPIO_PQ1_SSI3FSS);
	MAP_GPIOPinConfigure(GPIO_PQ2_SSI3XDAT0);
	MAP_GPIOPinTypeSSI(GPIO_PORTQ_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);
	/*
	 * Configure SSI module
	 */
	MAP_SSIConfigSetExpClk(SSI3_BASE, g_ui32SysClock, SSI_FRF_MOTO_MODE_0,
			SSI_MODE_MASTER, SSI_BAUDRATE, 8); // SPI freq = 1e6 Hz, 12 bits
	MAP_SSIEnable(SSI3_BASE);
}

void setupIOExpander(void) {
	/*
	 * Configure pins for the MCP23S08 I/O expander
	 *
	 * Peripheral: 	SSI3
	 * Pins: 		Pins Q0 (CLK), Q1 (GPIO [CS]), Q2 (Tx), Q3 (Rx)
	 * Interrupts: 	N/A
	 *
	 */
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
	SysCtlDelay(10000);
	MAP_GPIOPinConfigure(GPIO_PQ0_SSI3CLK);
	MAP_GPIOPinConfigure(GPIO_PQ1_SSI3FSS);
	MAP_GPIOPinConfigure(GPIO_PQ2_SSI3XDAT0);
	MAP_GPIOPinConfigure(GPIO_PQ3_SSI3XDAT1);
	MAP_GPIOPinTypeSSI(GPIO_PORTQ_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

	/*
	 * Configure SSI module
	 */
	SSIAdvModeSet(SSI3_BASE,SSI_ADV_MODE_READ_WRITE);
	SSIAdvFrameHoldEnable(SSI3_BASE);
	MAP_SSIConfigSetExpClk(SSI3_BASE, g_ui32SysClock, SSI_FRF_MOTO_MODE_0,
			SSI_MODE_MASTER, SSI_BAUDRATE, 8); // SPI freq = 1e6 Hz, 12 bits
	MAP_SSIEnable(SSI3_BASE);

	uint32_t debug_buf1 = 0;
	uint32_t debug_buf2 = 0;
	uint32_t count = 0;
	//SSIDataPut(SSI3_BASE,0x40);
	//SSIDataPut(SSI3_BASE,0x05);
	//SSIAdvDataPutFrameEnd(SSI3_BASE,0x30);
	//SSIDataPut(SSI3_BASE,0x40);
	//SSIDataPut(SSI3_BASE,0x06);
	//SSIAdvDataPutFrameEnd(SSI3_BASE,0xFF);
	while(debug_buf2!=0x02){
		SSIDataPut(SSI3_BASE,0x41);
		SSIDataPut(SSI3_BASE,0x00);
		SSIDataGet(SSI3_BASE,&debug_buf2);
		count++;
	}
	SSIDataGet(SSI3_BASE,&debug_buf1);
}

void setupDigitalInputs(void) {
	/*
	 * Configure GPIO pins (no peripherals)
	 *
	 * Peripheral: 	N/A
	 * Pins: 		Pin E0, E1, E2, E4, E5
	 * Interrupts: 	N/A
	 *
	 */
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	////
	// Enable GPIOInput pins for buttons
	MAP_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, ALL_E_BUTTONS);
	MAP_GPIOPadConfigSet(GPIO_PORTE_BASE, ALL_E_BUTTONS, GPIO_STRENGTH_2MA,
			GPIO_PIN_TYPE_STD_WPU);
}

void setupAnalogInputs() {
	/*
	 * Configure ADC
	 *
	 * Peripheral: 	ADC0
	 * Pins: 		Pin K0, K1
	 * Interrupts: 	N/A
	 *
	 */
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	////
	// Enable K pins for ADC
	MAP_GPIOPinTypeADC(GPIO_PORTK_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	MAP_GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_1);
	MAP_ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_TIMER, 0);
	MAP_ADCHardwareOversampleConfigure(ADC0_BASE, 4);
	/* Sample Sequence 1 */
	// sample from pin K0
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH16);
	// sample from pin K1
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH17);
	// sample from pin K2
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH18);
	// sample from pin K3
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH19);
	// sample from pin D1
	MAP_ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_CH14 | ADC_CTL_IE | ADC_CTL_END);

	MAP_ADCSequenceEnable(ADC0_BASE, 0);
	MAP_ADCIntEnable(ADC0_BASE, 0);
	MAP_IntEnable(INT_ADC0SS0);
	MAP_ADCIntClear(ADC0_BASE, 0);
}

void setupTimers() {
	/*
	 * Configure Timers
	 *
	 * Peripheral: 	Timers 0A, 2A
	 * Pins: 		N/A
	 * Interrupts: 	Timer0AIntHandler, Timer2AIntHandler (all TIMER_TIMA_TIMEOUT)
	 *
	 */
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	MAP_TimerClockSourceSet(TIMER0_BASE,TIMER_CLOCK_SYSTEM);
	MAP_TimerClockSourceSet(TIMER2_BASE,TIMER_CLOCK_SYSTEM);
	MAP_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	MAP_TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
	MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, g_ui32SysClock / FS);
	MAP_TimerLoadSet(TIMER2_BASE, TIMER_A, g_ui32SysClock / INPUT_FS);
	MAP_TimerControlTrigger(TIMER2_BASE, TIMER_A, true);

	MAP_IntEnable(INT_TIMER0A);
	MAP_IntEnable(INT_TIMER2A);
	MAP_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	MAP_TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

	MAP_TimerEnable(TIMER0_BASE, TIMER_A);
	MAP_TimerEnable(TIMER2_BASE, TIMER_A);
}

