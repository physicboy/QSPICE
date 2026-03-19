#ifndef _PLL_H
#define _PLL_H

#include "__pll.c"

extern struct pll_var;
extern void pll_init(struct pll_var *a);
extern void pll_calc(struct pll_var *a, float *va, float *vb, float *vc);

#endif