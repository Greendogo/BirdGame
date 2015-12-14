/*
 * speaker.h
 *
 *  Created on: Sep 14, 2015
 *	Updated on: Sep 28, 2015
 *      Author: pmcelroy
 *	speakerInit(void) initializes the speaker by enabling port H and setting pins 0 and 1 to low and high respectively; it also sets the initial target time to execute to sysTickCount + period, where sysTickCount is set outside of the function to give the sound a frequency close 440Hz.
 */

#ifndef EECS_388_FREERTOS__SRC_SPEAKER_H_
#define EECS_388_FREERTOS__SRC_SPEAKER_H_

//this is external because buttons.c sets this depending on which of five buttons is pressed down
extern unsigned long speakerPeriod;

//This is 0 when the speaker is off, 1 when the speaker is on
extern int speakerPlay;

//initialize the speaker by enabling Port H pins 0 and 1 to opposite values 0 and 1 and setting the target time to sysTickCount + period.
extern void speakerTask(void*);

extern void playSound(short freq, short duration);

extern void playSong(int messageNum);//, unsigned long* song[2]);

#endif /* EECS_388_FREERTOS__SRC_SPEAKER_H_ */
