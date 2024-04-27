// Automatically generated C++ file on Wed Apr  3 12:20:11 2024
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD vco.cpp kernel32.lib

#include <malloc.h>
#include <math.h>

#define TCLK   25e-9
#define DTIME  300e-9

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
#undef vcap_int
#undef vcap_ic
#undef out1
#undef out2
#undef reset
#undef vcomp

struct sVCO
{
   // declare the structure here
   long long int xcntr;
   double t_prev;

   double vcap_int_prev;
   double vcap_ic_prev;

   double reset;
   double time_reset;

   double treset_on;
   double treset_off;

   double tupper_on;
   double tlower_on;

   double status;
   double status_sampled;
};

extern "C" __declspec(dllexport) void vco(struct sVCO **opaque, double t, union uData *data)
{
   double  vcap_int = data[0].d; // input
   double  vcap_ic  = data[1].d; // input
   double  vcomp    = data[2].d; // input
   double &out1     = data[3].d; // output
   double &out2     = data[4].d; // output
   double &reset    = data[5].d; // output

   if(!*opaque)
   {
      *opaque = (struct sVCO *) malloc(sizeof(struct sVCO));
      bzero(*opaque, sizeof(struct sVCO));
   }
   struct sVCO *inst = *opaque;

// Implement module evaluation code here:
	double vcomp_ref = vcomp*(t > 500e-9);
	// vcomp_ref is forced to zero when simulation time is < 500ns
	// somehow, if vcomp is high at t=0, the VCO willnot work.

	//=================================================================
	// burst mode or normal operation =================================
	//=================================================================
	if(inst->status < 0.5)
	{
		// burst mode
		if(vcomp_ref <= 1.25)
		{
			inst->status = 1;
		}
	}
	else
	{
		if(vcomp_ref >= 1.3)
		{
			if(inst->status > 0.5)
			{
				inst->treset_off = ceil(t/TCLK)*TCLK;
			}
			inst->status = 0;
		}
	}

	if((inst->t_prev <= inst->treset_on) && (t >=inst->treset_on))
	{
		inst->xcntr++;
		out1 = 0;
		inst->reset = 1;
		inst->time_reset = t;
		inst->tlower_on   = inst->treset_on + DTIME;
		inst->treset_off  = 2*inst->treset_on - inst->treset_off;
	}

	if((inst->t_prev <= inst->treset_off) && (t >=inst->treset_off))
	{
		inst->xcntr++;
		out2 = 0;
		
		// burst mode entry is checked before turning on the upper MOSFET
		inst->status_sampled = inst->status;

		if(inst->status_sampled < 0.5)
		{
			// if confirmed to be in normal cycle, then compute the upper MOSFET turn on time and
			inst->reset = 0;
			inst->time_reset = t;
			inst->tupper_on   = inst->treset_off + DTIME;
		}
	}

	//=================================================================
	// Main Switch delayed turn on generation =========================
	//=================================================================
	if((inst->t_prev <= inst->tupper_on) && (t >=inst->tupper_on))
	{
		inst->xcntr++;
		if(out2 < 0.5) out1 = 1 - inst->status_sampled;
	}

	if((inst->t_prev <= inst->tlower_on) && (t >=inst->tlower_on))
	{
		inst->xcntr++;
		if(out1 < 0.5) out2 = 1 - inst->status_sampled;
	}

	//=================================================================
	// VCO timing sampler =============================================
	//=================================================================
	if((vcap_int >= 3) && (inst->reset < 0.5))
	{
		inst->xcntr++;
		inst->treset_on = ceil(t/TCLK)*TCLK;
	}

	if((vcap_ic >= vcomp_ref) && (inst->reset < 0.5))
	{
		inst->xcntr++;
		inst->treset_on = ceil(t/TCLK)*TCLK;
	}

	reset = 1 - out1;

	inst->vcap_int_prev = vcap_int;
	inst->vcap_ic_prev = vcap_ic;
	inst->t_prev = t;

}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sVCO *inst)
{
   return 1e308; // implement a good choice of max timestep size that depends on struct sVCO
}

extern "C" __declspec(dllexport) void Trunc(struct sVCO *inst, double t, union uData *data, double *timestep)
{ // limit the timestep to a tolerance if the circuit causes a change in struct sVCO
   const double ttol = 10e-9;
   double &out1     = data[3].d; // output
   double &out2     = data[4].d; // output
   double &reset    = data[5].d; // output

   // Save output vector
   const double _out1     = out1    ;
   const double _out2     = out2    ;
   const double _reset    = reset   ;

   struct sVCO tmp = *inst;
   vco(&(&tmp), t, data);
   if(inst->xcntr != tmp.xcntr) // implement a meaningful way to detect if the state has changed
   {
      *timestep = ttol;
   }

   // Restore output vector
   out1     = _out1    ;
   out2     = _out2    ;
   reset    = _reset   ;
}

extern "C" __declspec(dllexport) void Destroy(struct sVCO *inst)
{
   free(inst);
}
