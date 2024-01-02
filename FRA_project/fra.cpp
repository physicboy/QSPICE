// Automatically generated C++ file on Tue Nov 21 18:10:39 2023
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD fra.cpp kernel32.lib

//#define __DISPLAY

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

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

void display(const char *fmt, ...)
{ // for diagnostic print statements
   msleep(30);
   fflush(stdout);
   va_list args = { 0 };
   va_start(args, fmt);
   vprintf(fmt, args);
   va_end(args);
   fflush(stdout);
   msleep(30);
}

void bzero(void *ptr, unsigned int count)
{
   unsigned char *first = (unsigned char *) ptr;
   unsigned char *last  = first + count;
   while(first < last)
      *first++ = '\0';
}

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef ina1
#undef inb1
#undef ina2
#undef inb2
#undef freq
#undef amp
#undef mag1
#undef phase1
#undef mag2
#undef phase2
#undef status

struct sFRA
{
   // declare the structure here
   double t_prev;
   double t_step;
   double t_step_prev;
   double cntr;
   double fstep_now;
   double amp_now;

   double t_sample;

   double ina1_prev;
   double inb1_prev;
   double ina2_prev;
   double inb2_prev;

   double ina1_interp;
   double inb1_interp;
   double ina2_interp;
   double inb2_interp;

   double ina1_sample;
   double inb1_sample;
   double ina2_sample;
   double inb2_sample;

   double a1;
   double b1;
   double a2;
   double b2;

   double mag_out1;
   double ph_out1;
   double mag_out2;
   double ph_out2;

   double max_step;
};

double cntr_print;

clock_t t_begin;

FILE *fptr;
char *fname = "log_fra.txt";

