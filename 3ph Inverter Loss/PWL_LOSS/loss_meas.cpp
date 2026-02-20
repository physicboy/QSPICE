
// Automatically generated C++ file on Wed Jun  4 17:10:55 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD loss_meas.cpp kernel32.lib

#include <malloc.h>
#include <stdio.h>

extern "C" __declspec(dllexport) int (*Display)(const char *format, ...) = 0; // works like printf()

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
#undef gate
#undef vds
#undef id
#undef von
#undef ion
#undef voff
#undef ioff
#undef xxx

struct sLOSS_MEAS
{
   // declare the structure here
      double t_prev;
      double t_marker;
      double period;

   // losses table storage variables
      double xrdson;
      double xsw_ton;
      double xsw_toff;
      double xrg_nom;
      double xrg_int;
      double xvds[6];
      double xeoss[6];
      double xid[6];
      double xvsd[6];
      double xtj[6];
      double xr_tj[6];

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

      FILE *file = fopen(fname, "r");
      float table[7][6];
      for (int i = 0; i < 7; ++i)
      {
         for (int j = 0; j < 6; ++j)
         {
            fscanf(file, "%f", &table[i][j]);
         }
      }

      fclose(file);

      struct sLOSS_MEAS *inst = *opaque;

      inst->xrdson   = table[0][0] * Rds_k;
      inst->xsw_ton  = table[0][1];
      inst->xsw_toff = table[0][2];
      inst->xrg_nom  = table[0][3];
      inst->xrg_int  = table[0][4];

      for (int i = 0; i < 6; ++i)
      {
         inst->xvds[i]  = table[1][i];
         inst->xeoss[i] = table[2][i];
         inst->xid[i]   = table[3][i];
         inst->xvsd[i]  = table[4][i];
         inst->xtj[i]   = table[5][i];
         inst->xr_tj[i] = table[6][i];
      }

      /*
      for (int i = 0; i < 7; ++i)
      {
         for (int j = 0; j < 6; ++j)
         {
            Display("%.3e\t", table[i][j]);
         }
         Display("\n");
      }
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

         if(Tj >= inst->xtj[5])
         {
         rdson_k = inst->xr_tj[5];
         }
         else
         {
         for(int i = 0; i < 5; i++)
         {
            if((inst->xtj[i] <= Tj) && (Tj <= inst->xtj[i + 1]))
            {
               rdson_k = inst->xr_tj[i] + (inst->xr_tj[i + 1] - inst->xr_tj[i]) * (Tj - inst->xtj[i]) / (inst->xtj[i + 1] - inst->xtj[i]);
               break;
            }
         }
         }

         inst->rcond_int += inst->xrdson * rdson_k * (id_single * id_single + id_p_single * id_p_single) * (t - inst->t_prev) * 0.5;
      }
      else
      {
         // for the conduction loss integral where the device VGS is OFF (body diode forward conduction loss)
         double vsd;

         if(id_single < -MIN_CURRENT_LIMIT && id_p_single < -MIN_CURRENT_LIMIT)
         {
            if(- id_single >= inst->xid[5])
            {
               vsd = inst->xvsd[5];
            }
            else
            {
               for(int i = 0; i < 5; i++)
               {
                  if((inst->xid[i] <= - id_single) && (- id_single <= inst->xid[i + 1]))
                  {
                     vsd = inst->xvsd[i] + (inst->xvsd[i + 1] - inst->xvsd[i]) * (- id_single - inst->xid[i]) / (inst->xid[i + 1] - inst->xid[i]);
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
            inst->Eon = 0.5 * inst->xsw_ton * inst->vds_p * id_single *
                           ((rgon + inst->xrg_int) / (inst->xrg_nom + inst->xrg_int)) *
                           Pon_k;

            // Compute turn on capacitive loss energy
            //    if Vds is higher than the highest Vds point in the table,
            //    use the Eoss value at the highest Vds point as approximation
            if(inst->vds_p >= inst->xvds[5]) inst->Eoss = inst->xeoss[5];
            else
            {
               for(int i = 0; i < 5; i++)
               {
                  if((inst->xvds[i] <= inst->vds_p) && (inst->vds_p <= inst->xvds[i + 1]))
                  {
                     inst->Eoss = inst->xeoss[i] + (inst->xeoss[i + 1] - inst->xeoss[i]) *
                                    (inst->vds_p - inst->xvds[i]) / (inst->xvds[i + 1] - inst->xvds[i]);
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
            inst->Eoff = 1. / 6. * inst->xsw_toff * vds * id_p_single *
                           ((rgoff + inst->xrg_int) / (inst->xrg_nom + inst->xrg_int)) *
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
