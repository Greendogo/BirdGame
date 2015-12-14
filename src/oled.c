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

#include "time.h"
#include "stdio.h"
#include "buttons.h"
#include "game.h"
#include "speaker.h"

unsigned char lframe[94], rframe[94], hframe[64], blank[62];

char Row0[9];
char Row1[9];
char Row2[9];
char Row3[9];

unsigned long selectTimer;
int lastMessage;

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

    // The loop is used to initialize the arrays
    unsigned int i;
    for(i = 0; i < 94; i++){
    	lframe[i] = 0xF0; 				// Vertical line for the left edge
    	rframe[i] = 0x0F; 				// Vertical line for the right edge
    	if(i < 64) hframe[i] = 0xFF; 	// Horizontal line for top and bottom
    	if(i < 62) blank[i] = 0x00; 	// The blank line used to clear the screen
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
	RIT128x96x4StringDraw("Momma Bird (You): ", 3, 32-3, 15);
	RIT128x96x4StringDraw("Baby Bird:", 3, 40-3, 15);
	RIT128x96x4StringDraw("Worms:", 3, 48-3, 15);
	RIT128x96x4StringDraw("Dirt Patch:", 3, 56-3, 15);
	RIT128x96x4StringDraw("(Worms hide in Dirt)", 3, 64-3, 15);
	RIT128x96x4StringDraw("Keep Baby from Fox:", 3, 72-3, 15);
	RIT128x96x4StringDraw("Press \"Select\"", 20, 80-3, 15);
	RIT128x96x4StringDraw("To Continue", 32, 88-3, 15);

	drawPiece('@', 111 - 3, 29);
	drawPiece('&', 69, 37);
	drawPiece('~', 69 + 12 + 6, 45 - 3);
	drawPiece('#', 69 + 24 + 12, 53 - 7);
	drawPiece('F', 111, 69 + 9);
	unsigned long begin, end;

	selectTimer = 0;
	// Enable the select button, wait for the press, then disable and reset the port
	// The button is disabled after the splash screen and the port is reset to default for future use.
	GPIOPinTypeGPIOInput(GPIO_PORTG_BASE, GPIO_PIN_7); 	// Set the select button signal PG7 as input
	while(GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_7)) // Wait until the signal changes (button pressed)
	{
		selectTimer++;	//Used to seed a random number generator in game.c
	}
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
	firstMove = 1;
	while(true) {
		if(gameOver == 0)
		{
			drawPieces();
			drawHunger();
			drawMessage();
		}
		if(messageFlag == 6 || messageFlag == 12 || messageFlag == 18)
		{
			drawHunger();
			drawMessage();
		}
		if(gameOver == 3)
		{
			messageFlag = 0;
			RIT128x96x4Clear();
		    RIT128x96x4ImageDraw(hframe, 0, 0, 128, 1);
		    RIT128x96x4ImageDraw(hframe, 0, 95, 128, 1);
		    RIT128x96x4ImageDraw(lframe, 0, 1, 2, 94);
		    RIT128x96x4ImageDraw(rframe, 127, 1, 2, 94);
			drawGrid();
			createBoard();
		}
		//RIT128x96x4ImageDraw(hframe, 3, 80 - 4, 128, 1);
		vTaskDelay(delay);
	}
}

void drawGrid(){

		int i;
		char line[19];

		for(i = 0; i < 5; i++)
		{
			RIT128x96x4StringDraw(". . . . . . . . . .", 6, 4 + i*16, 2);
		}
}


