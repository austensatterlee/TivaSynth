#include "main.h"
#include "systeminit.h"
#include "pwm.h"
#include "wavetables.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"

int main(void)
{
	_ui32SysClock = ROM_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

	// Initialize system hardware & peripherals
	initSystem();
	ROM_IntMasterEnable();

	while(1)
	{
		_nextSample  = (128+WAVE128[(uint32_t)(_fSysTick)%128])/256.0*PwmPeriod;
		while(ROM_PWMGenIntStatus(PWM0_BASE,PWM_GEN_2,true));
		ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, _nextSample);
	}
}
