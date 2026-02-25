/*
 * Copyright 2026 Arief Noor Rahman - Power Control Design
 * 
 * Project  : FRA_project
 * Filename : fra_v7.cpp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Automatically generated C++ file on Wed Oct 16 12:21:52 2024
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD fra_v5.cpp kernel32.lib

#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

extern "C" __declspec(dllexport) int (*Display)(const char *format, ...) = 0; // works like printf()
extern "C" __declspec(dllexport) const double *DegreesC                  = 0; // pointer to current circuit temperature
extern "C" __declspec(dllexport) const int *StepNumber                   = 0; // pointer to current step number
extern "C" __declspec(dllexport) const int *NumberSteps                  = 0; // pointer to estimated number of steps
extern "C" __declspec(dllexport) const char **InstanceName               = 0; // pointer to address of instance name
extern "C" __declspec(dllexport) const char *QUX                         = 0; // path to QUX.exe
extern "C" __declspec(dllexport) int (*DFFT)(struct sComplex *u, bool inv, unsigned int N, double scale) = 0;

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

void bzero(void *ptr, unsigned int count)
{
   unsigned char *first = (unsigned char *) ptr;
   unsigned char *last  = first + count;
   while(first < last)
      *first++ = '\0';
}

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef sw_trig
#undef sense
#undef IN1A
#undef IN1B
#undef IN2A
#undef IN2B
#undef amp
#undef freq
#undef param1
#undef param2
#undef param3
#undef param4

struct fra_data
{
   double i;
   double freq;
   double mag;

   double ts;
   double tsampling;
   double tstep;

   double fmin;
   double fmax;
   double alo;
   double ahi;
   double flo;
   double fhi;
   int    fstep;

   bool lin0_log1;

   double dwell_mintime;
   double dwell_period;
   double meas_mintime;
   double meas_period;

   bool bpf_on_off;
   double bpf_k;

   double in1q_inp;
   double in1q_in;
   double in1d_inp;
   double in1d_in;
   double in2q_inp;
   double in2q_in;
   double in2d_inp;
   double in2d_in;

   double in1q;
   double in1d;
   double in2q;
   double in2d;

   double x1a;
   double x1ap;
   double x1b;
   double x1bp;
   double x2a;
   double x2ap;
   double x2b;
   double x2bp;

   double in1a;
   double in1b;
   double in2a;
   double in2b;

   double in1ph;
   double in2ph;
   double in1wrap;
   double in2wrap;
};

struct sFRA_V7
{
   // declare the structure here
   int   status;
   struct fra_data fra;

   double maxstep;
   double t_prev;

   bool plot;
};

void FRA_CORE(struct fra_data *f, bool *bpf_on_off,
   double *t, double *t_prev, double *in1, double *in2, double *th);

FILE *fptr;
char *fname = "fra.csv";

extern "C" __declspec(dllexport) void fra_v7(struct sFRA_V7 **opaque, double t, union uData *data)
{
   double  IN1           = data[ 0].d; // input
   double  IN2           = data[ 1].d; // input
   double  th            = data[ 2].d; // input
   double  param1        = data[ 3].d; // input
   double  param2        = data[ 4].d; // input
   double  param3        = data[ 5].d; // input
   double  param4        = data[ 6].d; // input
   double  f_min         = data[ 7].d; // input parameter
   double  f_max         = data[ 8].d; // input parameter
   double  a_lo          = data[ 9].d; // input parameter
   double  a_hi          = data[10].d; // input parameter
   double  f_lo          = data[11].d; // input parameter
   double  f_hi          = data[12].d; // input parameter
   bool    lin0_log1     = data[13].b; // input parameter
   double  tstep_factor  = data[14].d; // input parameter
   double  ss_tmax       = data[15].d; // input parameter
   double  dwell_mintime = data[16].d; // input parameter
   double  dwell_period  = data[17].d; // input parameter
   double  meas_mintime  = data[18].d; // input parameter
   double  meas_period   = data[19].d; // input parameter
   int     f_step        = data[20].i; // input parameter
   bool    plot          = data[21].b; // input parameter
   bool    bpf_off0_on1  = data[22].b; // input parameter
   double &amp           = data[23].d; // output
   double &freq          = data[24].d; // output

   if(!*opaque)
   {
      *opaque = (struct sFRA_V7 *) malloc(sizeof(struct sFRA_V7));
      bzero(*opaque, sizeof(struct sFRA_V7));

      struct sFRA_V7 *inst = *opaque;

      inst->fra.fmin = f_min;
      inst->fra.fmax = f_max;
      inst->fra.alo  = a_lo;
      inst->fra.ahi  = a_hi;
      inst->fra.flo  = f_lo;
      inst->fra.fhi  = f_hi;
      inst->fra.lin0_log1 = lin0_log1;
      inst->fra.fstep = f_step;
      inst->fra.dwell_mintime = dwell_mintime;
      inst->fra.dwell_period = dwell_period;
      inst->fra.meas_mintime = meas_mintime;
      inst->fra.meas_period = meas_period;

      inst->plot = plot;
   }
   struct sFRA_V7 *inst = *opaque;

// Implement module evaluation code here:
   if(inst->status == 0)
   {
      if(t > ss_tmax)inst->status = 1;
      inst->fra.i = -0.5;

      amp = inst->fra.i;
      freq = inst->fra.freq;

      if(inst->status == 1)
      {
         Display("\n\tparam1 : %f\n\tparam2 : %f\n\tparam3 : %f\n\tparam4 : %f\n", param1, param2, param3, param4);
         Display("\ti\tfreq\tmag(in2/in1)[dB]\targ(in2/in1)[deg]\tmag(in1)[dB]\targ(in1)[deg]\tmag(in2)[dB]\targ(in2)[deg]\n");

         if(*StepNumber == 1)
         {
            fptr = fopen(fname,"w");
            fprintf(fptr,"Frequency,IN2/IN1\n");
            fclose(fptr);
         }
      }

      // BPF initialization
      inst->fra.in1d = 0;
      inst->fra.in1q = IN1 * 1.414213562373; // x sqrt(2)
      inst->fra.in2d = 0;
      inst->fra.in2q = IN2 * 1.414213562373;
   }
   else
   {
      FRA_CORE(&inst->fra, &bpf_off0_on1, &t, &inst->t_prev, &IN1, &IN2, &th);

      amp = inst->fra.mag;
      freq = inst->fra.freq;


      // Timing control ===============================================
      if(inst->fra.i >= inst->fra.fstep)inst->maxstep = -1e+308;
      else
      {
         inst->maxstep = inst->fra.tsampling - t;
         if(inst->maxstep > tstep_factor/inst->fra.freq)
         {
            inst->maxstep = tstep_factor/inst->fra.freq;
         }
      }
   }
   inst->t_prev = t;

}

void FRA_CORE(struct fra_data *f, bool *bpf_off0_on1, double *t, double *t_prev, double *in1, double *in2, double *th)
{
   // band pass filter
   f->in1q_inp = f->in1q_in;
   f->in1q_in = 2 * M_PI * f->freq * f->in1d;
   f->in1q += 0.5 * (f->in1q_in + f->in1q_inp) * (*t - *t_prev);
   f->in1d_inp = f->in1d_in;
   f->in1d_in = 2 * M_PI * f->freq * (f->bpf_k * (*in1 - f->in1d) - f->in1q);
   f->in1d += 0.5 * (f->in1d_in + f->in1d_inp) * (*t - *t_prev);

   f->in2q_inp = f->in2q_in;
   f->in2q_in = 2 * M_PI * f->freq * f->in2d;
   f->in2q += 0.5 * (f->in2q_in + f->in2q_inp) * (*t - *t_prev);
   f->in2d_inp = f->in2d_in;
   f->in2d_in = 2 * M_PI * f->freq * (f->bpf_k * (*in2 - f->in2d) - f->in2q);
   f->in2d += 0.5 * (f->in2d_in + f->in2d_inp) * (*t - *t_prev);

   // select whether to use the input signal directly or use BPF output
   double signal1;
   double signal2;
   if(f->bpf_on_off == 0)
   {
      signal1 = *in1;
      signal2 = *in2;
   }
   else
   {
      if(*bpf_off0_on1 == 1)
      {
         signal1 = f->in1d;
         signal2 = f->in2d;
      }
      else
      {
         signal1 = *in1;
         signal2 = *in2;
      }
   }

   // fourier series integral using variable step trapezoidal integration
   double cth = cos(*th);
   double sth = sin(*th);

   f->x1ap = f->x1a;
   f->x1bp = f->x1b;
   f->x2ap = f->x2a;
   f->x2bp = f->x2b;

   f->x1a = sth * signal1;
   f->x1b = cth * signal1;
   f->x2a = sth * signal2;
   f->x2b = cth * signal2;

   f->in1a += 0.5 * (f->x1a + f->x1ap) * (*t - *t_prev);
   f->in1b += 0.5 * (f->x1b + f->x1bp) * (*t - *t_prev);
   f->in2a += 0.5 * (f->x2a + f->x2ap) * (*t - *t_prev);
   f->in2b += 0.5 * (f->x2b + f->x2bp) * (*t - *t_prev);

   if(*t >= f->tsampling)
   {
      f->i = f->i + 0.5;
      double ts;
      double fx;
      double freq_p = f->freq;
      if(f->i == floor(f->i))
      {
         if(f->lin0_log1)
         {
            // injection frequency computation in log space
            f->freq = f->fmin*pow(10,log10(f->fmax/f->fmin)*floor(f->i)/((double)f->fstep-1));

            // injection frequency computation in log space
            if(f->freq > f->fhi)f->mag = f->ahi;
            else
            {
               if(f->freq < f->flo)f->mag = f->alo;
               else f->mag = f->alo + (f->ahi - f->alo)*(log10(f->freq) - log10(f->flo))/(log10(f->fhi) - log10(f->flo));
            }
         }
         else
         {
            // injection frequency computation in linear space
            f->freq = f->fmin + (f->fmax - f->fmin)*floor(f->i)/((double)f->fstep-1);

            // injection amplitude computation in linear space
            if(f->freq > f->fhi)f->mag = f->ahi;
            else
            {
               if(f->freq < f->flo)f->mag = f->alo;
               else f->mag = f->alo + (f->ahi - f->alo)*(f->freq - f->flo)/(f->fhi - f->flo);
            }
         }

         // calculate the bpf damping constant
         f->bpf_k = 8 / (0.5 * ts * 2 * M_PI * f->freq);
         if(f->bpf_k > 2) f->bpf_k = 2;
         if(f->bpf_k < 0.1) f->bpf_k = 0.1;

         // activate bpf according to the following rule
         ts = f->dwell_period/f->freq;
         if(ts < f->dwell_mintime)
         {
            ts = f->dwell_mintime;
            if(f->bpf_k <= 1) f->bpf_on_off = 1;
            else f->bpf_on_off = 0;

         }
         else
         {
            f->bpf_on_off = 0;
         }

         f->tsampling = *t + ts;
      }
      else
      {
         ts = f->meas_period/f->freq;
         if(ts < f->meas_mintime) ts = ceil(f->meas_mintime*f->freq)/f->freq;
         f->tsampling = *t + ts;
      }
      //============================================================================
      if(f->i > 0)
      {
         if(f->i != floor(f->i))
         {
            // reset the integrator value
            // at the beginning of measurement period
            f->in1a = 0;
            f->in1b = 0;
            f->in2a = 0;
            f->in2b = 0;
         }
         else
         {
            double in1am, in1bm, in2am, in2bm;
            double in1mag, in1ph, in2mag, in2ph;
            double in21mag, in21ph;
            in1am = 2/f->ts*(f->in1a);
            in1bm = 2/f->ts*(f->in1b);
            in2am = 2/f->ts*(f->in2a);
            in2bm = 2/f->ts*(f->in2b);

            in1mag = 20*log10(sqrt(in1am*in1am + in1bm*in1bm));
            in1ph  = 180/M_PI*atan2(in1bm,in1am);
            in2mag = 20*log10(sqrt(in2am*in2am + in2bm*in2bm));
            in2ph  = 180/M_PI*atan2(in2bm,in2am);

            in1ph = in1ph + f->in1wrap;
            in2ph = in2ph + f->in2wrap;

            if(in1ph > (f->in1ph + 270))
            {
               f->in1wrap -= 360;
               in1ph += f->in1wrap;
            }
            else
            {
               if(in1ph < (f->in1ph - 270))
               {
                  f->in1wrap += 360;
                  in1ph += f->in1wrap;
               }
            }

            if(in2ph > (f->in2ph + 270))
            {
               f->in2wrap -= 360;
               in2ph += f->in2wrap;
            }
            else
            {
               if(in2ph < (f->in2ph - 270))
               {
                  f->in2wrap += 360;
                  in2ph += f->in2wrap;
               }
            }

            f->in1ph = in1ph;
            f->in2ph = in2ph;

            in21mag = in2mag - in1mag;
            in21ph = in2ph - in1ph;
            Display("\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",(int)f->i,freq_p,in21mag,in21ph,in1mag,in1ph,in2mag,in2ph);

            double re,im;
            re = pow(10,in21mag/20)*cos(in21ph*M_PI/180);
            im = pow(10,in21mag/20)*sin(in21ph*M_PI/180);

            fptr = fopen(fname,"a");
            fprintf(fptr,"%f\t%f,%f\n",freq_p,re,im);
            fclose(fptr);
         }
      }
      f->ts = ts;
   }
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sFRA_V7 *inst)
{
   return inst->maxstep; // implement a good choice of max timestep size that depends on struct sFRA_V3
}

extern "C" __declspec(dllexport) void Destroy(struct sFRA_V7 *inst)
{
   if(*StepNumber == *NumberSteps)
   {
      if(inst->plot) system("\"c:\\Program Files\\QSPICE\\QUX\" fra.csv");
   }
   free(inst);
}