//This draws the whole game board
//Updated to be compatible with drawPiece instead of StringDraw.
void drawPieces()
{
	int i;
	for(i = 0; i < 9; i++)
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
			Row0[i] = '@';
		if(piecesArray[i][0] == 2)
			Row0[i] = '&';
		if(piecesArray[i][0] == 3)
			Row0[i] = '~';
		if(piecesArray[i][0] == 4)
			Row0[i] = '#';
		if(piecesArray[i][0] == 5)
			Row0[i] = '#';
		if(piecesArray[i][0] == 6)
			Row0[i] = '@';
		if(piecesArray[i][0] == 7)
			Row0[i] = '@';
		if(piecesArray[i][0] == 8)
			Row0[i] = '@';
		if(piecesArray[i][0] == 9)
			Row0[i] = '@';
		if(piecesArray[i][0] == 10)
			Row0[i] = 'F';
		if(piecesArray[i][0] == 11)
			Row0[i] = '@';
		if(piecesArray[i][0] == 12)
			Row0[i] = 'O';
		if(piecesArray[i][0] == 13)
			Row0[i] = 'F';
		if(piecesArray[i][0] == 14)
			Row0[i] = '@';
		if(piecesArray[i][0] == 15)
			Row0[i] = '@';

		//Row1:
		if(piecesArray[i][1] == 1)
			Row1[i] = '@';
		if(piecesArray[i][1] == 2)
			Row1[i] = '&';
		if(piecesArray[i][1] == 3)
			Row1[i] = '~';
		if(piecesArray[i][1] == 4)
			Row1[i] = '#';
		if(piecesArray[i][1] == 5)
			Row1[i] = '#';
		if(piecesArray[i][1] == 6)
			Row1[i] = '@';
		if(piecesArray[i][1] == 7)
			Row1[i] = '@';
		if(piecesArray[i][1] == 8)
			Row1[i] = '@';
		if(piecesArray[i][1] == 9)
			Row1[i] = '@';
		if(piecesArray[i][1] == 10)
			Row1[i] = 'F';
		if(piecesArray[i][1] == 11)
			Row1[i] = '@';
		if(piecesArray[i][1] == 12)
			Row1[i] = 'O';
		if(piecesArray[i][1] == 13)
			Row1[i] = 'F';
		if(piecesArray[i][1] == 14)
			Row1[i] = '@';
		if(piecesArray[i][1] == 15)
			Row1[i] = '@';

		//Row2:
		if(piecesArray[i][2] == 1)
			Row2[i] = '@';
		if(piecesArray[i][2] == 2)
			Row2[i] = '&';
		if(piecesArray[i][2] == 3)
			Row2[i] = '~';
		if(piecesArray[i][2] == 4)
			Row2[i] = '#';
		if(piecesArray[i][2] == 5)
			Row2[i] = '#';
		if(piecesArray[i][2] == 6)
			Row2[i] = '@';
		if(piecesArray[i][2] == 7)
			Row2[i] = '@';
		if(piecesArray[i][2] == 8)
			Row2[i] = '@';
		if(piecesArray[i][2] == 9)
			Row2[i] = '@';
		if(piecesArray[i][2] == 10)
			Row2[i] = 'F';
		if(piecesArray[i][2] == 11)
			Row2[i] = '@';
		if(piecesArray[i][2] == 12)
			Row2[i] = 'O';
		if(piecesArray[i][2] == 13)
			Row2[i] = 'F';
		if(piecesArray[i][2] == 14)
			Row2[i] = '@';
		if(piecesArray[i][2] == 15)
			Row2[i] = '@';

		//Row3:
		if(piecesArray[i][3] == 1)
			Row3[i] = '@';
		if(piecesArray[i][3] == 2)
			Row3[i] = '&';
		if(piecesArray[i][3] == 3)
			Row3[i] = '~';
		if(piecesArray[i][3] == 4)
			Row3[i] = '#';
		if(piecesArray[i][3] == 5)
			Row3[i] = '#';
		if(piecesArray[i][3] == 6)
			Row3[i] = '@';
		if(piecesArray[i][3] == 7)
			Row3[i] = '@';
		if(piecesArray[i][3] == 8)
			Row3[i] = '@';
		if(piecesArray[i][3] == 9)
			Row3[i] = '@';
		if(piecesArray[i][3] == 10) //fox
			Row3[i] = 'F';
		if(piecesArray[i][3] == 11) //fox + player
			Row3[i] = '@';
		if(piecesArray[i][3] == 12) //den
			Row3[i] = 'O';
		if(piecesArray[i][3] == 13) //den + fox
			Row3[i] = 'F';
		if(piecesArray[i][3] == 14) //den + player
			Row3[i] = '@';
		if(piecesArray[i][3] == 15) //den + player + fox
			Row3[i] = '@';
	}

	//Print the rows (these four are strings containing the whole game layout
	//RIT128x96x4StringDraw(Row0, 8 + 4, 16 - 2,15);
	//RIT128x96x4StringDraw(Row1, 8 + 4, 32 - 2,15);
	//RIT128x96x4StringDraw(Row2, 8 + 4, 48 - 2,15);
	//RIT128x96x4StringDraw(Row3, 8 + 4, 64 - 2,15);
	i = 0;

	//Draw the piece tiles (uses drawPiece instead which uses their icons)
	for(i = 0; i < 9; i++)
	{
		drawPiece(Row0[i], 9 + 12*i, 11 + 16*0);
		drawPiece(Row1[i], 9 + 12*i, 11 + 16*1);
		drawPiece(Row2[i], 9 + 12*i, 11 + 16*2);
		drawPiece(Row3[i], 9 + 12*i, 11 + 16*3);
	}
}


