#include <stdbool.h>
#include <stdint.h>
#include <driverlib/ssi.h>
#include <driverlib/adc.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <driverlib/interrupt.h>
#include <driverlib/fpu.h>
#include <driverlib/timer.h>
#include <driverlib/rom_map.h>
#include <math.h>

#include "input.h"
#include "systeminit.h"
#include "oscillator.h"

void Timer0AIntHandler(void);
void Timer1AIntHandler(void);
void Timer2AIntHandler(void);
void PWMGen2IntHandler(void);
void PortEIntHandler(void);

Osc mainOsc1;
Osc mainOsc2;
Osc pitchLFO;
Env volEnv;
Env pitchAmpEnv;
Knob knobs[NUM_KNOBS];
float[5] buffer;

int main(void) {
	g_ui32SysClock = MAP_SysCtlClockFreqSet(
			(SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL
					| SYSCTL_CFG_VCO_480), 120000000);

	// Initialize system hardware & peripherals
	setupDigitalInputs();
	setupAnalogInputs();
	setupSSI();
	setupTimers();

	// Setup oscillator
	initOsc(&mainOsc1, FS);
	// Setup LFOs
	initOsc(&pitchLFO, MOD_FS);
	setOscType(&pitchLFO, TRI_WV);
	setOscGain(&pitchLFO, 0.49);
	setOscFreq(&pitchLFO, 1);
	// Setup envelopes
	initEnv(&volEnv, MOD_FS);
	setEnvAtkTime(&volEnv,0.01);
	setEnvHold(&volEnv,1.0);
	setEnvRelTime(&volEnv,0.5);
	initEnv(&pitchAmpEnv, MOD_FS);
	setEnvAtkTime(&pitchAmpEnv,1.0);
	setEnvHold(&pitchAmpEnv,1.0);
	setEnvRelTime(&pitchAmpEnv,1.0);
	// Setup knob controls
	initKnob(&knobs[0], &pitchLFO, 0, 100.0);
	(knobs + 0)->send_fn = modifyOscFreq;
	initKnob(&knobs[1], &mainOsc1, 0, 2.0);
	(knobs + 1)->send_fn = modifyOscFreq;

	MAP_IntMasterEnable();
	MAP_FPUEnable();

	float nextSample;
	uint8_t ui8PortNLEDStates = GPIO_PIN_0;
	while (1) {
		if (system_flags.outputNextSample) {
			system_flags.outputNextSample = 0;
			incrOscPhase(&mainOsc1);
			nextSample = getOscSample(&mainOsc1);
			nextSample *= 0x3FF;
			while(SSIBusy(SSI3_BASE));
			SSIDataPut(SSI3_BASE, ((uint32_t) nextSample) << 2);
			ui8PortNLEDStates ^= (GPIO_PIN_0 | GPIO_PIN_1);
			GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, ui8PortNLEDStates);
		}
		if (system_flags.modulate){
			system_flags.modulate = 0;
			// Tick modifiers
			incrOscPhase(&pitchLFO);
			incrEnvPhase(&volEnv);
			incrEnvPhase(&pitchAmpEnv);
			// Modify mainOsc1
			modifyOscFreq(&mainOsc1, getOscSample(&pitchLFO), 1);
			setOscGain(&mainOsc1, getEnvSample(&volEnv));
			// Modify pitchLFO
			modifyOscGain(&pitchLFO, getEnvSample(&pitchAmpEnv), 1);
			// Apply mods
			applyMods(&mainOsc1);
			applyMods(&pitchLFO);
		}
		if (system_flags.readInputs){
			system_flags.readInputs = 0;
			handleDigitalInputs();
			handleAnalogInputs(knobs);
		}
	}
}

void setMainOscNote(uint16_t note) {
	setOscNote(&mainOsc1, note);
}

/*
 * Sample ouput interrupt.
 *
 * This timer is clocked at the sample frequency.
 *
 */
void Timer0AIntHandler(void) {
	MAP_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	g_sampleCount++;
	if (g_sampleCount == 0xFFFFFFFF) {
		g_sampleCount = 0;
	}
	system_flags.outputNextSample = 1;
}

/*
 * Modulation interrupt.
 *
 */
void Timer1AIntHandler(void) {
	MAP_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	system_flags.modulate = 1;
}

/*
 * Input interrupt.
 *
 * - Read and update values from hardware input peripherals.
 *
 */
void Timer2AIntHandler(void) {
	MAP_TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	system_flags.readInputs = 1;
}
