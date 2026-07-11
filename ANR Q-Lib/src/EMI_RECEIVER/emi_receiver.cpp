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
 * http://www.apache.org/licenses/LICENSE-2.0
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

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Frequency bin resolution 1kHz, sample length 2^17 to aim for max BW of ~6MHz
#define FFT_N        131072
#define FSAMPLING    131072000
#define FMAX         30000000
#define FMIN         150000

// RBW_LENGTH = 50 bins corresponds to Fcenter +/- 50kHz
#define RBW_LENGTH   50

// Required tuning parameters to aim -6dB at +/- 4.5kHz
// Smaller RBW_BI_LORENTZIAN_CONSTANT yields steeper drop
// do this tuning first!
#define RBW_BI_LORENTZIAN_CONSTANT  5900.

// Required tuning parameters to aim 2Vpp single tone input signal to yield 117dBuV
// do this tuning for the second!
#define RBW_GAIN_CORRECTION         1.595

// maximum buffer length 0.1sec / update_rate
// update_rate = (FFT_N / FSAMPLING) * (1 - %OVERLAP) = 62.5us
#define QP_BUFFER_LENGTH            1600

// QP_LPF_CONSTANT = tau / (tau + Ts)
#define QP_LPF_RISE                 0.94117647058  /*tau = 1ms,   Ts = update_rate*/
#define QP_LPF_FALL                 0.99960952752  /*tau = 160ms, Ts = update_rate*/

extern "C" __declspec(dllexport) void (*Display)(const char *format, ...)       = 0; // works like printf()
extern "C" __declspec(dllexport) void (*EXIT)(const char *format, ...)          = 0; // print message like printf() but exit(0) afterward
extern "C" __declspec(dllexport) const double *DegreesC                         = 0; // pointer to current circuit temperature
extern "C" __declspec(dllexport) const int *StepNumber                          = 0; // pointer to current step number
extern "C" __declspec(dllexport) const int *NumberSteps                          = 0; // pointer to estimated number of steps
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
extern "C" __declspec(dllexport) const char *(*BinaryFormat)(unsigned int data)          = 0; // BinaryFormat(0x1C) returns "0b00011100"
extern "C" __declspec(dllexport) const char *(*EngFormat   )(double x, const char *units, int numDgts)   = 0; // EngFormat(1e-6, "s", 6) returns "1�s"
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

int __stdcall DllMain(void *module, unsigned int reason, void *reserved) { return 1; }


struct sComplex {
    double re;
    double im;
};

struct sDATA_BLOCK
{
   double tprev;
   double tstart;
   double tsampling;
   int set;
   double in1prev, in2prev;

   double window[FFT_N];
   double window_sum;
   float rbw_window[RBW_LENGTH + 1];

   // 16 Overlapping Counters & Data Buffers (A through P)
   int counterA; struct sComplex ddA[FFT_N][2];
   int counterB; struct sComplex ddB[FFT_N][2];
   int counterC; struct sComplex ddC[FFT_N][2];
   int counterD; struct sComplex ddD[FFT_N][2];
   int counterE; struct sComplex ddE[FFT_N][2];
   int counterF; struct sComplex ddF[FFT_N][2];
   int counterG; struct sComplex ddG[FFT_N][2];
   int counterH; struct sComplex ddH[FFT_N][2];
   int counterI; struct sComplex ddI[FFT_N][2];
   int counterJ; struct sComplex ddJ[FFT_N][2];
   int counterK; struct sComplex ddK[FFT_N][2];
   int counterL; struct sComplex ddL[FFT_N][2];
   int counterM; struct sComplex ddM[FFT_N][2];
   int counterN; struct sComplex ddN[FFT_N][2];
   int counterO; struct sComplex ddO[FFT_N][2];
   int counterP; struct sComplex ddP[FFT_N][2];

   float rbw1[FMAX / (FSAMPLING / FFT_N)][6], rbw2[FMAX / (FSAMPLING / FFT_N)][6];
   // ======= rbw1, rbw2 ========
   // col0 = freq
   // col1 = raw rbw filter output
   // col2 = average rbw accumulator
   // col3 = peak rbw
   // col4 = temp quasi peak rbw
   // col5 = final quasi peak rbw

