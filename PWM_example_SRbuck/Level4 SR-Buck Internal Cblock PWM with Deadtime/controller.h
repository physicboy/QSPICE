#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include "main.h"
#include "controller.c"

extern struct controller_data;

extern void control_init(struct controller_data *a);
extern void control(struct controller_data *a, double *vout, double *iout, double iref_max, double out_max);

#endif