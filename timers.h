/*
 * timers.h
 *
 *  Created on: Jul 20, 2015
 *      Author: austen
 */

#ifndef TIMERS_H_
#define TIMERS_H_
#include <stdint.h>
#include <stdbool.h>
void initTimers(void);
void Timer0AIntHandler(void);
#define INIT_TIMER0A_PERIOD	65535
uint16_t timer0APeriod;

#endif /* TIMERS_H_ */