extern "C" __declspec(dllexport) void fra(struct sFRA **opaque, double t, union uData *data)
{
   double  ina1               = data[ 0].d; // input
   double  inb1               = data[ 1].d; // input
   double  ina2               = data[ 2].d; // input
   double  inb2               = data[ 3].d; // input
   double  param4             = data[ 4].d; // input
   double  param3             = data[ 5].d; // input
   double  param2             = data[ 6].d; // input
   double  param1             = data[ 7].d; // input
   double  a_lo               = data[ 8].d; // input parameter
   double  a_hi               = data[ 9].d; // input parameter
   double  f_lo               = data[10].d; // input parameter
   double  f_hi               = data[11].d; // input parameter
   double  f_min              = data[12].d; // input parameter
   double  f_max              = data[13].d; // input parameter
   double  lin0_log1          = data[14].d; // input parameter
   double  f_step             = data[15].d; // input parameter
   double  tstep_factor       = data[16].d; // input parameter
   double  pre_dwell          = data[17].d; // input parameter
   double  sine_dwell_mintime = data[18].d; // input parameter
   double  sine_dwell_period  = data[19].d; // input parameter
   double  sine_meas_mintime  = data[20].d; // input parameter
   double  ttol               = data[21].d; // input parameter
   double  sine_meas_period   = data[22].d; // input parameter
   double &status             = data[23].d; // output
   double &amp                = data[24].d; // output
   double &freq               = data[25].d; // output

   if(!*opaque)
   {
      *opaque = (struct sFRA *) malloc(sizeof(struct sFRA));
      bzero(*opaque, sizeof(struct sFRA));

      t_begin = clock();

      #ifdef __DISPLAY
      display("FRA analyzer by Arief Noor Rahman\n");
      #endif

      time_t sim_time;
      time(&sim_time);

      fptr = fopen(fname,"a");
      fprintf(fptr,ctime(&sim_time));
      fprintf(fptr,"FRA analyzer by Arief Noor Rahman\n");
      fclose(fptr);

      struct sFRA *inst = *opaque;
      inst->t_step = pre_dwell;
      if((sine_dwell_period/f_min) > sine_dwell_mintime)
      {
         inst->t_sample = pre_dwell + sine_dwell_period/f_min;
      }
      else
      {
         inst->t_sample = pre_dwell + ceil(sine_dwell_mintime*f_min)/f_min;
      }
      inst->fstep_now = 0;
      inst->amp_now = 0;
      inst->cntr = -1;
      cntr_print = inst->cntr;

      inst->max_step = pre_dwell*tstep_factor;

   }

   struct sFRA *inst = *opaque;

   // Implement module evaluation code here:
   if((inst->t_prev <= inst->t_step)&&(t >= inst->t_step))
   {
      if(inst->cntr < 0)
      {
        inst->cntr += 1;
        if(cntr_print<inst->cntr)
        {
          // print the steady state circuit parameter
          // print the title row

          #ifdef __DISPLAY
          display("param1:\t%f\n",param1);
          display("param2:\t%f\n",param2);
          display("param3:\t%f\n",param3);
          display("param4:\t%f\n",param4);
          display("No\tFreq\tMag[IN1](dB)\tPh[IN1](deg)\tMag[IN2](dB)\tPh[IN2](deg)\tMag[IN2-IN1](dB)\tPh[IN2-IN1](deg)\telapsed time\n");
          #endif

          fptr = fopen(fname,"a");
          fprintf(fptr,"param1:\t%f\n",param1);
          fprintf(fptr,"param2:\t%f\n",param2);
          fprintf(fptr,"param3:\t%f\n",param3);
          fprintf(fptr,"param4:\t%f\n",param4);
          fprintf(fptr,"No\tFreq\tMag[IN1](dB)\tPh[IN1](deg)\tMag[IN2](dB)\tPh[IN2](deg)\tMag[IN2-IN1](dB)\tPh[IN2-IN1](deg)\telapsed time\n");
          fclose(fptr);
          cntr_print = inst->cntr;
        }
      }
      else
      {
         // end of sine measurement period.
         // calculate the mag and phase of both input and output signals
         inst->cntr += 0.5;

         inst->ina1_interp = inst->ina1_prev + (ina1 - inst->ina1_prev)/(t - inst->t_prev)*(inst->t_step - inst->t_prev);
         inst->inb1_interp = inst->inb1_prev + (inb1 - inst->inb1_prev)/(t - inst->t_prev)*(inst->t_step - inst->t_prev);
         inst->ina2_interp = inst->ina2_prev + (ina2 - inst->ina2_prev)/(t - inst->t_prev)*(inst->t_step - inst->t_prev);
         inst->inb2_interp = inst->inb2_prev + (inb2 - inst->inb2_prev)/(t - inst->t_prev)*(inst->t_step - inst->t_prev);

         inst->a1 = 2*(inst->ina1_interp - inst->ina1_sample)/(inst->t_step - inst->t_sample);
         inst->b1 = 2*(inst->inb1_interp - inst->inb1_sample)/(inst->t_step - inst->t_sample);
         inst->mag_out1 = sqrt(inst->a1*inst->a1 + inst->b1*inst->b1);
         inst->ph_out1 = -1*atan2(inst->b1,inst->a1)*180/M_PI;

         inst->a2 = 2*(inst->ina2_interp - inst->ina2_sample)/(inst->t_step - inst->t_sample);
         inst->b2 = 2*(inst->inb2_interp - inst->inb2_sample)/(inst->t_step - inst->t_sample);
         inst->mag_out2 = sqrt(inst->a2*inst->a2 + inst->b2*inst->b2);
         inst->ph_out2 = -1*atan2(inst->b2,inst->a2)*180/M_PI;

         if(cntr_print<inst->cntr)
         {
            if(inst->cntr > 0)
            {
               if(inst->cntr <= f_step)
               {
                  // print the fourier spectrum result

                  #ifdef __DISPLAY
                  display("%i\t%05.5f\t%05.5f\t%05.5f\t%05.5f\t%05.5f\t%05.5f\t%05.5f\t%.3f\n",
                     (int)inst->cntr, inst->fstep_now, 20*log10(inst->mag_out1), inst->ph_out1,
                     20*log10(inst->mag_out2), inst->ph_out2, 20*log10(inst->mag_out2/inst->mag_out1), inst->ph_out2 - inst->ph_out1,
                     ((float)(clock() - t_begin)) / CLOCKS_PER_SEC);
                  #endif

                  fptr = fopen(fname,"a");
                  fprintf(fptr,"%i\t%05.5f\t%05.5f\t%05.5f\t%05.5f\t%05.5f\t%05.5f\t%05.5f\t%.3f\n",
                     (int)inst->cntr, inst->fstep_now, 20*log10(inst->mag_out1), inst->ph_out1,
                     20*log10(inst->mag_out2), inst->ph_out2, 20*log10(inst->mag_out2/inst->mag_out1), inst->ph_out2 - inst->ph_out1,
                     ((float)(clock() - t_begin)) / CLOCKS_PER_SEC);
                  fclose(fptr);
                  cntr_print = inst->cntr;
               }
            }
         }
      }
      if(inst->fstep_now < f_max)
      {
         if(lin0_log1 == 1)
         {
            inst->fstep_now = pow(10,(log10(f_min) + log10(f_max/f_min)*inst->cntr/(f_step-1)));
            if(inst->fstep_now <= f_lo)
            {
               // compute the sinewave injection amplitude
               inst->amp_now = a_lo;
            }
            else
            {
               if(inst->fstep_now >= f_hi)
               {
                  inst->amp_now = a_hi;
               }
               else
               {
                  inst->amp_now = a_lo + (a_lo - a_hi)/(log10(f_lo)-log10(f_hi))*(log10(inst->fstep_now)-log10(f_lo));
               }
            }
            if((sine_dwell_period/inst->fstep_now) > sine_dwell_mintime)
            {
               // compute the end of sine dwell time
               inst->t_sample = inst->t_step + sine_dwell_period/inst->fstep_now;
            }
            else
            {
               inst->t_sample = inst->t_step + ceil(sine_dwell_mintime*inst->fstep_now)/inst->fstep_now;
            }
            inst->t_step_prev = inst->t_step;
            if((sine_meas_period/inst->fstep_now) > sine_meas_mintime)
            {
               // compute the timing instance for the sine fourier measurement
               inst->t_step = inst->t_sample + sine_meas_period/inst->fstep_now;
            }
            else
            {
               inst->t_step = inst->t_sample + ceil(sine_meas_mintime*inst->fstep_now)/inst->fstep_now;
            }
            inst->max_step = tstep_factor/inst->fstep_now;
         }
         else
         {
            if(lin0_log1 == 0)
            {
               inst->fstep_now = f_min + (f_max - f_min)*inst->cntr/(f_step - 1);
               if(inst->fstep_now <= f_lo)
               {
                  inst->amp_now = a_lo;
               }
               else
               {
                  if(inst->fstep_now >= f_hi)
                  {
                     inst->amp_now = a_hi;
                  }
                  else
                  {
                     inst->amp_now = a_lo + (a_lo - a_hi)/(f_lo - f_hi)*(inst->fstep_now - f_lo);
                  }
               }
               if((sine_dwell_period/inst->fstep_now) > sine_dwell_mintime)
               {
                  inst->t_sample = inst->t_step + sine_dwell_period/inst->fstep_now;
               }
               else
               {
                  inst->t_sample = inst->t_step + ceil(sine_dwell_mintime*inst->fstep_now)/inst->fstep_now;
               }
               inst->t_step_prev = inst->t_step;
               if((sine_meas_period/inst->fstep_now) > sine_meas_mintime)
               {
                  inst->t_step = inst->t_sample + sine_meas_period/inst->fstep_now;
               }
               else
               {
                  inst->t_step = inst->t_sample + ceil(sine_meas_mintime*inst->fstep_now)/inst->fstep_now;
               }

               inst->max_step = tstep_factor/inst->fstep_now;
            }
            else
            {
               // invalid entry for the lin0_lig1
               inst->max_step = -1e+308;
            }
         }
      }
      else
      {
         // FRA simulation has completed
         inst->fstep_now = 0;
         inst->amp_now = 0;

         inst->t_step_prev = inst->t_step;

         inst->max_step = -1e+308;
      }
   }

   if((inst->t_prev <= inst->t_sample)&&(t >= inst->t_sample))
   {
      // perform sampling at the beginning of the sine measurement period
      inst->cntr += 0.5;

      inst->ina1_sample = inst->ina1_prev + (ina1 - inst->ina1_prev)/(t - inst->t_prev)*(inst->t_sample - inst->t_prev);
      inst->inb1_sample = inst->inb1_prev + (inb1 - inst->inb1_prev)/(t - inst->t_prev)*(inst->t_sample - inst->t_prev);
      inst->ina2_sample = inst->ina2_prev + (ina2 - inst->ina2_prev)/(t - inst->t_prev)*(inst->t_sample - inst->t_prev);
      inst->inb2_sample = inst->inb2_prev + (inb2 - inst->inb2_prev)/(t - inst->t_prev)*(inst->t_sample - inst->t_prev);
   }

   amp = inst->amp_now;
   freq = inst->fstep_now;
   status   = (inst->max_step < 0);

   inst->ina1_prev = ina1;
   inst->inb1_prev = inb1;
   inst->ina2_prev = ina2;
   inst->inb2_prev = inb2;

   inst->t_prev = t;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sFRA *inst)
{
   return inst->max_step;
}

