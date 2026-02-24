
// Automatically generated C++ file on Wed Jun  4 17:10:55 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD loss_meas.cpp kernel32.lib

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

extern "C" __declspec(dllexport) void (*Display)(const char *format, ...)       = 0; // works like printf()
extern "C" __declspec(dllexport) void (*EXIT)(const char *format, ...)          = 0; // print message like printf() but exit(0) afterward
extern "C" __declspec(dllexport) const double *DegreesC                         = 0; // pointer to current circuit temperature
extern "C" __declspec(dllexport) const int *StepNumber                          = 0; // pointer to current step number
extern "C" __declspec(dllexport) const int *NumberSteps                         = 0; // pointer to estimated number of steps
extern "C" __declspec(dllexport) const char* const *InstanceName                = 0; // pointer to address of instance name
extern "C" __declspec(dllexport) const char *QUX                                = 0; // path to QUX.exe
extern "C" __declspec(dllexport) const bool *ForKeeps                           = 0; // pointer to whether being evaluated non-hypothetically
extern "C" __declspec(dllexport) const bool *HoldICs                            = 0; // pointer to whether instance initial conditions are being held
extern "C" __declspec(dllexport) const void *GUI_HWND                           = 0; // pointer to Window handle of QUX.exe
extern "C" __declspec(dllexport) const double *CKTtime                          = 0;
extern "C" __declspec(dllexport) const double *CKTdelta                         = 0;
extern "C" __declspec(dllexport) const int *IntegrationOrder                    = 0;
extern "C" __declspec(dllexport) const char *InstallDirectory                   = 0;
extern "C" __declspec(dllexport) double (*EngAtof)(const char **string)         = 0;
extern "C" __declspec(dllexport) const char *(*BinaryFormat)(unsigned int data)                          = 0; // BinaryFormat(0x1C) returns "0b00011100"
extern "C" __declspec(dllexport) const char *(*EngFormat   )(double x, const char *units, int numDgts)   = 0; // EngFormat(1e-6, "s", 6) returns "1�s"
extern "C" __declspec(dllexport) int (*DFFT)(struct sComplex *u, bool inv, unsigned int N, double scale) = 0; // Discrete Fast Fourier Transform
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
#undef gate
#undef vds
#undef id
#undef von
#undef ion
#undef voff
#undef ioff
#undef xxx

struct LUT
{
   double Rdson;
   double Ton;
   double Toff;
   double Rg_test;
   double Rg_int;

   int size_Vds_Eoss;
   int size_Isd_Vsd;
   int size_Tj_Rds_factor;

   double Vds[16];   double Eoss[16];
   double Isd[16];   double Vsd[16];
   double Tj[16];    double Rds_factor[16];
};

struct sLOSS_MEAS
{
   // declare the structure here
      double t_prev;
      double t_marker;
      double period;

   // losses table storage variables
      struct LUT lut;

   // Vgs, Vds, Id storage for event detection
      bool vgs_p;
      double vds_p;
      double id_p;

      double von;
      double ion;
      double voff;
      double ioff;

   // conduction loss integration variables
      double rcond_int;
      double vfcond_int;

   // loss calculation results
      double Eon;
      double Eoss;
      double Eoff;


   double pon;
   double poff;
   double pcond;
   double pdt;
   double pcoss;
   double pdevice;
   double ptotal;
};

