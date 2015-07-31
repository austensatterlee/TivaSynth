#include <stdbool.h>
#include <stdint.h>
#include <driverlib/ssi.h>
#include <driverlib/gpio.h>
#include <driverlib/adc.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <driverlib/interrupt.h>
#include <driverlib/fpu.h>
#include <driverlib/timer.h>
#include <driverlib/rom_map.h>
#include <math.h>
#include <driverlib/debug.h>

#include "systeminit.h"
#include "input.h"
#include "oscillator.h"
extern void triggerGate();
#define APP_PI 3.141592653589793f

/* System variables */
struct {
	uint16_t readAInputs :1;
	uint16_t readDInputs :1;
	uint16_t modulate :1;
	uint16_t outputNextSample :1;
} system_flags;

uint32_t g_sampleCount;
uint32_t g_ui32SysClock;

/* Interrupts */
void Timer0AIntHandler(void);
void Timer1AIntHandler(void);
void Timer2AIntHandler(void);
void PWMGen2IntHandler(void);
void ADCInt0Handler(void);

/* Synth modules */
Osc mainOsc1;
Osc mainOsc2;
Osc pitchLFO;
Env volEnv;
Env pitchAmpEnv;
Knob knobs[NUM_KNOBS];
float buffer[2] = {0};

float SVFilter(float sample, float F,float Q){
	//F = 2*sin(APP_PI*F/FS);
	float LP = buffer[0] + F*buffer[1];
	float HP = sample - LP - Q*buffer[1];
	float BP = F*HP + buffer[1];
	float N	 = HP+LP;
	buffer[0] = LP;
	buffer[1] = BP;
	return 0.7*HP+0.3*LP;
}

int main(void) {
	g_ui32SysClock = MAP_SysCtlClockFreqSet(
			(SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL
					| SYSCTL_CFG_VCO_480), 120000000);

	// Initialize system hardware & peripherals
	setupDigitalInputs();
	setupSSI();
	setupTimers();
	setupAnalogInputs();

	/*
	 *  Set sound oscillators
	 *
	 */
	initOsc(&mainOsc1, FS);
	setOscType(&mainOsc1, SAW_WV, SOUND_OSC);
	/*
	 *  Set up LFOs
	 *
	 */
	initOsc(&pitchLFO, MOD_FS);
	setOscType(&pitchLFO, TRI_WV, LFO_OSC);
	setOscGain(&pitchLFO, 6.0);
	setOscNote(&pitchLFO, 50);
	/*
	 *  Set up envelopes
	 *
	 */
	initEnv(&volEnv, MOD_FS);
	setEnvAtkTime(&volEnv,0.01);
	setEnvHold(&volEnv,1.0);
	setEnvRelTime(&volEnv,0.0125);

	initEnv(&pitchAmpEnv, MOD_FS);
	setEnvAtkTime(&pitchAmpEnv,0.85);
	setEnvHold(&pitchAmpEnv,1.0);
	setEnvRelTime(&pitchAmpEnv,1.0);
	/*
	 *  Set up and route analog controllers
	 *
	 */
	initKnob(&knobs[0], &mainOsc1, 0, 1.0);
	(knobs + 0)->send_fn = modifyOscFreq;
	initKnob(&knobs[1], &pitchLFO, 0, 1.0);
	(knobs + 1)->send_fn = modifyOscGain;
	initKnob(&knobs[2], &pitchLFO, 0, 2.0);
	(knobs + 2)->send_fn = modifyOscFreq;
	initKnob(&knobs[3], 0, 0, 0.125);
	(knobs + 3)->send_fn = 0;

	MAP_IntMasterEnable();
	MAP_FPUEnable();

	float nextSample;
	uint8_t ui8PortNLEDStates = (uint8_t)GPIO_PIN_0;
	triggerGate();
	while (1) {
		if (system_flags.outputNextSample){
			system_flags.outputNextSample = 0;

			nextSample = getOscSample(&mainOsc1)*0x3FF;
			nextSample = SVFilter(nextSample,knobs[3].output,0.25);
			if(nextSample>1023){
				nextSample=1023;
				GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 1);
			}else if(nextSample<0){
				nextSample=0;
				GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 1);
			}
			ui8PortNLEDStates ^= (GPIO_PIN_0);
			GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, ui8PortNLEDStates);
			while(SSIBusy(SSI3_BASE));
			SSIDataPut(SSI3_BASE, ((uint32_t) (nextSample) << 2));
			GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0);
		}
		if (system_flags.modulate){
			system_flags.modulate = 0;
			// Tick modifiers
			incrOscPhase(&pitchLFO);
			incrEnvPhase(&volEnv);
			incrEnvPhase(&pitchAmpEnv);
			//handleAnalogInputs(knobs);
			// Modify mainOsc1
			modifyOscFreq(&mainOsc1, getOscSample(&pitchLFO), 1);
			setOscGain(&mainOsc1, getEnvSample(&volEnv));
			// Modify pitchLFO
			setOscGain(&pitchLFO, getEnvSample(&pitchAmpEnv));
			// Apply mods
			applyMods(&mainOsc1);
			applyMods(&pitchLFO);
		}
		if (system_flags.readDInputs){
			system_flags.readDInputs = 0;
			handleDigitalInputs();
		}
		if (system_flags.readAInputs){
			system_flags.readAInputs = 0;
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
	incrOscPhase(&mainOsc1);
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
	system_flags.readDInputs = 1;
}
void ADCInt0Handler(void) {
	ADCIntClear(ADC0_BASE, 0);
	system_flags.readAInputs = 1;
}
