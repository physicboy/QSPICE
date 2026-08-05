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
 */

// Automatically generated file for QSPICE Digital Control C-Block

#define TSAMPLING 5.000E-6
#define PWM_CH    1
#define TBPRD     1000
#define DTIME     20
#define DGTL_CLK  1.000E-8

#include <malloc.h>
#include <math.h>
#include "udata.h"
#include "pwm_edge_handler.h"
#include "interrupt_handler.h"

int __stdcall DllMain(void *module, unsigned int reason, void *reserved) { return 1; }

struct sDGTL_CTRL_BLK
{
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
   if(*ForKeeps == 0) return;

   #include "io_def.inc"

   if(!*opaque)
   {
      *opaque = (struct sDGTL_CTRL_BLK *) malloc(sizeof(struct sDGTL_CTRL_BLK));
      bzero(*opaque, sizeof(struct sDGTL_CTRL_BLK));

      (*opaque)->counter = -1;
      for(int i = 0; i < PWM_CH; i++) {
         (*opaque)->pwm[i].prd = TBPRD;
         (*opaque)->pwm[i].dt = DTIME;
      }
   }

   bool stepped = 0;
   struct sDGTL_CTRL_BLK *inst = *opaque;

   // Handle PWM state machine edge transitions (Unwrapped per channel for custom edge handler substitution)
   pwm_edge(&inst->pwm[0], &stepped);

   // Discrete Sampling & Interrupt Dispatch Logic
   if(*CKTtime - *CKTdelta <= inst->tsampling && *CKTtime >= inst->tsampling)
   {
      inst->tsampling += TSAMPLING;

      inst->counter += 1;
      if(inst->counter >= 4) inst->counter = 0;

      switch(inst->counter)
      {
         case 0:
            // Carrier Valley Interrupt
            interrupt0(&inst->data, inst->pwm, data);
            break;
         case 1:
            // Mid-Carrier Upslope (4x Oversampling)
            interrupt1(&inst->data, inst->pwm, data);
            break;
         case 2:
            // Carrier Peak Interrupt
            interrupt2(&inst->data, inst->pwm, data);
            break;
         case 3:
            // Mid-Carrier Downslope (4x Oversampling)
            interrupt3(&inst->data, inst->pwm, data);
            break;
      }
   }

   // Map PWM states and other outputs to available ports, example:
   hi = inst->pwm[0].outa;
   lo = inst->pwm[0].outb;

   // Adaptive Solver Timestep Control across all PWM channels
   if(stepped) inst->maxstep = DGTL_CLK;
   else
   {
      inst->maxstep = inst->tsampling - t;
      for(int i = 0; i < PWM_CH; i++) {
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