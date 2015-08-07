#include <stdbool.h>
#include <stdint.h>
#include <driverlib/pwm.h>
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

#include "arm_math.h"
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

struct {
	float delaybuf[3000];
	struct {
		float K;
		uint32_t numDelaySamples;
		uint32_t index;
		uint32_t previndex;
	} delaylines[3];
} delay1 = {{0}};

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

float SVFilter(float sample, float* buffer, float F, float Q){
	F = getFilterFreq(F);
	float LP = buffer[0] + F*buffer[1];
	float HP = sample - LP - Q*buffer[1];
	float BP = F*HP + buffer[1];
	float N	 = HP+LP;
	buffer[0] = LP;
	buffer[1] = BP;
	return LP;
}
void setMainOscNote(uint16_t note) {
	setOscNote(&mainOsc1, note);
}

uint16_t seq[] = {0,5,12,17};
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

	/* Initialize system hardware & peripherals */
	setupDigitalInputs();
	setupAnalogInputs();
	setupPWM();
	setupTimers();
	/* Enable output to the onboard LEDs */
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);

	/*
	 *  Set sound oscillators
	 *
	 */
	initOsc(&mainOsc1, FS);
	setOscType(&mainOsc1, TRI_WV, SOUND_OSC);
	setOscNote(&mainOsc1, 0);
	/*
	 *  Set up LFOs
	 *
	 */
	initOsc(&pitchLFO, MOD_FS);
	setOscType(&pitchLFO, SAW_WV, LFO_OSC);
	setOscGain(&pitchLFO, 2.0);
	setOscNote(&pitchLFO, 0);
	/*
	 *  Set up envelopes
	 *
	 */
	initEnv(&volEnv, MOD_FS);
	setEnvAtkTime(&volEnv,0.05);
	setEnvHold(&volEnv,1.0);
	setEnvRelTime(&volEnv,0.25);

	initEnv(&pitchAmpEnv, MOD_FS);
	setEnvAtkTime(&pitchAmpEnv,0.85);
	setEnvHold(&pitchAmpEnv,1.0);
	setEnvRelTime(&pitchAmpEnv,0.25);

	initEnv(&cutoffEnv, MOD_FS);
	setEnvAtkTime(&cutoffEnv,1.15);
	setEnvHold(&cutoffEnv,1.0);
	setEnvRelTime(&cutoffEnv,1.25);
	/*
	 *  Set up and route analog controllers
	 *
	 */
	initKnob(&knobs[0], 12.0); // main pitch
	initKnob(&knobs[1], 2.0);
	initKnob(&knobs[2], 12.0); // pitch lfo gain
	initKnob(&knobs[3], 0.125);
	initKnob(&knobs[4], 1.0);

	MAP_IntMasterEnable();
	MAP_FPUEnable();

	triggerGate();
	float nextSample1;
	float nextSample2;
	float buffer1[2] = {{0}};
	float buffer2[2] = {{0}};
	delay1.delaylines[0].K = 0.95;
	delay1.delaylines[0].numDelaySamples = 1483;
	delay1.delaylines[1].K = 0.96;
	delay1.delaylines[1].numDelaySamples = 1051;
	delay1.delaylines[2].K = 0.97;
	delay1.delaylines[2].numDelaySamples = 269;
	uint8_t i;
	while (1) {
		if (system_flags.outputNextSample){
			g_sampleCount++;
			system_flags.outputNextSample = 0;
			incrOscPhase(&mainOsc1);
			nextSample1 = mainOsc1.output;
			nextSample1 = SVFilter(nextSample1,buffer1,(cutoffEnv.sample)*(knobs[3].output),knobs[4].output+0.150);
			if(nextSample1>1){
				nextSample1 = 1;
			}else if(nextSample1<-1){
				nextSample1 = -1;
			}
			if(!(g_sampleCount&0x03)){
				PWMPulseWidthSet(PWM0_BASE,PWM_OUT_4,(uint32_t)(250*(nextSample1+0.5)));

				nextSample2 = nextSample1;
				for(i=0;i<1;i++){
					delay1.delaylines[i].index=(delay1.delaylines[i].index+1);
					if(delay1.delaylines[i].index>=3000){
						delay1.delaylines[i].index=0;
					}else if(delay1.delaylines[i].index<delay1.delaylines[i].numDelaySamples){
						delay1.delaylines[i].previndex = 3000-delay1.delaylines[i].numDelaySamples+delay1.delaylines[i].index;
					}else{
						delay1.delaylines[i].previndex = delay1.delaylines[i].index-delay1.delaylines[i].numDelaySamples;
					}
					delay1.delaybuf[delay1.delaylines[i].index]	= nextSample1 + delay1.delaylines[i].K * delay1.delaybuf[delay1.delaylines[i].previndex];
					nextSample2	+= -delay1.delaylines[i].K * delay1.delaybuf[delay1.delaylines[i].index] + delay1.delaybuf[delay1.delaylines[i].previndex];
				}
				buffer2[0]=nextSample2<buffer2[0]?nextSample2:buffer2[0];
				buffer2[1]=nextSample2>buffer2[1]?nextSample2:buffer2[1];
				if(nextSample2>1){
					nextSample2 = 1;
				}else if(nextSample2<-1){
					nextSample2 = -1;
				}
				PWMPulseWidthSet(PWM0_BASE,PWM_OUT_5,(uint32_t)(100*(nextSample2+2)));
			}
			if (!(g_sampleCount&0x7)){
				mainOsc1.noteMod = seq[knobs[1].currValue*3/255];
				pitchLFO.noteMod = mainOsc1.noteMod;
				// Tick modifiers
				incrEnvPhase(&volEnv);
				incrEnvPhase(&pitchAmpEnv);
				incrEnvPhase(&cutoffEnv);
				incrOscPhase(&pitchLFO);
				getEnvSample(&cutoffEnv);
				// Modify mainOsc1
				modifyOscGain(&mainOsc1, getEnvSample(&volEnv));
				modifyOscFreq(&mainOsc1, pitchLFO.output);
				modifyOscFreq(&mainOsc1, knobs[0].output);
				// Modify pitchLFO
				modifyOscGain(&pitchLFO, getEnvSample(&pitchAmpEnv));
				modifyOscGain(&pitchLFO, knobs[2].output);
				modifyOscFreq(&pitchLFO, knobs[0].output);
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
