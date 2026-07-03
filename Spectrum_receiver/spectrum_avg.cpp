// Automatically generated C++ file on Fri Jul  3 14:42:16 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD -o emi.cpp kernel32.lib

#include <malloc.h>
#include <math.h>

extern "C" __declspec(dllexport) void (*Display)(const char *format, ...)       = 0; // works like printf()
extern "C" __declspec(dllexport) void (*EXIT)(const char *format, ...)          = 0; // print message like printf() but exit(0) afterward
extern "C" __declspec(dllexport) const double *DegreesC                         = 0; // pointer to current circuit temperature
extern "C" __declspec(dllexport) const int *StepNumber                          = 0; // pointer to current step number
extern "C" __declspec(dllexport) const int *NumberSteps                         = 0; // pointer to estimated number of steps
extern "C" __declspec(dllexport) const char* const *InstanceName                = 0; // pointer to address of instance name
extern "C" __declspec(dllexport) const char *QUX                                = 0; // path to QUX.exe
extern "C" __declspec(dllexport) const bool *ForKeeps                           = 0; // pointer to whether being evaluated non-hypothetically
extern "C" __declspec(dllexport) const bool *HoldICs                            = 0; // pointer to whether instance initial conditions are being held
extern "C" __declspec(dllexport) const void *GUI_HWND                           = 0; // pointer to Window handle of QUX.exe
extern "C" __declspec(dllexport) const double *CKTtime                          = 0;
extern "C" __declspec(dllexport) const double *CKTdelta                         = 0;
extern "C" __declspec(dllexport) const int *IntegrationOrder                    = 0;
extern "C" __declspec(dllexport) const char *InstallDirectory                   = 0;
extern "C" __declspec(dllexport) double (*EngAtof)(const char **string)         = 0;
extern "C" __declspec(dllexport) const char *(*BinaryFormat)(unsigned int data)                          = 0; // BinaryFormat(0x1C) returns "0b00011100"
extern "C" __declspec(dllexport) const char *(*EngFormat   )(double x, const char *units, int numDgts)   = 0; // EngFormat(1e-6, "s", 6) returns "1µs"
extern "C" __declspec(dllexport) int (*DFFT)(struct sComplex *u, bool inv, unsigned int N, double scale) = 0; // Discrete Fast Fourier Transform
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
#undef in1

struct ff
{
   double f;
   double ww;
   double d;
   double q;

   double rms;
};

#define DATA_LENGTH 20000

struct sSPECTRUM_AVG
{
  // declare the structure here
  double tprev;
  double inprev;

  double beta;

  struct ff f[DATA_LENGTH];
};


double bpf(struct ff *f, double in, double tstep, double beta, bool acc);
extern "C" __declspec(dllexport) void spectrum_avg(struct sSPECTRUM_AVG **opaque, double t, union uData *data)
{
   double in     = data[0].d; // input
   double RBW    = data[1].d; // input parameter
   double tstart = data[2].d; // input parameter
   double tend   = data[3].d; // input parameter
   double fmin   = data[4].d; // input parameter
   double fstep  = data[5].d; // input parameter
   double fmax   = data[6].d; // input parameter

   long counter = 0;

   if(!*opaque)
   {
      *opaque = (struct sSPECTRUM_AVG *) malloc(sizeof(struct sSPECTRUM_AVG));
      bzero(*opaque, sizeof(struct sSPECTRUM_AVG));
      struct sSPECTRUM_AVG *inst = *opaque;

      counter = 0;

      inst->beta = 2.*M_PI*RBW/sqrt(3.);

      for(double freq = fmin; freq <= fmax; freq += fstep)
      {
         inst->f[counter].f = freq;
         inst->f[counter].ww = 2 * M_PI * freq * 2 * M_PI * freq;
         counter++;
      }
   }
   struct sSPECTRUM_AVG *inst = *opaque;

// Implement module evaluation code here:

   double tsampling  = t - inst->tprev;
   double tstep      = tsampling;
   double in_slope   = (in - inst->inprev) / tsampling;
   double in_interp;

   for(counter = 0; counter < DATA_LENGTH - 1; counter++)
   {
      tstep = tsampling;
      if(inst->f[counter].f == 0) break;
      else
      {
         double ts_max = 0.005 / inst->f[counter].f;
         while(tstep > ts_max)
         {
            tstep -= ts_max;
            in_interp = in - in_slope * tstep;
            bpf(&inst->f[counter], in_interp, ts_max, inst->beta, ((t - tstep) > tstart) & ((t - tstep) <= tend));
         }
         bpf(&inst->f[counter], in, tstep, inst->beta, (t > tstart) & (t <= tend));
      }
   }

   inst->tprev = t;
   inst->inprev = in;
}

double bpf(struct ff *f, double in, double tstep, double beta, bool acc)
{
   f->q    += f->d * tstep;
   f->d    += (beta * (in - f->d) - f->ww * f->q) * tstep;

   if(acc)
   {
      f->rms += f->d * f->d * tstep;
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sSPECTRUM_AVG *inst)
{
   long counter;
   for(counter = 0; counter < DATA_LENGTH - 1; counter++)
   {
      if(inst->f[counter].f == 0) break;
      else Display("freq: %e\trms = %e\n", inst->f[counter].f, sqrt(inst->f[counter].rms / 1e-3));
   }
   free(inst);
}
