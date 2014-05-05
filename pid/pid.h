/* 
 * File:   pid.h
 * Author: Federico Reghenzani <federico.dev@reghe.net>
 *
 * Created on May 5, 2014, 6:59 PM
 */

#ifndef PID_H
#define	PID_H


/* de-comment to compile tracking functions */
/* #define PID_ENABLE_TRACKING */

/**
 * PID_SETTINGS
 * Configuration values of PID.
 */
typedef struct {
    float K;            /* static gain */
    float Ti,Td;        /* integrative and derivative time */
    float Ts;           /* sampling step */
    float N;            /* derivative sensibility (usually 1<x<10) */
    float w_prop;       /* set point proportional weight (usually 0<x<1) */
    float w_deriv;      /* set point derivative weight (usually 0<x<1) */
    float CS_max,CS_min;/* anti-windup values, control signal max/min */
} PID_SETTINGS;

/**
 * pid_init
 * This function destroy all previous PID data and reset to new
 * PID instance.
 * @param PID_SETTINGS the pointer to configuration values of new PID
 */
void pid_init(PID_SETTINGS*);

/**
 * pid_init
 * This function destroy all previous PID data and reset to new
 * PID instance.
 * @param set_point the current set_point values
 * @param output the current output value (feedback)
 * @return the new control signal value
 */
float pid_data(float set_point, float output);

/**
 * If you define ENABLE_TRACKING, you compile also a 'tracking switch mode'.
 */
#ifdef PID_ENABLE_TRACKING
    void pid_tracking_mode(short);
    void pid_tracking_data(float tr);
#endif

#endif	/* PID_H */

