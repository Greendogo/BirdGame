/*
 * game.h
 *
 *  Created on: Nov 2, 2015
 *      Author: pmcelroy
 */
//
//updated by Paul McElroy on 11/9/2015
//For use in the Baby Bird Feeding Game

#ifndef SRC_BUTTONS_H_
#define SRC_BUTTONS_H_

extern void buttonsTask(void*);

extern int buttonsThisTick[12];
extern int buttonsLastTick[12];
extern int buttonsReleased[12];

//Movement Buttons
#define NORTH buttonsReleased[4]
#define NORTHWEST buttonsReleased[0]
#define WEST buttonsReleased[1]
#define SOUTHWEST buttonsReleased[2]
#define SOUTH buttonsReleased[6]
#define SOUTHEAST buttonsReleased[10]
#define EAST buttonsReleased[9]
#define NORTHEAST buttonsReleased[8]

//Control Buttons
#define CENTER buttonsReleased[5]
#define ASTERISK buttonsReleased[3]
#define ZERO buttonsReleased[7]
#define POUND buttonsReleased[11]


#endif /* SRC_BUTTONS_H_ */
