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

Osc mainOsc1;
Osc mainOsc2;
Osc pitchLFO;
Knob knobs[NUM_KNOBS];

int main(void)
{
	g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

	// Initialize system hardware & peripherals
	setupDigitalInputs();
    setupAnalogInputs();
    setupSSI();
    setupTimers();

    // Setup oscillators and envelopes
    initOsc(&mainOsc1,FS);
    initOsc(&mainOsc2,FS);
    initOsc(&pitchLFO,MOD_FS);
    mainOsc1.gain = 1.0;
    mainOsc2.gain = 1.0;
    pitchLFO.gain = 3.0/12.0;
    setOscFreq(&pitchLFO,1);
	// Setup knob controls
	initKnob(&knobs[0],&pitchLFO,0,8.0);
	(knobs+0)->send_fn 	= modifyOscFreq;
	initKnob(&knobs[1],&mainOsc2,0,2.0);
	(knobs+1)->send_fn 	= modifyOscFreq;
	// Setup sequencer
	setSequenceFreq(6);

	MAP_IntMasterEnable();
	MAP_FPUEnable();

	float nextSample;
	while(1)
	{
		if(system_flags.outputNextSample){
			system_flags.outputNextSample = 0;
			nextSample = getOscSample(&mainOsc1);
			nextSample += getOscSample(&mainOsc2);
			nextSample *= 0x3FF*0.5;
			while(SSIBusy(SSI3_BASE));
			SSIDataPut(SSI3_BASE, ((uint32_t)nextSample)<<2);
		}
		if(system_flags.modulate){
			incrementOscPhase(&pitchLFO);
			modifyOscFreq(&mainOsc1,getOscSample(&pitchLFO),1);
			modifyOscFreq(&mainOsc2,getOscSample(&pitchLFO),1);
			applyMods(&mainOsc1);
			applyMods(&mainOsc2);
			applyMods(&pitchLFO);
			system_flags.modulate = 0;
		}
		if(system_flags.readInputs){
			system_flags.readInputs = 0;
			handleDigitalInputs();
			handleAnalogInputs(knobs);
		}
	}
}
