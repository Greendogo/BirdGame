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

short songNotes[312];
short songTimes[312];
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
			note = 0;
			length = 312;
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

void playSong(int lengthOfSong)//, unsigned long* song[2])
{
	if(speakerBusy == 0)
	{
		speakerBusy = 1;
//		short song1[2][9] ={{440,0,880,0,440,0,880,0,220},{100,100,100,100,100,100,100,100,100}};
		short song1[2][312] =
		{
				{
				660,
				0,
				660,
				0,
				660,
				0,
				510,
				0,
				660,
				0,
				770,
				0,
				380,
				0,
				510,
				0,
				380,
				0,
				320,
				0,
				440,
				0,
				480,
				0,
				450,
				0,
				430,
				0,
				380,
				0,
				660,
				0,
				760,
				0,
				860,
				0,
				700,
				0,
				760,
				0,
				660,
				0,
				520,
				0,
				580,
				0,
				480,
				0,
				510,
				0,
				380,
				0,
				320,
				0,
				440,
				0,
				480,
				0,
				450,
				0,
				430,
				0,
				380,
				0,
				660,
				0,
				760,
				0,
				860,
				0,
				700,
				0,
				760,
				0,
				660,
				0,
				520,
				0,
				580,
				0,
				480,
				0,
				500,
				0,
				760,
				0,
				720,
				0,
				680,
				0,
				620,
				0,
				650,
				0,
				380,
				0,
				430,
				0,
				500,
				0,
				430,
				0,
				500,
				0,
				570,
				0,
				500,
				0,
				760,
				0,
				720,
				0,
				680,
				0,
				620,
				0,
				650,
				0,
				1020,
				0,
				1020,
				0,
				1020,
				0,
				380,
				0,
				500,
				0,
				760,
				0,
				720,
				0,
				680,
				0,
				620,
				0,
				650,
				0,
				380,
				0,
				430,
				0,
				500,
				0,
				430,
				0,
				500,
				0,
				570,
				0,
				585,
				0,
				550,
				0,
				500,
				0,
				380,
				0,
				500,
				0,
				500,
				0,
				500,
				0,
				500,
				0,
				760,
				0,
				720,
				0,
				680,
				0,
				620,
				0,
				650,
				0,
				380,
				0,
				430,
				0,
				500,
				0,
				430,
				0,
				500,
				0,
				570,
				0,
				500,
				0,
				760,
				0,
				720,
				0,
				680,
				0,
				620,
				0,
				650,
				0,
				1020,
				0,
				1020,
				0,
				1020,
				0,
				380,
				0,
				500,
				0,
				760,
				0,
				720,
				0,
				680,
				0,
				620,
				0,
				650,
				0,
				380,
				0,
				430,
				0,
				500,
				0,
				430,
				0,
				500,
				0,
				570,
				0,
				585,
				0,
				550,
				0,
				500,
				0,
				380,
				0,
				500,
				0,
				500,
				0,
				500,
				0,
				500,
				0,
				500,
				0,
				500,
				0,
				500,
				0,
				580,
				0,
				660,
				0,
				500,
				0,
				430,
				0,
				380,
				0,
				500,
				0,
				500,
				0,
				500,
				0,
				500,
				0,
				580,
				0,
				660,
				0,
				870,
				0,
				760,
				0,
				500,
				0,
				500,
				0,
				500,
				0,
				500,
				0,
				580,
				0,
				660,
				0,
				500,
				0,
				430,
				0,
				380,
				0,
				660,
				0,
				660,
				0,
				660,
				0,
				510,
				0,
				660,
				0,
				770,
				0,
				380,
				0
				},
				{
				100,
				150,
				100,
				300,
				100,
				300,
				100,
				100,
				100,
				300,
				100,
				550,
				100,
				575,
				100,
				450,
				100,
				400,
				100,
				500,
				100,
				300,
				80,
				330,
				100,
				150,
				100,
				300,
				100,
				200,
				80,
				200,
				50,
				150,
				100,
				300,
				80,
				150,
				50,
				350,
				80,
				300,
				80,
				150,
				80,
				150,
				80,
				500,
				100,
				450,
				100,
				400,
				100,
				500,
				100,
				300,
				80,
				330,
				100,
				150,
				100,
				300,
				100,
				200,
				80,
				200,
				50,
				150,
				100,
				300,
				80,
				150,
				50,
				350,
				80,
				300,
				80,
				150,
				80,
				150,
				80,
				500,
				100,
				300,
				100,
				100,
				100,
				150,
				100,
				150,
				150,
				300,
				150,
				300,
				100,
				150,
				100,
				150,
				100,
				300,
				100,
				150,
				100,
				100,
				100,
				220,
				100,
				300,
				100,
				100,
				100,
				150,
				100,
				150,
				150,
				300,
				200,
				300,
				80,
				300,
				80,
				150,
				80,
				300,
				100,
				300,
				100,
				300,
				100,
				100,
				100,
				150,
				100,
				150,
				150,
				300,
				150,
				300,
				100,
				150,
				100,
				150,
				100,
				300,
				100,
				150,
				100,
				100,
				100,
				420,
				100,
				450,
				100,
				420,
				100,
				360,
				100,
				300,
				100,
				300,
				100,
				150,
				100,
				300,
				100,
				300,
				100,
				100,
				100,
				150,
				100,
				150,
				150,
				300,
				150,
				300,
				100,
				150,
				100,
				150,
				100,
				300,
				100,
				150,
				100,
				100,
				100,
				220,
				100,
				300,
				100,
				100,
				100,
				150,
				100,
				150,
				150,
				300,
				200,
				300,
				80,
				300,
				80,
				150,
				80,
				300,
				100,
				300,
				100,
				300,
				100,
				100,
				100,
				150,
				100,
				150,
				150,
				300,
				150,
				300,
				100,
				150,
				100,
				150,
				100,
				300,
				100,
				150,
				100,
				100,
				100,
				420,
				100,
				450,
				100,
				420,
				100,
				360,
				100,
				300,
				100,
				300,
				100,
				150,
				100,
				300,
				60,
				150,
				80,
				300,
				60,
				350,
				80,
				150,
				80,
				350,
				80,
				150,
				80,
				300,
				80,
				150,
				80,
				600,
				60,
				150,
				80,
				300,
				60,
				350,
				80,
				150,
				80,
				150,
				80,
				550,
				80,
				325,
				80,
				600,
				60,
				150,
				80,
				300,
				60,
				350,
				80,
				150,
				80,
				350,
				80,
				150,
				80,
				300,
				80,
				150,
				80,
				600,
				100,
				150,
				100,
				300,
				100,
				300,
				100,
				100,
				100,
				300,
				100,
				550,
				100,
				575
				}
		};
		for(length = 0; length < lengthOfSong; length++)
		{
			songNotes[length] = song1[0][length];
			songTimes[length] = song1[1][length];
		}
		note = 0;
		length = lengthOfSong;
	}
}
