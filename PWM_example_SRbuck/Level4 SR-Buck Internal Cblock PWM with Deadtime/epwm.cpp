// Automatically generated C++ file on Mon Jan 15 10:32:54 2024
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD epwm.cpp kernel32.lib

#include "main.h"

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

void display(const char *fmt, ...)
{ // for diagnostic print statements
   msleep(30);
   fflush(stdout);
   va_list args = { 0 };
   va_start(args, fmt);
   vprintf(fmt, args);
   va_end(args);
   fflush(stdout);
   msleep(30);
}

void bzero(void *ptr, unsigned int count)
{
   unsigned char *first = (unsigned char *) ptr;
   unsigned char *last  = first + count;
   while(first < last)
      *first++ = '\0';
}

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef vout
#undef isense
#undef pwmhi
#undef pwmlo
#undef duty
#undef iref
#undef out1
#undef out2

struct sEPWM
{
   // declare the structure here
   struct pwm_data pwm1;
   struct controller_data ctrl;

   long long int xcntr;
   bool pwm_trigger;
   double maxstep;

   bool interrupt_flag;

   double t_prev;
};

extern "C" __declspec(dllexport) void epwm(struct sEPWM **opaque, double t, union uData *data)
{
   double  vout   = data[0].d; // input
   double  isense = data[1].d; // input
   double &pwmhi  = data[2].d; // output
   double &pwmlo  = data[3].d; // output
   double &duty   = data[4].d; // output
   double &iref   = data[5].d; // output
   double &out1   = data[6].d; // output
   double &out2   = data[7].d; // output

   if(!*opaque)
   {
      *opaque = (struct sEPWM *) malloc(sizeof(struct sEPWM));
      bzero(*opaque, sizeof(struct sEPWM));

      struct sEPWM *inst = *opaque;

      inst->maxstep = 1e-9;

      pwm_gen_init(&inst->pwm1);
      control_init(&inst->ctrl);
   }
   struct sEPWM *inst = *opaque;

// Implement module evaluation code here:
   inst->pwm_trigger = false;
   inst->interrupt_flag = false;

   pwm_gen(&inst->pwm1, &inst->interrupt_flag, &inst->xcntr,
      &t, &inst->t_prev, &inst->maxstep, &inst->pwm_trigger, true);

   if(inst->interrupt_flag)
   {
      control(&inst->ctrl, &vout, &isense, 20, inst->pwm1.peak);

      inst->pwm1.cmp_rise = round(inst->ctrl.out);
      inst->pwm1.cmp_fall = inst->pwm1.cmp_rise;
   }

   pwmhi = (double)inst->pwm1.pwm_hi;
   pwmlo = (double)inst->pwm1.pwm_lo;

   duty = inst->ctrl.out;
   iref = inst->ctrl.iref;

   out1 = inst->pwm1.trg_zero;
   out2 = inst->maxstep;

   inst->t_prev = t;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sEPWM *inst)
{
   return inst->maxstep; // implement a good choice of max timestep size that depends on struct sEPWM
}

//#define TRUNC_METHOD

extern "C" __declspec(dllexport) void Trunc(struct sEPWM *inst, double t, union uData *data, double *timestep)
{ // limit the timestep to a tolerance if the circuit causes a change in struct sEPWM
   const double ttol = 1e-9;

   #ifdef TRUNC_METHOD
   if(*timestep > ttol)
   {
      double &pwmhi  = data[2].d; // output
      double &pwmlo  = data[3].d; // output
      double &duty   = data[4].d; // output
      double &iref   = data[5].d; // output
      double &out1   = data[6].d; // output
      double &out2   = data[7].d; // output

      // Save output vector
      const double _pwmhi  = pwmhi ;
      const double _pwmlo  = pwmlo ;
      const double _duty   = duty  ;
      const double _iref   = iref  ;
      const double _out1   = out1  ;
      const double _out2   = out2  ;

      struct sEPWM tmp = *inst;
      epwm(&(&tmp), t, data);
      if(tmp.xcntr != inst->xcntr) // implement a meaningful way to detect if the state has changed
         *timestep = ttol;

      // Restore output vector
      pwmhi  = _pwmhi ;
      pwmlo  = _pwmlo ;
      duty   = _duty  ;
      iref   = _iref  ;
      out1   = _out1  ;
      out2   = _out2  ;
   }
   #else

   pwm_trunc_handler(&inst->pwm1, &inst->t_prev, timestep, &inst->pwm_trigger);

   #endif

}

extern "C" __declspec(dllexport) void Destroy(struct sEPWM *inst)
{
   free(inst);
}
