// Automatically generated C++ file on Wed Dec 17 09:20:16 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD _3ph_analyzer.cpp kernel32.lib

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
#undef v1
#undef i1
#undef v2
#undef i2
#undef v3
#undef i3

struct SOGI_VAR
{
   double in_d;
   double inp_d;
   double d;

   double in_q;
   double inp_q;
   double q;
   double qp;

   double in_os;
   double inp_os;
   double os;
};

struct sDATA
{
  // declare the structure here
  double tprev;

  double p1; double p2; double p3;
  double q1; double q2; double q3;

  struct SOGI_VAR sogi1;
  struct SOGI_VAR sogi2;
  struct SOGI_VAR sogi3;

  double vins1p; double vinc1p; double vs1; double vc1;
  double vins2p; double vinc2p; double vs2; double vc2;
  double vins3p; double vinc3p; double vs3; double vc3;

  double vsum1; double vsum2; double vsum3;

  double v1p; double v2p; double v3p;

  double iins1p; double iinc1p; double is1; double ic1;
  double iins2p; double iinc2p; double is2; double ic2;
  double iins3p; double iinc3p; double is3; double ic3;

  double isum1; double isum2; double isum3;

  double i1p; double i2p; double i3p;

  bool meas;

  double start;
  double stop;

  bool printed;
};



void SOGI(SOGI_VAR *a, double in, double vnom, double w, double bpf_k, double dt);

