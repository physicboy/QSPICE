#ifndef _PFC_H
#define _PFC_H

#include "__pfc.c"

extern struct pfc_var;
extern void pfc_init(struct pfc_var *a, float *vdc);
extern void pfc_calc(struct pfc_var *a, struct pll_var *b,
    float vdc_ref, float *vdc, float *ia, float *ib, float *ic);

#endif