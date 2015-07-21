#include <stdint.h>
#include <stdbool.h>
#include "systeminit.h"
#include "interrupts.h"
#include "oscillators.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
const int8_t WAVE128[] = {-1, 5, 12, 18, 24, 30, 36, 42, 48, 54, 60,
        65, 70, 75, 80, 85, 90, 94, 98, 102, 106,
        109, 112, 115, 117, 119, 121, 123, 124, 125, 126,
        126, 127, 126, 126, 125, 124, 122, 120, 118, 116,
        113, 110, 107, 104, 100, 96, 92, 87, 83, 78,
        73, 68, 62, 57, 51, 45, 39, 33, 27, 21,
        15, 8, 2, -4, -10, -17, -23, -29, -35, -41,
        -47, -53, -59, -64, -70, -75, -80, -85, -89, -94,
        -98, -102, -106, -109, -112, -115, -118, -120, -122, -124,
        -126, -127, -128, -128, -128, -128, -128, -127, -126, -125,
        -123, -121, -119, -117, -114, -111, -108, -104, -100, -96,
        -92, -87, -82, -77, -72, -67, -62, -56, -50, -44,
        -38, -32, -26, -20, -14, -7, -1};

int main(void)
{
	_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

	// Initialize variable system parameters
	gate=false;
	_ui32SysTick = 0;
	_ui32SysMiniTick = 0;
	PwmPeriod = INIT_PWMPERIOD;
	TickLength = INIT_TICKLENGTH;
	TickPeriod = INIT_TICKPERIOD;

	// Initialize system hardware & peripherals
	PortFunctionInit();
    TimerInit();
    AnalogInputInit();
    PWMInit();

	MAP_IntMasterEnable();

	 // Initialize system state variables
	while(1)
	{
		_nextSample  = (128+WAVE128[_ui32SysTick])/256.0*PwmPeriod*gate;
		while(PWMGenIntStatus(PWM0_BASE,PWM_GEN_2,true));
		MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, _nextSample*0.5);
	}
}
