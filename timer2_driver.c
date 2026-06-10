/****************************************************************************************************************************************************************************
Author: heretic	< Andrew G >
File: timer2_driver.c
Date Created: 30/1/2026
Dependencies: <avr/interrupt.h> & "timer2_driver.h"
Description: TOTAL functions for Timer2 (8-bit) on AVR microcontrollers
Notes:
    + 
History:
    + 30/1/2026: Copied from Timer0 Driver (Release 1.0)
    + 31/1/2026: Added extra 2 prescalers (32 and 128) (Update 1.2)
    + 2/2/2026: Added Register Definitions (Update 1.3)
    + 
****************************************************************************************************************************************************************************/

#include "timer2_driver.h"
#include <avr/interrupt.h>

// ========== OC2 is on PD7 (Pin of timer2 output) ==========

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

ui8 currentMode;

void T2_init(void) {                        // Step << 1 >>
    // Initialize Timer2 with default settings
    TCCR2 = 0x00;       // Disable Timer2 & set to normal mode
    TCNT2 = 0x00;       // Reset counter
    OCR2 = 0xFF;        // Set compare value to maximum
    TIMSK &= ~0xC0;     // Disable all Timer2 interrupts   - ONLY
    TIFR |= 0xC0;       // Clear any pending Timer2 flags  - ONLY
    currentMode = T2_MODE_NORMAL;
}

void T2_setMode(ui8 mode) {                 // Step << 2 >>
    // Set the mode of Timer2
    TCCR2 = (TCCR2 & ~(0x48)) | mode;
    currentMode = mode;
}

void T2_setOutputAction(ui8 action) {       // Step << 3 >>
    // Set Timer2 to have no output action
    TCCR2 &= ~(0x30);
    TCCR2 |= (action << 4);
}

void T2_setCompareValue(ui8 value) {        // Step << 4 >>
    // Set the compare value for Timer2
    OCR2 = value;
}

void T2_manageInterrupts(void) {            // Step << 5 >>
    // Clear both Timer2 interrupt bits first
    TIMSK &= ~((1 << 6) | (1 << 7));  
    if(currentMode == T2_MODE_CTC) {
        // Enable Output Compare Match Interrupt for CTC mode
        TIMSK |= (1 << 7);  // OCF2 -- Bit 7
    } else if(currentMode == T2_MODE_NORMAL) {
        // Enable Overflow Interrupt for Normal mode
        TIMSK |= (1 << 6);  // TOV2 -- Bit 6
    }   
    /* 
    else if(currentMode == T2_MODE_FASTPWM) {
        Enable Overflow Interrupt for Fast PWM mode
        TIMSK |= (1 << 6) | (1 << 7);   // TOV2 AND OCF2
    }
    */
}

void T2_start(ui8 prescaler) {             // Step << 6 >> -- START
    // Set the prescaler for Timer2 
    // AND thus START the timer automatically
    TCCR2 = (TCCR2 & 0xF8) | prescaler;
}

void T2_doPWM(ui8 dutyCycle, ui8 prescaler){        // => QoL Feature (Simple PWM Control))
    T2_init();
    T2_setMode(T2_MODE_FASTPWM);
    T2_setOutputAction(T2_OUT_LOW);
    /* T2_manageInterrupts(); */   // Interrupts not needed for basic PWM
    T2_setCompareValue((dutyCycle / 100) * 255);  // Convert duty cycle % to OCR2 value
    T2_start(prescaler);
}     

void T2_stop(void) {                         // -- Optional Step
    // Stop Timer2 by clearing the prescaler bits
    // AND the counting register is reset to 0
    TCCR2 &= 0xF8;
    TCNT2 = 0x00;
}

void T2_doOutputAction(void) {               // -- Optional Step
    // Set the output action for Timer2
    TCCR2 |= (1 << 7);
}

ui8 T2_getCount(void) {                      // -- Optional Step
    // Get the current count value of Timer2
    return TCNT2;
}



                /* ==== PUT THOSE IN MAIN.C ==== */

//             // -- T2 Overflow ISR --
// ISR(TIMER2_OVF_vect) {
//     // Handle overflow interrupt
//     // This runs when timer overflows in NORMAL mode
// }

//             // -- T2 Output Compare Match ISR --
// ISR(TIMER2_COMP_vect) {
//     // Handle compare match interrupt
//     // This runs when TCNT2 == OCR2 in CTC mode
// }
