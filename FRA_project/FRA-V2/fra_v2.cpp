// Automatically generated C++ file on Wed Oct 16 12:21:52 2024
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD fra_v2.cpp kernel32.lib

#include <malloc.h>
#include <math.h>

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

struct ss_data
{
  bool trig_edge;
  bool use_ss_detect;
  int moving_avg_length;
  int steady_ctr_lim;
  double error_lim;
  double tmax;

  double trigger_time;
  double sense_val[100];
  double sense_avg;
  int counter;
  int counter_ovf;
  int steady_counter;

  double sense_prev;
  double trig_prev;

  double Tsw;
  bool ss_flag;
};

struct fra_data
{
   double i;
   double ii;
   double freq;
   double mag;

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
   bool use_qlog_optimizer;

   double dwell_mintime;
   double dwell_period;
   double meas_mintime;
   double meas_period;

   double in1ap;
   double in1bp;
   double in2ap;
   double in2bp;

   double in1ph;
   double in2ph;
   double in1wrap;
   double in2wrap;
};

struct sFRA_V2
{
   // declare the structure here
   struct ss_data ss;
   struct fra_data fra;

   double maxstep;
   double t_prev;
};

void SS_DETECT(struct ss_data *s, double *t,
      double *t_prev, double *sense, double *sw_trig);
void FRA_CORE(struct fra_data *f, struct ss_data *s, double *t,
      double *in1a, double *in1b, double *in2a, double *in2b);

