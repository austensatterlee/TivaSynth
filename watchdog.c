/*
 * watchdog.c
 *
 * Uses watchdog, pin N0 (out)
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */

#include "driverlib/watchdog.h"

void WatchdogIntHandler(void){
	if(!g_bFeedWatchdog){
		return;
	}
	ROM_WatchdogIntClear(WATCHDOG0_BASE);
	// Blink LED when watchdog is fed
	ROM_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0,
			(ROM_GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_0) ^
            GPIO_PIN_0));
}

void initWatchdog(void){
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    // Enable status LED output
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
    ROM_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);

    ROM_IntEnable(INT_WATCHDOG);
    ROM_WatchdogReloadSet(WATCHDOG0_BASE, g_ui32SysClock); // 1sec feeding-time interval
    ROM_WatchdogResetEnable(WATCHDOG0_BASE);
    ROM_WatchdogEnable(WATCHDOG0_BASE);
}

