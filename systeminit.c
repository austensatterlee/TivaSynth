/*
 * systeminit.c
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */
#include "systeminit.h"
#include "pwm.h"
#include "timers.h"
#include "inputs.h"
#include "sequencer.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"

void initSystem(){
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG); // PWM output pin
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); // button inputs
	// Configure peripherals
	initPWM();
	initTimers();
	initDigitalInputs();
	initSequencer();
}