extern "C" __declspec(dllexport) void fra_v2(struct sFRA_V2 **opaque, double t, union uData *data)
{
   double  sw_trig            = data[ 0].d; // input
   double  sense              = data[ 1].d; // input
   double  IN1A               = data[ 2].d; // input
   double  IN1B               = data[ 3].d; // input
   double  IN2A               = data[ 4].d; // input
   double  IN2B               = data[ 5].d; // input
   double  param1             = data[ 6].d; // input
   double  param2             = data[ 7].d; // input
   double  param3             = data[ 8].d; // input
   double  param4             = data[ 9].d; // input
   double  f_min              = data[10].d; // input parameter
   double  f_max              = data[11].d; // input parameter
   double  a_lo               = data[12].d; // input parameter
   double  a_hi               = data[13].d; // input parameter
   double  f_lo               = data[14].d; // input parameter
   double  f_hi               = data[15].d; // input parameter
   bool    lin0_log1          = data[16].d; // input parameter
   double  tstep_factor       = data[17].d; // input parameter
   double  ss_tmax            = data[18].d; // input parameter
   bool    use_ss_detect      = data[19].b; // input parameter
   double  dwell_mintime      = data[20].d; // input parameter
   double  dwell_period       = data[21].d; // input parameter
   double  meas_mintime       = data[22].d; // input parameter
   double  meas_period        = data[23].d; // input parameter
   bool    use_qlog_optimizer = data[24].b; // input parameter
   int     ss_mavg_window     = data[25].i; // input parameter
   double  ss_detect_tol      = data[26].d; // input parameter
   int     ss_detect_window   = data[27].i; // input parameter
   bool    trig_edge          = data[28].b; // input parameter
   int     f_step             = data[29].i; // input parameter
   double &amp                = data[30].d; // output
   double &freq               = data[31].d; // output

   if(!*opaque)
   {
      *opaque = (struct sFRA_V2 *) malloc(sizeof(struct sFRA_V2));
      bzero(*opaque, sizeof(struct sFRA_V2));

      struct sFRA_V2 *inst = *opaque;

      inst->ss.trig_edge = trig_edge;
      inst->ss.use_ss_detect = use_ss_detect;
      inst->ss.moving_avg_length = ss_mavg_window;
      inst->ss.steady_ctr_lim = ss_detect_window;
      inst->ss.error_lim = ss_detect_tol;
      inst->ss.tmax = ss_tmax;

      inst->fra.fmin = f_min;
      inst->fra.fmax = f_max;
      inst->fra.alo  = a_lo;
      inst->fra.ahi  = a_hi;
      inst->fra.flo  = f_lo;
      inst->fra.fhi  = f_hi;
      inst->fra.lin0_log1 = lin0_log1;
      inst->fra.fstep = f_step;
      inst->fra.use_qlog_optimizer = use_qlog_optimizer;
      inst->fra.dwell_mintime = dwell_mintime;
      inst->fra.dwell_period = dwell_period;
      inst->fra.meas_mintime = meas_mintime;
      inst->fra.meas_period = meas_period;
   }
   struct sFRA_V2 *inst = *opaque;

// Implement module evaluation code here:
   if(inst->ss.ss_flag == 0)
   {
      SS_DETECT(&inst->ss, &t, &inst->t_prev, &sense, &sw_trig);
      inst->fra.i = -0.5;

      amp = inst->fra.i;
      freq = inst->fra.freq;

      if(inst->ss.ss_flag == 1)
      {
         Display("\ti\tfreq\tin1mag\tin2mag\tmag(in2-in1)\tin1ph\tin2ph\tph(in2-in1\n)");
      }
   }
   else
   {
      FRA_CORE(&inst->fra, &inst->ss, &t, &IN1A, &IN1B, &IN2A, &IN2B);

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

void FRA_CORE(struct fra_data *f, struct ss_data *s, double *t, double *in1a, double *in1b, double *in2a, double *in2b)
{
   if(*t >= f->tsampling)
   {
      f->i = f->i + 0.5;

      double fx;
      double freq_p = f->freq;
      if(f->i == floor(f->i))
      {
         if(f->lin0_log1)
         {
            // injection frequency computation in log space
            fx = f->fmin*pow(10,log10(f->fmax/f->fmin)*floor(f->i)/((double)f->fstep-1));
            if((s->Tsw > 0) && f->use_qlog_optimizer)
            {
               f->ii = 1;
               while(fabs(fx - f->ii/(s->Tsw*round(f->ii/(fx*s->Tsw))))/fx > 0.01)
               {
                  f->ii++;
               }
               f->freq = f->ii/(s->Tsw*round(f->ii/(fx*s->Tsw)));
            }
            else f->freq = fx;

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
            fx = f->fmin + (f->fmax - f->fmin)*floor(f->i)/((double)f->fstep-1);
            if((s->Tsw > 0) && f->use_qlog_optimizer)
            {
               f->ii = 1;
               while(fabs(fx - f->ii/(s->Tsw*round(f->ii/(fx*s->Tsw))))/fx > 0.01)
               {
                  f->ii++;
               }
               f->freq = f->ii/(s->Tsw*round(f->ii/(fx*s->Tsw)));
            }
            else f->freq = fx;

            // injection amplitude computation in linear space
            if(f->freq > f->fhi)f->mag = f->ahi;
            else
            {
               if(f->freq < f->flo)f->mag = f->alo;
               else f->mag = f->alo + (f->ahi - f->alo)*(f->freq - f->flo)/(f->fhi - f->flo);
            }
         }
         double ts;
         ts = f->dwell_period/f->freq;
         if(ts < f->dwell_mintime) ts = f->dwell_mintime;
         f->tsampling = *t + ts;
      }
      else
      {
         if((s->Tsw > 0) && f->use_qlog_optimizer)
         {
            double ts;
            ts = f->ii/f->freq;
            f->tsampling = *t + ts;
         }
         else
         {
            double ts;
            ts = f->meas_period/f->freq;
            if(ts < f->meas_mintime) ts = ceil(f->meas_mintime*f->freq)/f->freq;
            f->tsampling = *t + ts;
         }
      }
      //============================================================================
      /*if(f->i < f->fstep)
      {
         Display("%f\t%f\t%f\t%f\t%e\n", f->i, f->freq, f->mag, f->ii, f->tsampling);
      }*/
      if(f->i > 0)
      {
         if(f->i != floor(f->i))
         {
            f->in1ap = *in1a;
            f->in1bp = *in1b;
            f->in2ap = *in2a;
            f->in2bp = *in2b;
         }
         else
         {
            double in1am, in1bm, in2am, in2bm;
            double in1mag, in1ph, in2mag, in2ph;
            double in21mag, in21ph;
            in1am = *in1a - f->in1ap;
            in1bm = *in1b - f->in1bp;
            in2am = *in2a - f->in2ap;
            in2bm = *in2b - f->in2bp;

            in1mag = 20*log10(sqrt(in1am*in1am + in1bm*in1bm));
            in1ph  = 180/M_PI*atan2(in1bm,in1am);
            in2mag = 20*log10(sqrt(in2am*in2am + in2bm*in2bm));
            in2ph  = 180/M_PI*atan2(in2bm,in2am);

            in1ph = in1ph + f->in1wrap;
            in2ph = in2ph + f->in2wrap;

            if(in1ph > (f->in1ph + 1800))
            {
               f->in1wrap -= 360;
               in1ph += f->in1wrap;
            }
            else
            {
               if(in1ph < (f->in1ph - 180))
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
            Display("\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",f->i,freq_p,in1mag,in2mag,in21mag,in1ph,in2ph,in21ph);
         }
      }
   }
}

void SS_DETECT(struct ss_data *s, double *t,
   double *t_prev, double *sense, double *sw_trig)
{
   bool trigger;

   if(!s->ss_flag)
   {
      if(s->trig_edge)trigger = *sw_trig > s->trig_prev;
      else trigger = *sw_trig < s->trig_prev;

      if(trigger && s->use_ss_detect)
      {
         s->sense_avg = s->sense_avg + (*sense - s->sense_val[s->counter])/s->moving_avg_length;
         s->sense_val[s->counter] = *sense;

         if(s->counter_ovf)
         {
            double error = fabs((s->sense_avg - *sense)/s->sense_avg);
            if((error < s->error_lim) || (*t > s->tmax))
            {
               if(s->steady_counter == 0)s->trigger_time = *t;

               s->steady_counter++;
               if(s->steady_counter > s->steady_ctr_lim)
               {
                  s->ss_flag = 1;
                  s->Tsw = (*t - s->trigger_time)/s->steady_ctr_lim;
                  Display("ss_time = %f\tsense_avg = %f\tTsw = %e\n",*t, s->sense_avg, s->Tsw);
               }
            }
            else
            {
               s->steady_counter = 0;
            }
         }
         s->counter++;
         if(s->counter >= s->moving_avg_length)s->counter = 0;s->counter_ovf = 1;
      }

      if((*t > s->tmax) && !s->use_ss_detect)
      {
         s->ss_flag = 1;
         s->sense_avg = 0;
         s->Tsw = -1;
         Display("ss_time = %f\tsense_avg = %f\tTsw = %e\n",*t, s->sense_avg, s->Tsw);
      }
   }
   s->trig_prev = *sw_trig;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sFRA_V2 *inst)
{
   return inst->maxstep; // implement a good choice of max timestep size that depends on struct sFRA_V2
}

extern "C" __declspec(dllexport) void Destroy(struct sFRA_V2 *inst)
{
   free(inst);
}
