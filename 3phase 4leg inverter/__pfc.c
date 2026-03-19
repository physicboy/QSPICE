#include <math.h>
#include "__pll.h"

#define PI(X, X_PREV, KP, KITS)         \
            KP*(X - X_PREV) + X*KITS;   \
            X_PREV = X;

#define LIMIT(X, MAX, MIN)      \
            if(X > MAX) X = MAX;\
            if(X < MIN) X = MIN;

struct pfc_var
{
    float vdc_error;
    float vdc_error_prev;

    float idcref;
    float idclim;

    float idref;
    float iqref;

    float ia;
    float ib;
    float id;
    float iq;

    float id_error;
    float iq_error;
    
    float id_error_prev;
    float iq_error_prev;

    float ud;
    float uq;

    float vd;
    float vq;

    float ua;
    float ub;

    float va;
    float vb;
    float vc;

    float vdc_temp;

    int   status1;
    int   status2;
    int   status3;
};

void pfc_init(struct pfc_var *a, float *vdc)
{
    a->status1  = 0;
    a->status2  = 0;
    a->status3  = 0;

    a->idclim   = 0;
    a->idref    = 0;
    a->iqref    = 0;

    a->id_error = 0;
    a->id_error_prev = 0;

    a->iq_error = 0;
    a->iq_error_prev = 0;

    a->ud = 0;
    a->uq = 0;

    a->vdc_temp = *vdc;
}

#define KP_V    0.08217046
#define KITS_V  0.000362532

#define KP_I    7.528965945
#define KITS_I  0.127105731

#define L_INV   0.002

#define IDC_LIM 25
#define ID_LIM  40
#define UDQ_LIM 25

#define VDCRAMP 1
#define IDCRAMP 0.5

#define VDC_TOL 20

void pfc_calc(struct pfc_var *a, struct pll_var *b,
    float vdc_ref, float *vdc, float *ia, float *ib, float *ic)
{
    if(a->idclim > IDC_LIM)
    {
        a->idclim = IDC_LIM;
    }
    else
    {
        a->idclim += IDCRAMP;
    }

    if(a->vdc_temp < vdc_ref)
    {
        a->vdc_temp += VDCRAMP;
        if(a->vdc_temp >  vdc_ref)
        {
            a->vdc_temp = vdc_ref;
        }
    }
    else
    {
        a->vdc_temp -= VDCRAMP;
        if(a->vdc_temp <  vdc_ref)
        {
            a->vdc_temp = vdc_ref;
        }
    }

    if(a->idclim == IDC_LIM)
    {
        a->status1 = 1;
    }
    if(a->vdc_temp == vdc_ref)
    {
        a->status2 = 1;
    }
    if(fabsf(vdc_ref - *vdc) < VDC_TOL)
    {
        a->status3 = 1;
    }

    a->vdc_error = vdc_ref - *vdc;
    a->idcref += PI(a->vdc_error, a->vdc_error_prev, KP_V, KITS_V);

    LIMIT(a->idcref, a->idclim, -1 * a->idclim);

    a->idref = -1. * a->idcref * 2./3. * vdc_ref / b->vd;
    a->iqref = 0;

    LIMIT(a->idref, ID_LIM, -1 * ID_LIM);

    a->ia = 2./3. * (*ia - 0.5 * (*ib + *ic));
    a->ib = 2./3. * 0.86602540378 * (*ib - *ic);

    a->id = a->ia * b->costh + a->ib * b->sinth;
    a->iq = - a->ia * b->sinth + a->ib * b->costh;

    a->id_error = a->idref - a->id;
    a->iq_error = a->iqref - a->iq;

    a->ud += PI(a->id_error, a->id_error_prev, KP_I, KITS_I);
    a->uq += PI(a->iq_error, a->iq_error_prev, KP_I, KITS_I);

    LIMIT(a->ud, UDQ_LIM, -1 * UDQ_LIM);
    LIMIT(a->uq, UDQ_LIM, -1 * UDQ_LIM);

    a->vd = b->vd + (a->ud - a->iqref * fabsf(b->w) * L_INV);
    a->vq = b->vq + (a->uq + a->idref * fabsf(b->w) * L_INV);

    a->ua = a->vd * b->costh - a->vq * b->sinth;
    a->ub = a->vd * b->sinth + a->vq * b->costh;

    a->va = a->ua;
    a->vb = -0.5 * a->ua + 0.86602540378 *a->ub;
    a->vc = -0.5 * a->ua - 0.86602540378 *a->ub;
}