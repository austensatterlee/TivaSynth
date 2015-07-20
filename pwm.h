/*
 * pwm.h
 *
 *  Created on: Jul 20, 2015
 *      Author: austen
 */

#ifndef PWM_H_
#define PWM_H_
#include <stdint.h>
#include <stdbool.h>
void initPWM(void);
void PWMGen2IntHandler(void);

// pwm
#define INIT_TICK_LENGTH 	1
#define INIT_PWM_PERIOD 	1024
#define INIT_TICK_PERIOD 	127

uint32_t PwmPeriod;  // PWM compare value (in CPU clock cycles)
uint32_t _nextSample;
volatile float _fSysTick;
volatile uint32_t _ui32SysMiniTick;
float BaseTickStep;
float TickStep_0;
uint32_t TickPeriod; // reset value of _ui32SysTick
uint32_t TickLength; // reset value of _ui32SysMiniTick

#endif /* PWM_H_ */
