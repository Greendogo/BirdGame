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

#include "stdlib.h"
#include "time.h"

int piecesArray[9][4];
//empty space = 0
//momma bird = 1
//baby bird = 2
//worm = 3
//dirt = 4
//dirt with worm = 5
//momma bird with baby bird = 6
//momma bird with worm = 7 //immediately pick it up
//momma bird with dirt = 8
//momma bird with dirt with worm = 9

int totalWormsNeeded;
int wormsFound;
int wormsFed;
int messageFlag;
int PlayerX, PlayerY;
int babyX, babyY;
int foxX, foxY;
int foxOriginX, foxOriginY;

int playerFacingRight;
int babyFacingRight;
int foxFacingRight;
int babyCarryFlag;
int foxMovedLastTurn; //Used to toggle a walk/wait timing when fox has baby
int gameOver;
int firstMove;
int babyHunger;

void gameTask(void *pvParameters)
{
	unsigned long delay = TICK_R * 50.0;
	srand ( selectTimer );
	createBoard();

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
			if(gameOver == 1 || gameOver == 2 || gameOver == 4)
			{
				gameOver = 3;
			}
			else if(piecesArray[getPlayerCol()][getPlayerRow()] == 8)
			{
				piecesArray[getPlayerCol()][getPlayerRow()] = 1;
				setMessage(7);
			}
			else if(piecesArray[getPlayerCol()][getPlayerRow()] == 9)
			{
				piecesArray[getPlayerCol()][getPlayerRow()] = 1;
				wormsFound++;
				setMessage(4);
			}
			else if(piecesArray[getPlayerCol()][getPlayerRow()] == 11 && babyCarryFlag == 2)
			{
				dropBaby();
				babyCarryFlag = 0;
				setMessage(16);
			}
			else if(piecesArray[getPlayerCol()][getPlayerRow()] == 15 && babyCarryFlag == 2)
			{
				dropBaby();
				babyCarryFlag = 0;
				setMessage(16);
			}
			ASTERISK = 0;
		}
		if(POUND == 1)
		{
			if(piecesArray[getPlayerCol()][getPlayerRow()] == 6)
			{
				if(wormsFound - wormsFed > 0)
				{
					babyHunger += (wormsFound - wormsFed)*4;
					wormsFed += wormsFound - wormsFed;
					setMessage(8);
				}
			}
			POUND = 0;
		}
		if(CENTER == 1)
		{
			if(piecesArray[getPlayerCol()][getPlayerRow()] == 6 && babyCarryFlag == 0)
			{
				//pick up
				setMessage(17);
				piecesArray[getPlayerCol()][getPlayerRow()] = 1;
				babyCarryFlag = 1;
			}
			else if(piecesArray[getPlayerCol()][getPlayerRow()] == 1 && babyCarryFlag == 1)
			{
				//drop baby
				setMessage(16);
				piecesArray[getPlayerCol()][getPlayerRow()] = 6;
				babyCarryFlag = 0;
			}
			else if(piecesArray[getPlayerCol()][getPlayerRow()] != 1 && babyCarryFlag == 1)
			{
				//drop baby
				setMessage(16);
				dropBaby();
				babyCarryFlag = 0;
			}
			CENTER = 0;
		}

		if(wormsFed == totalWormsNeeded && gameOver != 2 && gameOver != 3 && gameOver != 4)
		{
			setMessage(6);
			gameOver = 2;
		}
		else if(wormsFound == totalWormsNeeded && piecesArray[getPlayerCol()][getPlayerRow()] != 6 && gameOver == 0 && babyCarryFlag != 2)
		{
			setMessage(5);
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
	if(gameOver == 0)
	{
		if(col < getPlayerCol())
			playerFacingRight = 0;
		else if(col > getPlayerCol())
			playerFacingRight = 1;

		if(isValid(col, row) == 1)
		{
			if(piecesArray[col][row] == 0)
			{
				setMessage(1);
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
					setMessage(2);
				}
				else
					setMessage(1);
			}
			else if(piecesArray[col][row] == 3)
			{
				setMessage(4);
				removePlayer();
				PlayerX = col;
				PlayerY = row;
				piecesArray[col][row] = 1;
				wormsFound++;
			}
			else if(piecesArray[col][row] == 4)
			{
				setMessage(3);
				removePlayer();
				PlayerX = col;
				PlayerY = row;
				piecesArray[col][row] = 8;
			}
			else if(piecesArray[col][row] == 5)
			{
				setMessage(3);
				removePlayer();
				PlayerX = col;
				PlayerY = row;
				piecesArray[col][row] = 9;
			}
			else if(piecesArray[col][row] == 10)
			{
				if(babyCarryFlag == 2)
					setMessage(15);
				removePlayer();
				PlayerX = col;
				PlayerY = row;
				piecesArray[col][row] = 11;
			}
			else if(piecesArray[col][row] == 12)
			{
				removePlayer();
				PlayerX = col;
				PlayerY = row;
				piecesArray[col][row] = 14;
			}
			else if(piecesArray[col][row] == 13)
			{
				if(babyCarryFlag == 2)
					setMessage(15);
				removePlayer();
				PlayerX = col;
				PlayerY = row;
				piecesArray[col][row] = 15;
			}

			if(firstMove == 0)
			{
				checkFox();
				babyHunger--;
				if(babyHunger <= 0)
				{
					setMessage(18);
					gameOver = 4;
				}
				else if(babyHunger <= 4)
				{
					setMessage(19);
				}
			}
			else
				firstMove = 0;
		}
	}
	else if(gameOver == 1)
	{
		setMessage(12);
	}
	else if(gameOver == 2)
	{
		setMessage(6);
	}
	else if(gameOver == 4)
	{
		setMessage(18);
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
	else if(piecesArray[PlayerX][PlayerY] == 11 && foxX != -1 && foxY != -1)
	{
		piecesArray[PlayerX][PlayerY] = 10; //
	}
	else if(piecesArray[PlayerX][PlayerY] == 14)
	{
		piecesArray[PlayerX][PlayerY] = 12;
	}
	else if(piecesArray[PlayerX][PlayerY] == 15)
	{
		piecesArray[PlayerX][PlayerY] = 13;
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

void createBoard()
{
	totalWormsNeeded = 5;
	wormsFound = 0;
	wormsFed = 0;

	babyHunger = 13;
	firstMove = 1;

	playerFacingRight = 0;
	babyFacingRight = 0;
	foxFacingRight = 0;

	int i, j;
	for(i = 0; i < 9; i++)
	{
		for(j = 0; j < 4; j++)
		{
			piecesArray[i][j] = 0;
		}
	}
	int col, row;
	col = rand() % 9;
	row = rand() % 4;
	//Place and orient player
	setPlayer(col, row);
	if(getPlayerCol() > 5)
		playerFacingRight = 0;
	else
		playerFacingRight = 1;

	dropBaby();

	randomPlacement(3);
	randomPlacement(3);
	randomPlacement(3);
	randomPlacement(4);
	randomPlacement(4);
	randomPlacement(5);
	randomPlacement(5);

	foxX = -1;
	foxY = -1;
	gameOver = 0;
	piecesArray[getPlayerCol()][getPlayerRow()] = 1;
	setMessage(9);

	for(i = 0; i < 12; i++)
	{
		buttonsReleased[i] = 0;
	}
}

void dropBaby()
{
	//Place and orient baby bird around momma
	babyFacingRight = playerFacingRight;
	int babyPos = 0;
	int babyIsPlaced = 0;
	do{
		babyPos = (rand() % 8);
		if(babyPos == 0 && isValid(getPlayerCol() - 1, getPlayerRow() - 1) && piecesArray[getPlayerCol() - 1][getPlayerRow() - 1] == 0)
		{
			piecesArray[getPlayerCol() - 1][getPlayerRow() - 1] = 2;
			babyIsPlaced = 1;
		}
		else if(babyPos == 1 && isValid(getPlayerCol(), getPlayerRow() - 1) && piecesArray[getPlayerCol()][getPlayerRow() - 1] == 0)
		{
					piecesArray[getPlayerCol()][getPlayerRow() - 1] = 2;
					babyIsPlaced = 1;
		}
		else if(babyPos == 2 && isValid(getPlayerCol() + 1, getPlayerRow() - 1) && piecesArray[getPlayerCol() + 1][getPlayerRow() - 1] == 0)
		{
					piecesArray[getPlayerCol() + 1][getPlayerRow() - 1] = 2;
					babyIsPlaced = 1;
		}
		else if(babyPos == 3 && isValid(getPlayerCol() + 1, getPlayerRow()) && piecesArray[getPlayerCol() + 1][getPlayerRow()] == 0)
		{
					piecesArray[getPlayerCol() + 1][getPlayerRow()] = 2;
					babyIsPlaced = 1;
		}
		else if(babyPos == 4 && isValid(getPlayerCol() + 1, getPlayerRow() + 1) && piecesArray[getPlayerCol() + 1][getPlayerRow() + 1] == 0)
		{
					piecesArray[getPlayerCol() + 1][getPlayerRow() + 1] = 2;
					babyIsPlaced = 1;
		}
		else if(babyPos == 5 && isValid(getPlayerCol(), getPlayerRow() + 1) && piecesArray[getPlayerCol()][getPlayerRow() + 1] == 0)
		{
					piecesArray[getPlayerCol()][getPlayerRow() + 1] = 2;
					babyIsPlaced = 1;
		}
		else if(babyPos == 6 && isValid(getPlayerCol() - 1, getPlayerRow() + 1) && piecesArray[getPlayerCol() - 1][getPlayerRow() + 1] == 0)
		{
					piecesArray[getPlayerCol() - 1][getPlayerRow() + 1] = 2;
					babyIsPlaced = 1;
		}
		else if(babyPos == 7 && isValid(getPlayerCol() - 1, getPlayerRow()) && piecesArray[getPlayerCol() - 1][getPlayerRow()] == 0)
		{
					piecesArray[getPlayerCol() - 1][getPlayerRow()] = 2;
					babyIsPlaced = 1;
		}
	} while(babyIsPlaced == 0);

	babyCarryFlag = 0;
}

void randomPlacement(int type)
{
	int placed = 0;
	int nextSpace, col, row;
	do{
		nextSpace = rand() % 35;
		col = nextSpace % 9;
		row = nextSpace/9;
		if(piecesArray[col][row] == 0)
		{
			if(type != 10)
			{
				piecesArray[col][row] = type;
				placed = 1;
			}
			else if(type == 10 && (col == 0 || col == 8 || row == 0 || row == 3))
			{
				piecesArray[col][row] = 13;
				foxX = col;
				foxY = row;
				foxOriginX = col;
				foxOriginY = row;
				placed = 1;
			}
		}
	} while(placed == 0);
}

void checkFox()
{
	//fox isn't on the board
		if(rand() % 10 == 5 && (foxX == -1 && foxY == -1))
		{
			randomPlacement(10);
			setMessage(10);
		}//fox is on the board
		else if((foxX != -1 && foxY != -1) && babyCarryFlag != 2)
		{
			int foxPos = (rand() % 8);
			if(foxPos == 0)
				moveFox(-1,-1);
			else if(foxPos == 1)
				moveFox(0,-1);
			else if(foxPos == 2)
				moveFox(1,-1);
			else if(foxPos == 3)
				moveFox(1,0);
			else if(foxPos == 4)
				moveFox(1,1);
			else if(foxPos == 5)
				moveFox(0,1);
			else if(foxPos == 6)
				moveFox(-1,1);
			else if(foxPos == 7)
				moveFox(-1,0);
		}//fox is on the board with the baby!
		else if((foxX != -1 && foxY != -1) && babyCarryFlag == 2)
		{
			if(foxMovedLastTurn == 1)
				foxMovedLastTurn = 0;
			else if(foxMovedLastTurn == 0)
			{
				foxMovedLastTurn = 1;
				if(piecesArray[foxX][foxY] != 13 && piecesArray[foxX][foxY] != 15)
				{
					int foxPos = (rand() % 8);
					if(foxPos == 0)
						moveFox(-1,-1);
					else if(foxPos == 1)
						moveFox(0,-1);
					else if(foxPos == 2)
						moveFox(1,-1);
					else if(foxPos == 3)
						moveFox(1,0);
					else if(foxPos == 4)
						moveFox(1,1);
					else if(foxPos == 5)
						moveFox(0,1);
					else if(foxPos == 6)
						moveFox(-1,1);
					else if(foxPos == 7)
						moveFox(-1,0);
				}
				else if(piecesArray[foxX][foxY] == 13)
				{
					piecesArray[foxX][foxY] = 12;
					foxX = -1;
					foxY = -1;
					gameOver = 1;
				}
				else if(piecesArray[foxX][foxY] == 15)
				{
					piecesArray[foxX][foxY] = 14;
					foxX = -1;
					foxY = -1;
					gameOver = 1;
				}
			}
		}
}

void moveFox(int col, int row)
{
	if(isValid(foxX + col, foxY + row) && foxX != -1 && foxY != -1)
	{
		if(piecesArray[foxX + col][foxY + row] == 0 || piecesArray[foxX + col][foxY + row] == 2)
		{
			if(piecesArray[foxX][foxY] != 11 && piecesArray[foxX][foxY] != 13 && piecesArray[foxX][foxY] != 15)
				piecesArray[foxX][foxY] = 0;
			else if(piecesArray[foxX][foxY] == 11)
				piecesArray[foxX][foxY] = 1;
			else if(piecesArray[foxX][foxY] == 13)
				piecesArray[foxX][foxY] = 12;
			else if(piecesArray[foxX][foxY] == 15)
				piecesArray[foxX][foxY] = 14;

			if(piecesArray[foxX + col][foxY + row] == 2 && babyCarryFlag == 0)
			{
				babyCarryFlag = 2; //Fox has baby!
				setMessage(11);
			}

			piecesArray[foxX + col][foxY + row] = 10;
			foxX += col;
			foxY += row;
		}
		else if(piecesArray[foxX + col][foxY + row] == 12 && babyCarryFlag == 2)
		{
			piecesArray[foxX][foxY] = 0;
			foxX += col;
			foxY += row;
			piecesArray[foxX][foxY] = 13;
		}
	}
}

void setMessage(int messageNum)
{
	if(messageFlag == 18 || messageFlag == 6 || messageFlag == 12)
		;
	if(messageFlag == 2 && messageNum == 19)
		;
	if(messageFlag == 2 && messageNum == 5)
		;
	else
		messageFlag = messageNum;
}
//empty space = 0
//momma bird = 1
//baby bird = 2
//worm = 3
//dirt = 4
//dirt with worm = 5
//momma bird with baby bird = 6
//momma bird with worm = 7 //immediately pick it up
//momma bird with dirt = 8
//momma bird with dirt with worm = 9
//fox = 10
//momma bird with fox = 11
