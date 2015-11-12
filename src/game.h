/*
 * game.h
 *
 *  Created on: Nov 8, 2015
 *      Author: pmcelroy
 */
//
//updated by Paul McElroy on 11/9/2015
//For use in the Baby Bird Feeding Game

#ifndef SRC_GAME_H_
#define SRC_GAME_H_

extern void gameTask(void*);

extern int piecesArray[9][4];

extern int messageFlag;

extern int totalWormsNeeded;
extern int wormsFound;
extern int wormsFed;

extern int PlayerX, PlayerY;

int getPlayerRow();
int getPlayerCol();
void setPlayer(int col, int row);
void removePlayer();
int isValid(int col, int row);

#endif /* SRC_GAME_H_ */
