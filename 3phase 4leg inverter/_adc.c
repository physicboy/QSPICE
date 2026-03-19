#include "mcu.h"

struct adc_var
{
    double vdc1; double vdc2; double vdc3; double vdc4;
    double vmid1; double vmid2; double vmid3; double vmid4;

    double va11; double va12; double va13; double va14;
    double vb11; double vb12; double vb13; double vb14;
    double vc11; double vc12; double vc13; double vc14;

    double ia11; double ia12; double ia13; double ia14;
    double ib11; double ib12; double ib13; double ib14;
    double ic11; double ic12; double ic13; double ic14;

    double va21; double va22; double va23; double va24;
    double vb21; double vb22; double vb23; double vb24;
    double vc21; double vc22; double vc23; double vc24;

    double ia21; double ia22; double ia23; double ia24;
    double ib21; double ib22; double ib23; double ib24;
    double ic21; double ic22; double ic23; double ic24;
};

void adc_meas1(struct adc_var *a, double *vdc, double *vmid,
    double *va1, double *vb1, double *vc1, double *ia1, double *ib1, double *ic1,
    double *va2, double *vb2, double *vc2, double *ia2, double *ib2, double *ic2)
{
    a->vdc1     = *vdc;
    a->vmid1    = *vmid;

    a->va11     = *va1; a->vb11     = *vb1; a->vc11     = *vc1;
    a->ia11     = *ia1; a->ib11     = *ib1; a->ic11     = *ic1;
    a->va21     = *va2; a->vb21     = *vb2; a->vc21     = *vc2;
    a->ia21     = *ia2; a->ib21     = *ib2; a->ic21     = *ic2;
}

void adc_meas2(struct adc_var *a, double *vdc, double *vmid,
    double *va1, double *vb1, double *vc1, double *ia1, double *ib1, double *ic1,
    double *va2, double *vb2, double *vc2, double *ia2, double *ib2, double *ic2)
{
    a->vdc2     = *vdc;
    a->vmid2    = *vmid;

    a->va12     = *va1; a->vb12     = *vb1; a->vc12     = *vc1;
    a->ia12     = *ia1; a->ib12     = *ib1; a->ic12     = *ic1;
    a->va22     = *va2; a->vb22     = *vb2; a->vc22     = *vc2;
    a->ia22     = *ia2; a->ib22     = *ib2; a->ic22     = *ic2;
}

void adc_meas3(struct adc_var *a, double *vdc, double *vmid,
    double *va1, double *vb1, double *vc1, double *ia1, double *ib1, double *ic1,
    double *va2, double *vb2, double *vc2, double *ia2, double *ib2, double *ic2)
{
    a->vdc3     = *vdc;
    a->vmid3    = *vmid;

    a->va13     = *va1; a->vb13     = *vb1; a->vc13     = *vc1;
    a->ia13     = *ia1; a->ib13     = *ib1; a->ic13     = *ic1;
    a->va23     = *va2; a->vb23     = *vb2; a->vc23     = *vc2;
    a->ia23     = *ia2; a->ib23     = *ib2; a->ic23     = *ic2;
}

void adc_meas4(struct adc_var *a, double *vdc, double *vmid,
    double *va1, double *vb1, double *vc1, double *ia1, double *ib1, double *ic1,
    double *va2, double *vb2, double *vc2, double *ia2, double *ib2, double *ic2)
{
    a->vdc4     = *vdc;
    a->vmid4    = *vmid;

    a->va14     = *va1; a->vb14     = *vb1; a->vc14     = *vc1;
    a->ia14     = *ia1; a->ib14     = *ib1; a->ic14     = *ic1;
    a->va24     = *va2; a->vb24     = *vb2; a->vc24     = *vc2;
    a->ia24     = *ia2; a->ib24     = *ib2; a->ic24     = *ic2;
}