/*
 * watchdog.c
 *
 * Peripheral: 	watchdog
 * Pins: 		N 0 (led,out)
 * Interrupts: 	WatchdogIntHandler (INT_WATCHDOG)
 *
 *
 *  Created on: Jul 14, 2015
 *      Author: austen
 */

#include "watchdog.h"

void initWatchdog(void){
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    MAP_IntMasterEnable();
    char watchdog_lock_status = MAP_WatchdogLockState(WATCHDOG0_BASE);

    // Enable status LED output
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
    MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);

    MAP_IntEnable(INT_WATCHDOG);

    MAP_WatchdogReloadSet(WATCHDOG0_BASE, _ui32SysClock); // 1sec feeding-time interval
    MAP_WatchdogResetEnable(WATCHDOG0_BASE);
    MAP_WatchdogEnable(WATCHDOG0_BASE);
}

void WatchdogIntHandler(void){
	// Blink LED when watchdog is fed
	MAP_WatchdogIntClear(WATCHDOG0_BASE);
	MAP_GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0,
			(MAP_GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_0) ^
            GPIO_PIN_0));
}
