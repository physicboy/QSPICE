// Automatically generated C++ file on Tue Aug 26 11:19:50 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD noise_x1.cpp kernel32.lib

#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void box_muller(double mean, double stddev, double *z0, double *z1);
void marsaglia_polar(double mean, double stddev, double *z0, double *z1);

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
#undef o1
#undef o2

struct sGAUSSIAN
{
  // declare the structure here
  double step;
  double t_prev;

  /*
  double o1mean;
  double o1stdev;
  double o2mean;
  double o2stdev;
  */
};

extern "C" __declspec(dllexport) void gaussian(struct sGAUSSIAN **opaque, double t, union uData *data)
{
   double  ts    = data[0].d; // input parameter
   double  mean  = data[1].d; // input parameter
   double  stdev = data[2].d; // input parameter
   double &o1    = data[3].d; // output
   double &o2    = data[4].d; // output

   if(!*opaque)
   {
      *opaque = (struct sGAUSSIAN *) malloc(sizeof(struct sGAUSSIAN));
      bzero(*opaque, sizeof(struct sGAUSSIAN));
      srand(time(NULL));
   }
   struct sGAUSSIAN *inst = *opaque;

// Implement module evaluation code here:
   if((t >= inst->step)&&(inst->t_prev <= inst->step))
   {
      inst->step += ts;
      //box_muller(mean, stdev, &o1, &o2);
      marsaglia_polar(mean, stdev, &o1, &o2);
   }
   inst->t_prev = t;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sGAUSSIAN *inst, double t)
{
   return inst->step - inst->t_prev; // implement a good choice of max timestep size that depends on struct sNOISE_X1
}

extern "C" __declspec(dllexport) void Destroy(struct sGAUSSIAN *inst)
{
   //Display("o1Mean: %f\to1Stdev: %f\n", inst->o1mean/(1E6 + 1), inst->o1stdev/(1E6 + 1));
   //Display("o2Mean: %f\to2Stdev: %f\n", inst->o2mean/(1E6 + 1), inst->o2stdev/(1E6 + 1));
   free(inst);
}


void box_muller(double mean, double stddev, double *z0, double *z1)
{
    double u1, u2;

    // generate two uniform(0,1) randoms in (0,1] (avoid log(0))
    u1 = ((double)rand() + 1.0) / (RAND_MAX + 2.0);
    u2 = ((double)rand() + 1.0) / (RAND_MAX + 2.0);

    double r = sqrt(-2.0 * log(u1));
    double theta = 2.0 * M_PI * u2;

    *z0 = mean + stddev * r * cos(theta);
    *z1 = mean + stddev * r * sin(theta);
}

void marsaglia_polar(double mean, double stddev, double *z0, double *z1)
{
    double u, v, s;
    do {
        u = 2.0 * (double)rand() / (RAND_MAX + 1.0) - 1.0; // uniform(-1,1)
        v = 2.0 * (double)rand() / (RAND_MAX + 1.0) - 1.0; // uniform(-1,1)
        s = u * u + v * v;
    } while (s >= 1.0 || s == 0.0);

    double factor = sqrt(-2.0 * log(s) / s);
    *z0 = mean + stddev * u * factor;
    *z1 = mean + stddev * v * factor;
}
