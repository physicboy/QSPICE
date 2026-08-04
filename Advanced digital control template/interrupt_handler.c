struct sINTERRUPT
{
   double temp;
};

void interrupt0(struct sINTERRUPT *a, struct sPWM pwm[], union uData *data)
{
   #include "io_def.inc"
   // add control algorithm that is executed at the carrier valley here
   pwm[0].cmpa = (int)800;
}

void interrupt1(struct sINTERRUPT *a, struct sPWM pwm[], union uData *data)
{
   #include "io_def.inc"
}

void interrupt2(struct sINTERRUPT *a, struct sPWM pwm[], union uData *data)
{
   // add control algorithm that is executed at the carrier peak here
   #include "io_def.inc"
}

void interrupt3(struct sINTERRUPT *a, struct sPWM pwm[], union uData *data)
{
   #include "io_def.inc"
}