/* 
 * File:   configuration_bits.h
 * Author: Federico Reghenzani
 *
 * Created on February 5, 2014, 7:00 PM
 */


#include <xc.h>

// PIC16F84A Configuration Bit Settings
#ifdef _16F84A

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (Power-up Timer is disabled)
#pragma config CP = OFF         // Code Protection bit (Code protection disabled)

#endif