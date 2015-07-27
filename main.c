#include <stdbool.h>
#include <stdint.h>
#include <driverlib/ssi.h>
#include <driverlib/adc.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <driverlib/interrupt.h>
#include <driverlib/fpu.h>
#include <driverlib/rom_map.h>

#include "input.h"
#include "interrupts.h"
#include "systeminit.h"
#include "oscillator.h"

int main(void)
{
	g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

	// Initialize system hardware & peripherals
	setupWavetables();
	setupDigitalInputs();
    setupAnalogInputs();
    setupSSI();
    setupTimers();

    // Setup oscillators and envelopes
    mainOsc.gain = 1.0;
    pitchLFO.gain = 2.0;
    initOsc(&mainOsc,FS);
    initOsc(&pitchLFO,MOD_FS);
	// Setup knob controls
	(knobs+0)->send_fn = modifyOscFreq;
	(knobs+0)->send_target = &pitchLFO;
	(knobs+1)->out_port = 0;
	(knobs+1)->send_fn = modifyOscFreq;
	(knobs+1)->send_target = &mainOsc;
	(knobs+1)->out_port = 0;

	MAP_IntMasterEnable();
	MAP_FPUEnable();

	uint32_t nextSample;
	while(1)
	{
		if(system_flags.outputNextSample){
			system_flags.outputNextSample = 0;
			nextSample = getOscSample(&mainOsc);
			while(SSIBusy(SSI3_BASE));
			SSIDataPut(SSI3_BASE, nextSample<<2);
		}
		if(system_flags.modulate){
			incrementOscPhase(&pitchLFO);
			modifyOscFreq(&mainOsc,getOscSample(pitchLFO),1);
			system_flags.modulate = 0;
		}
		if(system_flags.readInputs){
			system_flags.readInputs = 0;
			handleDigitalInputs();
			handleAnalogInputs();
		}
	}
}
