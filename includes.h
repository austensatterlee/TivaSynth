/*
 * includes.h
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/tm4c1294ncpdt.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/pwm.h"
// Math constants
#define APP_PI      3.1415926535897932384626433832795f
// System parameters
#define TA0_PERIOD	65536
#define INIT_TICKLENGTH 1
#define INIT_PWMPERIOD 1024
#define INIT_TICKPERIOD 127
float TickLength; // reset value of _ui32SysMiniTick
uint32_t TickPeriod; // reset value of _ui32SysTick
uint32_t PwmPeriod;  // PWM compare value (in CPU clock cycles)
// System states
uint32_t _ui32SysClock; // system clock speed
uint32_t _nextSample;
volatile uint32_t _ui32SysTick;
volatile uint32_t _ui32SysMiniTick;
float _fFreq;
#endif /* INCLUDES_H_ */
