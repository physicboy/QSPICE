/*
 * Copyright 2026 Arief Noor Rahman - Power Control Design
 *
 * Project  : buck
 * Filename : buck.cpp
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

// Automatically generated C++ file on Tue Aug  4 16:52:50 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD -o buck.cpp kernel32.lib

#define TSAMPLING 5E-6
#define PWM_CH    1
#define TBPRD     1000
#define DTIME     20
#define DGTL_CLK  10E-9

#include <malloc.h>
#include <math.h>
#include "udata.h"
#include "pwm_edge_handler.h"
#include "interrupt_handler.h"

// int DllMain() must exist and return 1 for a process to load the .DLL
// See https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain for more information.
int __stdcall DllMain(void *module, unsigned int reason, void *reserved) { return 1; }

struct sDGTL_CTRL_BLK
{
   // declare the structure here
   double tprev;
   double tsampling;

   int    counter;
   double maxstep;

   struct sPWM pwm[PWM_CH];

   struct sINTERRUPT data;
};

#define MAXSTEP(PWM)    if(t < inst->pwm[PWM].t_prd && inst->maxstep > (inst->pwm[PWM].t_prd - t))inst->maxstep = inst->pwm[PWM].t_prd - t;\
                        if(t < inst->pwm[PWM].t_zero && inst->maxstep > (inst->pwm[PWM].t_zero - t))inst->maxstep = inst->pwm[PWM].t_zero - t;\
                        if(t < inst->pwm[PWM].t_on && inst->maxstep > (inst->pwm[PWM].t_on - t))inst->maxstep = inst->pwm[PWM].t_on - t;\
                        if(t < inst->pwm[PWM].t_off && inst->maxstep > (inst->pwm[PWM].t_off - t))inst->maxstep = inst->pwm[PWM].t_off - t;\
                        if(t < inst->pwm[PWM].t_dly && inst->maxstep > (inst->pwm[PWM].t_dly - t))inst->maxstep = inst->pwm[PWM].t_dly - t;

extern "C" __declspec(dllexport) void buck(struct sDGTL_CTRL_BLK **opaque, double t, union uData *data)
{
   if(*ForKeeps == 0)
   {
      return;
   }

   #include "io_def.inc"

   if(!*opaque)
   {
      *opaque = (struct sDGTL_CTRL_BLK *) malloc(sizeof(struct sDGTL_CTRL_BLK));
      bzero(*opaque, sizeof(struct sDGTL_CTRL_BLK));

      // initialize the C-block parameters

      (*opaque)->counter = -1;
      (*opaque)->pwm[0].prd = TBPRD;
      (*opaque)->pwm[0].dt = DTIME;
   }

   bool stepped = 0;
   struct sDGTL_CTRL_BLK *inst = *opaque;

   // pwm timing handling
   pwm_edge(&inst->pwm[0], &stepped);

   if(inst->tprev <= inst->tsampling && t >= inst->tsampling)
   {
      inst->tsampling += TSAMPLING;

      inst->counter += 1;
      if(inst->counter >= 4) inst->counter = 0;


      switch(inst->counter)
      {
         case 0:
            // carrier valley
            interrupt0(&inst->data, inst->pwm, data);
            break;
         case 1:
            //
            interrupt1(&inst->data, inst->pwm, data);
            break;
         case 2:
            // carrier peak
            interrupt2(&inst->data, inst->pwm, data);
            break;
         case 3:
            interrupt3(&inst->data, inst->pwm, data);
            break;
      }

   }

   // ===================================================
   hi   = inst->pwm[0].outa;
   lo   = inst->pwm[0].outb;
   iref = (double)inst->pwm[0].cmpa;
   dbg0 = 0;
   dbg1 = 0;
   dbg2 = 0;
   // ===================================================

   inst->tprev = t;

   if(stepped) inst->maxstep = DGTL_CLK;
   else
   {
      inst->maxstep = inst->tsampling - t;
      for(int i = 0; i < PWM_CH; i++)
      {
         MAXSTEP(i);
      }
      if(inst->maxstep <= DGTL_CLK / 2) inst->maxstep = DGTL_CLK;
   }
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sDGTL_CTRL_BLK *inst, double t)
{
   return inst->maxstep;
}

extern "C" __declspec(dllexport) void Destroy(struct DGTL_CTRL_BLK *inst)
{
   free(inst);
}
