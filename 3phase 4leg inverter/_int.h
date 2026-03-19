#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include "_int.c"

extern struct int_var;
extern void int_calc(struct int_var *a, struct adc_var *adc, struct pwm_var *pwm, int *on_off);

#endif