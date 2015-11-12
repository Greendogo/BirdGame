/*
 * oled.c
 *
 *  Created on: Aug 28, 2015
 *      Author: Brad Torrence
 */
//
//updated by Paul McElroy on 11/9/2015
//For use in the Baby Bird Feeding Game
//Updates the screen with the game pieces from the piecesArray from game.c and with
//helpful messages set by the messageFlag in game.c

// Header File
#include "oled.h"

// Base library
#include "system.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

// OLED driver
#include "rit128x96x4.h" // This is a driver for the OLED display extension

// Standard C library
#include "stdio.h"
#include "buttons.h"
#include "game.h"

char Row0[17];
char Row1[17];
char Row2[17];
char Row3[17];

// The function to generate and hold the initial start screen
void startScreen(void){
    /*
	 * The RIT128x96x4** functions are a part of the driver API for the OLED
	 * The details for this API can be found in drivers/rit128x96x4.c
	 *
     * Create a frame for the OLED display
     * 		The ImageDraw function takes an array of unsigned char
     * 		each 8-bit char represents 2 pixels (side-by-side)
     * 		so, 4 of these bits represent a single pixel's brightness
     * 		with valid brightness values ranging (0-15)
     * 		and the pixels are represented in pairs.
     *
     *  Since the pixels are represented in pairs along the horizontal,
     *  	and the OLED is 128x96 pixels,
     *  	the horizontal frame only needs 64 elements (64 * 2 = 128).
     *  However the vertical frames will require two separate arrays,
     *  	each will have 94 elements (the OLED is 96 pixels high
     *  	but we have taken care of the two ends with the horizontal lines)
     *  The left frame will set the right column of pixels to zero,
     *  	and the right frame will set the left column of pixels to zero.
     *  The blank array elements are initialized to 0 to turn-off pixels later.
     *  It only needs 62 elements because we don't want to erase the frame.
     */
    unsigned char lframe[94], rframe[94], hframe[64];

    // The loop is used to initialize the arrays
    unsigned int i;
    for(i = 0; i < 94; i++){
    	lframe[i] = 0xF0; 				// Vertical line for the left edge
    	rframe[i] = 0x0F; 				// Vertical line for the right edge
    	if(i < 64) hframe[i] = 0xFF; 	// Horizontal line for top and bottom
    }

    //  Initialize the OLED display
    RIT128x96x4Init(1000000);
    /*
     *  Draw the frame
     *  ImageDraw takes an array of unsigned char as the first argument
     *  	the next two arguments are the x and y coordinates (respectively)
     *  	of the screen position to start drawing, and the final two arguments
     *  	are the width and height (respectively) to which the image is scaled.
     */
    // Draw upper frame starting at (0,0) with width = 128 pixels and height = 1 pixel
    // 	(width and height should match the size of array measured in pixels)
    RIT128x96x4ImageDraw(hframe, 0, 0, 128, 1);
    // Draws lower frame starting at (0,95) width = 128, height = 1
    RIT128x96x4ImageDraw(hframe, 0, 95, 128, 1);
    // Draws left frame starting at (0,1) width = 2, height = 94
    RIT128x96x4ImageDraw(lframe, 0, 1, 2, 94);
    // Draws right frame starting at (127,1) width = 2, height = 94
    RIT128x96x4ImageDraw(rframe, 127, 1, 2, 94);

    //Write the game instructions
	RIT128x96x4StringDraw("Paul McElroy ^_^", 8, 8-3, 15);
	RIT128x96x4StringDraw("Collect Worms to", 3, 16-3, 15);
	RIT128x96x4StringDraw("feed your baby bird!", 3, 24-3, 15);
	RIT128x96x4StringDraw("Momma Bird (You): @", 3, 32-3, 15);
	RIT128x96x4StringDraw("Baby Bird: &", 3, 40-3, 15);
	RIT128x96x4StringDraw("Worms: ~", 3, 48-3, 15);
	RIT128x96x4StringDraw("Dirt Patch: #", 3, 56-3, 15);
	RIT128x96x4StringDraw("(Worms hide in Dirt)", 3, 64-3, 15);
	RIT128x96x4StringDraw(" Press the", 32, 72-3, 15);
	RIT128x96x4StringDraw("\"Select\" Button", 16, 80-3, 15);
	RIT128x96x4StringDraw("To Continue", 32, 88-3, 15);

	// Enable the select button, wait for the press, then disable and reset the port
	// The button is disabled after the splash screen and the port is reset to default for future use.
	GPIOPinTypeGPIOInput(GPIO_PORTG_BASE, GPIO_PIN_7); 	// Set the select button signal PG7 as input
	while(GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_7));	// Wait until the signal changes (button pressed)
	SysCtlPeripheralReset(SYSCTL_PERIPH_GPIOG);

	/*
	 *  When the select button is pressed we need to wipe the message leaving the frame on the OLED.
	 *   The loop sets the pixels to 0 everywhere except the screen's edges.
	 */

	//reset the frame
	RIT128x96x4Clear();
    RIT128x96x4ImageDraw(hframe, 0, 0, 128, 1);
    RIT128x96x4ImageDraw(hframe, 0, 95, 128, 1);
    RIT128x96x4ImageDraw(lframe, 0, 1, 2, 94);
    RIT128x96x4ImageDraw(rframe, 127, 1, 2, 94);

    //print the grid to the screen (helps with coordinating movement
	drawGrid();


}

