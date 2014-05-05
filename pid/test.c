#include "pid.h"
#include <xc.h>

/* set point testing generator */
float gen_sp(float u, float Ts)
{
#define mu 1
#define T 10
    static float x0=0;
    static float y=0;
    float x;
    x  = (T*(x0+u))/(T+Ts);
    y  = mu*Ts/(T+Ts)*(x+u);
    x0 = x;
    return y;
}

/* impulse function */
float sca(float t) { return t>=0?1:0; }
/* ramp function */
float ramp(float t) { return t*sca(t); }

void main()
{
    TRISA = 0b11111111;
    TRISB = 0b11111111;

    
    PID_SETTINGS test_pid;
    test_pid.K       = 10;
    test_pid.Ti      = 7;
    test_pid.Td      = 0;
    test_pid.N       = 1;
    test_pid.w_prop  = 1;
    test_pid.w_deriv = 0;
    test_pid.CS_max  = 5;
    test_pid.CS_min  = -5;
#define TS 0.25
    test_pid.Ts      = TS;

    pid_init(&test_pid);

    float cs=0, t=0, gen=0, sp=0;

    for ( int i=0; i<50; i++ )
    {
        t = i*TS;
        sp = ramp(t-1)-ramp(t-5)-0.1*ramp(t-50);
        cs = pid_data( sp , gen );

        gen = gen_sp(cs, TS);
    }

    return;
}