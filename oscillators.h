/*
 * includes.h
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */

#ifndef OSCILLATORS_H_
#define OSCILLATORS_H_
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#include <stdint.h>
#include <stdbool.h>
void setPWMFrequencyIndex(uint16_t);
// Math constants
#define APP_PI      3.1415926535897932384626433832795f
// System parameters
#define INIT_TICKLENGTH 1
#define INIT_PWMPERIOD 1024
#define INIT_TICKPERIOD 127
float TickLength; // reset value of _ui32SysMiniTick
volatile uint32_t _ui32SysTick;
volatile uint32_t _ui32SysMiniTick;
uint32_t TickPeriod; // reset value of _ui32SysTick
uint32_t PwmPeriod;  // PWM compare value (in CPU clock cycles)
uint32_t _nextSample;
bool gate;
// System states
uint32_t _ui32SysClock; // system clock speed
#endif /* OSCILLATORS_H_ */
