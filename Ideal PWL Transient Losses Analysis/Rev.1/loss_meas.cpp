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

  int vds_p;
  int id_p;
  int gate_p1;
  int gate_p2;
  double prev_toff;

  double von;
  double ion;
  double voff;
  double ioff;

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

  double mult_vi;
  double mult_vi_int;
  double mult_vi_prev;

  double pon;
  double poff;
  double pcond;
  double pdt;
  double pcoss;
  double pdevice;
  double ptotal;
};


extern "C" __declspec(dllexport) void loss_meas(struct sLOSS_MEAS **opaque, double t, union uData *data)
{
   double       vgs           = data[ 0].d  ; // input
   double       vds           = data[ 1].d  ; // input
   double       id            = data[ 2].d  ; // input
   double       Tj            = data[ 3].d  ; // input
   double       rgon          = data[ 4].d  ; // input parameter
   double       rgoff         = data[ 5].d  ; // input parameter
   const char * fname         = data[ 6].str; // input parameter
   double       dtime         = data[ 7].d  ; // input parameter
   double       Npara         = data[ 8].d  ; // input parameter
   double       Rds_factor    = data[ 9].d  ; // input parameter
   double       Pon_factor    = data[10].d  ; // input parameter
   double       Poff_factor   = data[11].d  ; // input parameter
   double       Pcoss_factor  = data[12].d  ; // input parameter
   double       Pdt_factor    = data[13].d  ; // input parameter
   double      &pon           = data[14].d  ; // output
   double      &poff          = data[15].d  ; // output
   double      &pcond         = data[16].d  ; // output
   double      &pdt           = data[17].d  ; // output
   double      &pcoss         = data[18].d  ; // output
   double      &pdevice       = data[19].d  ; // output
   double      &ptotal        = data[20].d  ; // output

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

      inst->xrdson   = table[0][0] * Rds_factor;
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

      for (int i = 0; i < 7; ++i)
      {
         for (int j = 0; j < 6; ++j)
         {
            Display("%.3e\t", table[i][j]);
         }
         Display("\n");
      }
   }
   struct sLOSS_MEAS *inst = *opaque;

// Implement module evaluation code here:
   int gate = vgs > 0.5;

   // conduction loss integral calculation
   inst->mult_vi = inst->xrdson * id/Npara * id/Npara;
   inst->mult_vi_int  += (inst->mult_vi + inst->mult_vi_prev) * (t - inst->t_prev) * 0.5;
   inst->mult_vi_prev = inst->mult_vi;

   // Turn on event
   if(gate > inst->gate_p1)
   {
      inst->von = inst->vds_p;
   }

   if(inst->gate_p1 > inst->gate_p2)
   {
      inst->ion = id/Npara;
   }

   // Turn off event
   if(gate < inst->gate_p1)
   {
      inst->ioff = id/Npara;
   }

   if(inst->gate_p1 < inst->gate_p2)
   {
      inst->voff = vds;

      // update all the losses measurement

      // compute the switching period between two latest turn off event
      double period = t - inst->prev_toff;
      if(period < 1e-9)period = 1e-9;
      inst->prev_toff = t;

      //conduction loss update
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

      rdson_k *= 1;

      inst->pcond = rdson_k * inst->mult_vi_int / period;
      inst->mult_vi_int = 0;

      if(inst->ion >= 0)
      {
         inst->pon = 0.5 * inst->xsw_ton * inst->von * inst->ion / period * ((rgon + inst->xrg_int) / (inst->xrg_nom + inst->xrg_int)) * Pon_factor;

         double eoss;

         if(inst->von >= inst->xvds[5])
         {
            eoss = inst->xeoss[5];
         }
         else
         {
            for(int i = 0; i < 5; i++)
            {
               if((inst->xvds[i] <= inst->von) && (inst->von <= inst->xvds[i + 1]))
               {
                  eoss = inst->xeoss[i] + (inst->xeoss[i + 1] - inst->xeoss[i]) * (inst->von - inst->xvds[i]) / (inst->xvds[i + 1] - inst->xvds[i]);
                  break;
               }
            }
         }

         inst->pcoss = eoss / period * Pcoss_factor;
         inst->pdt = 0;
      }
      else
      {
         inst->pon = 0;
         inst->pcoss = 0;

         double xisd = -1 * inst->ion;
         double vsd;

         if(xisd >= inst->xid[5])
         {
            vsd = inst->xvsd[5];
         }
         else
         {
            for(int i = 0; i < 5; i++)
            {
               if((inst->xid[i] <= xisd) && (xisd <= inst->xid[i + 1]))
               {
                  vsd = inst->xvsd[i] + (inst->xvsd[i + 1] - inst->xvsd[i]) * (xisd - inst->xid[i]) / (inst->xid[i + 1] - inst->xid[i]);
                  break;
               }
            }
         }

         inst->pdt = vsd * xisd * dtime / period  * Pcoss_factor;
      }

      if(inst->ioff >= 0)
      {
         inst->poff = 0.5 * inst->xsw_toff * inst->voff * inst->ioff / period  * ((rgoff + inst->xrg_int) / (inst->xrg_nom + inst->xrg_int)) * Poff_factor;
      }
      else
      {
         inst->poff = 0;

         double xisd = -1 * inst->ioff;
         double vsd;

         if(xisd >= inst->xid[5])
         {
            vsd = inst->xvsd[5];
         }
         else
         {
            for(int i = 0; i < 5; i++)
            {
               if((inst->xid[i] <= xisd) && (xisd <= inst->xid[i + 1]))
               {
                  vsd = inst->xvsd[i] + (inst->xvsd[i + 1] - inst->xvsd[i]) * (xisd - inst->xid[i]) / (inst->xid[i + 1] - inst->xid[i]);
                  break;
               }
            }
         }

         inst->pdt += vsd * xisd * dtime / period  * Pcoss_factor;
      }

      inst->pdevice = (inst->pon + inst->poff + inst->pcond + inst->pdt + inst->pcoss);
      inst->ptotal  = Npara * inst->pdevice;
   }

   pon   = inst->pon;
   poff  = inst->poff;
   pcond = inst->pcond;
   pdt   = inst->pdt;
   pcoss = inst->pcoss;
   pdevice = inst->pdevice;
   ptotal  = inst->ptotal;


   inst->vds_p = vds;
   inst->id_p = id;

   inst->gate_p2 = inst->gate_p1;
   inst->gate_p1 = gate;

   inst->t_prev = t;

}

extern "C" __declspec(dllexport) void Destroy(struct sLOSS_MEAS *inst)
{
   free(inst);
}
