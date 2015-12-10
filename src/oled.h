/*
 * oled.h
 *
 *  Created on: Apr 21, 2015
 *      Author: Brad Torrence
 */

/*****************************************************************************
 * Task to update the OLED.
 ****************************************************************************/

#ifndef OLED_H_
#define OLED_H_

/*
 *  The task definition for the LED controller
 */
extern void startScreen(void);
extern void OLEDTask(void*);

void drawGrid();
void drawPieces();
void drawMessage();
void drawHunger();
void drawPiece(char type, unsigned long ulx, unsigned long uly);
void drawPopup(int x, int y, int length, char* text);

#endif /* OLED_H_ */
