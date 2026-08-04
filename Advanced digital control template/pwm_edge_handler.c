struct sPWM
{
   int prd, dt;
   int cmpa, cmpb;
   double t_zero, t_prd;
   double t_on, t_off, t_dly;
   bool pwm, pwm_dly;
   bool outa, outb;
};

void pwm_edge(struct sPWM *a, bool *stepped)
{
   double tprev = *CKTtime - *CKTdelta;
   if(tprev <= a->t_on && *CKTtime >= a->t_on)
   {
      *stepped = 1;
      a->pwm = 1;
      a->t_dly = *CKTtime + a->dt * DGTL_CLK;
   }

   if(tprev <= a->t_off && *CKTtime >= a->t_off)
   {
      *stepped = 1;
      a->pwm = 0;
      a->t_dly = *CKTtime + a->dt * DGTL_CLK;
   }

   if(tprev <= a->t_dly && *CKTtime >= a->t_dly)
   {
      *stepped = 1;
      a->pwm_dly = a->pwm;
   }

   if(tprev <= a->t_prd && *CKTtime >= a->t_prd)
   {
      // not used now
   }

   if(tprev <= a->t_zero && *CKTtime >= a->t_zero)
   {
      a->t_prd  = *CKTtime + 1. * (double)a->prd * DGTL_CLK;
      a->t_zero = *CKTtime + 2. * (double)a->prd * DGTL_CLK;
      if(a->cmpa == a->prd)
      {
         a->t_on = *CKTtime + DGTL_CLK * (double)a->prd;
      }
      else if(a->cmpa == 0)
      {
         a->t_off = *CKTtime + 2. * (double)a->prd * DGTL_CLK;
      }
      else
      {
         a->t_on = *CKTtime + DGTL_CLK * (double)(2*a->prd - a->cmpa);
         a->t_off = *CKTtime + DGTL_CLK * (double)a->cmpa;
      }
   }

   a->outa = a->pwm && a->pwm_dly;
   a->outb = !a->pwm && !a->pwm_dly;
}