/*
 * systeminit.h
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */

#ifndef SYSTEMINIT_H_
#define SYSTEMINIT_H_

#define TA0_PERIOD	10000

extern void PortFunctionInit(void);
extern void PWMInit(void);
extern void AnalogInputInit(void);
extern void TimerInit(void);

#endif /* SYSTEMINIT_H_ */