//This Prints the messages to the user about functionality and what to do
void drawMessage()
{
	char wormsFoundMessage[19];

	if(messageFlag != lastMessage || messageFlag == 1)
	{
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
			RIT128x96x4StringDraw("      You Win!      ", 3, 80 - 2,15);
			drawPopup(24, 28, 13, " New Game: * ");
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
		if(messageFlag == 10)
		{
			RIT128x96x4StringDraw(" A Fox has appeared! ", 3, 80 - 2,15);
		}
		if(messageFlag == 11)
		{
			RIT128x96x4StringDraw("Fox stole your Baby! ", 3, 80 - 2,15);
		}
		if(messageFlag == 12)
		{
			RIT128x96x4StringDraw("      You Lose!     ", 3, 80 - 2,15);
			drawPopup(24, 28, 13, " New Game: * ");
		}
		if(messageFlag == 13)
		{
			RIT128x96x4StringDraw("   Pick up Baby: 5   ", 3, 80 - 2,15);
		}
		if(messageFlag == 14)
		{
			RIT128x96x4StringDraw("   Drop the Baby: 5  ", 3, 80 - 2,15);
		}
		if(messageFlag == 15)
		{
			RIT128x96x4StringDraw("   Peck the Fox! *   ", 3, 80 - 2,15);
		}
		if(messageFlag == 16)
		{
			RIT128x96x4StringDraw("    Dropped Baby!    ", 3, 80 - 2,15);
		}
		if(messageFlag == 17)
		{
			RIT128x96x4StringDraw("   Picked up Baby!   ", 3, 80 - 2,15);
		}
		if(messageFlag == 18)
		{
			RIT128x96x4StringDraw("      You Lose!      ", 3, 80 - 2,15);
			drawPopup(24, 28, 13, " New Game: * ");
		}
		if(messageFlag == 19)
		{
			RIT128x96x4StringDraw("Baby is Hungry! Feed!", 3, 80 - 2,15);
		}
		if(messageFlag == 20)
		{
			RIT128x96x4StringDraw("    Baby is full!    ", 3, 80 - 2,15);
		}
		RIT128x96x4ImageDraw(rframe, 127, 1, 2, 94);
		int i = 0;
		RIT128x96x4ImageDraw(hframe, 0, 80 - 4, 128, 1);
	}
	lastMessage = messageFlag;
}

