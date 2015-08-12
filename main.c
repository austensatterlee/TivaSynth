#include <stdbool.h>
#include <stdint.h>
#include <driverlib/pwm.h>
#include <driverlib/gpio.h>
#include <driverlib/adc.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <driverlib/interrupt.h>
#include <driverlib/timer.h>
#include <driverlib/rom_map.h>
#include <driverlib/debug.h>

#include "systeminit.h"
#include "input.h"
#include "oscillator.h"
#include "tables.h"
#define APP_PI 3.141592653589793f

/* System variables */
struct {
	uint16_t readAInputs :1;
	uint16_t readDInputs :1;
	uint16_t modulate :1;
	uint16_t outputNextSample :1;
} system_flags;

volatile uint32_t g_sampleCount;
uint32_t g_ui32SysClock;

/* Interrupts */
void Timer0AIntHandler(void);
void Timer1AIntHandler(void);
void Timer2AIntHandler(void);
void PWMGen2IntHandler(void);
void ADCInt0Handler(void);

/* Synth modules */
Osc mainOsc1,mainOsc2;
Osc pitchLFO;
Env volEnv;
Env pitchAmpEnv;
Env cutoffEnv;
Knob knobs[NUM_KNOBS];

q31_t SVFilter(q31_t sample, q31_t* buffer, q31_t F, q31_t Q){
	q31_t LP=0;
	q31_t HP=0;
	q31_t BP=0;
	F = getFilterFreq(F);
	LP = __SSAT(((q63_t)F*buffer[1])>>31,31);
	LP = __SSAT(buffer[0] + LP,31);
	HP = __SSAT(((q63_t)buffer[1]*Q)>>31,31);
	HP = __SSAT(sample-HP-LP,31);
	BP = __SSAT(((q63_t)F*HP)>>31,31);
	BP = __SSAT(BP + buffer[1],31);
	buffer[1] = BP;
	buffer[0] = LP;
	return LP;
}
void setMainOscNote(uint16_t note) {
	setOscNote(&mainOsc1, note);
	setOscNote(&mainOsc2, note+12);
}

int16_t seq[] = {
		0,12,24,36,48,60};
uint8_t seqLength = 6;
int16_t arpseq[] = {
		0,3,7,12
};
uint8_t arpseqlength = 4;
volatile uint16_t seqind = 0;
volatile uint16_t seqCounter=0;
void triggerGate() {
	/*
	 * Trigger envelopes here
	 */
	triggerEnv(&volEnv);
	triggerEnv(&pitchAmpEnv);
	triggerEnv(&cutoffEnv);
	seqind = 0;
	seqCounter = 0;
	return;
}
void releaseGate() {
	/*
	 * Release envelopes here
	 */
	releaseEnv(&volEnv);
	releaseEnv(&pitchAmpEnv);
	releaseEnv(&cutoffEnv);
	return;
}

