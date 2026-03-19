#include <math.h>

#define PI(X, X_PREV, KP, KITS)         \
            KP*(X - X_PREV) + X*KITS;   \
            X_PREV = X;

#define LIMIT(X, MAX, MIN)      \
            if(X > MAX) X = MAX;\
            if(X < MIN) X = MIN;

struct pll_var
{
    float costh;
    float sinth;

    float va;
    float vb;

    float vd;
    float vq;
    float vq_prev;

    float w;
    float th;
};

void pll_init(struct pll_var *a)
{
    a->w = 0;
    a->th = 0;
}

void pll_calc(struct pll_var *a, float *va, float *vb, float *vc)
{
    /*
    https://imperix.com/doc/implementation/synchronous-reference-frame-pll
    */
    a->costh = cosf(a->th);
    a->sinth = sinf(a->th);

    a->va = 2./3. * (*va - 0.5 * (*vb + *vc));
    a->vb = 2./3. * 0.86602540378 * (*vb - *vc);

    a->vd = a->va * a->costh + a->vb * a->sinth;
    a->vq = - a->va * a->sinth + a->vb * a->costh;

    /*
    Parameter is designed for normalized AC input
    wc  = 2 * PI * 30;
    Kp  = 2 * 1/sqrt(2) * wc
    Ki  = wc * wc;

    Set normalization factor at 160V peak

    actual Kp   = Kp / 160
    actual KiTs = Ki * Ts / 160
    */

    a->w += PI(a->vq, a->vq_prev,
        sqrt(2) * 2 * M_PI * 30. / 160.,
        (2 * M_PI * 30)*(2 * M_PI * 30) * 2 * 4166E-8 / 160);

    LIMIT(a->w, 2 * M_PI * 65, -2 * M_PI * 65);

    a->th += a->w * 2 * 4166E-8;
    if(a->th > 2 * M_PI) a->th -= 2 * M_PI;
    if(a->th < 0) a->th += 2 * M_PI;
}