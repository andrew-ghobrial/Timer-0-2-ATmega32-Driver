/****************************************************************************************************************************************************************************
Author: heretic	< Andrew G >
File: timer2_driver.h
Date Created: 30/1/2026
Dependencies: "portable_dts.h"
Description: TOTAL functions for Timer2 (8-bit) on AVR microcontrollers
Notes:
    +
History:
    + 30/1/2026: Copied from Timer0 Driver (Release 1.0)
    + 31/1/2026: Added extra 2 prescalers (32 and 128) (Update 1.2)
    + 2/2/2026: Added Register Definitions (Update 1.3)
    + 
****************************************************************************************************************************************************************************/

#ifndef TIMER2_DRIVER_H
#define TIMER2_DRIVER_H

#include "portable_dts.h"

// ========== OC2 is on PD7 (Pin of timer2 output) ==========


// Prescaler values
enum {
    T2_PRE_1 = 1, 
    T2_PRE_8 = 2, 
    T2_PRE_32 = 3,
    T2_PRE_64 = 4,
    T2_PRE_128 = 5,
    T2_PRE_256 = 6, 
    T2_PRE_1024 = 7
};

// Timer modes
enum {
    T2_MODE_NORMAL = 0,
    T2_MODE_CTC = (1 << 3),
    T2_MODE_FASTPWM = ((1 << 6) | (1 << 3)),
    T2_MODE_PHASEPWM = (1 << 6)
};

// Output compare actions
enum {
    T2_OUT_NONE = 0, 
    T2_OUT_TOGGLE = 1, 
    T2_OUT_LOW = 2, 
    T2_OUT_HIGH = 3
};

// Function declarations
void T2_init(void);
void T2_setMode(ui8 mode);
void T2_setOutputAction(ui8 action);
void T2_setCompareValue(ui8 value);
void T2_manageInterrupts(void);
void T2_start(ui8 prescaler);
void T2_doPWM(ui8 dutyCycle, ui8 prescaler);
void T2_stop(void);
void T2_doOutputAction(void);
ui8 T2_getCount(void);

// External state
extern ui8 currentMode;


#define TCCR2   (*(volatile uint8_t*)0x45)  
#define TCNT2   (*(volatile uint8_t*)0x44) 
#define OCR2    (*(volatile uint8_t*)0x43) 
#define TIMSK   (*(volatile uint8_t*)0x59) 
#define TIFR    (*(volatile uint8_t*)0x58)


// === PRESCALER VALUE DEPENDS ON PERIPHERAL NEEDED FREQUENCY ===
/*
 * TIMER PWM FREQUENCY CALCULATION
 * * F_CPU is typically 16,000,000 Hz.
 * * 8-bit Timers (Timer0/2): TOP is fixed at 255 (0xFF).
 * * f_PWM = F_CPU / (Prescaler * (255 + 1))
 * * Example: F_CPU=16MHz, Prescaler=64: f_PWM = 16000000 / (64 * 256) = 976.56 Hz
 */

#endif