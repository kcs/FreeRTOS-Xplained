/* ========================================================================== */
/* FreeRTOS-Xplained                                                          */
/* Demo project featuring FreeRTOS on XMEGA-B1 Xplained board                 */
/*                                                                            */
/* © 2015-2016 KCs                                                            */
/*                                                                            */
/* This program is free software; you can redistribute it and/or modify       */
/* it under the terms of the GNU General Public License as published by       */
/* the Free Software Foundation; either version 2 of the License, or          */
/* (at your option) any later version.                                        */
/*                                                                            */
/* This program is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              */
/* GNU General Public License for more details.                               */
/*                                                                            */
/* You should have received a copy of the GNU General Public License along    */
/* with this program; if not, write to the Free Software Foundation, Inc.,    */
/* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                */
/* ========================================================================== */
/*                                                                            */
/* main.c                                                                     */
/*                                                                            */
/* ========================================================================== */
/*                                                                            */
/* initialization of the demo project, setting up hardware and demo tasks     */
/* running the operating system                                               */
/*                                                                            */
/* public functions:                                                          */
/*   main                                                                     */
/*                                                                            */
/* ========================================================================== */

/* ========================================================================== */
/* Headers                                                                    */
/* ========================================================================== */

#include <avr/io.h>
#include <avr/eeprom.h>

#include "xplained.h"

/* FreeRTOS include files. */
#include "FreeRTOS.h"
#include "task.h"

/* ========================================================================== */
/* Private function declaration                                               */
/* ========================================================================== */

static void blinky(void* pvParameters);

/* ========================================================================== */
/* Public function implementation                                             */
/* ========================================================================== */

/* ========================================================================== */
/* main                                                                       */
/*                                                                            */
/* application starting point                                                 */
/* initialize hardware, tasks, OS objects                                     */
/* switch to the scheduler                                                    */
/*                                                                            */
/* parameters:                                                                */
/*   none                                                                     */
/* return value:                                                              */
/*   int (compatibility reasons, never reached)                               */
/*                                                                            */
/* ========================================================================== */

int main( void )
{
    // initialize the Xplained board
    XplainedInit();

    // create tasks
    xTaskCreate(blinky, "Blinky", configMINIMAL_STACK_SIZE, NULL, 1, NULL );

    // start the scheduler
	vTaskStartScheduler();

    // ... should not reach this
	return 0;
}

/* ========================================================================== */
/* Private function implementation                                            */
/* ========================================================================== */

/* ========================================================================== */
/* blinky                                                                     */
/*                                                                            */
/* simple task blinking LED0                                                  */
/*                                                                            */
/* parameters:                                                                */
/*   pvParameters - not used                                                  */
/* return value:                                                              */
/*   none                                                                     */
/*                                                                            */
/* ========================================================================== */

static void blinky(void* pvParameters)
{
    (void)pvParameters; // parameters not used

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t delay = 500;

    // forever loop
    for(;;)
    {
        vTaskDelayUntil(&xLastWakeTime, delay);
        LED_TOGGLE(LED0);
    }
}

/* -- kcs -- */
