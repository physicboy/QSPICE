#ifndef _PWM_GEN_H
#define _PWM_GEN_H

#include "main.h"
#include "pwm_gen.c"

extern struct pwm_data;

extern void pwm_gen_init(struct pwm_data *a);
extern void pwm_gen(struct pwm_data *a, bool *interrupt_flag,
   long long int *xcntr, double *t, double *t_prev,
   double *maxstep, bool *pwm_trigger, bool out_en);

void pwm_trunc_handler(struct pwm_data a, double *t, double *timestep, bool *pwm_trigger);

#endif