static const unsigned char gameTiles[10][70+14] =
{
		//Blank Space (Clear)
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		},
		//Momma Bird (Player)
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00,
			0x00, 0x0F, 0xFF, 0xFF, 0x00, 0x00,
			0x00, 0xF7, 0xFF, 0xF7, 0x00, 0x00,
			0x0F, 0xFF, 0xFF, 0xFF, 0x00, 0xF0,
			0x0F, 0xFF, 0xFF, 0xF7, 0x0F, 0xF0,
			0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x70,
			0x00, 0x0F, 0xFF, 0xF7, 0xF7, 0x00,
			0x00, 0x0F, 0xF7, 0x7F, 0xF0, 0x00,
			0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
			0x00, 0xFF, 0x00, 0x0F, 0xF7, 0x00,
			0x0F, 0xF7, 0x00, 0x00, 0x0F, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x0F, 0x00
		},
		//Momma Bird (Player) - Facing Right
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00,
			0x00, 0x00, 0xFF, 0xFF, 0xF0, 0x00,
			0x00, 0x00, 0x7F, 0xFF, 0x7F, 0x00,
			0x0F, 0x00, 0xFF, 0xFF, 0xFF, 0xF0,
			0x0F, 0xF0, 0x7F, 0xFF, 0xFF, 0xF0,
			0x07, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
			0x00, 0x7F, 0x7F, 0xFF, 0xF0, 0x00,
			0x00, 0x0F, 0xF7, 0x7F, 0xF0, 0x00,
			0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
			0x00, 0x7F, 0xF0, 0x00, 0xFF, 0x00,
			0x00, 0xF0, 0x00, 0x00, 0x7F, 0xF0,
			0x00, 0xF0, 0x00, 0x00, 0x00, 0x00
		},
		//Baby
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0xD6, 0x00, 0x00, 0x00,
			0x00, 0x0D, 0x2D, 0x00, 0x00, 0x00,
			0x00, 0xEE, 0xDD, 0xDD, 0x66, 0x00,
			0x00, 0x00, 0xD6, 0x66, 0xD0, 0x00,
			0x00, 0x00, 0x0D, 0xDD, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x60, 0x00, 0x00,
			0x00, 0x00, 0x06, 0x60, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		},
		//Baby - Facing Right
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x6D, 0x00, 0x00,
			0x00, 0x00, 0x00, 0xD2, 0xD0, 0x00,
			0x00, 0x66, 0xDD, 0xDD, 0xEE, 0x00,
			0x00, 0xD0, 0x66, 0x6D, 0x00, 0x00,
			0x00, 0x00, 0xDD, 0xD0, 0x00, 0x00,
			0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x06, 0x60, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		},
		//Worm
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00,
			0x00, 0xFF, 0x0F, 0xF0, 0x00, 0x00,
			0x00, 0xF0, 0x00, 0xF0, 0x00, 0x00,
			0x00, 0x00, 0x00, 0xF0, 0x00, 0x00,
			0x00, 0x00, 0x0F, 0xF0, 0x0F, 0x00,
			0x00, 0x00, 0x0F, 0x00, 0xF0, 0xF0,
			0x00, 0x00, 0x00, 0xFF, 0xF0, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		},
		//Dirt
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x06, 0x60, 0x66, 0x00, 0x00,
			0x09, 0x00, 0x97, 0x30, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x44, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x09, 0x60, 0x00, 0x90, 0x00,
			0x00, 0x90, 0x00, 0x00, 0x66, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		},
		//Fox
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x09, 0xF0, 0x00,
			0x06, 0x00, 0x06, 0x09, 0x9F, 0x00,
			0x0D, 0x60, 0x6D, 0x09, 0x99, 0xF0,
			0x09, 0x99, 0x99, 0x00, 0x99, 0x90,
			0x09, 0x2C, 0x29, 0x00, 0x00, 0x90,
			0x0F, 0xC9, 0xCF, 0x00, 0x99, 0x90,
			0x00, 0xF2, 0xF9, 0x99, 0x99, 0x90,
			0x00, 0x09, 0x99, 0x99, 0x99, 0x00,
			0x00, 0x00, 0x96, 0x00, 0x69, 0x00,
			0x00, 0x00, 0x96, 0x00, 0x06, 0x90,
			0x00, 0x09, 0x60, 0x00, 0x69, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		},
		//Fox - Facing Right
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x0F, 0x90, 0x00, 0x00, 0x00,
			0x00, 0xF9, 0x90, 0x60, 0x00, 0x60,
			0x0F, 0x99, 0x90, 0xD6, 0x06, 0xD0,
			0x09, 0x99, 0x00, 0x99, 0x99, 0x90,
			0x09, 0x00, 0x00, 0x92, 0xC2, 0x90,
			0x09, 0x99, 0x00, 0xFC, 0x9C, 0xF0,
			0x09, 0x99, 0x99, 0x9F, 0x2F, 0x00,
			0x00, 0x99, 0x99, 0x99, 0x90, 0x00,
			0x00, 0x96, 0x00, 0x69, 0x00, 0x00,
			0x09, 0x60, 0x00, 0x69, 0x00, 0x00,
			0x00, 0x96, 0x00, 0x06, 0x90, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		},
		//Fox Den
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x66, 0x66, 0x66, 0x66, 0x00,
			0x06, 0x6F, 0xFF, 0xFF, 0xF6, 0x60,
			0x06, 0xFC, 0xCC, 0xCC, 0xCF, 0x60,
			0x0F, 0x96, 0x66, 0x66, 0x69, 0xF0,
			0x06, 0xF0, 0x00, 0x00, 0x0F, 0x60,
			0x06, 0x6F, 0xFF, 0xFF, 0xF6, 0x60,
			0x00, 0x66, 0x66, 0x66, 0x66, 0x00
		}
};