extern "C" __declspec(dllexport) void _3ph_analyzer(struct sDATA **opaque, double t, union uData *data)
{
   double v1    = data[0].d; // input
   double i1    = data[1].d; // input
   double v2    = data[2].d; // input
   double i2    = data[3].d; // input
   double v3    = data[4].d; // input
   double i3    = data[5].d; // input
   double start = data[6].d; // input parameter
   double stop  = data[7].d; // input parameter
   double fund  = data[8].d; // input parameter
   double vnom  = data[9].d; // input parameter

   if(!*opaque)
   {
      *opaque = (struct sDATA *) malloc(sizeof(struct sDATA));
      bzero(*opaque, sizeof(struct sDATA));

      struct sDATA *inst = *opaque;
      inst->start = start;
      inst->stop = stop;
   }
   struct sDATA *inst = *opaque;

// Implement module evaluation code here:
   double dt = t - inst->tprev;

   double vn    = (v1 + v2 + v3)/3;
   SOGI(&inst->sogi1, (v1 - vn), vnom, 2 * M_PI* fund, sqrt(2), dt);
   SOGI(&inst->sogi2, (v2 - vn), vnom, 2 * M_PI* fund, sqrt(2), dt);
   SOGI(&inst->sogi3, (v3 - vn), vnom, 2 * M_PI* fund, sqrt(2), dt);

   if(t >= start && t <= stop)
   {
      double cc = cos(2*M_PI*fund*t);
      double ss = sin(2*M_PI*fund*t);

      double vins1 = (v1 - vn) * ss; double vinc1 = (v1 - vn) * cc;
      double vins2 = (v2 - vn) * ss; double vinc2 = (v2 - vn) * cc;
      double vins3 = (v3 - vn) * ss; double vinc3 = (v3 - vn) * cc;

      double iins1 = i1 * ss; double iinc1 = i1 * cc;
      double iins2 = i2 * ss; double iinc2 = i2 * cc;
      double iins3 = i3 * ss; double iinc3 = i3 * cc;

      if(inst->meas == 0)
      {
         inst->meas = 1;

         // ============================================================
         inst->vins1p = vins1; inst->vinc1p = vinc1;
         inst->vins2p = vins2; inst->vinc2p = vinc2;
         inst->vins3p = vins3; inst->vinc3p = vinc3;

         inst->v1p = v1 - vn;
         inst->v2p = v2 - vn;
         inst->v3p = v3 - vn;

         // ============================================================
         inst->iins1p = iins1; inst->iinc1p = iinc1;
         inst->iins2p = iins2; inst->iinc2p = iinc2;
         inst->iins3p = iins3; inst->iinc3p = iinc3;

         inst->i1p = i1;
         inst->i2p = i2;
         inst->i3p = i3;
      }
      else
      {
         // ============================================================
         inst->p1 += 0.5 * ((v1 - vn) * i1 + inst->v1p * inst->i1p) * dt;
         inst->p2 += 0.5 * ((v2 - vn) * i2 + inst->v2p * inst->i2p) * dt;
         inst->p3 += 0.5 * ((v3 - vn) * i3 + inst->v3p * inst->i3p) * dt;

         inst->q1 += 0.5 * (inst->sogi1.q * i1 + inst->sogi1.qp * inst->i1p) * dt;
         inst->q2 += 0.5 * (inst->sogi2.q * i2 + inst->sogi2.qp * inst->i2p) * dt;
         inst->q3 += 0.5 * (inst->sogi3.q * i3 + inst->sogi3.qp * inst->i3p) * dt;

         // ============================================================
         inst->vs1 += 0.5 * (vins1 + inst->vins1p) * dt;
         inst->vc1 += 0.5 * (vinc1 + inst->vinc1p) * dt;
         inst->vsum1 += 0.5 * ((v1 - vn) * (v1 -vn) + inst->v1p * inst->v1p) * dt;

         inst->vs2 += 0.5 * (vins2 + inst->vins2p) * dt;
         inst->vc2 += 0.5 * (vinc2 + inst->vinc2p) * dt;
         inst->vsum2 += 0.5 * ((v2 - vn) * (v2 - vn) + inst->v2p * inst->v2p) * dt;

         inst->vs3 += 0.5 * (vins3 + inst->vins3p) * dt;
         inst->vc3 += 0.5 * (vinc3 + inst->vinc3p) * dt;
         inst->vsum3 += 0.5 * ((v3 - vn) * (v3 - vn) + inst->v3p * inst->v3p) * dt;

         // ============================================================
         inst->is1 += 0.5 * (iins1 + inst->iins1p) * dt;
         inst->ic1 += 0.5 * (iinc1 + inst->iinc1p) * dt;
         inst->isum1 += 0.5 * (i1 * i1 + inst->i1p * inst->i1p) * dt;

         inst->is2 += 0.5 * (iins2 + inst->iins2p) * dt;
         inst->ic2 += 0.5 * (iinc2 + inst->iinc2p) * dt;
         inst->isum2 += 0.5 * (i2 * i2 + inst->i2p * inst->i2p) * dt;

         inst->is3 += 0.5 * (iins3 + inst->iins3p) * dt;
         inst->ic3 += 0.5 * (iinc3 + inst->iinc3p) * dt;
         inst->isum3 += 0.5 * (i3 * i3 + inst->i3p * inst->i3p) * dt;

         // ============================================================
         inst->vins1p = vins1; inst->vinc1p = vinc1;
         inst->vins2p = vins2; inst->vinc2p = vinc2;
         inst->vins3p = vins3; inst->vinc3p = vinc3;

         inst->v1p = v1 - vn;
         inst->v2p = v2 - vn;
         inst->v3p = v3 - vn;

         inst->iins1p = iins1; inst->iinc1p = iinc1;
         inst->iins2p = iins2; inst->iinc2p = iinc2;
         inst->iins3p = iins3; inst->iinc3p = iinc3;

         inst->i1p = i1;
         inst->i2p = i2;
         inst->i3p = i3;
      }
   }

   if(t > stop && *ForKeeps && !inst->printed)
   {
      inst->printed = 1;

      double a;
      double b;
      double x;

      double T = stop - start;

      // ============================================================
      a = inst->vc1 * 2. / T;
      b = inst->vs1 * 2. / T;
      double vfund1 = sqrt(a * a + b * b)/sqrt(2.);
      double vrms1 = sqrt(inst->vsum1 / T); x = vrms1 / vfund1;
      double vthd1 = sqrt(fabs(x * x - 1));

      a = inst->vc2 * 2. / T;
      b = inst->vs2 * 2. / T;
      double vfund2 = sqrt(a * a + b * b)/sqrt(2);
      double vrms2 = sqrt(inst->vsum2 / T); x = vrms2 / vfund2;
      double vthd2 = sqrt(fabs(x * x - 1));

      a = inst->vc3 * 2. / T;
      b = inst->vs3 * 2. / T;
      double vfund3 = sqrt(a * a + b * b)/sqrt(2.);
      double vrms3 = sqrt(inst->vsum3 / T); x = vrms3 / vfund3;
      double vthd3 = sqrt(fabs(x * x - 1));

      // ============================================================
      a = inst->ic1 * 2. / T;
      b = inst->is1 * 2. / T;
      double ifund1 = sqrt(a * a + b * b)/sqrt(2.);
      double irms1 = sqrt(inst->isum1 / T); x = irms1 / ifund1;
      double ithd1 = sqrt(fabs(x * x - 1));

      a = inst->ic2 * 2. / T;
      b = inst->is2 * 2. / T;
      double ifund2 = sqrt(a * a + b * b)/sqrt(2);
      double irms2 = sqrt(inst->isum2 / T); x = irms2 / ifund2;
      double ithd2 = sqrt(fabs(x * x - 1));

      a = inst->ic3 * 2. / T;
      b = inst->is3 * 2. / T;
      double ifund3 = sqrt(a * a + b * b)/sqrt(2.);
      double irms3 = sqrt(inst->isum3 / T); x = irms3 / ifund3;
      double ithd3 = sqrt(fabs(x * x - 1));

      // ============================================================

      double p1 = inst->p1 / T;
      double p2 = inst->p2 / T;
      double p3 = inst->p3 / T;

      double q1 = inst->q1 / T;
      double q2 = inst->q2 / T;
      double q3 = inst->q3 / T;

      double pf1 = p1 / (vrms1 * irms1);
      double pf2 = p2 / (vrms2 * irms2);
      double pf3 = p3 / (vrms3 * irms3);
      // ============================================================
      Display("Ptotal = %.5f\n", p1 + p2 + p3);
      Display("Qtotal = %.5f\n", q1 + q2 + q3);
      Display("Stotal = %.5f\n", vrms1 * irms1 + vrms2 * irms2 + vrms3 * irms3);
      Display("PFavg  = %.5f\n", (pf1 + pf2 + pf3) / 3);
      Display("Vrmsavg= %.5f, Irmsavg= %.5f\n\n",(vrms1 + vrms2 + vrms3) / 3, (irms1 + irms2 + irms3) / 3);

      Display("P1     = %.5f, Q1     = %.5f, PF1 = %.5f\n", p1, q1, pf1);
      Display("vfund1 = %.5f, vrms1 = %.5f, vthd1 = %.5f %%\n", vfund1, vrms1, 100. * vthd1);
      Display("ifund1 = %.5f, irms1 = %.5f, ithd1 = %.5f %%\n\n", ifund1, irms1, 100. * ithd1);

      Display("P2     = %.5f, Q2     = %.5f, PF2 = %.5f\n", p2, q2, pf2);
      Display("vfund2 = %.5f, vrms2 = %.5f, vthd2 = %.5f %%\n", vfund2, vrms2, 100. * vthd2);
      Display("ifund2 = %.5f, irms2 = %.5f, ithd2 = %.5f %%\n\n", ifund2, irms2, 100. * ithd2);

      Display("P3     = %.5f, Q3     = %.5f, PF3 = %.5f\n", p3, q3, pf3);
      Display("vfund3 = %.5f, vrms3 = %.5f, vthd3 = %.5f %%\n", vfund3, vrms3, 100. * vthd3);
      Display("ifund3 = %.5f, irms3 = %.5f, ithd3 = %.5f %%\n", ifund3, irms3, 100. * ithd3);
   }

   inst->tprev = t;
}

