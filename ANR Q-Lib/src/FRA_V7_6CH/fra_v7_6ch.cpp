// Automatically generated C++ file on Wed Oct 16 12:21:52 2024
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD fra_v5.cpp kernel32.lib

#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define NUM_CHANNELS 6

extern "C" __declspec(dllexport) int (*Display)(const char *format, ...) = 0; // works like printf()
extern "C" __declspec(dllexport) const double *DegreesC                  = 0; // pointer to current circuit temperature
extern "C" __declspec(dllexport) const int *StepNumber                   = 0; // pointer to current step number
extern "C" __declspec(dllexport) const int *NumberSteps                  = 0; // pointer to estimated number of steps
extern "C" __declspec(dllexport) const char **InstanceName               = 0; // pointer to address of instance name
extern "C" __declspec(dllexport) const char *QUX                         = 0; // path to QUX.exe
extern "C" __declspec(dllexport) const bool *ForKeeps                    = 0; // pointer to whether being evaluated non-hypothetically
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

   double inq_inp[NUM_CHANNELS];
   double inq_in[NUM_CHANNELS];
   double ind_inp[NUM_CHANNELS];
   double ind_in[NUM_CHANNELS];

   double inq[NUM_CHANNELS];
   double ind[NUM_CHANNELS];

   double xa[NUM_CHANNELS];
   double xap[NUM_CHANNELS];
   double xb[NUM_CHANNELS];
   double xbp[NUM_CHANNELS];

   double ina[NUM_CHANNELS];
   double inb[NUM_CHANNELS];

   double inph[NUM_CHANNELS];
   double inwrap[NUM_CHANNELS];
};

struct sFRA_V7_6CH
{
   // declare the structure here
   int    status;
   struct fra_data fra;

   double maxstep;
   double t_prev;

   bool plot;
};

void FRA_CORE(struct fra_data *f, bool *bpf_on_off, double *t, double *t_prev,
   double in[NUM_CHANNELS], double *th);

FILE *fptr;
char *fname = "fra.csv";

