/*
 * game.c
 *
 *  Created on: Nov 8, 2015
 *      Author: pmcelroy
 */
//
//updated by Paul McElroy on 11/9/2015
//For use in the Baby Bird Feeding Game
//Controls game logic and updates the piecesArray used by OLED.c
//Recieves commands via the releasedButtons array from buttons.c


#include "game.h"
#include "system.h"
#include "buttons.h"

int piecesArray[9][4];
//empty space = 0
//momma bird = 1
//baby bird = 2
//worm = 3
//grass = 4
//grass with worm = 5
//momma bird with baby bird = 6
//momma bird with worm = 7 //immediately pick it up
//momma bird with dirt = 8
//momma bird with dirt with worm = 9

int totalWormsNeeded;
int wormsFound;
int wormsFed;
int messageFlag;
int PlayerX, PlayerY;

void gameTask(void *pvParameters)
{
	unsigned long delay = TICK_R * 50.0;
	totalWormsNeeded = 5;
	wormsFound = 0;
	wormsFed = 0;

	int i, j;
	for(i = 0; i < 9; i++)
	{
		for(j = 0; j < 4; j++)
		{
			piecesArray[i][j] = 0;
		}
	}
	piecesArray[1][0] = 3;
	piecesArray[7][0] = 3;
	piecesArray[8][2] = 3;

	piecesArray[3][3] = 5;
	piecesArray[7][3] = 5;
	piecesArray[5][3] = 4;
	piecesArray[2][1] = 4;

	setPlayer(4,2);

	messageFlag = 9;
	piecesArray[6][1] = 2;

	for(i = 0; i < 12; i++)
	{
		buttonsReleased[i] = 0;
	}


	while(true) {
		if(NORTH == 1)
		{
			setPlayer(getPlayerCol(), getPlayerRow() - 1);
			NORTH = 0;
		}
		if(NORTHEAST == 1)
		{
			setPlayer(getPlayerCol() + 1, getPlayerRow() - 1);
			NORTHEAST = 0;
		}
		if(EAST == 1)
		{
			setPlayer(getPlayerCol() + 1, getPlayerRow());
			EAST = 0;
		}
		if(SOUTHEAST == 1)
		{
			setPlayer(getPlayerCol() + 1, getPlayerRow() + 1);
			SOUTHEAST = 0;
		}
		if(SOUTH == 1)
		{
			setPlayer(getPlayerCol(), getPlayerRow() + 1);
			SOUTH = 0;
		}
		if(SOUTHWEST == 1)
		{
			setPlayer(getPlayerCol() - 1, getPlayerRow() + 1);
			SOUTHWEST = 0;
		}
		if(WEST == 1)
		{
			setPlayer(getPlayerCol() - 1, getPlayerRow());
			WEST = 0;
		}
		if(NORTHWEST == 1)
		{
			setPlayer(getPlayerCol() - 1, getPlayerRow() - 1);
			NORTHWEST = 0;
		}
		if(ASTERISK == 1)
		{
			if(piecesArray[getPlayerCol()][getPlayerRow()] == 8)
			{
				piecesArray[getPlayerCol()][getPlayerRow()] = 1;
				messageFlag = 7;
			}
			else if(piecesArray[getPlayerCol()][getPlayerRow()] == 9)
			{
				piecesArray[getPlayerCol()][getPlayerRow()] = 1;
				wormsFound++;
				messageFlag = 4;
			}
			ASTERISK = 0;
		}
		if(POUND == 1)
		{
			if(piecesArray[getPlayerCol()][getPlayerRow()] == 6)
			{
				if(wormsFound - wormsFed > 0)
				{
					wormsFed += wormsFound - wormsFed;
					messageFlag = 8;
				}
			}
			POUND = 0;
		}

		if(wormsFound == totalWormsNeeded && piecesArray[getPlayerCol()][getPlayerRow()] != 6)
		{
			messageFlag = 5;
		}

		if(wormsFed == totalWormsNeeded)
		{
			messageFlag = 6;
		}
		vTaskDelay(delay);
	}
}

int getPlayerCol()
{
	return PlayerX;
}

int getPlayerRow()
{
	return PlayerY;
}

void setPlayer(int col, int row)
{
	if(isValid(col, row) == 1)
	{
		if(piecesArray[col][row] == 0)
		{
			messageFlag = 1;
			removePlayer();
			PlayerX = col;
			PlayerY = row;
			piecesArray[col][row] = 1;
		}
		else if(piecesArray[col][row] == 2)
		{
			removePlayer();
			PlayerX = col;
			PlayerY = row;
			piecesArray[col][row] = 6;
			if(wormsFound - wormsFed > 0)
			{
				messageFlag = 2;
			}
			else
				messageFlag = 1;
		}
		else if(piecesArray[col][row] == 3)
		{
			messageFlag = 4;
			removePlayer();
			PlayerX = col;
			PlayerY = row;
			piecesArray[col][row] = 1;
			wormsFound++;
		}
		else if(piecesArray[col][row] == 4)
		{
			messageFlag = 3;
			removePlayer();
			PlayerX = col;
			PlayerY = row;
			piecesArray[col][row] = 8;
		}
		else if(piecesArray[col][row] == 5)
		{
			messageFlag = 3;
			removePlayer();
			PlayerX = col;
			PlayerY = row;
			piecesArray[col][row] = 9;
		}
	}
}

void removePlayer()
{
	if(piecesArray[PlayerX][PlayerY] == 1)
	{
		piecesArray[PlayerX][PlayerY] = 0;
	}
	else if(piecesArray[PlayerX][PlayerY] == 6)
	{
		piecesArray[PlayerX][PlayerY] = 2;
	}
	else if(piecesArray[PlayerX][PlayerY] == 7)
	{
		piecesArray[PlayerX][PlayerY] = 3;
	}
	else if(piecesArray[PlayerX][PlayerY] == 8)
	{
		piecesArray[PlayerX][PlayerY] = 4;
	}
	else if(piecesArray[PlayerX][PlayerY] == 9)
	{
		piecesArray[PlayerX][PlayerY] = 5;
	}
}

int isValid(int col, int row)
{
	int valid = 1;
	if(row > 3 || row < 0)
		valid = 0;
	else if(col > 8 || col < 0)
		valid = 0;
	return valid;
}
//empty space = 0
//momma bird = 1
//baby bird = 2
//worm = 3
//grass = 4
//grass with worm = 5
//momma bird with baby bird = 6
//momma bird with worm = 7 //immediately pick it up
//momma bird with dirt = 8
//momma bird with dirt with worm = 9
