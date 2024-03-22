// Automatically generated C++ file on Mon Dec 11 12:54:23 2023
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD epwm.cpp kernel32.lib

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
#undef isense
#undef carrier
#undef pwm
#undef duty
#undef iref
#undef out1
#undef out2

struct sEPWM
{
  // declare the structure here
  long long int xcntr;
  double maxstep;
  double t_prev;

  double trg1; // trigger cmp at rising
  double trg2; // trigger at half period
  double trg3; // trigger cmp at falling
  double trg4; // trigger at full period

  double xpeak;
  double xcmp;

  bool pwm_trigger;

  double verror;
  double verror_prev;
  double iref;
  double ierror;
  double ierror_prev;
  double out;
};

extern "C" __declspec(dllexport) void epwm(struct sEPWM **opaque, double t, union uData *data)
{
   const double mcu_clk = 100E6;
   const double peak    = 200;

   //====================================
   // Control Variable ==================
   //====================================
   const double vin = 48;
   const double vout_ref = 14;



   double  vout    = data[0].d; // input
   double  isense  = data[1].d; // input
   double &carrier = data[2].d; // output
   double &pwm     = data[3].d; // output
   double &duty    = data[4].d; // output
   double &iref    = data[5].d; // output
   double &out1    = data[6].d; // output
   double &out2    = data[7].d; // output

   if(!*opaque)
   {
      *opaque = (struct sEPWM *) malloc(sizeof(struct sEPWM));
      bzero(*opaque, sizeof(struct sEPWM));

      struct sEPWM *inst = *opaque;

      carrier = 0;
      pwm = 0;

      inst->xpeak= peak;
      inst->xcmp = 0;

      inst->trg1 = 2*inst->xpeak/mcu_clk;
      inst->trg2 = 2*inst->xpeak/mcu_clk;
      inst->trg3 = 2*inst->xpeak/mcu_clk;
      inst->trg4 = 2*inst->xpeak/mcu_clk;

      inst->maxstep = 1e-9;
   }
   struct sEPWM *inst = *opaque;

// Implement module evaluation code here:
   inst->pwm_trigger = false;

   const double _pwm = pwm;

   if((inst->t_prev <= inst->trg4)&&(t >= inst->trg4))
   {
      inst->xcntr++;
      inst->xpeak= peak;
      inst->xcmp = round(inst->out);

      carrier = 0;

      inst->trg1 = inst->trg4 + inst->xcmp/mcu_clk;
      inst->trg2 = inst->trg4 + inst->xpeak/mcu_clk;
      inst->trg3 = inst->trg4 + (2*inst->xpeak - inst->xcmp)/mcu_clk;
      inst->trg4 = inst->trg4 + 2*inst->xpeak/mcu_clk;

      inst->maxstep = peak/mcu_clk;

      //===================================================================
      // control algorithm interrupt - START ==============================
      //===================================================================

      inst->verror = vout_ref - vout;
      inst->iref = PI(inst->iref, inst->verror, inst->verror_prev, 1.39213, 0.1623);
      if(inst->iref > 20) inst->iref = 20;
      if(inst->iref < 0) inst->iref = 0;
      inst->verror_prev = inst->verror;

      inst->ierror = inst->iref - isense;
      inst->out = PI(inst->out, inst->ierror, inst->ierror_prev, 1.84017, 0.128283);
      if(inst->out > peak) inst->out = peak;
      if(inst->out < 0) inst->out = 0;
      inst->ierror_prev = inst->ierror;

      duty = inst->out;
      iref = inst->iref;

      //===================================================================
      // control algorithm interrupt - END   ==============================
      //===================================================================
   }

   if((inst->t_prev <= inst->trg2)&&(t >= inst->trg2))
   {
      inst->xcntr++;
      carrier = 1;
   }

   if(t < inst->trg2)
   {
      if((inst->t_prev <= inst->trg1)&&(t >= inst->trg1))
      {
         inst->xcntr++;
         pwm = 0;
         inst->pwm_trigger = true;
      }
   }
   else
   {
      if((inst->t_prev <= inst->trg3)&&(t >= inst->trg3))
      {
         inst->xcntr++;
         pwm = 1;
         inst->pwm_trigger = true;
      }
   }

   inst->t_prev = t;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sEPWM *inst)
{
   return inst->maxstep; // implement a good choice of max timestep size that depends on struct sEPWM
}

//#define TRUNC_METHOD

extern "C" __declspec(dllexport) void Trunc(struct sEPWM *inst, double t, union uData *data, double *timestep)
{ // limit the timestep to a tolerance if the circuit causes a change in struct sEPWM
   const double ttol = 10e-9;

   #ifdef TRUNC_METHOD
   if(*timestep > ttol)
   {
      double &carrier = data[2].d; // output
      double &pwm     = data[3].d; // output
      double &duty    = data[4].d; // output
      double &iref    = data[5].d; // output
      double &out1    = data[6].d; // output
      double &out2    = data[7].d; // output

      // Save output vector
      const double _carrier = carrier;
      const double _pwm     = pwm    ;
      const double _duty    = duty   ;
      const double _iref    = iref   ;
      const double _out1    = out1   ;
      const double _out2    = out2   ;

      struct sEPWM tmp = *inst;
      epwm(&(&tmp), t, data);
      if(tmp.xcntr != inst->xcntr) // implement a meaningful way to detect if the state has changed
         *timestep = ttol;

      // Restore output vector
      carrier = _carrier;
      pwm     = _pwm    ;
      duty    = _duty   ;
      iref    = _iref   ;
      out1    = _out1   ;
      out2    = _out2   ;
   }
   #else
   struct sEPWM tmp = *inst;
	if(inst->t_prev < inst->trg1){*timestep = (inst->trg1 - inst->t_prev);}
	if(inst->t_prev < inst->trg2){if(*timestep > (inst->trg2 - inst->t_prev)){*timestep = (inst->trg2 - inst->t_prev);}}
	if(inst->t_prev < inst->trg3){if(*timestep > (inst->trg3 - inst->t_prev)){*timestep = (inst->trg3 - inst->t_prev);}}
	if(inst->t_prev < inst->trg4){if(*timestep > (inst->trg4 - inst->t_prev)){*timestep = (inst->trg4 - inst->t_prev);}}
    if(inst->pwm_trigger)
	{
		if(*timestep > ttol) *timestep = ttol;
	}
   #endif
}

extern "C" __declspec(dllexport) void Destroy(struct sEPWM *inst)
{
   free(inst);
}
