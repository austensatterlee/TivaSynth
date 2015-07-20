/*
 * pwm.c
 *
 *  Created on: Jul 20, 2015
 *      Author: austen
 */
#include "pwm.h"
#include "systeminit.h"
#include "inc/tm4c1294ncpdt.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"

void initPWM(){
	/*
	 * Configure PWM
	 * Peripheral: 	PWM
	 * Pins: 		Pin G0 (out)
	 * Interrupts: 	PWMGen2IntHandler (PWM_INT_CNT_ZERO)
	 *
	 */
	// Init PWM parameters
	PwmPeriod = INIT_PWM_PERIOD;
	TickLength = INIT_TICK_LENGTH;
	TickPeriod = INIT_TICK_PERIOD;
	BaseTickStep = 1;
	TickStep_0 = 0;
	_fSysTick = 0;
	_ui32SysMiniTick = 0;
	// Configure hardware
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
	ROM_PWMClockSet(PWM0_BASE,PWM_SYSCLK_DIV_1);// 120MHz
	ROM_GPIOPinConfigure(GPIO_PG0_M0PWM4);
	ROM_GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_0);
	ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_GEN_SYNC_LOCAL);
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, PwmPeriod-1);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, 1);

	ROM_PWMIntEnable(PWM0_BASE,PWM_INT_GEN_2);
	ROM_PWMGenIntTrigEnable(PWM0_BASE,PWM_GEN_2,PWM_INT_CNT_ZERO);
	ROM_IntEnable(INT_PWM0_2);
	ROM_PWMSyncTimeBase(PWM0_BASE,PWM_GEN_2_BIT);

    ROM_PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);
	ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_2);
}


void PWMGen2IntHandler(void){
	ROM_PWMGenIntClear(PWM0_BASE, PWM_GEN_2, PWM_INT_CNT_ZERO);

	_ui32SysMiniTick++;
	if (_ui32SysMiniTick >= TickLength){
		_ui32SysMiniTick = 0;
		_fSysTick+=(BaseTickStep*TickStep_0);
		if (_fSysTick >= TickPeriod)
		{
			_fSysTick = 0;
		}
	}
}
