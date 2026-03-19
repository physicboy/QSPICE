#ifndef _ADC_H
#define _ADC_H

#include "_adc.c"

extern struct adc_var;

extern void adc_meas1(struct adc_var *a, double *vdc, double *vmid,
    double *va1, double *vb1, double *vc1, double *ia1, double *ib1, double *ic1,
    double *va2, double *vb2, double *vc2, double *ia2, double *ib2, double *ic2);
extern void adc_meas2(struct adc_var *a, double *vdc, double *vmid,
    double *va1, double *vb1, double *vc1, double *ia1, double *ib1, double *ic1,
    double *va2, double *vb2, double *vc2, double *ia2, double *ib2, double *ic2);
extern void adc_meas3(struct adc_var *a, double *vdc, double *vmid,
    double *va1, double *vb1, double *vc1, double *ia1, double *ib1, double *ic1,
    double *va2, double *vb2, double *vc2, double *ia2, double *ib2, double *ic2);
extern void adc_meas4(struct adc_var *a, double *vdc, double *vmid,
    double *va1, double *vb1, double *vc1, double *ia1, double *ib1, double *ic1,
    double *va2, double *vb2, double *vc2, double *ia2, double *ib2, double *ic2);

#endif