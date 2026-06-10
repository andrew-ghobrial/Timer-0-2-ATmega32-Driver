/****************************************************************************************************************************************************************************
Author: heretic	< Andrew G >
File: timer0_driver.c
Date Created: 29/1/2026
Dependencies: <avr/interrupt.h> & "timer0_driver.h"
Description: TOTAL functions for Timer0 (8-bit) on AVR microcontrollers
Notes:
    +
History:
    + 29/1/2026: (Release 1.0)
    + 30/1/2026: Added PWM function (Update 1.1)
    + 2/2/2026: Added Register Definitions (Update 1.2)
    + 
****************************************************************************************************************************************************************************/

#include "timer0_driver.h"
#include <avr/interrupt.h>

// ========== OC0 is on PB3 (Pin of timer0 output) ==========

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

ui8 currentMode;

void T0_init(void) {                        // Step << 1 >>
    // Initialize Timer0 with default settings
    TCCR0 = 0x00;       // Disable Timer0 & set to normal mode
    TCNT0 = 0x00;       // Reset counter
    OCR0 = 0xFF;        // Set compare value to maximum
    TIMSK &= ~0x03;     // Disable all Timer0 interrupts   - ONLY
    TIFR |= 0x03;       // Clear any pending Timer0 flags  - ONLY
    currentMode = T0_MODE_NORMAL;
}

void T0_setMode(ui8 mode) {                 // Step << 2 >>
    // Set the mode of Timer0
    TCCR0 = (TCCR0 & ~(0x48)) | mode;
    currentMode = mode;
}

void T0_setOutputAction(ui8 action) {       // Step << 3 >>
    // Set Timer0 to have no output action
    TCCR0 &= ~(0x30);
    TCCR0 |= (action << 4);
}

void T0_setCompareValue(ui8 value) {        // Step << 4 >>
    // Set the compare value for Timer0
    OCR0 = value;
}

void T0_manageInterrupts(void) {            // Step << 5 >>
    // Clear both Timer0 interrupt bits first
    TIMSK &= ~((1 << 0) | (1 << 1));  
    if(currentMode == T0_MODE_CTC) {
        // Enable Output Compare Match Interrupt for CTC mode
        TIMSK |= (1 << 1);  // OCF0 -- Bit 1
    } else if(currentMode == T0_MODE_NORMAL) {
        // Enable Overflow Interrupt for Normal mode
        TIMSK |= (1 << 0);  // TOV0 -- Bit 0
    }   
    /* 
    else if(currentMode == T0_MODE_FASTPWM) {
        Enable Overflow Interrupt for Fast PWM mode
        TIMSK |= (1 << 0) | (1 << 1);   // TOV0 AND OCF0
    }
    */
}

void T0_start(ui8 prescaler) {             // Step << 6 >> -- START
    // Set the prescaler for Timer0 
    // AND thus START the timer automatically
    TCCR0 = (TCCR0 & 0xF8) | prescaler;
}

void T0_doPWM(ui8 dutyCycle, ui8 prescaler){        // => QoL Feature (Simple PWM Control))
    T0_init();
    T0_setMode(T0_MODE_FASTPWM);
    T0_setOutputAction(T0_OUT_LOW);
    /* T0_manageInterrupts(); */   // Interrupts not needed for basic PWM
    T0_setCompareValue((dutyCycle / 100) * 255);  // Convert duty cycle % to OCR0 value
    T0_start(prescaler);
}     

void T0_stop(void) {                         // -- Optional Step
    // Stop Timer0 by clearing the prescaler bits
    // AND the counting register is reset to 0
    TCCR0 &= 0xF8;
    TCNT0 = 0x00;
}

void T0_doOutputAction(void) {               // -- Optional Step
    // Set the output action for Timer0
    TCCR0 |= (1 << 7);
}

ui8 T0_getCount(void) {                      // -- Optional Step
    // Get the current count value of Timer0
    return TCNT0;
}



                /* ==== PUT THOSE IN MAIN.C ==== */

//             // -- T0 Overflow ISR --
// ISR(TIMER0_OVF_vect) {
//     // Handle overflow interrupt
//     // This runs when timer overflows in NORMAL mode
// }

//             // --T0 Output Compare Match ISR --
// ISR(TIMER0_COMP_vect) {
//     // Handle compare match interrupt
//     // This runs when TCNT0 == OCR0 in CTC mode
// }