// if absolute drain current less than this limit, consider losses are negligible and skip the loss calculation.
#define MIN_CURRENT_LIMIT  1e-3
#define MINSTEP_LOCK
// #define DEBUG
extern "C" __declspec(dllexport) void loss_meas(struct sLOSS_MEAS **opaque, double t, union uData *data)
{
   bool         vgs          = data[ 0].b  ; // input
   double       vds          = data[ 1].d  ; // input
   double       id           = data[ 2].d  ; // input
   double       Tj           = data[ 3].d  ; // input
   double       rgon         = data[ 4].d  ; // input parameter
   double       rgoff        = data[ 5].d  ; // input parameter
   const char * fname        = data[ 6].str; // input parameter
   double       max_prd      = data[ 7].d  ; // input parameter
   double       Npara        = data[ 8].d  ; // input parameter
   double       Rds_k        = data[ 9].d  ; // input parameter
   double       Pon_k        = data[10].d  ; // input parameter
   double       Poff_k       = data[11].d  ; // input parameter
   double       Pcoss_k      = data[12].d  ; // input parameter
   double       Pdt_k        = data[13].d  ; // input parameter
   double      &pon          = data[14].d  ; // output
   double      &poff         = data[15].d  ; // output
   double      &pcond        = data[16].d  ; // output
   double      &pdt          = data[17].d  ; // output
   double      &pcoss        = data[18].d  ; // output
   double      &pdevice      = data[19].d  ; // output
   double      &ptotal       = data[20].d  ; // output
   #ifdef DEBUG
   bool        &dbg1         = data[21].b  ; // output
   double      &dbg2         = data[22].d  ; // output
   #endif
   if(!*opaque)
   {
      *opaque = (struct sLOSS_MEAS *) malloc(sizeof(struct sLOSS_MEAS));
      bzero(*opaque, sizeof(struct sLOSS_MEAS));

      struct sLOSS_MEAS *inst = *opaque;

      // 1. Read JSON file
      FILE *file = fopen("mosfet_loss_model.json", "r");
      if (!file)
      {
         EXIT("error with model_file.json");
      }

      fseek(file, 0, SEEK_END);
      long length = ftell(file);
      rewind(file);

      char *json_data = (char*)malloc(length + 1);
      if (!json_data)
      {
         EXIT("Memory allocation failed");
      }

      fread(json_data, 1, length, file);
      json_data[length] = '\0';
      fclose(file);

      // 2. Parse JSON
      cJSON *root = cJSON_Parse(json_data);
      if (!root)
      {
         free(json_data);
         EXIT("Error parsing JSON");
      }

      // 3. Store the JSON data into local pointers
      cJSON *model_param   = cJSON_GetObjectItem(root, fname);
      // non-Array
         cJSON *Rdson_ptr        = cJSON_GetObjectItem(model_param, "Rdson");
         cJSON *Ton_ptr          = cJSON_GetObjectItem(model_param, "Ton");
         cJSON *Toff_ptr         = cJSON_GetObjectItem(model_param, "Toff");
         cJSON *Rg_test_ptr      = cJSON_GetObjectItem(model_param, "Rg_test");
         cJSON *Rg_int_ptr       = cJSON_GetObjectItem(model_param, "Rg_int");
      // Array 
         cJSON *Vds_ptr          = cJSON_GetObjectItem(model_param, "Vds");
         cJSON *Eoss_ptr         = cJSON_GetObjectItem(model_param, "Eoss");
         cJSON *Isd_ptr          = cJSON_GetObjectItem(model_param, "Isd");
         cJSON *Vsd_ptr          = cJSON_GetObjectItem(model_param, "Vsd");
         cJSON *Tj_ptr           = cJSON_GetObjectItem(model_param, "Tj");
         cJSON *Rds_factor_ptr   = cJSON_GetObjectItem(model_param, "Rds_factor");

      // 4. Extract non-Array values
      inst->lut.Rdson   = Rdson_ptr->valuedouble;
      inst->lut.Ton     = Ton_ptr->valuedouble;
      inst->lut.Toff    = Toff_ptr->valuedouble;
      inst->lut.Rg_test = Rg_test_ptr->valuedouble;
      inst->lut.Rg_int  = Rg_int_ptr->valuedouble;

      inst->lut.size_Vds_Eoss = 0;
      inst->lut.size_Isd_Vsd = 0;
      inst->lut.size_Tj_Rds_factor = 0;

      // 5. Extract Array values
      inst->lut.size_Vds_Eoss = cJSON_GetArraySize(Vds_ptr);
      for (int i = 0; i < inst->lut.size_Vds_Eoss; i++)
      {
         cJSON *element1 = cJSON_GetArrayItem(Vds_ptr, i);
         cJSON *element2 = cJSON_GetArrayItem(Eoss_ptr, i);
         inst->lut.Vds[i]         = element1->valuedouble;
         inst->lut.Eoss[i]        = element2->valuedouble;
      }
      inst->lut.size_Isd_Vsd = cJSON_GetArraySize(Isd_ptr);
      for (int i = 0; i < inst->lut.size_Isd_Vsd; i++)
      {
         cJSON *element3 = cJSON_GetArrayItem(Isd_ptr, i);
         cJSON *element4 = cJSON_GetArrayItem(Vsd_ptr, i);
         inst->lut.Isd[i]         = element3->valuedouble;
         inst->lut.Vsd[i]         = element4->valuedouble;
      }
      inst->lut.size_Tj_Rds_factor = cJSON_GetArraySize(Tj_ptr);
      for (int i = 0; i < inst->lut.size_Tj_Rds_factor; i++)
      {
         cJSON *element5 = cJSON_GetArrayItem(Tj_ptr, i);
         cJSON *element6 = cJSON_GetArrayItem(Rds_factor_ptr, i);
         inst->lut.Tj[i]          = element5->valuedouble;
         inst->lut.Rds_factor[i]  = element6->valuedouble;
      }
      // 7. Cleanup
      cJSON_Delete(root);
      free(json_data);

      /* print out the json parsing result for debugging
      
      Display("double Rdson = %f\n", inst->lut.Rdson);
      Display("double Ton = %f\n", inst->lut.Ton);
      Display("double Toff = %f\n", inst->lut.Toff);
      Display("double Rg_test = %f\n", inst->lut.Rg_test);
      Display("double Rg_int = %f\n", inst->lut.Rg_int);

      Display("size_Vds_Eoss = %d\n", inst->lut.size_Vds_Eoss);
      Display("size_Isd_Vsd = %d\n", inst->lut.size_Isd_Vsd);
      Display("size_Tj_Rds_factor = %d\n", inst->lut.size_Tj_Rds_factor);

      Display("double Vds[] = {");
      for (int i = 0; i < inst->lut.size_Vds_Eoss; i++)
      {
         Display("%f ", inst->lut.Vds[i]);
      }
      Display("}\n");
      Display("double Eoss[] = {");
      for (int i = 0; i < inst->lut.size_Vds_Eoss; i++)
      {
         Display("%f ", inst->lut.Eoss[i]);
      }
      Display("}\n");
      
      Display("double Isd[] = {");
      for (int i = 0; i < inst->lut.size_Isd_Vsd; i++)
      { 
         Display("%f ", inst->lut.Isd[i]);
      }
      Display("}\n");
      Display("double Vsd[] = {");
      for (int i = 0; i < inst->lut.size_Isd_Vsd; i++) 
      {
         Display("%f ", inst->lut.Vsd[i]);
      }
      Display("}\n");

      Display("double Tj[] = {");
      for (int i = 0; i < inst->lut.size_Tj_Rds_factor; i++)
      {
         Display("%f ", inst->lut.Tj[i]);
      }
      Display("}\n");
      Display("double Rds_factor[] = {");
      for (int i = 0; i < inst->lut.size_Tj_Rds_factor; i++)
      {
         Display("%f ", inst->lut.Rds_factor[i]);
      }
      Display("}\n");
      */
   }
   struct sLOSS_MEAS *inst = *opaque;

   /*
   xrdson = MOSFET nominal RDSON
   */


// Implement module evaluation code here:
   #ifdef MINSTEP_LOCK
   // to avoid missed switching detection when time step is too small
   // or non-physical (detected as negative time step due to either numerical or Qspice issues)
   if(t - inst->t_prev > 1E-12)
   {
   #endif

   #ifdef DEBUG
   dbg1 = vgs;
   dbg2 = t - inst->t_prev;
   #endif
      // update the loss integration and event detection only when time advances

      double id_single = id / Npara; // current per device
      double id_p_single = inst->id_p / Npara; // previous current per device

      // Conduction loss integration update
      if(vgs)
      {
         // for the conduction loss integral where the device VGS is ON (resistive loss)

         double rdson_k;

         if(Tj >= inst->lut.Tj[inst->lut.size_Tj_Rds_factor - 1])
         {
            rdson_k = inst->lut.Rds_factor[inst->lut.size_Tj_Rds_factor - 1];
         }
         else
         {
         for(int i = 0; i < inst->lut.size_Tj_Rds_factor; i++)
         {
            if((inst->lut.Tj[i] <= Tj) && (Tj <= inst->lut.Tj[i + 1]))
            {
               rdson_k = inst->lut.Rds_factor[i] +
                  (inst->lut.Rds_factor[i + 1] - inst->lut.Rds_factor[i]) *
                  (Tj - inst->lut.Tj[i]) / (inst->lut.Tj[i + 1] - inst->lut.Tj[i]);
               break;
            }
         }
         }

         inst->rcond_int += inst->lut.Rdson * rdson_k *
               (id_single * id_single + id_p_single * id_p_single) *
               (t - inst->t_prev) * 0.5;
      }
      else
      {
         // for the conduction loss integral where the device VGS is OFF (body diode forward conduction loss)
         double vsd;

         if(id_single < -MIN_CURRENT_LIMIT && id_p_single < -MIN_CURRENT_LIMIT)
         {
            if(- id_single >= inst->lut.Isd[inst->lut.size_Isd_Vsd - 1])
            {
               vsd = inst->lut.Vsd[inst->lut.size_Isd_Vsd - 1];
            }
            else
            {
               for(int i = 0; i < inst->lut.size_Isd_Vsd; i++)
               {
                  if((inst->lut.Isd[i] <= - id_single) && (- id_single <= inst->lut.Isd[i + 1]))
                  {
                     vsd = inst->lut.Vsd[i] +
                           (inst->lut.Vsd[i + 1] - inst->lut.Vsd[i]) *
                           (- id_single - inst->lut.Isd[i]) /
                           (inst->lut.Isd[i + 1] - inst->lut.Isd[i]);
                     break;
                  }
               }
            }

            inst->vfcond_int += vsd * (-id_single - id_p_single) * (t - inst->t_prev) * 0.5;
         }
      }


      // Turn on event
      if((vgs == 1) && (inst->vgs_p == 0))
      {
         // Eon calculation (MOSFET turn on energy + Eoss energy)
         if(id_single >= MIN_CURRENT_LIMIT)
         {
            // Compute turn on Loss energy
            inst->Eon = 0.5 * inst->lut.Ton * inst->vds_p * id_single *
                           ((rgon + inst->lut.Rg_int) / (inst->lut.Rg_test + inst->lut.Rg_int)) *
                           Pon_k;

            // Compute turn on capacitive loss energy
            //    if Vds is higher than the highest Vds point in the table,
            //    use the Eoss value at the highest Vds point as approximation
            if(inst->vds_p >= inst->lut.Vds[inst->lut.size_Vds_Eoss - 1])
               inst->Eoss = inst->lut.Eoss[inst->lut.size_Vds_Eoss - 1];
            else
            {
               for(int i = 0; i < inst->lut.size_Vds_Eoss; i++)
               {
                  if((inst->lut.Vds[i] <= inst->vds_p) && (inst->vds_p <= inst->lut.Vds[i + 1]))
                  {
                     inst->Eoss = inst->lut.Eoss[i] + (inst->lut.Eoss[i + 1] - inst->lut.Eoss[i]) *
                           (inst->vds_p - inst->lut.Vds[i]) / (inst->lut.Vds[i + 1] - inst->lut.Vds[i]);
                     break;
                  }
               }
            }
            inst->Eoss = inst->Eoss * Pcoss_k;
         }
         else
         {
            inst->Eon = 0;
            inst->Eoss = 0;
         }
      }

      // Turn off event
      if((vgs == 0) && (inst->vgs_p == 1))
      {
         // Eoff calculation (MOSFET turn off energy)
         if(id_p_single >= MIN_CURRENT_LIMIT)
         {
            // Compute turn off Loss energy
            inst->Eoff = 1. / 6. * inst->lut.Toff * vds * id_p_single *
                           ((rgoff + inst->lut.Rg_int) / (inst->lut.Rg_test + inst->lut.Rg_int)) *
                           Poff_k;
         }
         else
         {
            inst->Eoff = 0;
         }
      }

      // update Power loss calculation
      if(((t - inst->t_marker) >= max_prd) || ((vgs == 0) && (inst->vgs_p == 1)))
      {
         // update all the losses measurement
         inst->period = t - inst->t_marker;
         inst->t_marker = t;

         inst->pon   = inst->Eon / inst->period;
         inst->poff  = inst->Eoff / inst->period;
         inst->pcond = inst->rcond_int / inst->period;
         inst->pdt   = inst->vfcond_int / inst->period;
         inst->pcoss = inst->Eoss / inst->period;

         inst->pdevice = (inst->pon + inst->poff + inst->pcond + inst->pdt + inst->pcoss);
         inst->ptotal  = Npara * inst->pdevice;

         inst->rcond_int = 0;
         inst->vfcond_int = 0;
      }

      pon   = inst->pon;
      poff  = inst->poff;
      pcond = inst->pcond;
      pdt   = inst->pdt;
      pcoss = inst->pcoss;
      pdevice = inst->pdevice;
      ptotal  = inst->ptotal;

      inst->vgs_p = vgs;
      inst->vds_p = vds;
      inst->id_p = id;

      inst->t_prev = t;
   #ifdef MINSTEP_LOCK
   }
   #endif
}

extern "C" __declspec(dllexport) void Destroy(struct sLOSS_MEAS *inst)
{
   free(inst);
}
