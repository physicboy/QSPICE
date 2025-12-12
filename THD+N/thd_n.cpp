// Automatically generated C++ file on Thu Dec 11 14:44:05 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD thd_n.cpp kernel32.lib

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
#undef i1
#undef i2
#undef i3

struct sTHD_N
{
  // declare the structure here
  double tprev;

  double ins1p; double inc1p; double is1; double ic1;
  double ins2p; double inc2p; double is2; double ic2;
  double ins3p; double inc3p; double is3; double ic3;

  double isum1; double isum2; double isum3;

  double i1p; double i2p; double i3p;

  bool meas;

  double start;
  double stop;
};

bool printed = 0;

extern "C" __declspec(dllexport) void thd_n(struct sTHD_N **opaque, double t, union uData *data)
{
   double i1    = data[0].d; // input
   double i2    = data[1].d; // input
   double i3    = data[2].d; // input
   double start = data[3].d; // input parameter
   double stop  = data[4].d; // input parameter
   double fund  = data[5].d; // input parameter

   if(!*opaque)
   {
      *opaque = (struct sTHD_N *) malloc(sizeof(struct sTHD_N));
      bzero(*opaque, sizeof(struct sTHD_N));
      struct sTHD_N *inst = *opaque;

      inst->start = start;
      inst->stop = stop;
   }
   struct sTHD_N *inst = *opaque;

// Implement module evaluation code here:
   double dt = t - inst->tprev;

   if(t >= start && t <= stop)
   {
      double cc = cos(2*M_PI*fund*t);
      double ss = sin(2*M_PI*fund*t);

      double ins1 = i1 * ss; double inc1 = i1 * cc;
      double ins2 = i2 * ss; double inc2 = i2 * cc;
      double ins3 = i3 * ss; double inc3 = i3 * cc;

      if(inst->meas == 0)
      {
         inst->meas = 1;

         inst->ins1p = ins1; inst->inc1p = inc1;
         inst->ins2p = ins2; inst->inc2p = inc2;
         inst->ins3p = ins3; inst->inc3p = inc3;

         inst->i1p = i1;
         inst->i2p = i2;
         inst->i3p = i3;
      }
      else
      {
         inst->is1 += 0.5 * (ins1 + inst->ins1p) * dt;
         inst->ic1 += 0.5 * (inc1 + inst->inc1p) * dt;
         inst->isum1 += 0.5 * (i1 * i1 + inst->i1p * inst->i1p) * dt;

         inst->is2 += 0.5 * (ins2 + inst->ins2p) * dt;
         inst->ic2 += 0.5 * (inc2 + inst->inc2p) * dt;
         inst->isum2 += 0.5 * (i2 * i2 + inst->i2p * inst->i2p) * dt;

         inst->is3 += 0.5 * (ins3 + inst->ins3p) * dt;
         inst->ic3 += 0.5 * (inc3 + inst->inc3p) * dt;
         inst->isum3 += 0.5 * (i3 * i3 + inst->i3p * inst->i3p) * dt;

         inst->ins1p = ins1; inst->inc1p = inc1;
         inst->ins2p = ins2; inst->inc2p = inc2;
         inst->ins3p = ins3; inst->inc3p = inc3;

         inst->i1p = i1;
         inst->i2p = i2;
         inst->i3p = i3;
      }
   }

   if(!printed)
   {
      if(t > stop)
      {
         printed = 1;

         double a;
         double b;
         double x;

         double T = stop - start;

         a = inst->ic1 * 2. / T;
         b = inst->is1 * 2. / T;
         double fund1 = sqrt(a * a + b * b)/sqrt(2.);
         double rms1 = sqrt(inst->isum1 / T); x = rms1 / fund1;
         double thd1 = sqrt(fabs(x * x - 1));

         a = inst->ic2 * 2. / T;
         b = inst->is2 * 2. / T;
         double fund2 = sqrt(a * a + b * b)/sqrt(2);
         double rms2 = sqrt(inst->isum2 / T); x = rms2 / fund2;
         double thd2 = sqrt(fabs(x * x - 1));

         a = inst->ic3 * 2. / T;
         b = inst->is3 * 2. / T;
         double fund3 = sqrt(a * a + b * b)/sqrt(2.);
         double rms3 = sqrt(inst->isum3 / T); x = rms3 / fund3;
         double thd3 = sqrt(fabs(x * x - 1));

         Display("fund1 = %.5f, rms1 = %.5f, thd1 = %.5f %%\n", fund1, rms1, 100. * thd1);
         Display("fund2 = %.5f, rms2 = %.5f, thd2 = %.5f %%\n", fund2, rms2, 100. * thd2);
         Display("fund3 = %.5f, rms3 = %.5f, thd3 = %.5f %%\n", fund3, rms3, 100. * thd3);
      }
   }

   inst->tprev = t;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sTHD_N *inst, double t)
{
   if(t < inst->start){return inst->start - inst->tprev;}
   else if(t < inst->stop){return inst->stop - inst->tprev;}
   else return 1e308; // implement a good choice of max timestep size that depends on struct sTHD_N
}

extern "C" __declspec(dllexport) void Destroy(struct sTHD_N *inst)
{
   free(inst);
}
