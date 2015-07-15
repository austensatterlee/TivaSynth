/*
 * includes.h
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/tm4c1294ncpdt.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/fpu.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/pwm.h"
#include "driverlib/systick.h"
// Math constants
#define APP_PI      3.1415926535897932384626433832795f
// System parameters
#define PWM_PERIOD	512
#define TICK_PERIOD	65000
// System states
uint32_t _ui32SysClock;
uint32_t _ui32SysTick;
#endif /* INCLUDES_H_ */
