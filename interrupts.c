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
#include "envelope.h"
#include "inc/hw_memmap.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/rom_map.h"

#include "input.h"
#include "oscillator.h"
/*
 * Sample ouput interrupt.
 *
 * This timer is clocked at the sample frequency.
 *
 */
void Timer0AIntHandler(void){
	MAP_TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
	incrementOscPhase(&mainOsc);
	g_sampleCount++;
	if (g_sampleCount==0xFFFFFFFF){
		g_sampleCount = 0;
	}
	system_flags.outputNextSample = 1;
}


/*
 * Modulation interrupt.
 *
 */
uint8_t ui8PortNLEDStates 	= GPIO_PIN_0;
void Timer1AIntHandler(void){
	MAP_TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
	system_flags.modulate = 1;
}

/*
 * Input interrupt.
 *
 * Read and update values from hardware input peripherals.
 */
void Timer2AIntHandler(void){
	MAP_TimerIntClear(TIMER2_BASE,TIMER_TIMA_TIMEOUT);
	system_flags.readInputs = 1;

	ui8PortNLEDStates ^= (GPIO_PIN_0|GPIO_PIN_1);
	GPIOPinWrite(GPIO_PORTN_BASE,GPIO_PIN_0|GPIO_PIN_1,ui8PortNLEDStates);
}