extern "C" __declspec(dllexport) void fra_v7_6ch(struct sFRA_V7_6CH **opaque, double t, union uData *data)
{
if(!*ForKeeps) return;

   double  IN1           = data[ 0].d; // input
   double  IN2           = data[ 1].d; // input
   double  th            = data[ 2].d; // input
   double  param1        = data[ 3].d; // input
   double  param2        = data[ 4].d; // input
   double  param3        = data[ 5].d; // input
   double  param4        = data[ 6].d; // input
   double  IN3           = data[ 7].d; // input
   double  IN4           = data[ 8].d; // input
   double  IN5           = data[ 9].d; // input
   double  IN6           = data[10].d; // input
   double  f_min         = data[11].d; // input parameter
   double  f_max         = data[12].d; // input parameter
   double  a_lo          = data[13].d; // input parameter
   double  a_hi          = data[14].d; // input parameter
   double  f_lo          = data[15].d; // input parameter
   double  f_hi          = data[16].d; // input parameter
   bool    lin0_log1     = data[17].b; // input parameter
   double  tstep_factor  = data[18].d; // input parameter
   double  ss_tmax       = data[19].d; // input parameter
   double  dwell_mintime = data[20].d; // input parameter
   double  dwell_period  = data[21].d; // input parameter
   double  meas_mintime  = data[22].d; // input parameter
   double  meas_period   = data[23].d; // input parameter
   int     f_step        = data[24].i; // input parameter
   bool    plot          = data[25].b; // input parameter
   bool    bpf_off0_on1  = data[26].b; // input parameter
   double &amp           = data[27].d; // output
   double &freq          = data[28].d; // output

   if(!*opaque)
   {
      *opaque = (struct sFRA_V7_6CH *) malloc(sizeof(struct sFRA_V7_6CH));
      bzero(*opaque, sizeof(struct sFRA_V7_6CH));

      struct sFRA_V7_6CH *inst = *opaque;

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
   struct sFRA_V7_6CH *inst = *opaque;

// Implement module evaluation code here:
   double IN_VECTOR[] = {IN1, IN2, IN3, IN4, IN5, IN6};

   if(inst->status == 0)
   {
      if(t > ss_tmax)inst->status = 1;
      inst->fra.i = -0.5;

      amp = 0;
      freq = inst->fra.freq;

      if(inst->status == 1)
      {
         Display("\ti\tfreq\tsrc_mag[dB]\tsrc_arg[deg]\tin1_mag[dB]\tin1_arg[deg]\tin2_mag[dB]\tin2_arg[deg]\tin3_mag[dB]\tin3_arg[deg]\tin4_mag[dB]\tin4_arg[deg]\tin5_mag[dB]\tin5_arg[deg]\tin6_mag[dB]\tin6_arg[deg]\n");

         if(*StepNumber == 1)
         {
            fptr = fopen(fname,"w");
            fprintf(fptr,"Frequency,src,in1,in2,in3,in4,in5,in6\n");
            fclose(fptr);
         }
      }

      // BPF initialization
      for(int ch = 0; ch < NUM_CHANNELS; ch++)
      {
         inst->fra.ind[ch] = 0;
         inst->fra.inq[ch] = IN_VECTOR[ch] * 1.414213562373; // x sqrt(2)
      }
   }
   else
   {
      FRA_CORE(&inst->fra, &bpf_off0_on1, &t, &inst->t_prev, IN_VECTOR, &th);

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

void FRA_CORE(struct fra_data *f, bool *bpf_off0_on1, double *t, double *t_prev,
   double in[NUM_CHANNELS], double *th)
{
   // band pass filter
   for(int ch = 0; ch < NUM_CHANNELS; ch++)
   {
      f->inq_inp[ch] = f->inq_in[ch];
      f->inq_in[ch]  = 2 * M_PI * f->freq * f->ind[ch];
      f->inq[ch]    += 0.5 * (f->inq_in[ch] + f->inq_inp[ch]) * (*t - *t_prev);
      f->ind_inp[ch] = f->ind_in[ch];
      f->ind_in[ch]  = 2 * M_PI * f->freq * (f->bpf_k * (in[ch] - f->ind[ch]) - f->inq[ch]);
      f->ind[ch]    += 0.5 * (f->ind_in[ch] + f->ind_inp[ch]) * (*t - *t_prev);
   }

   // select whether to use the input signal directly or use BPF output
   double signal[NUM_CHANNELS];
   for(int ch = 0; ch < NUM_CHANNELS; ch++)
   {
      if(f->bpf_on_off == 0)
      {
         signal[ch] = in[ch];
      }
      else
      {
         if(*bpf_off0_on1 == 1)
         {
            signal[ch] = f->ind[ch];
         }
         else
         {
            signal[ch] = in[ch];
         }
      }
   }

   // fourier series integral using variable step trapezoidal integration
   double cth = cos(*th);
   double sth = sin(*th);

   for(int ch = 0; ch < NUM_CHANNELS; ch++)
   {
      f->xap[ch] = f->xa[ch];
      f->xbp[ch] = f->xb[ch];

      f->xa[ch] = sth * signal[ch];
      f->xb[ch] = cth * signal[ch];

      f->ina[ch] += 0.5 * (f->xa[ch] + f->xap[ch]) * (*t - *t_prev);
      f->inb[ch] += 0.5 * (f->xb[ch] + f->xbp[ch]) * (*t - *t_prev);
   }

   if(*t >= f->tsampling)
   {
      f->i = f->i + 0.5;
      double ts;
      double fx;
      double freq_p = f->freq;
      double mag_p  = f->mag;
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
            for(int ch = 0; ch < NUM_CHANNELS; ch++)
            {
               f->ina[ch] = 0;
               f->inb[ch] = 0;
            }
         }
         else
         {
            double inam[NUM_CHANNELS], inbm[NUM_CHANNELS];
            double inmag[NUM_CHANNELS], inph[NUM_CHANNELS];

            for(int ch = 0; ch < NUM_CHANNELS; ch++)
            {
               inam[ch] = 2/f->ts*(f->ina[ch]);
               inbm[ch] = 2/f->ts*(f->inb[ch]);

               inmag[ch] = 20*log10(sqrt(inam[ch]*inam[ch] + inbm[ch]*inbm[ch]));
               inph[ch]  = 180/M_PI*atan2(inbm[ch],inam[ch]);

               inph[ch] = inph[ch] + f->inwrap[ch];

               if(inph[ch] > (f->inph[ch] + 270))
               {
                  f->inwrap[ch] -= 360;
                  inph[ch] += f->inwrap[ch];
               }
               else
               {
                  if(inph[ch] < (f->inph[ch] - 270))
                  {
                     f->inwrap[ch] += 360;
                     inph[ch] += f->inwrap[ch];
                  }
               }

               f->inph[ch] = inph[ch];
            }

            Display("\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",
               (int)f->i, freq_p, 20*log10(mag_p), 0.0,
                  inmag[0], inph[0],
                  inmag[1], inph[1],
                  inmag[2], inph[2],
                  inmag[3], inph[3],
                  inmag[4], inph[4],
                  inmag[5], inph[5]);
            fptr = fopen(fname,"a");
            fprintf(fptr, "%f\t%f,%f\t%f,%f\t%f,%f\t%f,%f\t%f,%f\t%f,%f\t%f,%f\n",
                  freq_p, mag_p, 0.0,
                  inam[0], inbm[0],
                  inam[1], inbm[1],
                  inam[2], inbm[2],
                  inam[3], inbm[3],
                  inam[4], inbm[4],
                  inam[5], inbm[5]);

            fclose(fptr);
         }
      }
      f->ts = ts;
   }
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sFRA_V7_6CH *inst)
{
   return inst->maxstep; // implement a good choice of max timestep size that depends on struct sFRA_V3
}

extern "C" __declspec(dllexport) void Destroy(struct sFRA_V7_6CH *inst)
{
   if(*StepNumber == *NumberSteps)
   {
      if(inst->plot) system("\"c:\\Program Files\\QSPICE\\QUX\" fra.csv");
   }
   free(inst);
}
