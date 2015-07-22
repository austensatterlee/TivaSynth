#include <stdint.h>
#include <stdbool.h>
#include "systeminit.h"
#include "interrupts.h"
#include "oscillators.h"
#include "inputs.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"

int main(void)
{
	_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

	// Initialize system hardware & peripherals
	PortFunctionInit();
    TimerInit();
    AnalogInputInit();
    PWMInit();
    OscillatorsInit();
    InputQueueInit();

	MAP_IntMasterEnable();

	 // Initialize system state variables
	while(1)
	{
		while(MAP_PWMGenIntStatus(PWM0_BASE,PWM_GEN_2,true));
		MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, getNextSample());
	}
}
