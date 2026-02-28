// Automatically generated C++ file on Thu Sep 25 10:02:51 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD srf.cpp kernel32.lib

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
#undef va
#undef vb
#undef vc
#undef srf_d
#undef srf_q
#undef srf_w
#undef srf_th

struct sSRF
{
  // declare the structure here
  double tstep;
  bool switched;
  double tprev;

  double va;
  double vb;
  double vd;
  double vq;
  double w;
  double vq_prev;
  double th;
  double cth;
  double sth;
};

#define PI(X, X_PREV, KP, KITS)         \
            KP*(X - X_PREV) + X*KITS;   \
            X_PREV = X;

#define LIMIT(X, MAX, MIN)      \
            if(X > MAX) X = MAX;\
            if(X < MIN) X = MIN;

#define LPF(Y, X, K) Y=Y*(1-K)+X*K

#define TSAMPLING 100E-6
#define VNOM      310.

#define LPF_K     TSAMPLING/(TSAMPLING + sqrt(2.)/(2 * M_PI * 50))

#define KP        (2./sqrt(2.) * (2.*M_PI*30.) / VNOM)
#define KITS      TSAMPLING * (2.*M_PI*30.)*(2.*M_PI*30.) / VNOM

extern "C" __declspec(dllexport) void srf(struct sSRF **opaque, double t, union uData *data)
{
   double  va     = data[0].d; // input
   double  vb     = data[1].d; // input
   double  vc     = data[2].d; // input
   double &vd  = data[3].d; // output
   double &vq  = data[4].d; // output
   double &w  = data[5].d; // output
   double &th = data[6].d; // output

   if(!*opaque)
   {
      *opaque = (struct sSRF *) malloc(sizeof(struct sSRF));
      bzero(*opaque, sizeof(struct sSRF));
   }
   struct sSRF *inst = *opaque;

// Implement module evaluation code here:
   if((t >= inst->tstep) && (inst->tprev <=inst->tstep))
   {
      inst->tstep += TSAMPLING;
      inst->switched = 1;

      vd = inst->vd;
      vq = inst->vq;
      w = inst->w;
      th = inst->th;


      // 3phase SRF-PLL

      inst->cth = cos(inst->th);
      inst->sth = sin(inst->th);

      inst->va    = 2./3. * (va - 0.5 * (vb + vc));
      inst->vb    = 2./3. * (0.86602540378 * (vb - vc));

      inst->vd    =      inst->cth * inst->va + inst->sth * inst->vb;
      inst->vq    = -1 * inst->sth * inst->va + inst->cth * inst->vb;

      inst->w    += PI(inst->vq, inst->vq_prev, KP, KITS)
      inst->th   += inst->w * TSAMPLING;

      if(inst->th > 2 * M_PI) inst->th -= 2 * M_PI;
      if(inst->th < 0) inst->th += 2 * M_PI;
   }

   inst->tprev = t;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sSRF *inst, double t)
{
   if(inst->switched)
   {
      inst->switched = 0;
      return 1E-8;
   }
   else
   {
      return inst->tstep - inst->tprev;
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sSRF *inst)
{
   free(inst);
}
