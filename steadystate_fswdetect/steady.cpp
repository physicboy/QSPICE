// Automatically generated C++ file on Tue Oct 15 13:37:09 2024
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD test_x2.cpp kernel32.lib

#include <malloc.h>
#include <math.h>

extern "C" __declspec(dllexport) int (*Display)(const char *format, ...) = 0; // works like printf()
extern "C" __declspec(dllexport) const double *DegreesC                  = 0; // pointer to current circuit temperature
extern "C" __declspec(dllexport) const int *StepNumber                   = 0; // pointer to current step number
extern "C" __declspec(dllexport) const int *NumberSteps                  = 0; // pointer to estimated number of steps
extern "C" __declspec(dllexport) const char **InstanceName               = 0; // pointer to address of instance name
extern "C" __declspec(dllexport) const char *QUX                         = 0; // path to QUX.exe
extern "C" __declspec(dllexport) int (*DFFT)(struct sComplex *u, bool inv, unsigned int N, double scale) = 0;

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
#undef sw_trig
#undef sense

#define MOVING_AVG_LENGTH     100
#define STEADY_CTR_LIM        20
#define ERROR_LIM             1e-3

struct sSTEADY
{
  // declare the structure here
  double trig_time;
  double sense_val[100];
  double sense_avg;
  int counter;
  int counter_ovf;
  int steady_counter;
  double sense_prev;
  double trig_prev;
  double max_step;

  double error;
};

extern "C" __declspec(dllexport) void steady(struct sSTEADY **opaque, double t, union uData *data)
{
   double sw_trig    = data[0].d; // input
   double sense      = data[1].d; // input
   bool   trig_edge  = data[2].b; // input parameter
   double tmax       = data[3].d; // input parameter
   double &out1      = data[4].d; // output
   double &out2      = data[5].d; // output
   double &out3      = data[6].d; // output

   if(!*opaque)
   {
      *opaque = (struct sSTEADY *) malloc(sizeof(struct sSTEADY));
      bzero(*opaque, sizeof(struct sSTEADY));
   }
   struct sSTEADY *inst = *opaque;

// Implement module evaluation code here:
   bool trigger;

   if(!trig_edge)trigger = sw_trig > inst->trig_prev;
   else trigger = sw_trig < inst->trig_prev;

   if(trigger)
   {
      inst->sense_avg = inst->sense_avg + (sense - inst->sense_val[inst->counter])/MOVING_AVG_LENGTH;
      inst->sense_val[inst->counter] = sense;

      if(inst->counter_ovf)
      {
         inst->error = fabs(inst->sense_avg - sense);
         if((inst->error < ERROR_LIM) || (t > tmax))
         {
            if(inst->steady_counter == 0)inst->trig_time = t;

            inst->steady_counter++;
            if(inst->steady_counter > STEADY_CTR_LIM)
            {
               if(out3 == 0)
               {
                  Display("sampling = %e",(t - inst->trig_time)/STEADY_CTR_LIM);
                  out2 = (t - inst->trig_time)/STEADY_CTR_LIM;
               }
               out3 = 1;
            }
         }
         else
         {
            inst->steady_counter = 0;
         }
      }

      inst->counter++;
      if(inst->counter >= MOVING_AVG_LENGTH)inst->counter = 0;inst->counter_ovf = 1;
   }

   inst->sense_prev = sense;
   inst->trig_prev = sw_trig;

   out1 = inst->sense_avg;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sSTEADY *inst)
{
   return inst->max_step;
}
extern "C" __declspec(dllexport) void Destroy(struct sSTEADY *inst)
{
   free(inst);
}
