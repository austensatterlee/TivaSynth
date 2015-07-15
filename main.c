#include "includes.h"
#include "systeminit.h"
#include "interrupts.h"

int main(void)
{
	_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);
	initSystem();
	uint32_t waveformAmplitude;
	while(1)
	{
		waveformAmplitude  = (_ui32SysTick/TICK_PERIOD+1)*PWM_PERIOD/2;
		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, waveformAmplitude);
		while(!TimerIntStatus(TIMER0_BASE,0)&TIMER_TIMA_TIMEOUT);
	}
}
