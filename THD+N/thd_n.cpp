/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Automatically generated C++ file on Thu Dec 11 14:44:05 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD thd_n.cpp kernel32.lib
#include <malloc.h>
#include <math.h>

extern "C" __declspec(dllexport) void (*Display)(const char *format, ...)       = 0; // works like printf()
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
#undef i1
#undef i2
#undef i3

struct sTHD_N
{
  // declare the structure here
  double tprev;

  double ins1p; double inc1p; double is1; double ic1;
  double ins2p; double inc2p; double is2; double ic2;
  double ins3p; double inc3p; double is3; double ic3;

  double isum1; double isum2; double isum3;

  double i1p; double i2p; double i3p;

  bool meas;

  double start;
  double stop;
};

bool printed = 0;

extern "C" __declspec(dllexport) void thd_n(struct sTHD_N **opaque, double t, union uData *data)
{
   double i1    = data[0].d; // input
   double i2    = data[1].d; // input
   double i3    = data[2].d; // input
   double start = data[3].d; // input parameter
   double stop  = data[4].d; // input parameter
   double fund  = data[5].d; // input parameter

   if(!*opaque)
   {
      *opaque = (struct sTHD_N *) malloc(sizeof(struct sTHD_N));
      bzero(*opaque, sizeof(struct sTHD_N));
      struct sTHD_N *inst = *opaque;

      inst->start = start;
      inst->stop = stop;
   }
   struct sTHD_N *inst = *opaque;

// Implement module evaluation code here:
   double dt = t - inst->tprev;

   if(t >= start && t <= stop)
   {
      double cc = cos(2*M_PI*fund*t);
      double ss = sin(2*M_PI*fund*t);

      double ins1 = i1 * ss; double inc1 = i1 * cc;
      double ins2 = i2 * ss; double inc2 = i2 * cc;
      double ins3 = i3 * ss; double inc3 = i3 * cc;

      if(inst->meas == 0)
      {
         inst->meas = 1;