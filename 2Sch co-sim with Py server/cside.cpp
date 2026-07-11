// Automatically generated C++ Berkeley Socket Client on Fri Jul 10 23:52:09 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD -o cside.cpp kernel32.lib

#include <malloc.h>

extern "C" __declspec(dllexport) void (*Display)(const char *format, ...) = 0; // works like printf()
extern "C" __declspec(dllexport) void (*EXIT)(const char *format, ...)    = 0; // print message like printf() but exit(0) afterward
extern "C" __declspec(dllexport) const double *DegreesC                   = 0; // pointer to current circuit temperature

// Berkeley Socket API:
#define PORT_EVALUATE      2
#define PORT_MAX_STEPSIZE  3
#define PORT_TRUNCATE      4
extern "C" __declspec(dllexport) unsigned int (*BerkeleySocket)(const char *hostPortServiceString, int Ninputs, int Noutputs, unsigned char **buffer) = 0;
extern "C" __declspec(dllexport) int  (*SocketSend )(unsigned int socket, unsigned char *buffer, int count) = 0;
extern "C" __declspec(dllexport) int  (*SocketRecv )(unsigned int socket, unsigned char *buffer, int count) = 0;
extern "C" __declspec(dllexport) void (*SocketClose)(unsigned int socket)                                   = 0;
extern "C" __declspec(dllexport) double *(*ConfigureBuffer)(unsigned char *buffer, int code)                = 0;

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

int __stdcall DllMain(void *module, unsigned int reason, void *reserved) { return 1; }

const int Ninputs  = 1;
const int Noutputs = 1;

struct sCSIDE
{
   unsigned int ConnectSocket;
   unsigned char *buffer;

   double time_goal;
   double max_step;
};

extern "C" __declspec(dllexport) void cside(struct sCSIDE **opaque, double t, union uData *data)
{
   double       in1    = data[0].d  ; // input
   const char * server = data[1].str; // input parameter
   double      &out1   = data[2].d  ; // output
   if(!*opaque)
   {
      *opaque = (struct sCSIDE *) calloc(1, sizeof(struct sCSIDE));
      (*opaque)->ConnectSocket = BerkeleySocket(server, Ninputs, Noutputs, &(*opaque)->buffer);
   }
   struct sCSIDE *inst = *opaque;


   if(t >= inst->time_goal)
   {
      double *vector = ConfigureBuffer(inst->buffer, PORT_EVALUATE);
      vector[0]      = t;
      vector[1]      = in1;
      SocketSend(inst->ConnectSocket, inst->buffer, 12 + 8 * Ninputs );


      SocketRecv(inst->ConnectSocket, inst->buffer,      8 * Noutputs);
      inst->time_goal = ((double *) inst->buffer)[0];
   }

   inst->max_step = inst->time_goal - t;
   if(inst->max_step < 5E-9) inst->max_step = 10E-9;

   out1 = in1 * in1;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sCSIDE *inst, double t)
{
   return inst->max_step;
}

extern "C" __declspec(dllexport) void Destroy(struct sCSIDE *inst)
{
   SocketClose(inst->ConnectSocket);
   free(inst->buffer);
   free(inst);
}