//#define SOGI1
void SOGI(SOGI_VAR *a, double in, double vnom, double w, double bpf_k, double dt)
{
   double err;

   err = (in - a->d - a->os);

   #ifdef SOGI1
   a->qp = a->q;

   a->inp_os = a->in_os;
   a->inp_d = a->in_d;
   a->inp_q = a->in_q;

   a->in_os  = err / vnom * 0.25 * w;
   a->in_d  = w * (bpf_k * err - a->q);
   a->in_q  = w * a->d;

   a->os +=  0.5 * (a->inp_os + a->in_os) * dt;
   a->d += 0.5 * (a->inp_d + a->in_d) * dt;
   a->q += 0.5 * (a->inp_q + a->in_q) * dt;
   #else
   a->qp = a->q;

   a->in_os  = err / vnom * 0.25 * w;
   a->os +=  a->in_os * dt;

   a->in_q  = w * a->d;
   a->q += a->in_q * dt;

   a->in_d  = w * (bpf_k * err - a->q);
   a->d += a->in_d * dt;
   #endif
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sDATA *inst, double t)
{
   if(t < inst->start){return inst->start - inst->tprev;}
   else if(t < inst->stop){return inst->stop - inst->tprev;}
   else return 1e308; // implement a good choice of max timestep size that depends on struct sTHD_N
}

extern "C" __declspec(dllexport) void Destroy(struct sDATA *inst)
{
   free(inst);
}
