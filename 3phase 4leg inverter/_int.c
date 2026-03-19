#include <math.h>
#include "_adc.h"
#include "_pwm.h"
#include "__pll.h"
#include "__pfc.h"

#define LIMIT(X, MAX, MIN)      \
            if(X > MAX) X = MAX;\
            if(X < MIN) X = MIN;

#define EPWM1   pwm->duty1
#define EPWM2   pwm->duty2
#define EPWM3   pwm->duty3
#define EPWM4   pwm->duty4
#define EPWM5   pwm->duty5
#define EPWM6   pwm->duty6
#define EPWM7   pwm->duty7

struct int_var
{
    float vdc; float vmid;
    float va1; float vb1; float vc1; float ia1; float ib1; float ic1;
    float va2; float vb2; float vc2; float ia2; float ib2; float ic2;

    int counter;

    int fault;

    int   relay;
    int   pwm123_en;
    int   pwm4_en;
    int   pwm567_en;

    pll_var pll_data;
    pfc_var pfc_data;

    float theta_fast;
};

#define VDCREF  500

#define IAC_PROTECTION  40
#define VDC_PROTECTION  750

void int_calc(struct int_var *a, struct adc_var *adc, struct pwm_var *pwm, int *on_off)
{
    /*
    ===================================================================================
    compute the feedback measurement after 4x oversampling
    ===================================================================================
    */
    a->vdc  = (float)(0.25 * (adc->vdc1 + adc->vdc2 + adc->vdc3 + adc->vdc4));
    a->vmid = (float)(0.25 * (adc->vmid1 + adc->vmid2 + adc->vmid3 + adc->vmid4));

    a->va1 = (float)(0.25 * (adc->va11 + adc->va12 + adc->va13 + adc->va14));
    a->vb1 = (float)(0.25 * (adc->vb11 + adc->vb12 + adc->vb13 + adc->vb14));
    a->vc1 = (float)(0.25 * (adc->vc11 + adc->vc12 + adc->vc13 + adc->vc14));
    a->ia1 = (float)(0.25 * (adc->ia11 + adc->ia12 + adc->ia13 + adc->ia14));
    a->ib1 = (float)(0.25 * (adc->ib11 + adc->ib12 + adc->ib13 + adc->ib14));
    a->ic1 = (float)(0.25 * (adc->ic11 + adc->ic12 + adc->ic13 + adc->ic14));

    a->va2 = (float)(0.25 * (adc->va21 + adc->va22 + adc->va23 + adc->va24));
    a->vb2 = (float)(0.25 * (adc->vb21 + adc->vb22 + adc->vb23 + adc->vb24));
    a->vc2 = (float)(0.25 * (adc->vc21 + adc->vc22 + adc->vc23 + adc->vc24));
    a->ia2 = (float)(0.25 * (adc->ia21 + adc->ia22 + adc->ia23 + adc->ia24));
    a->ib2 = (float)(0.25 * (adc->ib21 + adc->ib22 + adc->ib23 + adc->ib24));
    a->ic2 = (float)(0.25 * (adc->ic21 + adc->ic22 + adc->ic23 + adc->ic24));

    /*
    ===================================================================================
    actual control algorithm placement
    ===================================================================================
    */

    if(a->vdc > VDC_PROTECTION)         a->fault = 1;
    if(fabsf(a->ia1) > IAC_PROTECTION)  a->fault = 1;
    if(fabsf(a->ib1) > IAC_PROTECTION)  a->fault = 1;
    if(fabsf(a->ic1) > IAC_PROTECTION)  a->fault = 1;
    if(fabsf(a->ia2) > IAC_PROTECTION)  a->fault = 1;
    if(fabsf(a->ib2) > IAC_PROTECTION)  a->fault = 1;
    if(fabsf(a->ic2) > IAC_PROTECTION)  a->fault = 1;

    a->counter += *on_off * (1 + 5) - 5;
    LIMIT(a->counter, 250, 0);

    if(a->counter <= 50)
    {
        a->fault = 0;

        a->relay = 0;
        a->pwm123_en = 0;
        a->pwm4_en   = 0;
        a->pwm567_en = 0;

        pll_init(&a->pll_data);
        pfc_init(&a->pfc_data, &a->vdc);
    }
    else
    {
        if(a->fault)
        {
            a->pwm123_en = 0;
            a->pwm4_en = 0;
            a->pwm567_en = 0;
        }
        else
        {
            pll_calc(&a->pll_data, &a->va1, &a->vb1, &a->vc1);
            if(a->counter <= 200)
            {
                a->pwm123_en = 0;
                a->pwm4_en = 0;
                a->pwm567_en = 0;

                if(a->counter <= 150)
                {
                    a->relay = 0;
                    if(a->counter == 150)
                    {
                        if((fabsf(a->pll_data.vq) > 10) || (fabsf(a->pll_data.w) < 2 * M_PI * 45))
                        {
                            a->counter -= 1;
                        }
                    }
                }
                else
                {
                    a->relay = 1;
                }
            }
            else
            {
                pfc_calc(&a->pfc_data, &a->pll_data, VDCREF, &a->vdc, &a->ia1, &a->ib1, &a->ic1);
                a->pwm123_en = 1;
                a->pwm4_en = 1;
                if(a->pfc_data.status1 && a->pfc_data.status2 && a->pfc_data.status3)
                {
                    a->pwm567_en = 0;   
                }
            }
        }
    }

    a->theta_fast = 10 * a->pll_data.th;
    while(a->theta_fast >= 2 * M_PI)
    {
        a->theta_fast -= 2 * M_PI;
    }

    /*
    ===================================================================================
    send the computation output to pwm registers
    ===================================================================================
    */
    EPWM1 = 4166 / 2 + (4166 / 2) * a->pfc_data.va / (0.5 * a->pfc_data.vdc_temp);
    EPWM2 = 4166 / 2 + (4166 / 2) * a->pfc_data.vb / (0.5 * a->pfc_data.vdc_temp);
    EPWM3 = 4166 / 2 + (4166 / 2) * a->pfc_data.vc / (0.5 * a->pfc_data.vdc_temp);

    EPWM4 = 4166 / 2;

    EPWM5 = 4166 / 2;
    EPWM6 = 4166 / 2;
    EPWM7 = 4166 / 2;
}