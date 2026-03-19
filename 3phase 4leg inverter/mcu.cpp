// Automatically generated C++ file on Fri May 16 11:51:12 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD mcu.cpp kernel32.lib

#include <malloc.h>
#include "mcu.h"
#include "_pwm.h"
#include "_adc.h"
#include "_int.h"

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

struct sMCU
{
 // declare the structure here
   pwm_var  pwm_data;
   adc_var  adc_data;
   int_var  int_data;
};

extern "C" __declspec(dllexport) void mcu(struct sMCU **opaque, double t, union uData *data)
{
   if(!*opaque)
   {
      *opaque = (struct sMCU *) malloc(sizeof(struct sMCU));
      bzero(*opaque, sizeof(struct sMCU));
      struct sMCU *inst = *opaque;
      pwm_init(&inst->pwm_data);
   }
   struct sMCU *inst = *opaque;

// Implement module evaluation code here:

	pwm_calc(&inst->pwm_data, t);

   RELAY = (double)inst->int_data.relay;

   inst->pwm_data.p1en = inst->int_data.pwm123_en;
   inst->pwm_data.p2en = inst->int_data.pwm123_en;
   inst->pwm_data.p3en = inst->int_data.pwm123_en;

   inst->pwm_data.p4en = inst->int_data.pwm4_en;

   inst->pwm_data.p5en = inst->int_data.pwm567_en;
   inst->pwm_data.p6en = inst->int_data.pwm567_en;
   inst->pwm_data.p7en = inst->int_data.pwm567_en;

   P1A = inst->pwm_data.p1a;
	P2A = inst->pwm_data.p2a;
	P3A = inst->pwm_data.p3a;
	P4A = inst->pwm_data.p4a;
	P5A = inst->pwm_data.p5a;
	P6A = inst->pwm_data.p6a;
	P7A = inst->pwm_data.p7a;

	P1B = inst->pwm_data.p1b;
	P2B = inst->pwm_data.p2b;
	P3B = inst->pwm_data.p3b;
	P4B = inst->pwm_data.p4b;
	P5B = inst->pwm_data.p5b;
	P6B = inst->pwm_data.p6b;
	P7B = inst->pwm_data.p7b;

   OUT1  = (double)inst->int_data.counter;
   OUT2  = (double)inst->int_data.pll_data.th;
   OUT3  = (double)inst->int_data.pll_data.w;
   OUT4  = (double)inst->int_data.pll_data.vd;
   OUT5  = (double)inst->int_data.pll_data.vq;
   OUT6  = (double)inst->int_data.pfc_data.idcref;
   OUT7  = (double)inst->int_data.pfc_data.idref;
   OUT8  = (double)inst->int_data.pfc_data.ud;
   OUT9  = (double)inst->int_data.pfc_data.uq;
   OUT10 = (double)inst->int_data.pfc_data.id;
   OUT11 = (double)inst->int_data.pfc_data.iq;
   OUT12 = 0;
 
	if(inst->pwm_data.mcu_trig_interrupt == 1)
	{
		// counter == zero
      // actual placement for the MCU interrupt
      adc_meas4(&inst->adc_data, &VDC, &MID, &VA1, &VB1, &VC1, &IA1, &IB1, &IC1, &VA2, &VB2, &VC2, &IA2, &IB2, &IC2);
      int_calc(&inst->int_data, &inst->adc_data, &inst->pwm_data, &(int)ON_OFF);

		inst->pwm_data.mcu_trig_interrupt = 0;
	}

	if(inst->pwm_data.mcu_trig_interrupt == 2)
	{
		// counter == PRD/2 at rising
      adc_meas1(&inst->adc_data, &VDC, &MID, &VA1, &VB1, &VC1, &IA1, &IB1, &IC1, &VA2, &VB2, &VC2, &IA2, &IB2, &IC2);

		inst->pwm_data.mcu_trig_interrupt = 0;
	}

	if(inst->pwm_data.mcu_trig_interrupt == 3)
	{
		// counter == PRD
      adc_meas2(&inst->adc_data, &VDC, &MID, &VA1, &VB1, &VC1, &IA1, &IB1, &IC1, &VA2, &VB2, &VC2, &IA2, &IB2, &IC2);

		inst->pwm_data.mcu_trig_interrupt = 0;
	}

	if(inst->pwm_data.mcu_trig_interrupt == 4)
	{
		// counter == PRD/2 at falling
   adc_meas3(&inst->adc_data, &VDC, &MID, &VA1, &VB1, &VC1, &IA1, &IB1, &IC1, &VA2, &VB2, &VC2, &IA2, &IB2, &IC2);

		inst->pwm_data.mcu_trig_interrupt = 0;
	}
 
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sMCU *inst, double t)
{
 return pwm_maxstep(&inst->pwm_data);
}

extern "C" __declspec(dllexport) void Trunc(struct sMCU *inst, double t, union uData *data, double *timestep)
{
 *timestep = pwm_trunc(&inst->pwm_data);
}

extern "C" __declspec(dllexport) void Destroy(struct sMCU *inst)
{
   free(inst);
}
