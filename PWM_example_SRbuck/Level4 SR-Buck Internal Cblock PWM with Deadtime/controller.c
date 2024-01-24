#include "controller.h"

#define PI(y_prev,x,x_prev,kp,kits) y_prev+kp*(x-x_prev)+kits*x

struct controller_data
{
   double vin;
   double vout_ref;

   double kp_v;
   double kits_v;

   double kp_i;
   double kits_i;

   double vout;
   double iout;

   double verror;
   double verror_prev;
   double iref;
   double ierror;
   double ierror_prev;
   double out;
};

void control_init(struct controller_data *a)
{
   a->vin = 48;
   a->vout_ref = 14;

   a->kp_v = 1.39213;
   a->kits_v = 0.1623;

   a->kp_i = 1.84017;
   a->kits_i = 0.128283;
}

void control(struct controller_data *a, double *vout, double *iout, double iref_max, double out_max)
{
   a->verror = a->vout_ref - *vout;
   a->iref = PI(a->iref, a->verror, a->verror_prev, a->kp_v, a->kits_v);
   if(a->iref > iref_max) a->iref = iref_max;
   if(a->iref < 0) a->iref = 0;
   a->verror_prev = a->verror;

   a->ierror = a->iref - *iout;
   a->out = PI(a->out, a->ierror, a->ierror_prev, a->kp_i, a->kits_i);
   if(a->out > out_max) a->out = out_max;
   if(a->out < 0) a->out = 0;
   a->ierror_prev = a->ierror;
}