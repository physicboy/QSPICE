#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H

#include "udata.h"
#include "interrupt_handler.c"

extern struct sINTERRUPT;
extern void interrupt0(struct sINTERRUPT *a, struct sPWM pwm[], union uData *data);
extern void interrupt1(struct sINTERRUPT *a, struct sPWM pwm[], union uData *data);
extern void interrupt2(struct sINTERRUPT *a, struct sPWM pwm[], union uData *data);
extern void interrupt3(struct sINTERRUPT *a, struct sPWM pwm[], union uData *data);

#endif // PWM_EDGE_HANDLER_H