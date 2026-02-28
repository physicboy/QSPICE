/*
 * Copyright 2026 Arief Noor Rahman - Power Control Design
 * 
 * Project  : Basic 3ph SPWM
 * Filename : inv_controller.cpp
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

// Automatically generated C++ file on Fri May  2 09:51:57 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD inv_controller.cpp kernel32.lib

#include <malloc.h>
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

void bzero(void *ptr, unsigned int count)
{
   unsigned char *first = (unsigned char *) ptr;
   unsigned char *last  = first + count;
   while(first < last)
      *first++ = '\0';
}

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef p1a
#undef p1b
#undef p2a
#undef p2b
#undef p3a
#undef p3b
#undef CLK

struct pwm_var
{
   double t_prev;

   double ttol_var;
   int    state_change;

   double mcu_trig;
   int    mcu_trig_count;

   double maxstep;

   double p1on;
   double p2on;
   double p3on;

   double p1off;
   double p2off;
   double p3off;

   double p1out;
   double p2out;
   double p3out;

   double p1dly;
   double p2dly;
   double p3dly;

   double p1dt;
   double p2dt;
   double p3dt;
};

struct sINV_CONTROLLER
{
   // declare the structure here

   struct pwm_var pwm_data;
};

#define TTOL_VAL        10E-9
#define PWM_PEAK        4166
#define MCU_CLK_PRD     1E-8
#define SAMPLING_PRD    2*PWM_PEAK*MCU_CLK_PRD

#define PWM_A_ON_OFF_CALC(OFF,ON, TNOW, DUTY, PRD_PEAK, PRD_VAL, CLK_PRD_VAL)   \
   if(DUTY <= 0.5){OFF = TNOW; ON = TNOW + 1;}                \
   else if(DUTY >= (PRD_PEAK - 0.5)){ON = TNOW + PRD_VAL/2; OFF = TNOW + 1;}  \
   else{OFF = TNOW + DUTY*CLK_PRD_VAL; ON = TNOW + PRD_VAL - DUTY*CLK_PRD_VAL;}
        /* OFF will be triggered on next period, ON wont be triggered */
        /* ON will be triggered on half period, OFF wont be triggered */
        /* Normal duty timing */

#define PWM_B_ON_OFF_CALC(OFF,ON, TNOW, DUTY, PRD_PEAK, PRD_VAL, CLK_PRD_VAL)   \
   if(DUTY <= 0.5){ON = TNOW; OFF = TNOW + 1;}                \
   else if(DUTY >= (PRD_PEAK - 0.5)){OFF = TNOW + PRD_VAL/2; ON = TNOW + 1;}  \
   else{OFF = TNOW + PRD_VAL - DUTY*CLK_PRD_VAL; ON = TNOW + DUTY*CLK_PRD_VAL;}
        /* ON will be triggered on next period, OFF wont be triggered */
        /* OFF will be triggered on half period, ON wont be triggered */
        /* Normal duty timing with reverted logic*/

#define PWM_C_ON_OFF_CALC(OFF, ON, TNOW, CMD_OFF, CMD_ON, CLK_PRD_VAL) \
   OFF = TNOW + CMD_OFF*CLK_PRD_VAL;                                 \
   ON  = TNOW + CMD_ON*CLK_PRD_VAL;

#define TRIGGER(TPREV,TNOW,TRIG_ON, TRIG_OFF,STATE_VAR)        \
   if((TPREV <= TRIG_ON)&&(TNOW >= TRIG_ON)) STATE_VAR = 1;    \
   if((TPREV <= TRIG_OFF)&&(TNOW >= TRIG_OFF)) STATE_VAR = 0;

#define DEADTIME(STATE_VAR_PREV, STATE_VAR, TPREV, TNOW, DELAY, DT, DELAY_VAR)  \
   if(STATE_VAR_PREV != STATE_VAR) DELAY = TNOW + DT;                     \
   if((TPREV <= DELAY)&&(TNOW >= DELAY)) DELAY_VAR = STATE_VAR;

#define PWM_OUTPUT(OUTA, OUTB, STATE_VAR, DELAY_VAR, TEMP, STATE_CHANGE, EN)    \
   TEMP = OUTA;                                             \
   OUTA = STATE_VAR*DELAY_VAR*EN;                           \
   if(TEMP != OUTA) STATE_CHANGE = 1;                       \
   TEMP = OUTB;                                             \
   OUTB = (1 - STATE_VAR)*(1 - DELAY_VAR)*EN;               \
   if(TEMP != OUTB) STATE_CHANGE = 1;


