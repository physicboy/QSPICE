// Automatically generated C++ file on Wed Dec  6 14:28:29 2023
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD buck.cpp kernel32.lib

#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#define PI(y_prev,x,x_prev,kp,kits) y_prev+kp*(x-x_prev)+kits*x

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
#undef IL
#undef pwm
#undef clk

struct sBUCK
{
  // declare the structure here
  long long int xcntr;
  double clk_prev;

  double verror;
  double verror_prev;
  double iref;
  double ierror;
  double ierror_prev;
  double out;
};

extern "C" __declspec(dllexport) void buck(struct sBUCK **opaque, double t, union uData *data)
{
   const double pwm_peak = 200;
   const double vin = 48;
   const double vout_ref = 14;

   double  vout = data[0].d; // input
   double  IL   = data[1].d; // input
   double  clk  = data[2].d; // input
   double &pwm  = data[3].d; // output
   double &iref = data[4].d; // output

   if(!*opaque)
   {
      *opaque = (struct sBUCK *) malloc(sizeof(struct sBUCK));
      bzero(*opaque, sizeof(struct sBUCK));
   }
   struct sBUCK *inst = *opaque;

// Implement module evaluation code here:
   if((clk>=0.5) && (inst->clk_prev<=0.5))
   {
      inst->xcntr++;

      pwm = round(inst->out);
      iref = inst->iref;

      inst->verror = vout_ref - vout;
      inst->iref = PI(inst->iref, inst->verror, inst->verror_prev, 1.39213, 0.1623);
      if(inst->iref > 20) inst->iref = 20;
      if(inst->iref < 0) inst->iref = 0;
      inst->verror_prev = inst->verror;

      inst->ierror = inst->iref - IL;
      inst->out = PI(inst->out, inst->ierror, inst->ierror_prev, 1.84017, 0.128283);
      if(inst->out > pwm_peak) inst->out = pwm_peak;
      if(inst->out < 0) inst->out = 0;
      inst->ierror_prev = inst->ierror;


   }
   inst->clk_prev = clk;

}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sBUCK *inst)
{
   return 1e308; // implement a good choice of max timestep size that depends on struct sBUCK
}

extern "C" __declspec(dllexport) void Trunc(struct sBUCK *inst, double t, union uData *data, double *timestep)
{ // limit the timestep to a tolerance if the circuit causes a change in struct sBUCK
   const double ttol = 1e-9;
   if(*timestep > ttol)
   {
      double &pwm  = data[3].d; // output
      double &iref = data[4].d; // output

      // Save output vector
      const double _pwm  = pwm ;
      const double _iref = iref;

      struct sBUCK tmp = *inst;
      buck(&(&tmp), t, data);
      if(tmp.xcntr != inst->xcntr) // implement a meaningful way to detect if the state has changed
         *timestep = ttol;

      // Restore output vector
      pwm  = _pwm ;
      iref = _iref;
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sBUCK *inst)
{
   free(inst);
}
