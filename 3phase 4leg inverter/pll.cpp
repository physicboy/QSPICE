// Automatically generated C++ file on Wed Sep 24 17:53:32 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD pll.cpp kernel32.lib

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
#undef ddsrf_dp
#undef ddsrf_qp
#undef ddsrf_dn
#undef ddsrf_qn
#undef ddsrf_w
#undef ddsrf_th

struct sPLL
{
  // declare the structure here
  double tstep;
  bool switched;
  double tprev;

  double vad;
  double vad_in;
  double vaq;
  double vaq_in;
  double vbd;
  double vbd_in;
  double vbq;
  double vbq_in;
  double vcd;
  double vcd_in;
  double vcq;
  double vcq_in;

  double vad_inp;
  double vaq_inp;
  double vbd_inp;
  double vbq_inp;
  double vcd_inp;
  double vcq_inp;

  double magA;
  double magA_lpf;
  double magB;
  double magB_lpf;
  double magC;
  double magC_lpf;

  double va;
  double vb;
  double vdp1;
  double vdp2;
  double vdp3;
  double vqp1;
  double vqp2;
  double vqp3;
  double vdn1;
  double vdn2;
  double vdn3;
  double vqn1;
  double vqn2;
  double vqn3;
  double vqp2_prev;
  double w;
  double th;
  double cthp;
  double sthp;
  double c2thp;
  double s2thp;
  double cthn;
  double sthn;
  double c2thn;
  double s2thn;
};

#define PI(X, X_PREV, KP, KITS)         \
            KP*(X - X_PREV) + X*KITS;   \
            X_PREV = X;

#define LIMIT(X, MAX, MIN)      \
            if(X > MAX) X = MAX;\
            if(X < MIN) X = MIN;

#define LPF(Y, X, K) Y=Y*(1-K)+X*K

#define SOGI(sig, d, d_in, d_inp, q, q_in, q_inp, w, k, ts)   \
      q_in  = w * d;                   \
      q     += q_in * ts;              \
      d_in  = w * (k * (sig - d) - q); \
      d     += d_in *ts;

#define TSAMPLING 100E-6
#define VNOM      310.

#define LPF_K       TSAMPLING/(TSAMPLING + sqrt(2.)/(2 * M_PI * 50))
#define LPF_K2       TSAMPLING/(TSAMPLING + 1/(2 * M_PI * 25))

#define KP    (2./sqrt(2.) * (2.*M_PI*30.) / VNOM)
#define KITS  TSAMPLING * (2.*M_PI*30.)*(2.*M_PI*30.) / VNOM

