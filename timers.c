/*
 * timers.c
 *
 *  Created on: Jul 20, 2015
 *      Author: austen
 */
#include "timers.h"
#include "sequencer.h"
#include "inputs.h"
#include "systeminit.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c1294ncpdt.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/rom.h"

uint32_t timer0A_counter;

void initTimers(){
	/*
	 * Configure Timers
	 * Peripheral: 	Timer 0A
	 * Pins: 		N/A
	 * Interrupts: 	Timer0AIntHandler (TIMER_TIMA_TIMEOUT)
	 *
	 */
	// Init timer parameters
	timer0APeriod = INIT_TIMER0A_PERIOD;
	// Configure hardware
	ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, timer0APeriod);

	ROM_IntEnable(INT_TIMER0A);
	ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	ROM_TimerEnable(TIMER0_BASE, TIMER_A);

	timer0A_counter = 0;
}


void Timer0AIntHandler(void){
	ROM_TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
	if (!(timer0A_counter%sequencerPeriod)){
		pokeSequencer();
	}
	if (!(timer0A_counter%pollPeriod)){
		pollInputs();
	}
	timer0A_counter+=1;
	if (timer0A_counter==0xFFFFFFFE){
		timer0A_counter=0;
	}
}
