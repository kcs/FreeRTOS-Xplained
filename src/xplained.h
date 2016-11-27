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
/* xplained.h                                                                 */
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


#ifndef __XPLAINED_H__
#define __XPLAINED_H__

/* ========================================================================== */
/* Constants                                                                  */
/* ========================================================================== */

#define LEDPORT PORTB
#define LED0 (1<<4)
#define LED1 (1<<5)
#define LED2 (1<<6)
#define LED3 (1<<7)

/* ========================================================================== */
/* Macros                                                                     */
/* ========================================================================== */

/* LEDs */
#define LED_ON(x)     ({LEDPORT.OUTCLR = (x);})
#define LED_OFF(x)    ({LEDPORT.OUTSET = (x);})
#define LED_TOGGLE(x) ({LEDPORT.OUTTGL = (x);})


/* configuration protection */
/* some registers need a CCP setup before they can be changed */
/* the CCPWrite macro can be used to write a value to such a register */
#define CCPWrite( ADDRESS, VALUE )          \
    ({                                      \
        uint8_t value = (VALUE);              \
        uint8_t iocode = CCP_IOREG_gc;        \
        __asm__ __volatile__ (              \
            "out %0, %1  \n\t"              \
            "sts %2, %3  \n\t"              \
            :                               \
            : "i" (&CCP),                   \
              "r" (iocode),                 \
              "i" (_SFR_MEM_ADDR(ADDRESS)), \
              "r" (value)                   \
        );                                  \
    })

/* ========================================================================== */
/* Public functions                                                           */
/* ========================================================================== */
void XplainedInit(void);

#endif /* __XPLAINED_H__ */

/* -- kcs -- */
