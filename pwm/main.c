/* 
 * File:   main.c
 * Author: Federico Reghenzani
 *
 * Created on February 5, 2014, 6:58 PM
 */

#include <xc.h>

#include <limits.h>       /* For *_MAX definitions */
#include <stdbool.h>       /* For true/false definition */

#include "pinout.h"

#define _XTAL_FREQ      10000000L

// Define the output behaviour:
// TRIS_STATE: 00, 01, 10 on 0-33, 34-66,67-100
// QUAD_STATE: 00, 01, 10, 11 on 0-25, 26-50, 51-75, 76-100
#define QUAD_STATE 0

// Define counters size and timeout (critical values! See wiki!)
#define LONG_INT 0
#define DELAY_US 50

#if LONG_INT
#define COUNTER_MAX ULONG_MAX
typedef unsigned long counter;
#else
#define COUNTER_MAX UINT_MAX
typedef unsigned short counter;
#endif

typedef enum {T_NONE, T_ON, T_OFF} pwm_status;

// Return true on (relative) high value or false on (relative) low value
// on pwm input pin "n".
bool get_pin(unsigned char n)
{
    switch(n)
    {
        case 0:
            return IN_A ^ IN_A_INVERTED;
        case 1:
            return IN_B ^ IN_B_INVERTED;
        case 2:
            return IN_C ^ IN_C_INVERTED;
        case 3:
            return IN_D ^ IN_D_INVERTED;
    }
    return false;
}

// Write value on pwm output pins "n". 0<=value<=3 (2 bit)
void set_pin(unsigned char n, char value)
{
    switch(n)
    {
        case 0:
            OUT_A_0 = ( value & 0b00000001 )          ^ OUT_A_0_INVERTED;
            OUT_A_1 = ( ( value & 0b00000010 ) >> 1 ) ^ OUT_A_1_INVERTED;
        break;
        case 1:
            OUT_B_0 = ( value & 0b00000001 )          ^ OUT_B_0_INVERTED;
            OUT_B_1 = ( ( value & 0b00000010 ) >> 1 ) ^ OUT_B_1_INVERTED;
        break;
        case 2:
            OUT_C_0 = ( value & 0b00000001 )          ^ OUT_C_0_INVERTED;
            OUT_C_1 = ( ( value & 0b00000010 ) >> 1 ) ^ OUT_C_1_INVERTED;
        break;
        case 3:
            OUT_D_0 = ( value & 0b00000001 )          ^ OUT_D_0_INVERTED;
            OUT_D_1 = ( ( value & 0b00000010 ) >> 1 ) ^ OUT_D_1_INVERTED;
        break;
    }
}

void translate_pwm(unsigned char n, counter cycle_ON, counter cycle_OFF)
{
    if ( cycle_OFF == 0 ) cycle_OFF++;  // Formal check: avoid zero division

    counter rapport = cycle_ON * 100 / cycle_OFF;

#if QUAD_STATE
    if      ( rapport <= 25)
        set_pin(n,0);
    else if ( rapport <= 50)
        set_pin(n,1);
    else if ( rapport <= 75)
        set_pin(n,2);
    else
        set_pin(n,3);
#else
    if      ( rapport <= 33)
        set_pin(n,0);
    else if ( rapport <= 66)
        set_pin(n,1);
    else
        set_pin(n,2);
#endif
}

/*
 * 
 */
int main(void) {

    // 2 byte unsigned int 0-65535
    pwm_status pwm[4];
    counter cycle_ON [4];
    counter cycle_OFF[4];


    TRISA = TRISA_CONFIG;
    TRISB = TRISB_CONFIG;

    // Phase 1: Turn off all output and wait 500 ms
    for ( unsigned char i=0; i<4; i++)
        set_pin(i,0);

    // Wait 500msec to wait oscillator stabilized.
    __delay_ms(500);

    // Clear all output and variables
    // TODO: restore output saved in EEPROM values
    for ( unsigned char i=0; i<4; i++) {
        pwm[i] = T_NONE;
        cycle_ON [i] = 0;
        cycle_OFF[i] = 0;
    }

    while (true) {
        for ( unsigned char i=0; i<4; i++) {    // For each PWM sections...
            if (get_pin(i)) { // Signal is HIGH

                // If previous was OFF, elaborate pwm output
                // and restart PWM cycle
                if (pwm[i] == T_OFF) {
                    translate_pwm(i, cycle_ON[i], cycle_OFF[i]);
                    cycle_OFF[i] = cycle_ON[i] = 0;
                }
                pwm[i] = T_ON;
                cycle_ON[i]++;          // Update counter...

            }
            else { // Signal is LOW
                if ( pwm[i] == T_NONE ) // No HIGH detected, countinue...
                    continue;
                else if ( pwm[i] == T_ON )
                    pwm[i] = T_OFF;
                cycle_OFF [i]++;        // Update counter...

            }

            // Overflow detected: No valid pwm signal.
            if ( cycle_OFF[i] >= COUNTER_MAX ||  cycle_ON[i] >= COUNTER_MAX ) {
                pwm[i]       = T_NONE;          // No signal
                set_pin(i,0);                   // Clear all output
                cycle_ON[i] = cycle_OFF[i] = 0; // Reset counters
            }

        }   // End for

        __delay_us(DELAY_US);

    }   // End While

    return 0;
}

