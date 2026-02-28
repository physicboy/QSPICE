/*
 * Copyright 2026 Arief Noor Rahman - Power Control Design
 * 
 * Project  : Droop Study
 * Filename : droop_x1.cpp
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

// Automatically generated C++ file on Tue Jul 30 12:14:45 2024
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD droop_x1.cpp kernel32.lib

#include <malloc.h>

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
#undef sense
#undef out

struct sDROOP_X1
{
  // declare the structure here
  double maxstep;

  double tstep;

  double iout;

  double error;
  double error_prev;
};

extern "C" __declspec(dllexport) void droop_x1(struct sDROOP_X1 **opaque, double t, union uData *data)
{
   double  sense       = data[0].d; // input
   double  en          = data[1].d; // input
   double  sense_gain  = data[2].d; // input parameter
   double  droop_gain  = data[3].d; // input parameter
   double &out         = data[4].d; // output

   if(!*opaque)
   {
      *opaque = (struct sDROOP_X1 *) malloc(sizeof(struct sDROOP_X1));
      bzero(*opaque, sizeof(struct sDROOP_X1));
   }
   struct sDROOP_X1 *inst = *opaque;

// Implement module evaluation code here:

   double vref;

   if(t >= inst->tstep)
   {
      inst->tstep += 10E-6;

      if(en>0.5)
      {
         vref = 10 - droop_gain*inst->iout;
         inst->error = vref - sense*sense_gain;
         inst->iout = inst->iout + 2*(inst->error - inst->error_prev) + 0.2*inst->error;
         inst->error_prev = inst->error;
         if(inst->iout > 10) inst->iout = 10;
         if(inst->iout < 0) inst->iout = 0;
      }
      else
      {
         inst->iout = 0;
         inst->error_prev = 0;
      }
   }
   out = inst->iout;

   inst->maxstep = inst->tstep - t;

}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sDROOP_X1 *inst)
{
   return inst->maxstep; // implement a good choice of max timestep size that depends on struct sDROOP_X1
}

extern "C" __declspec(dllexport) void Destroy(struct sDROOP_X1 *inst)
{
   free(inst);
}