   float qp_buf1[FMAX / (FSAMPLING / FFT_N)][QP_BUFFER_LENGTH], qp_buf2[FMAX / (FSAMPLING / FFT_N)][QP_BUFFER_LENGTH];
};

#define FFT_BUFFER_FILLING(COUNTER, BUFFER1, BUFFER2)                \
      if(COUNTER >= 0)                                               \
      {                                                              \
         BUFFER1[COUNTER].re = inst->window[COUNTER] * in1interp;    \
         BUFFER2[COUNTER].re = inst->window[COUNTER] * in2interp;    \
      }                                                              \
      COUNTER++;

#define FFT_EXECUTION(COUNTER, BUFFER1, BUFFER2)                                                                  \
      if(COUNTER >= FFT_N) {                                                                                      \
         compute_spectrum(BUFFER1, &inst->window_sum, inst->rbw1, inst->rbw_window, &inst->set, inst->qp_buf1);   \
         compute_spectrum(BUFFER2, &inst->window_sum, inst->rbw2, inst->rbw_window, &inst->set, inst->qp_buf2);   \
         inst->set++; COUNTER = 0;                                                                                \
      }

#define QP_LPF(Y,X)                                               \
      if(X > Y)   Y = Y * QP_LPF_RISE + X * (1. - QP_LPF_RISE);   \
      else        Y = Y * QP_LPF_FALL + X * (1. - QP_LPF_FALL);

void generate_flat_top_window(double *window, double *window_sum);
void generate_RBW_window(float *rbw_window);
void compute_spectrum(struct sComplex *data, double *window_sum, float rbw[][6], float *rbw_window, int *set, float qp_buf[][QP_BUFFER_LENGTH]);
float cispr32b_avg_limit(float *freq);
void qp_replay(struct sDATA_BLOCK *inst);

