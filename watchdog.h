/*
 * watchdog.h
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_
#include "includes.h"
#include "driverlib/watchdog.h"

void initWatchdog(void);
void WatchdogIntHandler(void);

#endif /* WATCHDOG_H_ */
