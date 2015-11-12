/*
 * game.c
 *
 *  Created on: Nov 2, 2015
 *      Author: pmcelroy
 */
//
//updated by Paul McElroy on 11/9/2015
//For use in the Baby Bird Feeding Game
//Controls the Buttons and updates the buttonsReleased array for use by game.c

#include "buttons.h"

#include "system.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

int buttonsThisTick[12];
int buttonsLastTick[12];
int buttonsReleased[12];

void buttonsTask(void *pvParameters)
{
	unsigned long delay = TICK_R * 25.0;
	int firstRun = 1;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0);
	GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1);
	GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2);
	GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);

	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0);
	GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_1);
	GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_2);
	GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_3);
	GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0x01); //because it's pin 0
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x02); //because it's pin 1
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x04); //because it's pin 2

	int j = 0;
	for(j = 0; j < 12; j++)
	{
		buttonsThisTick[j] = 0;
		buttonsLastTick[j] = 0;
		buttonsReleased[j] = 0;
	}

	while(true) {

		int i = 0;
		for(i = 0; i < 12; i++)
		{
			buttonsLastTick[i] = buttonsThisTick[i];
		}

		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0x00);
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x02);
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x04);
		buttonsThisTick[0] = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0);
		buttonsThisTick[1] = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1);
		buttonsThisTick[2] = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2);
		buttonsThisTick[3] = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_3);
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0x01);
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x00);
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x04);
		buttonsThisTick[4] = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0);
		buttonsThisTick[5] = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1);
		buttonsThisTick[6] = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2);
		buttonsThisTick[7] = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_3);
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0x01);
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x02);
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x00);
		buttonsThisTick[8] = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0);
		buttonsThisTick[9] = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_1);
		buttonsThisTick[10] = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2);
		buttonsThisTick[11] = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_3);
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x04);

		if(firstRun == 0)
		{
			for(i = 0; i < 12; i++)
			{
				if(buttonsThisTick[i] > buttonsLastTick[i])
				{
					buttonsReleased[i] = 1;
				}
			}
		}
		firstRun = 0;

		vTaskDelay(delay);
	}
}
