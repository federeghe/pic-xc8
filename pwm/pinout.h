/* 
 * File:   pinout.h
 * Author: federico
 *
 * Created on February 5, 2014, 7:05 PM
 */

#ifndef PINOUT_H
#define	PINOUT_H

#define TRISA_CONFIG 0b11111111
#define TRISB_CONFIG 0b00000000

#define IN_A PORTAbits.RA0
#define IN_A_INVERTED 0
#define IN_B PORTAbits.RA1
#define IN_B_INVERTED 0
#define IN_C PORTAbits.RA2
#define IN_C_INVERTED 0
#define IN_D PORTAbits.RA3
#define IN_D_INVERTED 0

#define OUT_A_0 PORTBbits.RB7
#define OUT_A_0_INVERTED 0
#define OUT_A_1 PORTBbits.RB6
#define OUT_A_1_INVERTED 0

#define OUT_B_0 PORTBbits.RB5
#define OUT_B_0_INVERTED 0
#define OUT_B_1 PORTBbits.RB4
#define OUT_B_1_INVERTED 0

#define OUT_C_0 PORTBbits.RB0
#define OUT_C_0_INVERTED 1
#define OUT_C_1 PORTBbits.RB1
#define OUT_C_1_INVERTED 1

#define OUT_D_0 PORTBbits.RB3
#define OUT_D_0_INVERTED 1
#define OUT_D_1 PORTBbits.RB2
#define OUT_D_1_INVERTED 1

#endif	/* PINOUT_H */

