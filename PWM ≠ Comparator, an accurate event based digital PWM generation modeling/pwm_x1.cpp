// Automatically generated C++ file on Sun Oct  5 01:58:46 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD pwm_x1.cpp kernel32.lib

#include <malloc.h>
#include <math.h>

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
#undef d1
#undef d2

struct sPWM_X1
{
  // declare the structure here
  double tri_prev;
  double tri_prev2;

};

extern "C" __declspec(dllexport) void pwm_x1(struct sPWM_X1 **opaque, double t, union uData *data)
{
   double  tri = data[0].d; // input
   double &d1  = data[1].d; // output
   double &d2  = data[2].d; // output

   if(!*opaque)
   {
      *opaque = (struct sPWM_X1 *) malloc(sizeof(struct sPWM_X1));
      bzero(*opaque, sizeof(struct sPWM_X1));
   }
   struct sPWM_X1 *inst = *opaque;

// Implement module evaluation code here:
   if((tri >= inst->tri_prev) && (inst->tri_prev2 >= inst->tri_prev))
   {
      double sine = 2000 * sinf(2*M_PI*50*t);

      if(sine >= 0)
      {
         d1 = fabs(sine);
         d2 = 0;
      }
      else
      {
         d1 = fabs(sine + 2500.);
         d2 = 2500.;
      }
   }


   inst->tri_prev2 = inst->tri_prev;
   inst->tri_prev = tri;

}

extern "C" __declspec(dllexport) void Destroy(struct sPWM_X1 *inst)
{
   free(inst);
}
