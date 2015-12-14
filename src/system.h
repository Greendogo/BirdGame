/*
 * system.h
 *
 *  Created on: Aug 28, 2015
 *      Author: Brad Torrence
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

// These are required TI hardware libraries
// Included here to avoid repeating the includes for every task.
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"

// Required FreeRTOS Libraries
#include "FreeRTOS.h"
#include "task.h"

//*************************************************************************************************
//  The system tick interrupt (systick) is used to divide the system clock to a slower rate,
//  and use it as the main timing mechanism in our program.
//  The SYSTICK_FREQUENCY is used to divide the system clock.
//  All events in the application occur at some fraction of this clock rate.
//  The BASE_FREQUENCY (1000 Hz) is used to create a tick multiplier -> TICK_R
//  By using TICK_R instead of SYSTICK_FREQUENCY we can freely change the system tick frequency
//  	without re-adjusting the timings of existing definitions.
//  TICK_R is multiplied by the number of milliseconds between executions when creating tasks
// 		to attain a stable execution frequency.
//*************************************************************************************************
#define SYSTICK_FREQUENCY configTICK_RATE_HZ
#define BASE_FREQUENCY 1000.0
#define TICK_R (SYSTICK_FREQUENCY / BASE_FREQUENCY)

//*************************************************************************************************
// This variable will keep track of the number of systicks that have occurred.
// The systick count is recorded to maintain task execution frequencies.
//*************************************************************************************************
extern volatile unsigned long xPortSysTickCount;

#define sysTickCount xPortSysTickCount

/*************************************************************************************************
 *	Initializes the system clock and registers the interrupt service routines.
 ************************************************************************************************/
extern void systemInit(void);

#endif /* SYSTEM_H_ */
