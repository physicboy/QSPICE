#ifndef _PWM_H
#define _PWM_H

#include "_pwm.c"

extern struct pwm_var;

extern void pwm_calc(struct pwm_var *a, double t);
extern double pwm_maxstep(struct pwm_var *a);
extern double pwm_trunc(struct pwm_var *a);


#endif