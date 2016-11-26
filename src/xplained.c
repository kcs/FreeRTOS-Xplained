
#include <avr/io.h>
#include "xplained.h"

void XplainedInit(void)
{
   // XOSC
    OSC.XOSCCTRL = OSC_FRQRANGE_2TO9_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;
// Enable
    OSC.CTRL = OSC_XOSCEN_bm;
    while (!(OSC.STATUS & OSC_XOSCRDY_bm));
// Prescaler
    CCPWrite(CLK.PSCTRL, CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc);
// Select
    CCPWrite(CLK.CTRL, CLK_SCLKSEL_XOSC_gc);
// Disable 2MHz internal osccillator
    OSC.CTRL = OSC_XOSCEN_bm;

    LEDPORT.DIR = LED0 | LED1 | LED2 | LED3;
    LED_OFF(LED0 | LED1 | LED2);
    LED_ON(LED3);

    PMIC.CTRL |= PMIC_LOLVLEX_bm;
}
