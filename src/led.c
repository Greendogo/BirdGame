/*
 * led.c
 *
 *  Created on: Aug 28, 2015
 *      Author: Brad Torrence
 */

// Header File
#include "led.h"

// The system.h module will be required in all future task definitions
// This will give the task access to the base libraries as well as the global variables
#include "system.h"

// Stellaris driver libraries required by these local function definitions
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

/*
 *  The task definition
 */
void LEDTask(void *pvParameters) {
	// FreeRTOS uses function definitions to define tasks
	// The first part of each task is the initializations steps for that task

	// The execution delay
	const unsigned long delay = TICK_R * 250.0;

    // The Status LED is attached to G<2> (Port G pin 2) it must be set as output
    GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);
    /*
     * Start G<2> signal HIGH so the LED is lit at the start
     * The GPIOPinWrite function requires the Port and pins as the first arguments
     * 		the third argument has to be a bit-packed byte that represents the
     * 		desired state of the given pins.
     * 		The least-significant-bit of this byte (bit 0) represents pin 0 on the specified port,
     * 		the next LSB (bit 1) represents pin 1 and so on...
     * 	To write pin G<2> HIGH we have to pass the value 0x04,
     * 		if we wanted pin G<1> HIGH => GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_1, 0x02)
     * 		and if we wanted pin G<7> HIGH => GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_7, 0x80)
     * 	The function can be used to write multiple pins as well,
     * 		if we wanted pins G<1> and G<2> HIGH =>
     * 			write GPIOPinWrite(GPIO_PORTG_BASE, (GPIO_PIN_1 | GPIO_PIN_2), 0x06)
     * 		the pin arguments are combined with a bit-wise OR operation
     * 		and the desired signal value given is a bit-wise OR'ing of the individual pin values.
     */
    // This sets G<2> to LOW
    GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0x00);

    // FreeRTOS task definitions also require an infinite loop to house execution steps
    while(true) {
        /*
         *  Toggle the LED.
         *  First pin G<2> is read and that value is XOR'd with the constant value GPIO_PIN_2
         *  	so that the bit representing G<2> is toggled
         *  	to either 0x00 or 0x04 depending on its current state
         *  	then the new value is written back to G<2>
         */
        GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_2) ^ GPIO_PIN_2);

        //	Advance next execution time for the LED task
        vTaskDelay(delay);
	}
}

