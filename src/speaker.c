/*
 * speaker.c
 *
 *  Created on: Sep 14, 2015
 *	Updated on: Sep 28, 2015
 *      Author: pmcelroy
 */

#include "speaker.h"
#include "buttons.h"
// The system.h module will be required in all future task definitions
// This will give the task access to the base libraries as well as the global variables
#include "system.h"

// Stellaris driver libraries required by these local function definitions
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

//initialize target time to 0; will be set correctly in-function.
static unsigned long target = 0;
//set period to make sure we get the correct frequency
unsigned long periodConstant = SYSTICK_FREQUENCY/(2);
//periodb is set by button presses in the button.c code
unsigned long speakerPeriod;

unsigned long countEnd;
int speakerBusy;

short songNotes[3];
short songTimes[3];
int length;
int note;

int rest;

//execute task
void speakerTask(void *pvParameters)
{
	speakerBusy = 0;
	//const unsigned long delay = periodb;
	//enable port H
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);

	//Set H pins 0 and 1 to output
	GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_0 | GPIO_PIN_1);//0x00000003);

	//Make pin 0 low and pin 1 high
	GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x01);

	countEnd = 0;
	length = 0;
	note = 1;
	speakerPeriod =  TICK_R*500;
	rest = 1;
	while(true)
	{
		//delay = periodb;
		if(sysTickCount <= countEnd)//check if it's time to execute and if the speaker has permission to play
		{
			//toggle the high/low status of pins 1 and 0
			if(rest == 0)
				GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIOPinRead(GPIO_PORTH_BASE, GPIO_PIN_0 | GPIO_PIN_1) ^ 0x03);
		}
		else if(note < length)
		{
			playSound(songNotes[note],songTimes[note]);
			note++;
		}
		else if(note == length)//To save processing
		{
			speakerPeriod = TICK_R;
			note = 1;
			length = 0;
			speakerBusy = 0;
		}
		vTaskDelay(speakerPeriod);
	}
}

void playSound(short freq, short duration)
{
	if(freq != 0)
		rest = 0;
	else
		rest = 1;
	countEnd = sysTickCount + duration*TICK_R;
	speakerPeriod = SYSTICK_FREQUENCY/(freq*2);
}

void playSong(int messageNum)//, unsigned long* song[2])
{
	int lengthOfSong = 0;
	if(speakerBusy == 0)
	{
//		short song1[2][9] ={{440,0,880,0,440,0,880,0,220},{100,100,100,100,100,100,100,100,100}};
		if(messageNum == 4)
		{
			speakerBusy = 1;
			lengthOfSong = 3;
			short sound[2][3] =	{
					{
					440,
					0,
					880
					},
					{
					75,
					75,
					75
					}
					};
			for(length = 0; length < lengthOfSong; length++)
			{
				songNotes[length] = sound[0][length];
				songTimes[length] = sound[1][length];
			}
			note = 0;
			length = lengthOfSong;
		}
		else if(messageNum == 7)
		{
			speakerBusy = 1;
			lengthOfSong = 3;
			short sound[2][3] = {
					{
					240,
					0,
					220
					},
					{
					75,
					75,
					75
					}
					};
			for(length = 0; length < lengthOfSong; length++)
			{
				songNotes[length] = sound[0][length];
				songTimes[length] = sound[1][length];
			}
			note = 0;
			length = lengthOfSong;
		}
		else if(messageNum == 6)
		{
			speakerBusy = 1;
			lengthOfSong = 3;
			short sound[2][3] = {
					{
					660,
					960,
					1060
					},
					{
					100,
					100,
					100
					}
					};
			for(length = 0; length < lengthOfSong; length++)
			{
				songNotes[length] = sound[0][length];
				songTimes[length] = sound[1][length];
			}
			note = 0;
			length = lengthOfSong;
		}
		else if(messageNum == 7)
		{
			speakerBusy = 1;
			lengthOfSong = 3;
			short sound[2][3] = {
					{
					240,
					0,
					220
					},
					{
					75,
					75,
					75
					}
					};
			for(length = 0; length < lengthOfSong; length++)
			{
				songNotes[length] = sound[0][length];
				songTimes[length] = sound[1][length];
			}
			note = 0;
			length = lengthOfSong;
		}
		else if(messageNum == 12 || messageNum == 18)
		{
			speakerBusy = 1;
			lengthOfSong = 3;
			short sound[2][3] = {
					{
					680,
					460,
					240
					},
					{
					100,
					100,
					100
					}
					};
			for(length = 0; length < lengthOfSong; length++)
			{
				songNotes[length] = sound[0][length];
				songTimes[length] = sound[1][length];
			}
			note = 0;
			length = lengthOfSong;
		}
		else if(messageNum == 8)
		{
			speakerBusy = 1;
			lengthOfSong = 3;
			short sound[2][3] = {
					{
					880,
					880,
					960
					},
					{
					100,
					100,
					100
					}
					};
			for(length = 0; length < lengthOfSong; length++)
			{
				songNotes[length] = sound[0][length];
				songTimes[length] = sound[1][length];
			}
			note = 0;
			length = lengthOfSong;
		}
		else if(messageNum == 10)
		{
			speakerBusy = 1;
			lengthOfSong = 3;
			short sound[2][3] = {
					{
					880,
					660,
					440
					},
					{
					100,
					100,
					100
					}
					};
			for(length = 0; length < lengthOfSong; length++)
			{
				songNotes[length] = sound[0][length];
				songTimes[length] = sound[1][length];
			}
			note = 0;
			length = lengthOfSong;
		}
		else if(messageNum == 17)
		{
			speakerBusy = 1;
			lengthOfSong = 3;
			short sound[2][3] = {
					{
					880,
					0,
					0
					},
					{
					100,
					20,
					20
					}
					};
			for(length = 0; length < lengthOfSong; length++)
			{
				songNotes[length] = sound[0][length];
				songTimes[length] = sound[1][length];
			}
			note = 0;
			length = lengthOfSong;
		}
		else if(messageNum == 16)
		{
			speakerBusy = 1;
			lengthOfSong = 3;
			short sound[2][3] = {
					{
					440,
					0,
					0
					},
					{
					100,
					20,
					20
					}
					};
			for(length = 0; length < lengthOfSong; length++)
			{
				songNotes[length] = sound[0][length];
				songTimes[length] = sound[1][length];
			}
			note = 0;
			length = lengthOfSong;
		}
		else if(messageNum == 11)
		{
			speakerBusy = 1;
			lengthOfSong = 3;
			short sound[2][3] = {
					{
					440,
					660,
					440
					},
					{
					100,
					100,
					100
					}
					};
			for(length = 0; length < lengthOfSong; length++)
			{
				songNotes[length] = sound[0][length];
				songTimes[length] = sound[1][length];
			}
			note = 0;
			length = lengthOfSong;
		}
	}
}
