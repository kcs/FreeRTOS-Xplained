/* ========================================================================== */
/* FreeRTOS-Xplained                                                          */
/* Demo project featuring FreeRTOS on XMEGA-B1 Xplained board                 */
/*                                                                            */
/* © 2016 KCs                                                                 */
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
/* xplained.c                                                                 */
/*                                                                            */
/* ========================================================================== */
/*                                                                            */
/* helper functions and macros to access the hardware available on the        */
/* Xmega-B1 Xplained board                                                    */
/*                                                                            */
/* public functions:                                                          */
/*   XplainedInit                                                             */
/*                                                                            */
/* ========================================================================== */

/* ========================================================================== */
/* Headers                                                                    */
/* ========================================================================== */

#include <avr/io.h>
#include "xplained.h"

/* ========================================================================== */
/* Public function implementation                                             */
/* ========================================================================== */

/* ========================================================================== */
/* XplainedInit                                                               */
/*                                                                            */
/* initialize the hardware onboard the Xplained board                         */
/* currently the following dedicated hardware and register settings will be   */
/* used:                                                                      */
/*   oscillator: the external 8MHz crystal oscillator is used                 */
/*   user LEDs: all 4 LEDs connected to the PORTB                             */
/*                                                                            */
/* parameters:                                                                */
/*   none                                                                     */
/* return value:                                                              */
/*   none                                                                     */
/*                                                                            */
/* ========================================================================== */

void XplainedInit(void)
{
    /* ---- Using the external crystal oscillator ---- */

    OSC.XOSCCTRL = OSC_FRQRANGE_2TO9_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;
    // enable it and wait for it to stabilize
    OSC.CTRL = OSC_XOSCEN_bm;
    while (!(OSC.STATUS & OSC_XOSCRDY_bm));
    // set the prescaler (protected instruction)
    CCPWrite(CLK.PSCTRL, CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc);
    // chose the new setting (also protected)
    CCPWrite(CLK.CTRL, CLK_SCLKSEL_XOSC_gc);
    // and finally disable the 2MHz internal oscillator
    OSC.CTRL = OSC_XOSCEN_bm;

    /* ---- user LEDs ---- */

    // set the LEDPORT as output and LEDs turned off
    LED_OFF(LED0 | LED1 | LED2 | LED3);
    LEDPORT.DIR = LED0 | LED1 | LED2 | LED3;

    /* ---- interrupt control ---- */

    // enable Low level interrupts (the OS tick needs one)
    PMIC.CTRL |= PMIC_LOLVLEX_bm;
}

/* -- kcs -- */