extern "C" __declspec(dllexport) void pll(struct sPLL **opaque, double t, union uData *data)
{
   double  va    = data[ 0].d; // input
   double  vb    = data[ 1].d; // input
   double  vc    = data[ 2].d; // input
   double &vdp   = data[ 3].d; // output
   double &vqp   = data[ 4].d; // output
   double &vdn   = data[ 5].d; // output
   double &vqn   = data[ 6].d; // output
   double &w     = data[ 7].d; // output
   double &th    = data[ 8].d; // output
   double &mag_a = data[ 9].d; // output
   double &mag_b = data[10].d; // output
   double &mag_c = data[11].d; // output

   if(!*opaque)
   {
      *opaque = (struct sPLL *) malloc(sizeof(struct sPLL));
      bzero(*opaque, sizeof(struct sPLL));
   }
   struct sPLL *inst = *opaque;

// Implement module evaluation code here:
   if((t >= inst->tstep) && (inst->tprev <=inst->tstep))
   {
      inst->tstep += TSAMPLING;
      inst->switched = 1;

      vdp = inst->vdp3;
      vqp = inst->vqp3;
      vdn = inst->vdn3;
      vqn = inst->vqn3;
      w  = inst->w;
      th = inst->th;

      mag_a = inst->magA_lpf;
      mag_b = inst->magB_lpf;
      mag_c = inst->magC_lpf;

      // SOGI for phase magnitude calculation
      double sogi_w = abs(inst->w);
      if(sogi_w > 2*M_PI*70) sogi_w = 2*M_PI*70;
      if(sogi_w < 2*M_PI*40) sogi_w = 2*M_PI*40;
      //sogi_w = 2*M_PI*50;

      SOGI(va, inst->vad, inst->vad_in, inst->vad_inp, inst->vaq, inst->vaq_in, inst->vaq_inp, sogi_w, sqrt(2), TSAMPLING)
      SOGI(vb, inst->vbd, inst->vbd_in, inst->vbd_inp, inst->vbq, inst->vbq_in, inst->vbq_inp, sogi_w, sqrt(2), TSAMPLING)
      SOGI(vc, inst->vcd, inst->vcd_in, inst->vcd_inp, inst->vcq, inst->vcq_in, inst->vcq_inp, sogi_w, sqrt(2), TSAMPLING)

      inst->magA = inst->vad * inst->cthp + inst->vaq * inst->sthp;
      inst->magB = inst->vbd * (inst->cthp * -0.5 + inst->sthp * sqrt(3)/2)
                     + inst->vbq * (inst->sthp * -0.5 - inst->cthp * sqrt(3)/2);
      inst->magC = inst->vcd * (inst->cthp * -0.5 - inst->sthp * sqrt(3)/2)
                     + inst->vcq * (inst->sthp * -0.5 + inst->cthp * sqrt(3)/2);

      LPF(inst->magA_lpf, inst->magA, LPF_K2);
      LPF(inst->magB_lpf, inst->magB, LPF_K2);
      LPF(inst->magC_lpf, inst->magC, LPF_K2);

      // 3phase DDSRF-PLL

      inst->va    = 2./3. * (va - 0.5 * (vb + vc));
      inst->vb    = 2./3. * (0.86602540378 * (vb - vc));

      // inst->va    = 2./3. * (inst->vad - 0.5 * (inst->vbd + inst->vcd));
      //inst->vb    = 2./3. * (0.86602540378 * (inst->vbd - inst->vcd));

      inst->cthp = cos(inst->th);
      inst->sthp = sin(inst->th);

      inst->c2thp = 2 * inst->cthp * inst->cthp - 1;
      inst->s2thp = 2 * inst->cthp * inst->sthp;

      inst->cthn =      inst->cthp;
      inst->sthn = -1 * inst->sthp;

      inst->c2thn = 2 * inst->cthn * inst->cthn - 1;
      inst->s2thn = 2 * inst->cthn * inst->sthn;

      inst->vdp1 =      inst->cthp * inst->va + inst->sthp * inst->vb;
      inst->vqp1 = -1 * inst->sthp * inst->va + inst->cthp * inst->vb;
      inst->vdn1 =      inst->cthn * inst->va + inst->sthn * inst->vb;
      inst->vqn1 = -1 * inst->sthn * inst->va + inst->cthn * inst->vb;

      inst->vdp2 = inst->vdp1 - inst->vdn3 * inst->c2thp - inst->vqn3 * inst->s2thp;
      inst->vqp2 = inst->vqp1 + inst->vdn3 * inst->s2thp - inst->vqn3 * inst->c2thp;
      inst->vdn2 = inst->vdn1 - inst->vdp3 * inst->c2thn - inst->vqp3 * inst->s2thn;
      inst->vqn2 = inst->vqn1 + inst->vdp3 * inst->s2thn - inst->vqp3 * inst->c2thn;

      LPF(inst->vdp3, inst->vdp2, LPF_K);
      LPF(inst->vqp3, inst->vqp2, LPF_K);
      LPF(inst->vdn3, inst->vdn2, LPF_K);
      LPF(inst->vqn3, inst->vqn2, LPF_K);

      inst->w  += PI(inst->vqp2, inst->vqp2_prev, KP, KITS)
      inst->th += inst->w * TSAMPLING;

      if(inst->th > 2 * M_PI) inst->th -= 2 * M_PI;
      if(inst->th < 0) inst->th += 2 * M_PI;

   }

   inst->tprev = t;

}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sPLL *inst, double t)
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

extern "C" __declspec(dllexport) void Destroy(struct sPLL *inst)
{
   free(inst);
}
