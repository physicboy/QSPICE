// Automatically generated C++ file on Tue Nov 11 00:50:07 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD code.cpp kernel32.lib

#include <malloc.h>
#include <math.h>

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
#undef in
#undef d
#undef q
#undef f
#undef mag

struct FLL_VAR
{
   double d;
   double q;

   double w;
   double os;
};

struct sCODE
{
  // declare the structure here
   double tstep;
   double tprev;
   bool stepped;

   struct FLL_VAR fll1;
   struct FLL_VAR fll2;
   struct FLL_VAR fll3;
};

#define TSAMPLING    20E-6
#define VIN_NOM      310
#define W_NOM        2*M_PI*55

void FLL(FLL_VAR *a, double *in, double bpf_k);

extern "C" __declspec(dllexport) void code(struct sCODE **opaque, double t, union uData *data)
{
   double  in1  = data[ 0].d; // input
   double  in2  = data[ 1].d; // input
   double  in3  = data[ 2].d; // input
   double &d1   = data[ 3].d; // output
   double &q1   = data[ 4].d; // output
   double &f1   = data[ 5].d; // output
   double &mag1 = data[ 6].d; // output
   double &d2   = data[ 7].d; // output
   double &q2   = data[ 8].d; // output
   double &f2   = data[ 9].d; // output
   double &mag2 = data[10].d; // output
   double &d3   = data[11].d; // output
   double &q3   = data[12].d; // output
   double &f3   = data[13].d; // output
   double &mag3 = data[14].d; // output

   if(!*opaque)
   {
      *opaque = (struct sCODE *) malloc(sizeof(struct sCODE));
      bzero(*opaque, sizeof(struct sCODE));
   }
   struct sCODE *inst = *opaque;

// Implement module evaluation code here:
   inst->stepped = 0;
   if((inst->tprev <= inst->tstep) && (t >= inst->tstep))
   {
      inst->stepped = 1;
      inst->tstep += TSAMPLING;

      FLL(&inst->fll1, &in1, 1.414);
      FLL(&inst->fll2, &in2, 1.414);
      FLL(&inst->fll3, &in3, 1.414);

      // d = inst->fll.in_nom;
      d1 = VIN_NOM * inst->fll1.d;
      q1 = VIN_NOM * inst->fll1.q;
      f1 = (W_NOM + inst->fll1.w) / (2 * M_PI);
      mag1 = VIN_NOM * sqrt(inst->fll1.d * inst->fll1.d + inst->fll1.q * inst->fll1.q);

      d2 = VIN_NOM * inst->fll2.d;
      q2 = VIN_NOM * inst->fll2.q;
      f2 = (W_NOM + inst->fll2.w) / (2 * M_PI);
      mag2 = VIN_NOM * sqrt(inst->fll2.d * inst->fll2.d + inst->fll2.q * inst->fll2.q);

      d3 = VIN_NOM * inst->fll3.d;
      q3 = VIN_NOM * inst->fll3.q;
      f3 = (W_NOM + inst->fll3.w) / (2 * M_PI);
      mag3 = VIN_NOM * sqrt(inst->fll3.d * inst->fll3.d + inst->fll3.q * inst->fll3.q);
   }
   inst->tprev = t;
}

void FLL(FLL_VAR *a, double *in, double bpf_k)
{
   double err;

   err = (*in / VIN_NOM; - a->d - a->os);

   a->w += err * a->q * -50 * bpf_k * W_NOM * TSAMPLING;

   if(a->w > 2 * M_PI * 15) a->w = 2 * M_PI * 15;
   if(a->w < -2 * M_PI * 15) a->w = -2 * M_PI * 15;

   a->os += err * 0.25 * W_NOM *TSAMPLING;

   a->q += (a->w + W_NOM) * a->d * TSAMPLING;
   a->d += (a->w + W_NOM) * (bpf_k * err - a->q) * TSAMPLING;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sCODE *inst, double t)
{
   if(inst->stepped) return 10E-9;
   else return inst->tstep - inst->tprev; // implement a good choice of max timestep size that depends on struct sCODE
}

extern "C" __declspec(dllexport) void Destroy(struct sCODE *inst)
{
   free(inst);
}
