#include "pwm_gen.h"

struct pwm_data
{
   double ttol;

   double mcu_clkprd;
   double peak;
   double dtime;

   double cmp_rise;
   double cmp_fall;

   double trg_zero;
   double trg_cmp_rise;
   double trg_peak;
   double trg_cmp_fall;
   double trg_delay;

   bool pwm;
   bool pwm_delay;

   bool pwm_hi;
   bool pwm_lo;
};

void pwm_gen_init(struct pwm_data *a)
{
   a->ttol = 10e-9;
   a->mcu_clkprd = 10e-9;
   a->peak = 200;
   a->dtime = 20;

   a->trg_zero = 1e-6;
   a->trg_cmp_rise = 1e-6;
   a->trg_peak = 1e-6;
   a->trg_cmp_fall = 1e-6;
}

void pwm_gen(struct pwm_data *a, bool *interrupt_flag,
   long long int *xcntr, double *t, double *t_prev,
   double *maxstep, bool *pwm_trigger, bool out_en)
{
    bool pwm_prev = a->pwm;
   if((*t_prev <= a->trg_zero)&&(*t >= a->trg_zero))
   {
      *xcntr = *xcntr + 1;
      *pwm_trigger = true;
      *interrupt_flag = true;

      *maxstep = a->peak*a->mcu_clkprd;

      // update the next trigger
      a->trg_cmp_rise = a->trg_zero + a->cmp_rise*a->mcu_clkprd;
      a->trg_peak = a->trg_zero + a->peak*a->mcu_clkprd;
      a->trg_zero = a->trg_zero + 2*a->peak*a->mcu_clkprd;
      a->trg_cmp_fall = a->trg_zero - a->cmp_fall*a->mcu_clkprd;
   }

   if(*t < a->trg_peak)
   {
      if((*t_prev <= a->trg_cmp_rise)&&(*t >= a->trg_cmp_rise))
      {
         *xcntr = *xcntr + 1;
         *pwm_trigger = true;
         a->pwm = false;
         a->trg_delay = a->trg_cmp_rise + a->dtime*a->mcu_clkprd;
      }
   }
   else
   {
      if((*t_prev <= a->trg_peak)&&(*t >= a->trg_peak))
      {
         *xcntr = *xcntr + 1;
         *pwm_trigger = true;
      }
      if((*t_prev <= a->trg_cmp_fall)&&(*t >= a->trg_cmp_fall))
      {
         *xcntr = *xcntr + 1;
         *pwm_trigger = true;
         a->pwm = true;
      }
   }

    if(!pwm_prev && a->pwm)
    {
        // pwm is rising
        a->trg_delay = a->trg_cmp_fall + a->dtime*a->mcu_clkprd;
    }
    if(pwm_prev && !a->pwm)
    {
        // pwm is falling
        a->trg_delay = a->trg_cmp_rise + a->dtime*a->mcu_clkprd;
    }

   if((*t_prev <= a->trg_delay)&&(*t >= a->trg_delay))
   {
        *xcntr = *xcntr + 1;
        *pwm_trigger = true;
        a->pwm_delay = a->pwm;
   }

   a->pwm_hi = out_en & a->pwm & a->pwm_delay;
   a->pwm_lo = out_en & (!a->pwm) & (!a->pwm_delay);
}

void pwm_trunc_handler(struct pwm_data *a, double *t, double *timestep, bool *pwm_trigger)
{
   if(*t < a->trg_zero){if(*timestep > (a->trg_zero - *t)){*timestep = (a->trg_zero - *t);}}
   if(*t < a->trg_cmp_rise){if(*timestep > (a->trg_cmp_rise - *t)){*timestep = (a->trg_cmp_rise - *t);}}
   if(*t < a->trg_peak){if(*timestep > (a->trg_peak - *t)){*timestep = (a->trg_peak - *t);}}
   if(*t < a->trg_cmp_fall){if(*timestep > (a->trg_cmp_fall - *t)){*timestep = (a->trg_cmp_fall - *t);}}
   if(*t < a->trg_delay){if(*timestep > (a->trg_delay - *t)){*timestep = (a->trg_delay - *t);}}
   if(*pwm_trigger)if(*timestep > a->ttol){*timestep = a->ttol;}
}