extern "C" __declspec(dllexport) void emi_receiver(struct sDATA_BLOCK **opaque, double t, union uData *data)
{
   if(!*ForKeeps) return;

   double in1        = data[0].d; // input
   double in2        = data[1].d; // input
   double tstart     = data[2].d; // input parameter

   if(!*opaque)
   {
      *opaque = (struct sDATA_BLOCK *) malloc(sizeof(struct sDATA_BLOCK));
      bzero(*opaque, sizeof(struct sDATA_BLOCK));

      struct sDATA_BLOCK *inst   = *opaque;
      inst->tstart               = tstart;
      inst->tsampling            = tstart;

      // 15/16 time domain window overlap setup
      int offset     = FFT_N / 16;
      inst->counterA =   0 * offset; inst->counterB =  -1 * offset; inst->counterC =  -2 * offset; inst->counterD =  -3 * offset;
      inst->counterE =  -4 * offset; inst->counterF =  -5 * offset; inst->counterG =  -6 * offset; inst->counterH =  -7 * offset;
      inst->counterI =  -8 * offset; inst->counterJ =  -9 * offset; inst->counterK = -10 * offset; inst->counterL = -11 * offset;
      inst->counterM = -12 * offset; inst->counterN = -13 * offset; inst->counterO = -14 * offset; inst->counterP = -15 * offset;

      generate_flat_top_window(inst->window, &inst->window_sum);
      generate_RBW_window(inst->rbw_window);
   }
   struct sDATA_BLOCK *inst = *opaque;

   if(inst->tprev <= inst->tsampling && inst->tsampling < t)
   {
      double in1interp, in2interp;
      double in1slope = (in1 * 1E6 - inst->in1prev) / (t - inst->tprev);
      double in2slope = (in2 * 1E6 - inst->in2prev) / (t - inst->tprev);
      while(1)
      {
         in1interp = inst->in1prev + in1slope *(inst->tsampling - inst->tprev);
         in2interp = inst->in2prev + in2slope *(inst->tsampling - inst->tprev);

         // Channel Streaming Real-Time Windowing
         // comment out the unnecessary window overlap level as needed
         // Non overlap
         FFT_BUFFER_FILLING(inst->counterA, inst->ddA[0], inst->ddA[1])

            // 50% overlap
            FFT_BUFFER_FILLING(inst->counterI, inst->ddI[0], inst->ddI[1])

               // 75% overlap
               FFT_BUFFER_FILLING(inst->counterE, inst->ddE[0], inst->ddE[1])
               FFT_BUFFER_FILLING(inst->counterM, inst->ddM[0], inst->ddM[1])

                  // 87.5% overlap
                  FFT_BUFFER_FILLING(inst->counterC, inst->ddC[0], inst->ddC[1])
                  FFT_BUFFER_FILLING(inst->counterG, inst->ddG[0], inst->ddG[1])
                  FFT_BUFFER_FILLING(inst->counterK, inst->ddK[0], inst->ddK[1])
                  FFT_BUFFER_FILLING(inst->counterO, inst->ddO[0], inst->ddO[1])

                     //93.75% overlap
                     FFT_BUFFER_FILLING(inst->counterB, inst->ddB[0], inst->ddB[1])
                     FFT_BUFFER_FILLING(inst->counterD, inst->ddD[0], inst->ddD[1])
                     FFT_BUFFER_FILLING(inst->counterF, inst->ddF[0], inst->ddF[1])
                     FFT_BUFFER_FILLING(inst->counterH, inst->ddH[0], inst->ddH[1])
                     FFT_BUFFER_FILLING(inst->counterJ, inst->ddJ[0], inst->ddJ[1])
                     FFT_BUFFER_FILLING(inst->counterL, inst->ddL[0], inst->ddL[1])
                     FFT_BUFFER_FILLING(inst->counterN, inst->ddN[0], inst->ddN[1])
                     FFT_BUFFER_FILLING(inst->counterP, inst->ddP[0], inst->ddP[1])

         // Execute Independent Spectrum Pipeline transformations upon reaching buffer ceilings
         // comment out the unnecessary window overlap level as needed
         // Non overlap
         FFT_EXECUTION(inst->counterA, inst->ddA[0], inst->ddA[1])

            // 50% overlap
            FFT_EXECUTION(inst->counterI, inst->ddI[0], inst->ddI[1])

               // 75% overlap
               FFT_EXECUTION(inst->counterE, inst->ddE[0], inst->ddE[1])
               FFT_EXECUTION(inst->counterM, inst->ddM[0], inst->ddM[1])

                  // 87.5% overlap
                  FFT_EXECUTION(inst->counterC, inst->ddC[0], inst->ddC[1])
                  FFT_EXECUTION(inst->counterG, inst->ddG[0], inst->ddG[1])
                  FFT_EXECUTION(inst->counterK, inst->ddK[0], inst->ddK[1])
                  FFT_EXECUTION(inst->counterO, inst->ddO[0], inst->ddO[1])

                     // 93.75% overlap
                     FFT_EXECUTION(inst->counterB, inst->ddB[0], inst->ddB[1])
                     FFT_EXECUTION(inst->counterD, inst->ddD[0], inst->ddD[1])
                     FFT_EXECUTION(inst->counterF, inst->ddF[0], inst->ddF[1])
                     FFT_EXECUTION(inst->counterH, inst->ddH[0], inst->ddH[1])
                     FFT_EXECUTION(inst->counterJ, inst->ddJ[0], inst->ddJ[1])
                     FFT_EXECUTION(inst->counterL, inst->ddL[0], inst->ddL[1])
                     FFT_EXECUTION(inst->counterN, inst->ddN[0], inst->ddN[1])
                     FFT_EXECUTION(inst->counterP, inst->ddP[0], inst->ddP[1])

         inst->tsampling += 1. / FSAMPLING;
         if(inst->tsampling > t) break;
      }
   }

   inst->in1prev = in1 * 1E6;
   inst->in2prev = in2 * 1E6;
   inst->tprev = t;
}