extern "C" __declspec(dllexport) void Trunc(struct sFRA *inst, double t, union uData *data, double *timestep)
{ // limit the timestep to a tolerance if the circuit causes a change in struct sFRA

   double  ttol               = data[21].d; // input parameter
   if(ttol < 0)
   {
      if(*timestep > ttol)
      {
      double &status             = data[23].d; // output
      double &amp                = data[24].d; // output
      double &freq               = data[25].d; // output

      // Save output vector
      const double _status             = status            ;
      const double _amp                = amp               ;
      const double _freq               = freq              ;


      struct sFRA tmp = *inst;
      fra(&(&tmp), t, data);
      if(tmp.cntr != inst->cntr) // implement a meaningful way to detect if the state has changed
      *timestep = ttol;

      // Restore output vector
      status             = _status            ;
      amp                = _amp               ;
      freq               = _freq              ;
      }
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sFRA *inst)
{
   // end of simulation
   if(inst->cntr >0)
   {
      #ifdef __DISPLAY
      display("\n--- end of simulation ---\n");
      display("execution time: %f sec\n", ((float)(clock() - t_begin)) / CLOCKS_PER_SEC);
      display("Hope your simulation converge well and thanks for waiting,\n ---Arief Noor Rahman---\n\n");
      #endif

      fptr = fopen(fname,"a");
      fprintf(fptr,"\n--- end of simulation ---\n");
      fprintf(fptr,"execution time: %f sec\n", ((float)(clock() - t_begin)) / CLOCKS_PER_SEC);
      fprintf(fptr,"Hope your simulation converge well and thanks for waiting,\n ---Arief Noor Rahman---\n\n");
      fclose(fptr);
   }

   free(inst);
}
