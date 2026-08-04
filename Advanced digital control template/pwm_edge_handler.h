#ifndef PWM_EDGE_HANDLER_H
#define PWM_EDGE_HANDLER_H

#include "udata.h"
#include "pwm_edge_handler.c"

extern struct sPWM;
extern void pwm_edge(struct sPWM *a, bool *stepped);

#endif // PWM_EDGE_HANDLER_H