// The OLED Task definition
void OLEDTask(void *pvParameters){
	// The delay between executions
	unsigned long delay = TICK_R * 250.0;
	int i = 0;
	while(true) {
		drawPieces();
		drawMessage();
		vTaskDelay(delay);
	}
}

void drawGrid(){

		int i;
		char line[19];

		for(i = 0; i < 5; i++)
		{
			RIT128x96x4StringDraw(". . . . . . . . . .", 6, 4 + i*16, 1);
		}
}


//This draws the whole game board
void drawPieces()
{
	int i;
	for(i = 0; i < 17; i++)
	{
		Row0[i] = 32;
		Row1[i] = 32;
		Row2[i] = 32;
		Row3[i] = 32;
	}
	for(i = 0; i < 9; i++)
	{
		//Row0:
		if(piecesArray[i][0] == 1)
			Row0[i*2] = '@';
		if(piecesArray[i][0] == 2)
			Row0[i*2] = '&';
		if(piecesArray[i][0] == 3)
			Row0[i*2] = '~';
		if(piecesArray[i][0] == 4)
			Row0[i*2] = '#';
		if(piecesArray[i][0] == 5)
			Row0[i*2] = '#';
		if(piecesArray[i][0] == 6)
			Row0[i*2] = '@';
		if(piecesArray[i][0] == 7)
			Row0[i*2] = '@';
		if(piecesArray[i][0] == 8)
			Row0[i*2] = '@';
		if(piecesArray[i][0] == 9)
			Row0[i*2] = '@';

		//Row1:
		if(piecesArray[i][1] == 1)
			Row1[i*2] = '@';
		if(piecesArray[i][1] == 2)
			Row1[i*2] = '&';
		if(piecesArray[i][1] == 3)
			Row1[i*2] = '~';
		if(piecesArray[i][1] == 4)
			Row1[i*2] = '#';
		if(piecesArray[i][1] == 5)
			Row1[i*2] = '#';
		if(piecesArray[i][1] == 6)
			Row1[i*2] = '@';
		if(piecesArray[i][1] == 7)
			Row1[i*2] = '@';
		if(piecesArray[i][1] == 8)
			Row1[i*2] = '@';
		if(piecesArray[i][1] == 9)
			Row1[i*2] = '@';

		//Row2:
		if(piecesArray[i][2] == 1)
			Row2[i*2] = '@';
		if(piecesArray[i][2] == 2)
			Row2[i*2] = '&';
		if(piecesArray[i][2] == 3)
			Row2[i*2] = '~';
		if(piecesArray[i][2] == 4)
			Row2[i*2] = '#';
		if(piecesArray[i][2] == 5)
			Row2[i*2] = '#';
		if(piecesArray[i][2] == 6)
			Row2[i*2] = '@';
		if(piecesArray[i][2] == 7)
			Row2[i*2] = '@';
		if(piecesArray[i][2] == 8)
			Row2[i*2] = '@';
		if(piecesArray[i][2] == 9)
			Row2[i*2] = '@';

		//Row3:
		if(piecesArray[i][3] == 1)
			Row3[i*2] = '@';
		if(piecesArray[i][3] == 2)
			Row3[i*2] = '&';
		if(piecesArray[i][3] == 3)
			Row3[i*2] = '~';
		if(piecesArray[i][3] == 4)
			Row3[i*2] = '#';
		if(piecesArray[i][3] == 5)
			Row3[i*2] = '#';
		if(piecesArray[i][3] == 6)
			Row3[i*2] = '@';
		if(piecesArray[i][3] == 7)
			Row3[i*2] = '@';
		if(piecesArray[i][3] == 8)
			Row3[i*2] = '@';
		if(piecesArray[i][3] == 9)
			Row3[i*2] = '@';
	}

	//Print the rows (these four are strings containing the whole game layout
	RIT128x96x4StringDraw(Row0, 8 + 4, 16 - 2,15);
	RIT128x96x4StringDraw(Row1, 8 + 4, 32 - 2,15);
	RIT128x96x4StringDraw(Row2, 8 + 4, 48 - 2,15);
	RIT128x96x4StringDraw(Row3, 8 + 4, 64 - 2,15);
}


