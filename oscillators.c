/*
 * oscillators.c
 *
 *  Created on: Jul 20, 2015
 *      Author: austen
 */
#include "oscillators.h"
#include "driverlib/pwm.h"
#include "inc/hw_memmap.h"

uint16_t pwmFrequencyScale[] = {1065, 1005,  949,  895,  845,  798,  753,  711,  671,  633,  597,
        564,  532};
void setPWMFrequencyIndex(uint16_t freqIndex){
	PwmPeriod = pwmFrequencyScale[freqIndex];
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_2,PwmPeriod);
}
