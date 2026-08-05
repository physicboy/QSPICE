struct sINTERRUPT
{
   double vout[4];
   double iout[4];

   double verror;
   double verror_prev;
   double iref;
   double ierror;
   double ierror_prev;
   double vctrl;
};

#define PID(X,XPREV,KP,KITS)  KP * ((X) - XPREV) + KITS * (X);  \
                              XPREV = X;
#define LPF(Y,X,TAU)          Y = (1. - TAU) * Y + TAU * (X); /*Compute TAU by w*Ts / (1 + w*Ts)  */


void interrupt0(struct sINTERRUPT *a, struct sPWM pwm[], union uData *data)
{
   #include "io_def.inc"
   // Control algorithm executed at carrier valley
   // Direct access to all PWM channels: pwm[0], pwm[1], ... pwm[PWM_CH - 1]

   a->vout[0] = vout;
   a->iout[0] = iout;

   double _vout = (a->vout[0] + a->vout[1] + a->vout[2] + a->vout[3]) / 4.;
   double _iout = (a->iout[0] + a->iout[1] + a->iout[2] + a->iout[3]) / 4.;

   a->verror = LPF(a->verror, 800. - _vout, 0.090909091);

   a->iref += PID(a->verror, a->verror_prev, 0.592, 0.00296);
   if(a->iref > 60.) a->iref = 60.;
   if(a->iref < -60.) a->iref = -60.;

   a->ierror = a->iref - _iout;
   a->vctrl += PID(a->ierror, a->ierror_prev, 7.9, 0.079);
   if(a->vctrl > 1000.) a->vctrl = 1000.;
   if(a->vctrl < 0.) a->vctrl = 0.;

   pwm[0].cmpa = (int)(a->vctrl * TBPRD / 1000.);

   iref = a->iref;
   dbg0 = a->vctrl;
   dbg1 = 0;
   dbg2 = 0;
   dbg3 = 0;

}

void interrupt1(struct sINTERRUPT *a, struct sPWM pwm[], union uData *data)
{
   #include "io_def.inc"
   // Mid-carrier up-slope (4x oversampling feedback measurement)

   a->vout[1] = vout;
   a->iout[1] = iout;
}

void interrupt2(struct sINTERRUPT *a, struct sPWM pwm[], union uData *data)
{
   #include "io_def.inc"
   // Control algorithm executed at carrier peak
   a->vout[2] = vout;
   a->iout[2] = iout;
}

void interrupt3(struct sINTERRUPT *a, struct sPWM pwm[], union uData *data)
{
   #include "io_def.inc"
   // Mid-carrier down-slope (4x oversampling feedback measurement)
   a->vout[3] = vout;
   a->iout[3] = iout;
}