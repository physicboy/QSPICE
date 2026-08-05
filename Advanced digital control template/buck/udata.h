#ifndef UDATA_H
#define UDATA_H

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

extern "C" __declspec(dllexport) void (*Display)(const char *format, ...)       = 0;
extern "C" __declspec(dllexport) void (*EXIT)(const char *format, ...)          = 0;
extern "C" __declspec(dllexport) const double *DegreesC                         = 0;
extern "C" __declspec(dllexport) const int *StepNumber                          = 0;
extern "C" __declspec(dllexport) const int *NumberSteps                         = 0;
extern "C" __declspec(dllexport) const char* const *InstanceName                = 0;
extern "C" __declspec(dllexport) const char *QUX                                = 0;
extern "C" __declspec(dllexport) const bool *ForKeeps                           = 0;
extern "C" __declspec(dllexport) const bool *HoldICs                            = 0;
extern "C" __declspec(dllexport) const void *GUI_HWND                           = 0;
extern "C" __declspec(dllexport) const double *CKTtime                          = 0;
extern "C" __declspec(dllexport) const double *CKTdelta                         = 0;
extern "C" __declspec(dllexport) const int *IntegrationOrder                    = 0;
extern "C" __declspec(dllexport) const char *InstallDirectory                   = 0;
extern "C" __declspec(dllexport) double (*EngAtof)(const char **string)         = 0;
extern "C" __declspec(dllexport) const char *(*BinaryFormat)(unsigned int data) = 0;
extern "C" __declspec(dllexport) const char *(*EngFormat)(double x, const char *units, int numDgts) = 0;
extern "C" __declspec(dllexport) int (*DFFT)(struct sComplex *u, bool inv, unsigned int N, double scale) = 0;
extern "C" __declspec(dllexport) void (*bzero)(void *ptr, unsigned int count)   = 0;

#endif // UDATA_H