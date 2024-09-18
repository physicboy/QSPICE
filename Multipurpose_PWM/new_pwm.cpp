// Automatically generated C++ file on Mon Sep 16 13:18:27 2024
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD new_cmc.cpp kernel32.lib

#include <malloc.h>

union uData
{
   bool b;
   char c;
   unsigned char uc;
   short s;
   unsigned short us;
   int i;
   unsigned int ui;
   float f;
   double d;
   long long int i64;
   unsigned long long int ui64;
   char *str;
   unsigned char *bytes;
};

// int DllMain() must exist and return 1 for a process to load the .DLL
// See https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain for more information.
int __stdcall DllMain(void *module, unsigned int reason, void *reserved) { return 1; }

void bzero(void *ptr, unsigned int count)
{
   unsigned char *first = (unsigned char *) ptr;
   unsigned char *last  = first + count;
   while(first < last)
      *first++ = '\0';
}

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef in
#undef hi
#undef lo
#undef carrier

struct sNEW_PWM
{
  // declare the structure here
  double t_prev;
  double t_change;

  double delta;

  bool output;
  bool output_delay;

  double carrier_prev1;
  double carrier_prev2;
};

extern "C" __declspec(dllexport) void new_pwm(struct sNEW_PWM **opaque, double t, union uData *data)
{
   double  ref       = data[0].d; // input
   double  carrier   = data[1].d; // input
   double  Isense    = data[2].d; // input
   double  dtime     = data[3].d; // input parameter
   double  maxref    = data[4].d; // input parameter
   bool    cmc0_vmc1 = data[5].d; // input parameter
   double &hi        = data[6].d; // output
   double &lo        = data[7].d; // output

   if(!*opaque)
   {
      *opaque = (struct sNEW_PWM *) malloc(sizeof(struct sNEW_PWM));
      bzero(*opaque, sizeof(struct sNEW_PWM));
   }
   struct sNEW_PWM *inst = *opaque;

   // Implement module evaluation code here:
   if(cmc0_vmc1)
   {
      // VMC operation mode
      if(inst->carrier_prev1 < carrier)
      {
         if(inst->output)
         {
            if(ref < maxref)
            {
               inst->delta = ref - carrier;
            }
            else
            {
               inst->delta = maxref - carrier;
            }
            if(inst->delta < 0)
            {
               inst->output = 0;
               inst->t_change = t + dtime;
            }
         }
      }
      else
      {
         if(!inst->output)
         {
            if(ref < maxref)
            {
               inst->delta = carrier - ref;
            }
            else
            {
               inst->delta = carrier - maxref;
            }
            if(inst->delta < 0)
            {
               inst->output = 1;
               inst->t_change = t + dtime;
            }
         }
      }
   }
   else
   {
      // CMC operation mode
      if((inst->carrier_prev2 > inst->carrier_prev1)&&(carrier > inst->carrier_prev1))
      {
         if(ref > inst->carrier_prev1)
         {
            inst->output = 1;
            inst->t_change = t + dtime;
         }
      }

      if(inst->carrier_prev1 < carrier)
      {
         if(inst->output)
         {
            if(ref < maxref)
            {
               inst->delta = ref - Isense;
            }
            else
            {
               inst->delta = maxref - Isense;
            }
            if(inst->delta < 0)
            {
               inst->output = 0;
               inst->t_change = t + dtime;
            }
         }
      }
   }

   // trigger the delayed output
   if((inst->t_prev <= inst->t_change)&&(t >= inst->t_change))
   {
      inst->output_delay = inst->output;
   }

   hi = inst->output & inst->output_delay;
   lo = (1 - inst->output) & (1 - inst->output_delay);

   inst->carrier_prev2 = inst->carrier_prev1;
   inst->carrier_prev1 = carrier;

   inst->t_prev = t;
}

/*
extern "C" __declspec(dllexport) double MaxExtStepSize(struct sNEW_PWM *inst)
{
   if(inst->t_prev < inst->t_change)
   {
      return inst->t_change - inst->t_prev;
   }
   else
   {
      return 1e308;
   }
}
*/

extern "C" __declspec(dllexport) void Trunc(struct sNEW_PWM *inst, double t, union uData *data, double *timestep)
{ // limit the timestep to a tolerance if the circuit causes a change in struct sNEW_PWM
   const double ttol = 1e-20;

   struct sNEW_PWM tmp = *inst;
   new_pwm(&(&tmp), t, data);

   if((inst->output != tmp.output)||(inst->output_delay != tmp.output_delay))
   {
      if((t - inst->t_prev)>ttol)
      {
         *timestep = (t - inst->t_prev)/2;
      }
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sNEW_PWM *inst)
{
   free(inst);
}
