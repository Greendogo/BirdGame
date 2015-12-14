//
//updated by Paul McElroy on 11/9/2015
//For use in the Baby Bird Feeding Game

// Project libraries
#include "src/system.h"
#include "src/oled.h"
#include "src/led.h"
#include "src/buttons.h"
#include "src/buttons.h"
#include "src/game.h"
#include "src/speaker.h"

//*************************************************************************************************
//	Main program to initialize hardware and execute Tasks.
//*************************************************************************************************
void main(void)  {
	// Create the tasks, the definitions are passed to the scheduler
	xTaskCreate(OLEDTask, "OLEDTask", 512 + 256, NULL, 3, NULL); // Note the higher priority for the display
	xTaskCreate(LEDTask, "LEDTask", 32, NULL, 1, NULL);
	xTaskCreate(buttonsTask, "buttonsTask", 256, NULL, 1, NULL);
	xTaskCreate(gameTask, "gameTask", 512, NULL, 2, NULL);
	xTaskCreate(speakerTask, "speakerTask", 256, NULL, 4, NULL);


	//  Initialize system
	systemInit();

	// Display the initial image
	startScreen();

	// Start the FreeRTOS Scheduler
	vTaskStartScheduler();

	// The infinite loop is necessary for the FreeRTOS Scheduler
    while(true);
}

