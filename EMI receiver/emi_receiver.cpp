/*
 * Copyright 2026 Arief Noor Rahman - Power Control Design
 *
 * Project  : EMI_receiver
 * Filename : emi_receiver.cpp
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

// Automatically generated C++ file on Sun Jul  5 11:10:00 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD -o dfft_test_x1.cpp kernel32.lib

// CISPR 16 EMI receiver realization with FFT for typical 150kHz~30MHz conducted EMI measurement
// The measurement is performed at fixed 120MSPS (4x the 30MHz limit) and sample window of 2^16
// This configuration yields measurement spectrum from 148315.4297Hz to 30.0MHz and
// temporal frequency resolution of 1831.05Hz
//
// Computation algorithm as following:
// 1. Input signal measurement
// 2. Uniform sampling period resampling to 1/120MHz and apply Flat top FFT window
// 3. Fill the uniform sampling waveform into buffer until 2^16 data is full
// 4. FFT with normalized magnitude scaling
// 5. Spectrum magnitude calculation
// 6. RBW IF filter realization in frequency domain
// 7. average and peak detection
//
// Just be aware, there is no buffer under/overflow guard.
// In the test and with the code as is, buffer under/overflow is not likely.
// since the the array index to be accessed has been identified with some lazy extra array accessible.

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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

#define FFT_N     65536
#define FSAMPLING 120E6

// RBW_LENGTH = 27 bins corresponds to Fcenter +/- 50kHz
// 27 ~ 50E3 / (FSAMPLING / FFT_N)
#define RBW_LENGTH   27

struct sComplex {
    double re;
    double im;
};

struct sEMI_RECEIVER
{
  // declare the structure here
   double tprev;
   double xprev;
   double tsampling;
   int counter;
   int set;

   double window[FFT_N];
   double window_sum;

   struct sComplex dd[FFT_N];

   double mag[FFT_N / 2];

   double rbw_window[RBW_LENGTH + 1];

   double rbw[FFT_N / 2][4];
   // mag_RBW col 0 : Freq in Hz
   // mag_RBW col 1 : Raw RBW output
   // mag_RBW col 2 : RBW average output by accumulator then divide by sample
   // mag_RBW col 3 : RBW peak output
};

void generate_flat_top_window(double *window, double *window_sum);
void generate_RBW_window(double *rbw_window);
void compute_spectrum(struct sComplex *data, double *window_sum, double *mag, double rbw[][4], double *rbw_window);
extern "C" __declspec(dllexport) void emi_receiver(struct sEMI_RECEIVER **opaque, double t, union uData *data)
{
   double in     = data[0].d; // input
   double tstart = data[1].d; // input parameter

   if(!*opaque)
   {
      *opaque = (struct sEMI_RECEIVER *) malloc(sizeof(struct sEMI_RECEIVER));
      bzero(*opaque, sizeof(struct sEMI_RECEIVER));

      struct sEMI_RECEIVER *inst = *opaque;

      inst->tsampling = tstart;
      generate_flat_top_window(inst->window, &inst->window_sum);
      generate_RBW_window(inst->rbw_window);
   }
   struct sEMI_RECEIVER *inst = *opaque;

// Implement module evaluation code here:
   if(inst->tprev <= inst->tsampling && inst->tsampling < t)
   {
      double xslope = (in * 1E6 - inst->xprev) / (t - inst->tprev);
      while(1)
      {
         inst->dd[inst->counter].re = inst->xprev + xslope *(inst->tsampling - inst->tprev);
         inst->dd[inst->counter].re *= inst->window[inst->counter];
         inst->counter++;
         inst->tsampling += 1. / FSAMPLING;

         if(inst->counter >= FFT_N)
         {
            inst->counter = 0;
            inst->set++;
            compute_spectrum(inst->dd, &inst->window_sum, inst->mag, inst->rbw, inst->rbw_window);
            int i = 0;
            while(inst->rbw[i][0] != 0)
            {
               inst->rbw[i][2] += inst->rbw[i][1];
               if(inst->rbw[i][3] < inst->rbw[i][1])inst->rbw[i][3] = inst->rbw[i][1];
               i++;
            }
         }

         if(inst->tsampling > t)
         {
            break;
         }
      }
   }

   inst->xprev = in * 1E6;
   inst->tprev = t;
}

extern "C" __declspec(dllexport) void Destroy(struct sEMI_RECEIVER *inst)
{
   FILE *fp = fopen("EMI_report.csv", "w");

   fprintf(fp, "Freq,");
   fprintf(fp, "CISPR32A_AVG,CISPR32A_QP,");
   fprintf(fp, "CISPR32B_AVG,CISPR32B_QP,");
   fprintf(fp, "MAG_RAW,MAG_AVG,MAG_PEAK\n");
   int i = 0;
   while(inst->rbw[i][0] != 0.0)
   {
      double cispr32b;
      if(inst->rbw[i][0] <= 500E3)
      {
         cispr32b = 630.957 * pow(150000/inst->rbw[i][0], 0.95625);
      }
      else
      {
         if(inst->rbw[i][0] <= 5E6)
         {
            cispr32b = 199.5;
         }
         else
         {
            cispr32b = 316.2;
         }
      }


      fprintf(fp, "%f,", inst->rbw[i][0]);
      fprintf(fp, "%.10f,0,%.10f,0,", inst->rbw[i][0]<500E3?8913.:4466.8, inst->rbw[i][0]<500E3?2000.:1000.);
      fprintf(fp, "%.10f,0,%.10f,0,", cispr32b, cispr32b*3.16);
      fprintf(fp, "%.10f,0,%.10f,0,%.10f,0\n", inst->rbw[i][1], inst->rbw[i][2]/inst->set, inst->rbw[i][3]);
      i++;
   }

   fclose(fp);

   char *a = "\"c:\\Program Files\\QSPICE\\QUX\" ";
   strcat(a, "EMI_report.csv");
   system(a);
   a = "";

   free(inst);
}

void generate_flat_top_window(double *window, double *window_sum)
{
/**
 * Generates a Flat Top window function.
 * @param window Array to store the generated window coefficients.
 * @param N      The total number of data points.
 */
    // Standard Flat Top window coefficients
    const double a0 = 0.21557895;
    const double a1 = 0.41663158;
    const double a2 = 0.27726315;
    const double a3 = 0.08357894;
    const double a4 = 0.00694736;

    double denom = (double)(FFT_N - 1);

    for (int n = 0; n < FFT_N; n++)
    {
      double theta = (2.0 * M_PI * n) / denom;

      window[n] = a0
                  - a1 * cos(theta)
                  + a2 * cos(2.0 * theta)
                  - a3 * cos(3.0 * theta)
                  + a4 * cos(4.0 * theta);
      *window_sum += window[n];
    }
}

