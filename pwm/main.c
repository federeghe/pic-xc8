/* 
 * File:   main.c
 * Author: Federico Reghenzani
 *
 * Created on February 5, 2014, 6:58 PM
 */

#include <xc.h>
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include "pinout.h"

#define _XTAL_FREQ      10000000L

#define QUAD_STATE 0
#define INT_MAX 65535
#define DELAY_US 50

typedef enum {T_NONE, T_ON, T_OFF} pwm_status;

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

void translate_pwm(unsigned char n, unsigned int cycle_ON, unsigned int cycle_OFF)
{
    if ( cycle_OFF == 0 ) cycle_OFF++;

    unsigned int rapport = cycle_ON*100/cycle_OFF;

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
    unsigned int cycle_ON [4];
    unsigned int cycle_OFF[4];


    TRISA = TRISA_CONFIG;
    TRISB = TRISB_CONFIG;

    // Phase 1: Turn off all output and wait 500 ms
    for ( unsigned char i=0; i<4; i++)
        set_pin(i,0);

    __delay_ms(500);

    for ( unsigned char i=0; i<4; i++) {
        pwm[i] = T_NONE;
        cycle_ON [i] = 0;
        cycle_OFF[i] = 0;
    }

    while (true) {
        // Aggiornamento contatori
        for ( unsigned char i=0; i<4; i++) {
            if (get_pin(i)) {
                if (pwm[i] == T_OFF) {
                    translate_pwm(i, cycle_ON[i], cycle_OFF[i]);
                    cycle_OFF[i] = cycle_ON[i] = 0;
                }
                pwm[i] = T_ON;
                cycle_ON[i]++;

            }
            else {
                if ( pwm[i] == T_NONE ) // Finche' non vedo un UP...
                    continue;
                else if ( pwm[i] == T_ON )
                    pwm[i] = T_OFF;
                cycle_OFF [i]++;

            }

            if ( cycle_OFF[i] >= INT_MAX ||  cycle_ON[i] >= INT_MAX ) {
                pwm[i]       = T_NONE;
                set_pin(i,0);
                cycle_ON[i] = cycle_OFF[i] = 0;
            }

        }   // End for

        __delay_us(DELAY_US);

    }   // End While

    return 0;
}

