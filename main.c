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
Osc mainOsc1;
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
	arm_mult_q31(&F,&buffer[1],&LP,1);
	arm_add_q31(&LP,&buffer[0],&LP,1);
	arm_mult_q31(&buffer[1],&Q,&HP,1);
	arm_add_q31(&LP,&HP,&HP,1);
	arm_sub_q31(&sample,&HP,&HP,1);
	arm_mult_q31(&F,&HP,&BP,1);
	arm_add_q31(&BP,&buffer[1],&BP,1);
	buffer[0] = LP;
	buffer[1] = BP;
	return LP;
}
void setMainOscNote(uint16_t note) {
	setOscNote(&mainOsc1, note);
}

int16_t seq[] = {48,44,40,36,32,28,24,20,16,12,8,4};
uint8_t seqLength = 12;
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

q31_t nextSample1;
q31_t buffer1[2] = {{0}};
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
	/*
	 *  Set up LFOs
	 *
	 */
	initOsc(&pitchLFO);
	setOscType(&pitchLFO, LFO_OSC, TriTable);
	setOscGain(&pitchLFO, 1024);
	setOscNote(&pitchLFO, 32);
	/*
	 *  Set up envelopes
	 *
	 */
	initEnv(&volEnv,MOD_FS);
	setEnvAtkTime(&volEnv,0.5);
	setEnvHold(&volEnv,1.0);
	setEnvRelTime(&volEnv,0.25);

	initEnv(&pitchAmpEnv,MOD_FS);
	setEnvAtkTime(&pitchAmpEnv,0.85);
	setEnvHold(&pitchAmpEnv,1.0);
	setEnvRelTime(&pitchAmpEnv,0.35);

	initEnv(&cutoffEnv,MOD_FS);
	setEnvAtkTime(&cutoffEnv,1.15);
	setEnvHold(&cutoffEnv,1.0);
	setEnvRelTime(&cutoffEnv,1.25);
	/*
	 *  Set up and route analog controllers
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

	triggerGate();
	q31_t OUTPUT_SCALE = (PWM_PERIOD-1)>>1;
	while (1) {
		if (system_flags.outputNextSample){
			g_sampleCount++;
			system_flags.outputNextSample = 0;
			incrOscPhase(&mainOsc1);
			incrOscPhase(&pitchLFO);
			nextSample1=mainOsc1.output>>1;
			nextSample1 = SVFilter(nextSample1,buffer1,knobs[3].output,knobs[4].output);
			arm_mult_q31(&nextSample1,&OUTPUT_SCALE,&nextSample1,1);
			nextSample1+=OUTPUT_SCALE>>1;
			PWMPulseWidthSet(PWM0_BASE,PWM_OUT_4,nextSample1);
			if (!(g_sampleCount&0xFF)){
				mainOsc1.noteMod = seq[(knobs[1].currValue*seqLength)>>10]<<20;
				// Tick modifiers
				incrEnvPhase(&volEnv);
				incrEnvPhase(&pitchAmpEnv);
				incrEnvPhase(&cutoffEnv);
				// Modify mainOsc1
				modifyOscGain(&mainOsc1, volEnv.output);
				modifyOscFreq(&mainOsc1, pitchLFO.output);
				modifyOscFreq(&mainOsc1, knobs[0].output>>18);
				// Modify pitchLFO
				modifyOscGain(&pitchLFO, pitchAmpEnv.output);
				modifyOscGain(&pitchLFO, knobs[2].output);
				modifyOscFreq(&pitchLFO, knobs[2].output>>16);
				// Apply mods
				applyMods(&mainOsc1);
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