extern "C" __declspec(dllexport) void Destroy(struct sDATA_BLOCK *inst)
{
   Display("EMI QP Detector Processing~~~");
   int i, ii, iii;
   FILE *fp = fopen("EMI_report.csv", "w");

   qp_replay(inst);

   fprintf(fp, "Freq,");
   fprintf(fp, "CISPR32A_AVG,CISPR32A_QP,");
   fprintf(fp, "CISPR32B_AVG,CISPR32B_QP,");
   fprintf(fp, "MAG1_RAW,MAG1_AVG,MAG1_PEAK,MAG1_QP,");
   fprintf(fp, "MAG2_RAW,MAG2_AVG,MAG2_PEAK,MAG2_QP\n");

   i = 0;
   while(inst->rbw1[i][0] != 0.0)
   {
      fprintf(fp, "%f,", inst->rbw1[i][0]);
      fprintf(fp, "%.10f,0,%.10f,0,", inst->rbw1[i][0]<500E3?8913.:4466.8, inst->rbw1[i][0]<500E3?2000.:1000.);
      fprintf(fp, "%.10f,0,%.10f,0,", cispr32b_avg_limit(&inst->rbw1[i][0]), cispr32b_avg_limit(&inst->rbw1[i][0]) * 3.16);
      fprintf(fp, "%.10f,0,%.10f,0,%.10f,0,%.10f,0,", inst->rbw1[i][1], inst->rbw1[i][2]/inst->set, inst->rbw1[i][3], inst->rbw1[i][5]);
      fprintf(fp, "%.10f,0,%.10f,0,%.10f,0,%.10f,0\n", inst->rbw2[i][1], inst->rbw2[i][2]/inst->set, inst->rbw2[i][3], inst->rbw2[i][5]);
      i++;
   }

   fclose(fp);

   char a[256] = "\"c:\\Program Files\\QSPICE\\QUX\" ";
   strcat(a, "EMI_report.csv");
   system(a);

   free(inst);
}

void generate_flat_top_window(double *window, double *window_sum)
{
    const double a0 = 0.21557895;
    const double a1 = 0.41663158;
    const double a2 = 0.27726315;
    const double a3 = 0.08357894;
    const double a4 = 0.00694736;

    double denom = (double)(FFT_N - 1);

    for (int n = 0; n < FFT_N; n++)
    {
      double theta = (2.0 * M_PI * n) / denom;
      window[n] = a0 - a1 * cos(theta) + a2 * cos(2.0 * theta) - a3 * cos(3.0 * theta) + a4 * cos(4.0 * theta);
      *window_sum += window[n];
    }
}

void generate_RBW_window(float *rbw_window)
{
   float rbw_sum = 0.;
   float k = 0.;
   int i;

   rbw_window[0] = 1.;
   rbw_sum = rbw_window[0];
   for(i = 1; i <= RBW_LENGTH; i++)
   {
      k = i * ((float)FSAMPLING / (float)FFT_N) / RBW_BI_LORENTZIAN_CONSTANT;
      k = 1. / (1. + k * k);
      rbw_window[i] = k * k;
      rbw_sum += 2. * rbw_window[i];
   }

   for(i = 0; i <= RBW_LENGTH; i++)
   {
      rbw_window[i] /= rbw_sum;
   }
}

void compute_spectrum(struct sComplex *data, double *window_sum, float rbw[][6], float *rbw_window, int *set, float qp_buf[][QP_BUFFER_LENGTH])
{
   int i, ii, iii;
   int imin = (int)floor((double)FMIN / ((double)FSAMPLING / (double)FFT_N));
   int imax = (int)ceil((double)FMAX / ((double)FSAMPLING / (double)FFT_N));
   double rere, imim;

   static double mag[FFT_N/2];

   // Call FFT function
   i = DFFT(data, false, FFT_N, 1./(double)FFT_N);

   // spectrum euclidean magnitude computation
   // limit the magnitude at frequency only until the required 30MHz max band
   // add a few bins extra to meet the RBW sideband bins
   for(i = 0; i <= imax + 2 * RBW_LENGTH; i++)
   {
      if(i == 0) mag[0] = data[0].re * FFT_N / *window_sum;
      else if(i != FFT_N/2)
      {
         rere = data[i].re + data[FFT_N-i].re;
         imim = data[i].im - data[FFT_N-i].im;
         mag[i] = sqrt(rere*rere + imim*imim) * FFT_N / *window_sum;
      }
      else mag[FFT_N/2] = sqrt(data[FFT_N / 2].re*data[FFT_N / 2].re + data[FFT_N / 2].im*data[FFT_N / 2].im) * FFT_N / *window_sum;
   }

   iii = 0;
   for(i = imin; i <= imax; i++)
   {
      // Apply weighted window RBW filter in frequency domain
      rbw[iii][0] = i * (float)FSAMPLING / (float)FFT_N;
      rbw[iii][1] = rbw_window[0] * (float)mag[i];
      for(ii = 1; ii <= RBW_LENGTH; ii++)
      {
         rbw[iii][1] += rbw_window[ii] * (mag[i-ii] + mag[i+ii]);
      }

      // RBW filter gain correction to match 2Vpp input signal to 117dBuV
      rbw[iii][1] *= RBW_GAIN_CORRECTION;

      // average detector accumulator
      rbw[iii][2] += rbw[iii][1];

      // peak detector
      if(rbw[iii][3] < rbw[iii][1]) rbw[iii][3] = rbw[iii][1];

      // QP low pass filter and QP peak detector
      QP_LPF(rbw[iii][4],rbw[iii][1])
      if(rbw[iii][5] < rbw[iii][4]) rbw[iii][5] = rbw[iii][4];

      // store the raw RBW output for later QP replay
      // Use modulo to wrap the index back to 0 if it exceeds the buffer length
      int circular_idx = (*set) % QP_BUFFER_LENGTH;
      qp_buf[iii][circular_idx] = rbw[iii][1];

      iii++;
   }
}