extern "C" __declspec(dllexport) void inv_controller(struct sINV_CONTROLLER **opaque, double t, union uData *data)
{
   double dtime = data[0].d;
   double &p1a  = data[1].d; // output
   double &p1b  = data[2].d; // output
   double &p2a  = data[3].d; // output
   double &p2b  = data[4].d; // output
   double &p3a  = data[5].d; // output
   double &p3b  = data[6].d; // output
   double &CLK  = data[7].d; // output

   if(!*opaque)
   {
      *opaque = (struct sINV_CONTROLLER *) malloc(sizeof(struct sINV_CONTROLLER));
      bzero(*opaque, sizeof(struct sINV_CONTROLLER));
   }
   struct sINV_CONTROLLER *inst = *opaque;

// Implement module evaluation code here:
   inst->pwm_data.state_change = 0;

   if((t >= inst->pwm_data.mcu_trig)&&(inst->pwm_data.t_prev <= inst->pwm_data.mcu_trig))
   {
      if(inst->pwm_data.mcu_trig_count >= 3)
      {
         inst->pwm_data.mcu_trig_count = 0;

         // calculate future discrete time PWM discontinuity event
         double duty1 = 4166/2 + 2000*sin(2*3.141592*50*t);
         double duty2 = 4166/2 + 2000*sin(2*3.141592*50*t + 2*3.141592/3);
         double duty3 = 4166/2 + 2000*sin(2*3.141592*50*t + 4*3.141592/3);

         PWM_A_ON_OFF_CALC(inst->pwm_data.p1off, inst->pwm_data.p1on, t, duty1, PWM_PEAK, SAMPLING_PRD, MCU_CLK_PRD)
         PWM_A_ON_OFF_CALC(inst->pwm_data.p2off, inst->pwm_data.p2on, t, duty2, PWM_PEAK, SAMPLING_PRD, MCU_CLK_PRD)
         PWM_A_ON_OFF_CALC(inst->pwm_data.p3off, inst->pwm_data.p3on, t, duty3, PWM_PEAK, SAMPLING_PRD, MCU_CLK_PRD)
      }
      else
      {
         if(inst->pwm_data.mcu_trig_count >= 2)
         {

         }
         else
         {
            if(inst->pwm_data.mcu_trig_count >= 1)
            {

            }
            else
            {

            }
         }
         inst->pwm_data.mcu_trig_count += 1;
      }

      inst->pwm_data.mcu_trig += SAMPLING_PRD/4;
   }

   double p1out_prev = inst->pwm_data.p1out;
   double p2out_prev = inst->pwm_data.p2out;
   double p3out_prev = inst->pwm_data.p3out;

   double p1dt_prev = inst->pwm_data.p1dt;
   double p2dt_prev = inst->pwm_data.p2dt;
   double p3dt_prev = inst->pwm_data.p3dt;

   TRIGGER(inst->pwm_data.t_prev, t, inst->pwm_data.p1on, inst->pwm_data.p1off,inst->pwm_data.p1out)
   TRIGGER(inst->pwm_data.t_prev, t, inst->pwm_data.p2on, inst->pwm_data.p2off,inst->pwm_data.p2out)
   TRIGGER(inst->pwm_data.t_prev, t, inst->pwm_data.p3on, inst->pwm_data.p3off,inst->pwm_data.p3out)

   DEADTIME(p1out_prev, inst->pwm_data.p1out, inst->pwm_data.t_prev, t, inst->pwm_data.p1dly, dtime, inst->pwm_data.p1dt)
   DEADTIME(p2out_prev, inst->pwm_data.p2out, inst->pwm_data.t_prev, t, inst->pwm_data.p2dly, dtime, inst->pwm_data.p2dt)
   DEADTIME(p3out_prev, inst->pwm_data.p3out, inst->pwm_data.t_prev, t, inst->pwm_data.p3dly, dtime, inst->pwm_data.p3dt)

   double temp = 0;
   PWM_OUTPUT(p1a, p1b, inst->pwm_data.p1out, inst->pwm_data.p1dt, temp, inst->pwm_data.state_change, 1)
   PWM_OUTPUT(p2a, p2b, inst->pwm_data.p2out, inst->pwm_data.p2dt, temp, inst->pwm_data.state_change, 1)
   PWM_OUTPUT(p3a, p3b, inst->pwm_data.p3out, inst->pwm_data.p3dt, temp, inst->pwm_data.state_change, 1)

   inst->pwm_data.t_prev = t;

   CLK = inst->pwm_data.mcu_trig_count;
}

#define MAXSTEP_CALC(X) if(inst->pwm_data.t_prev < X){if(maxstep > (X - inst->pwm_data.t_prev)){maxstep = X - inst->pwm_data.t_prev;}}
extern "C" __declspec(dllexport) double MaxExtStepSize(struct sINV_CONTROLLER *inst, double t)
{
   double maxstep = 0;
   if(inst->pwm_data.state_change)
   {
      maxstep = TTOL_VAL;
   }
   else
   {
      maxstep = SAMPLING_PRD;

      MAXSTEP_CALC(inst->pwm_data.mcu_trig)

      MAXSTEP_CALC(inst->pwm_data.p1on)
      MAXSTEP_CALC(inst->pwm_data.p1off)
      MAXSTEP_CALC(inst->pwm_data.p1dly)
      MAXSTEP_CALC(inst->pwm_data.p2on)
      MAXSTEP_CALC(inst->pwm_data.p2off)
      MAXSTEP_CALC(inst->pwm_data.p2dly)
      MAXSTEP_CALC(inst->pwm_data.p3on)
      MAXSTEP_CALC(inst->pwm_data.p3off)
      MAXSTEP_CALC(inst->pwm_data.p3dly)
   }
   return maxstep;
}

/*
extern "C" __declspec(dllexport) void Trunc(struct sINV_CONTROLLER *inst, double t, union uData *data, double *timestep)
{
   if(inst->pwm_data.state_change)*timestep = TTOL_VAL;
}
*/

extern "C" __declspec(dllexport) void Destroy(struct sINV_CONTROLLER *inst)
{
   free(inst);
}
