// Automatically generated C++ file on Tue Sep  2 22:45:03 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD psfb_pcmc.cpp kernel32.lib

#include <malloc.h>
#include <math.h>

extern "C" __declspec(dllexport) void (*Display)(const char *format, ...)       = 0; // works like printf()
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
#undef ref
#undef Isense
#undef PA_HI
#undef PA_LO
#undef PB_HI
#undef PB_LO
#undef rst

struct sPSFB_PCMC
{
  // declare the structure here
  double t_prev;

  double ttol;

  int switched;

  double OUTA_TMAX;
  double OUTA;
  double OUTA_DLY;
  double OUTA_DTIME;

  double diff;
  double diff_prev;

  double OUTB_TMAX;
  double OUTB;
  double OUTB_DLY;
  double OUTB_DTIME;

  double OUTB_FLG;
};

extern "C" __declspec(dllexport) void psfb_pcmc(struct sPSFB_PCMC **opaque, double t, union uData *data)
{
   double  ref     = data[ 0].d; // input
   double  Isense  = data[ 1].d; // input
   double  ttol    = data[ 2].d; // input parameter
   double  fsw     = data[ 3].d; // input parameter
   double  dtime   = data[ 4].d; // input parameter
   double  zero_os = data[ 5].d; // input parameter
   double  Dmax    = data[ 6].d; // input parameter
   double &PA_HI   = data[ 7].d; // output
   double &PA_LO   = data[ 8].d; // output
   double &PB_HI   = data[ 9].d; // output
   double &PB_LO   = data[10].d; // output
   double &rst     = data[11].d; // output
   double &duty    = data[12].d; // output

   if(!*opaque)
   {
      *opaque = (struct sPSFB_PCMC *) malloc(sizeof(struct sPSFB_PCMC));
      bzero(*opaque, sizeof(struct sPSFB_PCMC));
      struct sPSFB_PCMC *inst = *opaque;

      inst->ttol = ttol;
   }
   struct sPSFB_PCMC *inst = *opaque;

// Implement module evaluation code here:
   inst->switched = 0;

   double OUTA = inst->OUTA;
   // ====================================
   // OUTA modulation ====================
   // ====================================
   if((t >= inst->OUTA_TMAX)&&(inst->t_prev <= inst->OUTA_TMAX))
   {
      inst->OUTB_TMAX = inst->OUTA_TMAX + Dmax*0.5/fsw;
      inst->OUTA_TMAX = inst->OUTA_TMAX + 0.5/fsw;

      inst->switched = 1;

      inst->OUTA = 1 - inst->OUTA;
      inst->OUTA_DTIME = t + dtime;
   }

   if((t >= inst->OUTA_DTIME)&&(inst->t_prev <= inst->OUTA_DTIME))
   {
      inst->switched = 1;
      inst->OUTA_DLY = inst->OUTA;
   }

   double OUTB = inst->OUTB;
   // ====================================
   // OUTB modulation ====================
   // ====================================
   // use comparator logic between Ref and Isense with some zero_os to control the OUTB
   if(inst->OUTA != inst->OUTB)
   {
      if(ref < zero_os)
      {
         inst->switched = 1;
         inst->OUTB = inst->OUTA;
         inst->OUTB_DTIME = inst->OUTA_DTIME;
      }
      else
      {
         inst->diff = ref - (Isense + zero_os);
         if((inst->diff <= 0) && (inst->diff_prev >= 0))
         {
            inst->OUTB_FLG = 1 - inst->OUTB_FLG;

            inst->switched = 1;
            inst->OUTB = inst->OUTA;
            inst->OUTB_DTIME = t + dtime;
         }

      }
   }

   // apply max duty logic timing control
   if((t >= inst->OUTB_TMAX)&&(inst->t_prev <= inst->OUTB_TMAX))
   {
      inst->switched = 1;
      inst->OUTB = inst->OUTA;
      inst->OUTB_DTIME = t + dtime;
   }

   if((t >= inst->OUTB_DTIME)&&(inst->t_prev <= inst->OUTB_DTIME))
   {
      inst->switched = 1;
      inst->OUTB_DLY = inst->OUTB;
   }

   inst->diff_prev = inst->diff;
   inst->t_prev = t;

   // gating signal output
   rst = (OUTA != inst->OUTA)||(ref < (Isense + zero_os)||(inst->OUTA==inst->OUTB));

   PA_HI = inst->OUTA * inst->OUTA_DLY;
   PA_LO = (1 - inst->OUTA) * (1 - inst->OUTA_DLY);

   PB_HI = inst->OUTB * inst->OUTB_DLY;
   PB_LO = (1 - inst->OUTB) * (1 - inst->OUTB_DLY);

   if(OUTB != inst->OUTB)
   {
      duty = (t - (inst->OUTA_TMAX - 0.5/fsw))/(0.5/fsw);
   }
}

#define MAXSTEP_CALC(X) if(inst->t_prev < X){if(maxstep > (X - inst->t_prev)){maxstep = X - inst->t_prev;}}
extern "C" __declspec(dllexport) double MaxExtStepSize(struct sPSFB_PCMC *inst, double t)
{
   double maxstep = 1;

   MAXSTEP_CALC(inst->OUTA_TMAX)
   MAXSTEP_CALC(inst->OUTA_DTIME)

   MAXSTEP_CALC(inst->OUTB_TMAX)
   MAXSTEP_CALC(inst->OUTB_DTIME)

   return maxstep;
}

extern "C" __declspec(dllexport) void Trunc(struct sPSFB_PCMC *inst, double t, union uData *data, double *timestep)
{
   if(inst->switched)
   {
      // apply timestep = ttol right after discontinuity occurs
      *timestep = inst->ttol;
   }
   else
   {
      if(inst->OUTA != inst->OUTB)
      {
         // use template ttol logic to ensure comparator logic to occur within required time accuracy
         // only apply when OUTA != OUTB, which is the only time when comparator logic is required
         struct sPSFB_PCMC tmp =  *inst;
         psfb_pcmc(&(&tmp), t, data);

         if(inst->OUTB_FLG != tmp.OUTB_FLG)
         {
            *timestep = inst->ttol;
         }
         else
         {
            *timestep = INFINITY;
         }
      }
      else
      {
         *timestep = INFINITY;
      }
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sPSFB_PCMC *inst)
{
   free(inst);
}
