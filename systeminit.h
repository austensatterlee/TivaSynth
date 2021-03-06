/*
 * systeminit.h
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */

#ifndef SYSTEMINIT_H_
#define SYSTEMINIT_H_

#include <stdint.h>
#include <stdbool.h>


extern void setupDigitalInputs(void);
extern void setupPWM(void);
extern void setupDAC(void);
extern void setupIOExpander(void);
extern void setupAnalogInputs(void);
extern void setupTimers(void);

// System parameters
#define SSI_BAUDRATE 1000000
#define PWM_PERIOD 256
#define FS 192000
#define INPUT_FS 1000
#define MOD_FS FS/0xFF
// System states
extern uint32_t g_ui32SysClock; // system clock speed


#define NUM_BUTTONS 5
#define NUM_KNOBS 5
#define E_BUTTON_1 GPIO_PIN_0
#define E_BUTTON_2 GPIO_PIN_1
#define E_BUTTON_3 GPIO_PIN_2
#define E_BUTTON_4 GPIO_PIN_4
#define E_BUTTON_5 GPIO_PIN_5
#define ALL_E_BUTTONS (E_BUTTON_1|E_BUTTON_2|E_BUTTON_3|E_BUTTON_4|E_BUTTON_5)

#endif /* SYSTEMINIT_H_ */
