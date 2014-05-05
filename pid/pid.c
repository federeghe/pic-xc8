/*
 * File:   pid.c
 * Author: Federico Reghenzani <federico.dev@reghe.net>
 *
 * Created on May 5, 2014, 6:59 PM
 */

#include "pid.h"

static PID_SETTINGS *curr_pid;

/* Old values */
static float SP_old;    /* set point old */
static float PV_old;    /* process value old (output) */
static float CS_old;    /* control signal old */
static float D_old;     /* derivative coefficient old */

#ifdef PID_ENABLE_TRACKING
static short track_switch;
#endif

void pid_init(PID_SETTINGS* s)
{
    curr_pid = s;

#ifdef PV_STARTING_VALUE
    PV_old = SP_STARTING_VALUE;
#else
    PV_old = 0;
#endif

    SP_old = CS_old = D_old = 0;

#ifdef PID_ENABLE_TRACKING
    track_switch = 0;
#endif

}

float pid_data(float set_point, float output)
{

    float diff_SP, diff_PV, CS_new, D_cont;

#ifdef PID_ENABLE_TRACKING
    if ( track_switch ) {
        SP_old = set_point;
        PV_old = output;
        return CS_old;  /* setted by pid_tracking_data() */
    }
#endif

    diff_SP = set_point - SP_old;
    diff_PV = output - PV_old;

    /* proportional, integrative and derivative contributes.
       Formulaes are quite complex, and their discussions are
       too long for this comment :P
     */
#define P_CONTRIBUTE curr_pid->K * (curr_pid->w_prop*diff_SP - diff_PV)
#define I_CONTRIBUTE (curr_pid->K * curr_pid->Ts / curr_pid->Ti) * \
                     (set_point-output)
#define D_CONTRIBUTE ((curr_pid->Td * D_old) + (curr_pid->K * curr_pid->N * \
                     curr_pid->Td*(curr_pid->w_deriv*diff_SP - diff_PV ))) / \
                     (curr_pid->Td + curr_pid->N * curr_pid->Ts) - D_old

    D_cont = D_CONTRIBUTE;  /* saved here, we need that after */
    CS_new = CS_old + P_CONTRIBUTE + I_CONTRIBUTE + D_cont;

#undef P_CONTRIBUTE
#undef I_CONTRIBUTE
#undef D_CONTRIBUTE

    /* anti-windup */
    if ( CS_new > curr_pid->CS_max )
        CS_new = curr_pid->CS_max;
    if ( CS_new < curr_pid->CS_min )
        CS_new = curr_pid->CS_min;

    /* store values */
    SP_old = set_point;
    PV_old = output;
    CS_old = CS_new;
    D_old += D_cont;    /* this is the difference between D_new and D_old */

    return CS_new;
}

#ifdef PID_ENABLE_TRACKING
    void pid_tracking_mode(short onoff)
    {
        track_switch = onoff;
    }
    void pid_tracking_data(float tr)
    {
        if (!track_switch)
            return;
        CS_old = tr;
        D_old  = 0; /* We MUST reset the derivative contributes! I cannot
                       predict anything! */
    }

#endif