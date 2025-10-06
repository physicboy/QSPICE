// Automatically generated C++ file on Fri Sep 26 23:19:07 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD pwm.cpp kernel32.lib

#include <malloc.h>

extern "C" __declspec(dllexport) void (*bzero)(void *ptr, unsigned int count)   = 0;

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

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef duty1a
#undef tri_a
#undef p1a_hi
#undef p1a_lo
#undef p2a_hi
#undef p2a_lo
#undef duty2a

struct sPWM
{
  // declare the structure here
  double tprev;

  long long int counter;

  bool switched;

  double tri_prev;
  double tri_prev2;

  double d1_sample;
  double d2_sample;

  double p1dt;
  double p2dt;

  bool p1;
  bool p2;

  bool p1_d;
  bool p2_d;
};

extern "C" __declspec(dllexport) void pwm(struct sPWM **opaque, double t, union uData *data)
{
   double duty1 = data[0].d; // input
   double tri   = data[1].d; // input
   double duty2 = data[2].d; // input
   bool   mode  = data[3].b; // input parameter
   double prd   = data[4].d; // input parameter
   bool  &p1hi  = data[5].b; // output
   bool  &p1lo  = data[6].b; // output
   bool  &p2hi  = data[7].b; // output
   bool  &p2lo  = data[8].b; // output

   if(!*opaque)
   {
      *opaque = (struct sPWM *) malloc(sizeof(struct sPWM));
      bzero(*opaque, sizeof(struct sPWM));
   }
   struct sPWM *inst = *opaque;

// Implement module evaluation code here:
   const double dtime1 = 1E-6;
   const double dtime2 = 1E-6;

   // implement duty command sampling =====
   if((tri >= inst->tri_prev) && (inst->tri_prev2 >= inst->tri_prev))
   {
      inst->d1_sample = duty1;
      inst->d2_sample = duty2;

      // added a correction by a small 0.5 offset to force duty command
      // to be always higher than the carrier triangle

      if(mode == 0)
      {
         if(inst->d1_sample >= (prd - 0.5)) inst->d1_sample = prd + 0.5;
         if(inst->d2_sample >= (prd - 0.5)) inst->d2_sample = prd + 0.5;
      }
   }

   // PWM logic

   bool p1p = inst->p1;
   bool p2p = inst->p2;

   if(mode == 0)
   {
      inst->p1 = inst->d1_sample > tri;
      inst->p2 = inst->d2_sample > tri;
   }
   else
   {
      if(inst->d1_sample <= 0.5)
      {
         if((tri >= inst->tri_prev) && (inst->tri_prev2 >= inst->tri_prev))
         {
            inst->p1 = 0;
         }
      }
      else if(inst->d1_sample >= (prd - 0.5))
      {
         if((tri <= inst->tri_prev) && (inst->tri_prev2 <= inst->tri_prev))
         {
            inst->p1 = 1;
         }
      }
      else
      {
         if((tri >= inst->d1_sample) && (inst->tri_prev <= inst->d1_sample))
         {
            inst->p1 = 0;
         }
         if((tri <= inst->d1_sample) && (inst->tri_prev >= inst->d1_sample))
         {
            inst->p1 = 1;
         }
      }

      if(inst->d2_sample <= 0.5)
      {
         if((tri >= inst->tri_prev) && (inst->tri_prev2 >= inst->tri_prev))
         {
            inst->p2 = 0;
         }
      }
      else if(inst->d2_sample >= (prd - 0.5))
      {
         if((tri <= inst->tri_prev) && (inst->tri_prev2 <= inst->tri_prev))
         {
            inst->p2 = 1;
         }
      }
      else
      {
         if((tri >= inst->d2_sample) && (inst->tri_prev <= inst->d2_sample))
         {
            inst->p2 = 0;
         }
         if((tri <= inst->d2_sample) && (inst->tri_prev >= inst->d2_sample))
         {
            inst->p2 = 1;
         }
      }
   }
   // ==================================================================================
   inst->switched = 0;
   if(p1p != inst->p1){inst->counter++; inst->p1dt = t + dtime1; inst->switched = 1;}
   if(p2p != inst->p2){inst->counter++; inst->p2dt = t + dtime2; inst->switched = 1;}

   // ==================================================================================

   if((t >= inst->p1dt)&&(inst->tprev <= inst->p1dt)){inst->p1_d = inst->p1; inst->switched = 1;}
   if((t >= inst->p2dt)&&(inst->tprev <= inst->p2dt)){inst->p2_d = inst->p2; inst->switched = 1;}

   // ==================================================================================
   p1hi = inst->p1 & inst->p1_d;
   p2hi = inst->p2 & inst->p2_d;
   p1lo = (1 - inst->p1) & (1 - inst->p1_d);
   p2lo = (1 - inst->p2) & (1 - inst->p2_d);

   inst->tri_prev2 = inst->tri_prev;
   inst->tri_prev = tri;

   inst->tprev = t;
}


#define TTOL            10E-9
#define MAXSTEP_CALC(X) if(inst->tprev < X){if(maxstep > (X - inst->tprev)){maxstep = X - inst->tprev;}}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sPWM *inst, double t)
{
   double maxstep = 1e308;


   if(inst->switched)
   {
      maxstep = TTOL;
   }
   else
   {

      MAXSTEP_CALC(inst->p1dt)
      MAXSTEP_CALC(inst->p2dt)
   }
   return maxstep; // implement a good choice of max timestep size that depends on struct sPWM
}

extern "C" __declspec(dllexport) void Trunc(struct sPWM *inst, double t, union uData *data, double *timestep)
{
   if((t - inst->tprev) > TTOL)
   {
      struct sPWM tmp = *inst;
      pwm(&(&tmp), t, data);
      if(tmp.counter != inst->counter)
      {
         *timestep = t - inst->tprev;
         if(*timestep > TTOL) *timestep *= 0.4;
      }
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sPWM *inst)
{
   free(inst);
}