int main(void) {
	g_ui32SysClock = MAP_SysCtlClockFreqSet(
			(SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL
					| SYSCTL_CFG_VCO_480), 120000000);

	/*
	 *  Set sound oscillators
	 *
	 */
	initOsc(&mainOsc1);
	setOscType(&mainOsc1, SOUND_OSC, SawTable);
	setOscNote(&mainOsc1, 0);
	initOsc(&mainOsc2);
	setOscType(&mainOsc2, SOUND_OSC, SawTable);
	setOscNote(&mainOsc2, 0);
	/*
	 *  Set up LFOs
	 *
	 */
	initOsc(&pitchLFO);
	setOscType(&pitchLFO, LFO_OSC, SoftTable);
	setOscGain(&pitchLFO, 0x7FFFFFFF);
	setOscNote(&pitchLFO, 0);
	/*
	 *  Set up envelopes
	 *
	 */
	initEnv(&volEnv,SoftTable,SoftTable);
	setEnvAtkTime(&volEnv,0.01);
	setEnvRelTime(&volEnv,0.01);

	initEnv(&pitchAmpEnv,SoftTable,SoftTable);
	setEnvAtkTime(&pitchAmpEnv,0.01);
	setEnvRelTime(&pitchAmpEnv,0.01);

	initEnv(&cutoffEnv,SoftTable,SoftTable);
	setEnvAtkTime(&cutoffEnv,0.1);
	setEnvRelTime(&cutoffEnv,0.1);
	/*
	 *  Set up analog controllers
	 *
	 */
	initKnob(&knobs[0], 1.0); // main pitch
	initKnob(&knobs[1], 1.0);
	initKnob(&knobs[2], 1.0); // pitch lfo gain
	initKnob(&knobs[3], 1.0); // cutoff freq.
	initKnob(&knobs[4], 0.5); // resonance


	/* Initialize system hardware & peripherals */
	setupDigitalInputs();
	setupAnalogInputs();
	setupPWM();
	setupTimers();
	/* Enable output to the onboard LEDs */
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
	MAP_IntMasterEnable();

	setMainOscNote(24);
	triggerGate();
	q31_t OUTPUT_SCALE = (PWM_PERIOD-1)>>1;
	q31_t cutoff = 0;
	q31_t nextSample1,nextSample2;
	q31_t buffer1[2] = {{0}};
	q31_t buffer2[2] = {{0}};
	while (1) {
		if (system_flags.outputNextSample){
			g_sampleCount++;
			system_flags.outputNextSample = 0;
			incrOscPhase(&mainOsc1);
			incrOscPhase(&mainOsc2);
			incrOscPhase(&pitchLFO);

			cutoff=((q63_t)knobs[3].output*cutoffEnv.output)>>31;

			nextSample1=mainOsc1.output;
			nextSample1=SVFilter(nextSample1,&buffer1[0],cutoff,knobs[4].output);
			nextSample1=__SSAT(((q63_t)nextSample1*OUTPUT_SCALE)>>31,31);
			nextSample1+=OUTPUT_SCALE;

			nextSample2=mainOsc2.output;
			nextSample2=SVFilter(nextSample2,&buffer2[0],cutoff,knobs[4].output);
			nextSample2=__SSAT(((q63_t)nextSample2*OUTPUT_SCALE)>>31,31);
			nextSample2+=OUTPUT_SCALE;

			PWMPulseWidthSet(PWM0_BASE,PWM_OUT_5,nextSample1);
			PWMPulseWidthSet(PWM0_BASE,PWM_OUT_4,nextSample2);
			if (!(g_sampleCount&0xFF)){
//				seqCounter+=0x6d3;
//				if(seqCounter<0x6d3){
//					seqind+=1;
//					if(seqind==arpseqlength){
//						seqind=0;
//					}
//				}
				//modifyOscNote(&mainOsc1,arpseq[seqind]);
				//modifyOscNote(&mainOsc2,arpseq[seqind]);
				// Tick modifiers
				incrEnvPhase(&volEnv);
				incrEnvPhase(&pitchAmpEnv);
				incrEnvPhase(&cutoffEnv);
				// Modify mainOsc1
				modifyOscGain(&mainOsc1, volEnv.output);
				modifyOscFreq(&mainOsc1, knobs[0].output);
				modifyOscGain(&mainOsc2, volEnv.output);
				modifyOscFreq(&mainOsc2, knobs[2].output);
				// Modify pitchLFO
				modifyOscGain(&pitchLFO, pitchAmpEnv.output);
				modifyOscGain(&pitchLFO, knobs[1].output);
				// Apply mods
				applyMods(&mainOsc1);
				applyMods(&mainOsc2);
				applyMods(&pitchLFO);
			}
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
/*
 * Sample ouput interrupt.
 *
 * This timer is clocked at the sample frequency.
 *
 */
void SysTickIntHandler(void) {
	system_flags.outputNextSample = 1;
}
void Timer0AIntHandler(void) {
	MAP_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
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