void generate_RBW_window(double *rbw_window)
{
/**
 * Generates the weight sum normalized RBW window for -6dB attenuation at +/-4.5kHz sideband
 */
   double rbw_sum = 0.;

   rbw_window[0] = 1.;
   rbw_sum = rbw_window[0];
   for(int n = 1; n <= RBW_LENGTH; n++)
   {
      double k = n * ((double)FSAMPLING / (double)FFT_N) / 6993.64; // for RBW -6dB at 9kHz
      k = 1. / (1. + k * k);
      rbw_window[n] = k * k;

      rbw_sum += 2. * rbw_window[n];
   }

   for(int n = 0; n <= RBW_LENGTH; n++)
   {
      rbw_window[n] /= rbw_sum;
   }
}

void compute_spectrum(struct sComplex *data, double *window_sum, double *mag, double rbw[][4], double *rbw_window)
{
   int i, ii, iii;
   int imin = floor(150E3 / ((double)FSAMPLING / (double)FFT_N));
   int imax = ceil(30E6 / ((double)FSAMPLING / (double)FFT_N));
   double rere = 0;
   double imim = 0;

   i = DFFT(data, false, FFT_N, 1./(double)FFT_N);

   // Dont compute the spectrum magnitude until FFT_N/2
   // instead only until 30MHz + a few extra right sideband for RBW calculation
   for(i = 0; i <= imax + 2*RBW_LENGTH; i++)
   {
      if(i == 0) mag[0] = data[0].re * FFT_N / *window_sum;
      else if(i != FFT_N/2)
      {
         rere = data[i].re + data[FFT_N-i].re;
         imim = data[i].im - data[FFT_N-i].im;
         mag[i] = sqrt(rere*rere + imim*imim) * FFT_N / *window_sum;
      }
      else mag[FFT_N/2] = sqrt(data[FFT_N / 2].re*data[FFT_N / 2].re + data[FFT_N / 2].im*data[FFT_N / 2].im) *
                                 FFT_N / *window_sum;
   }

   iii = 0;
   for(i = imin; i <= imax; i++)
   {
      //in mag_RBW array, enforce the array element 0 to hold the minimum frequency (150kHz).
      rbw[iii][0] = i * (double)FSAMPLING / (double)FFT_N;
      rbw[iii][1] = rbw_window[0] * mag[i];
      for(int ii = 1; ii <= RBW_LENGTH; ii++)
      {
         rbw[iii][1] += rbw_window[ii] * (mag[i-ii] + mag[i+ii]);
      }

      // Final gain compensation to map 1V input signal to exactly -3dB.
      rbw[iii][1] *= 1.164056256;

      iii++;
   }
}
