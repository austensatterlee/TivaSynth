/*
 * systeminit.h
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */

#ifndef SYSTEMINIT_H_
#define SYSTEMINIT_H_
#include <stdint.h>
#include "input.h"
#include "oscillator.h"

extern void setupDigitalInputs(void);
extern void setupPWM(void);
extern void setupSSI(void);
extern void setupAnalogInputs(void);
extern void setupTimers(void);

// System parameters
#define SSI_BAUDRATE 1200000
#define PWMPERIOD 500
#define FS 150000
#define INPUT_FS 600
#define MOD_FS 20000
// System states
uint32_t g_sampleCount;
uint32_t g_ui32SysClock; // system clock speed

struct {
	uint16_t readInputs 		: 1;
	uint16_t modulate			: 1;
	uint16_t outputNextSample 	: 1;
} system_flags;

#define NUM_BUTTONS 5
#define NUM_KNOBS 2
#define E_BUTTON_1 GPIO_PIN_4
#define E_BUTTON_2 GPIO_PIN_0
#define E_BUTTON_3 GPIO_PIN_1
#define E_BUTTON_4 GPIO_PIN_2
#define E_BUTTON_5 GPIO_PIN_5
#define ALL_E_BUTTONS (E_BUTTON_1|E_BUTTON_2|E_BUTTON_3|E_BUTTON_4|E_BUTTON_5)

#endif /* SYSTEMINIT_H_ */