//This Prints the messages to the user about functionality and what to do
void drawMessage()
{
	char wormsFoundMessage[19];

	//use this to correct right frame
	unsigned char rframe[94];//, blank[62];
	int i;
	for(i = 0; i < 94; i++){
		rframe[i] = 0x0F; 				// Vertical line for the right edge
	}

	if(messageFlag == 0)
	{
		RIT128x96x4StringDraw("                     ", 3, 80 - 2,15);
	}
	if(messageFlag == 1)
	{
		sprintf(wormsFoundMessage, " Worms Found: %d of %d ", wormsFound - wormsFed, totalWormsNeeded - wormsFed);
		RIT128x96x4StringDraw(wormsFoundMessage, 3, 80 - 2,15);
	}
	if(messageFlag == 2)
	{
		RIT128x96x4StringDraw("    Drop Worms: #    ", 3, 80 - 2,15);
	}
	if(messageFlag == 3)
	{
		RIT128x96x4StringDraw("    Peck Dirt: *     ", 3, 80 - 2,15);
	}
	if(messageFlag == 4)
	{
		RIT128x96x4StringDraw("     Worm Found!     ", 3, 80 - 2,15);
	}
	if(messageFlag == 5)
	{
		RIT128x96x4StringDraw("Go to your baby bird!", 3, 80 - 2,15);
	}
	if(messageFlag == 6)
	{
		RIT128x96x4StringDraw("Baby's Full! You Win!", 3, 80 - 2,15);
	}
	if(messageFlag == 7)
	{
		RIT128x96x4StringDraw("  Beak full of mud!  ", 3, 80 - 2,15);
	}
	if(messageFlag == 8)
	{
		RIT128x96x4StringDraw("     Yummy Worms!    ", 3, 80 - 2,15);
	}
	if(messageFlag == 9)
	{
		RIT128x96x4StringDraw("Use Num Keys to move!", 3, 80 - 2,15);
	}
	RIT128x96x4ImageDraw(rframe, 127, 1, 2, 94);
}
