/****************************************************************************************************************************************************************************
Author: heretic	< Andrew G >
File: timer0_driver.h
Date Created: 29/1/2026
Dependencies: "portable_dts.h"
Description: TOTAL functions for Timer0 (8-bit) on AVR microcontrollers
Notes:
    +
History:
    + 29/1/2026: (Release 1.0)
    + 30/1/2026: Added PWM function (Update 1.1)
    + 2/2/2026: Added Register Definitions (Update 1.2)
    + 
****************************************************************************************************************************************************************************/

#ifndef TIMER0_DRIVER_H
#define TIMER0_DRIVER_H

#include "portable_dts.h"

// ========== OC0 is on PB3 (Pin of timer0 output) ==========


// Prescaler values
enum {
    T0_PRE_1 = 1, 
    T0_PRE_16 = 2, 
    T0_PRE_64 = 3, 
    T0_PRE_256 = 4, 
    T0_PRE_1024 = 5
};

// Timer modes
enum {
    T0_MODE_NORMAL = 0,
    T0_MODE_CTC = (1 << 3),
    T0_MODE_FASTPWM = ((1 << 6) | (1 << 3)),
    T0_MODE_PHASEPWM = (1 << 6)
};

// Output compare actions
enum {
    T0_OUT_NONE = 0, 
    T0_OUT_TOGGLE = 1, 
    T0_OUT_LOW = 2, 
    T0_OUT_HIGH = 3
};

// Function declarations
void T0_init(void);
void T0_setMode(ui8 mode);
void T0_setOutputAction(ui8 action);
void T0_setCompareValue(ui8 value);
void T0_manageInterrupts(void);
void T0_start(ui8 prescaler);
void T0_doPWM(ui8 dutyCycle, ui8 prescaler);
void T0_stop(void);
void T0_doOutputAction(void);
ui8 T0_getCount(void);

// External state
extern ui8 currentMode;


#define TCCR0   (*(volatile uint8_t*)0x53)
#define TCNT0   (*(volatile uint8_t*)0x52)
#define OCR0    (*(volatile uint8_t*)0x5C)   
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