float cispr32b_avg_limit(float *freq)
{
   if(*freq <= 500E3)
   {
      return 630.957 * pow(150000/ *freq, 0.95625);
   }
   else
   {
      if(*freq <= 5E6) return 199.5;
      else return 316.2;
   }
}

void qp_replay(struct sDATA_BLOCK *inst)
{
   // virtual qp detector replay processing loop function
   // this function will replay the stored time history RBW raw filter output
   // the duration for the QP detector processing is at least for total 1sec duration to ensure QP steady state
   //
   // note: max buffer size can only store up to 100ms RBW filter output
   // The buffer acts as circular buffer, means when duration overflow,
   // then the oldest data will be replaced with the newest data
   // and the qp replay will only take the latest 100ms duration value.

   int rep_max;
   double duration = *CKTtime - inst->tstart;
   if(duration > 0.1) rep_max = (int)ceil((1. - duration)/0.1);
   else rep_max = (int)ceil(1. / duration);

   int bin_idx = 0;

   // Determine how many valid elements are actually in the buffer
   int total_frames = (inst->set > QP_BUFFER_LENGTH) ? QP_BUFFER_LENGTH : inst->set;

   // If wrapped, the oldest data starts at (inst->set % QP_BUFFER_LENGTH)
   // If not wrapped, it starts at 0
   int start_ptr = (inst->set > QP_BUFFER_LENGTH) ? (inst->set % QP_BUFFER_LENGTH) : 0;

   // Loop through every valid frequency bin calculated in compute_spectrum
   while(inst->rbw1[bin_idx][0] != 0.0)
   {
      // Replay the sequence multiple times to allow the 160ms time constant to settle
      // start with run_count = 1 since the first run has occurred in actual real-time sim
      for(int run_count = 1; run_count < rep_max; run_count++)
      {
         for(int f_idx = 0; f_idx < total_frames; f_idx++)
         {
            // Calculate chronological index using modulo to handle wrap-around
            int time_idx = (start_ptr + f_idx) % QP_BUFFER_LENGTH;

            // Extract the historic instantaneous magnitude
            float raw_val1 = inst->qp_buf1[bin_idx][time_idx];
            float raw_val2 = inst->qp_buf2[bin_idx][time_idx];

            // Apply the asymmetrical Charge/Discharge LPF macro to Channel 1
            QP_LPF(inst->rbw1[bin_idx][4], raw_val1)
            if(inst->rbw1[bin_idx][5] < inst->rbw1[bin_idx][4])
            {
               inst->rbw1[bin_idx][5] = inst->rbw1[bin_idx][4];
            }

            // Apply the asymmetrical Charge/Discharge LPF macro to Channel 2
            QP_LPF(inst->rbw2[bin_idx][4], raw_val2)
            if(inst->rbw2[bin_idx][5] < inst->rbw2[bin_idx][4])
            {
               inst->rbw2[bin_idx][5] = inst->rbw2[bin_idx][4];
            }
         }
      }
      bin_idx++;
   }
   // =================================================================
}