void drawPiece(char type, unsigned long ulx, unsigned long uly)
{
	if(type == ' ')
		RIT128x96x4ImageDraw(gameTiles[0], ulx, uly, 2*6, 14);
	else if(type == '@')
	{
		if(playerFacingRight == 0)
			RIT128x96x4ImageDraw(gameTiles[1], ulx, uly, 2*6, 14);
		else
			RIT128x96x4ImageDraw(gameTiles[2], ulx, uly, 2*6, 14);
	}

	else if(type == '&')
	{
		if(babyFacingRight == 0)
			RIT128x96x4ImageDraw(gameTiles[3], ulx, uly, 2*6, 14);
		else
			RIT128x96x4ImageDraw(gameTiles[4], ulx, uly, 2*6, 14);
	}
	else if(type == '~')
		RIT128x96x4ImageDraw(gameTiles[5], ulx, uly, 2*6, 14);
	else if(type == '#')
		RIT128x96x4ImageDraw(gameTiles[6], ulx, uly, 2*6, 14);
	else if(type == 'F')
	{
		if(foxFacingRight == 0)
			RIT128x96x4ImageDraw(gameTiles[7], ulx, uly, 2*6, 14);
		else
			RIT128x96x4ImageDraw(gameTiles[8], ulx, uly, 2*6, 14);
	}
	else if(type == 'O')
		RIT128x96x4ImageDraw(gameTiles[9], ulx, uly, 2*6, 14);
}

void drawHunger()
{
	if(firstMove == 1)
		babyHunger = 13;
	else if(firstMove == 0)
	{
		if(messageFlag == 6)
		{
			RIT128x96x4StringDraw("Hunger: Baby's Full!", 3, 80 - 2 + 8,15);
		}
		else if(messageFlag == 12)
		{
			RIT128x96x4StringDraw("Hunger: Baby's Gone!", 3, 80 - 2 + 8,15);
		}
		else if(babyHunger >= 13)
		{
			RIT128x96x4StringDraw("Hunger:|||||||||||||", 3, 80 - 2 + 8,15);
		}
		else if(babyHunger == 12)
			RIT128x96x4StringDraw("Hunger:|||||||||||| ", 3, 80 - 2 + 8,15);
		else if(babyHunger == 11)
			RIT128x96x4StringDraw("Hunger:|||||||||||  ", 3, 80 - 2 + 8,15);
		else if(babyHunger == 10)
			RIT128x96x4StringDraw("Hunger:||||||||||   ", 3, 80 - 2 + 8,15);
		else if(babyHunger == 9)
			RIT128x96x4StringDraw("Hunger:|||||||||    ", 3, 80 - 2 + 8,15);
		else if(babyHunger == 8)
			RIT128x96x4StringDraw("Hunger:||||||||     ", 3, 80 - 2 + 8,15);
		else if(babyHunger == 7)
			RIT128x96x4StringDraw("Hunger:|||||||      ", 3, 80 - 2 + 8,15);
		else if(babyHunger == 6)
			RIT128x96x4StringDraw("Hunger:||||||       ", 3, 80 - 2 + 8,15);
		else if(babyHunger == 5)
			RIT128x96x4StringDraw("Hunger:|||||        ", 3, 80 - 2 + 8,15);
		else if(babyHunger == 4)
		{
			RIT128x96x4StringDraw("Hunger:||||         ", 3, 80 - 2 + 8,15);
		}
		else if(babyHunger == 3)
		{
			RIT128x96x4StringDraw("Hunger:|||          ", 3, 80 - 2 + 8,15);
		}
		else if(babyHunger == 2)
		{
			RIT128x96x4StringDraw("Hunger:||           ", 3, 80 - 2 + 8,15);
		}
		else if(babyHunger == 1)
		{
			RIT128x96x4StringDraw("Hunger:|            ", 3, 80 - 2 + 8,15);
		}
		else if(babyHunger <= 0)
		{
			RIT128x96x4StringDraw("Hunger: Baby Starved", 3, 80 - 2 + 8,15);
		}

	}
}

void drawPopup(int x, int y, int length, char* text)
{
	int i;
	int height = 18;
	for(i = y; i < y + height; i++)
		RIT128x96x4ImageDraw(blank, x, i, length*6, 1);
	RIT128x96x4StringDraw(text, x + 2, y + 6,15);
	RIT128x96x4ImageDraw(lframe, x, y, 2, height + 1);
	RIT128x96x4ImageDraw(rframe, x + length*6, y, 2, height + 1);
	RIT128x96x4ImageDraw(hframe, x, y, length*6 + 2, 1);
	RIT128x96x4ImageDraw(hframe, x, y + height, length*6 + 2, 1);
}
