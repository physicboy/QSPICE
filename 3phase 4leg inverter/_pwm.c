#include <math.h>

#define TTOL_VAL        10E-9
#define PWM_PEAK        4166
#define MCU_CLK_PRD     1E-8
#define SAMPLING_PRD    2*PWM_PEAK*MCU_CLK_PRD

#define PWM_A_ON_OFF_CALC(OFF,ON, TNOW, DUTY, PRD_PEAK, PRD_VAL, CLK_PRD_VAL)   \
   if(DUTY <= 0.5){OFF = TNOW; ON = TNOW + 1;}                \
   else if(DUTY >= (PRD_PEAK - 0.5)){ON = TNOW + PRD_VAL/2; OFF = TNOW + 1;}  \
   else{OFF = TNOW + DUTY*CLK_PRD_VAL; ON = TNOW + PRD_VAL - DUTY*CLK_PRD_VAL;}
        /* OFF will be triggered on next period, ON wont be triggered */
        /* ON will be triggered on half period, OFF wont be triggered */
        /* Normal duty timing */

#define PWM_B_ON_OFF_CALC(OFF,ON, TNOW, DUTY, PRD_PEAK, PRD_VAL, CLK_PRD_VAL)   \
   if(DUTY <= 0.5){ON = TNOW; OFF = TNOW + 1;}                \
   else if(DUTY >= (PRD_PEAK - 0.5)){OFF = TNOW + PRD_VAL/2; ON = TNOW + 1;}  \
   else{OFF = TNOW + PRD_VAL - DUTY*CLK_PRD_VAL; ON = TNOW + DUTY*CLK_PRD_VAL;}
        /* ON will be triggered on next period, OFF wont be triggered */
        /* OFF will be triggered on half period, ON wont be triggered */
        /* Normal duty timing with reverted logic*/

#define PWM_C_ON_OFF_CALC(OFF, ON, TNOW, CMD_OFF, CMD_ON, CLK_PRD_VAL) \
   OFF = TNOW + CMD_OFF*CLK_PRD_VAL;                                 \
   ON  = TNOW + CMD_ON*CLK_PRD_VAL;

#define TRIGGER(TPREV,TNOW,TRIG_ON, TRIG_OFF,STATE_VAR)        \
   if((TPREV <= TRIG_ON)&&(TNOW >= TRIG_ON)) STATE_VAR = 1;    \
   if((TPREV <= TRIG_OFF)&&(TNOW >= TRIG_OFF)) STATE_VAR = 0;

#define DEADTIME(STATE_VAR_PREV, STATE_VAR, TPREV, TNOW, DELAY, DT, DELAY_VAR)  \
   if(STATE_VAR_PREV != STATE_VAR) DELAY = TNOW + DT;                     \
   if((TPREV <= DELAY)&&(TNOW >= DELAY)) DELAY_VAR = STATE_VAR;

#define PWM_OUTPUT(OUTA, OUTB, STATE_VAR, DELAY_VAR, TEMP, STATE_CHANGE, EN)    \
   TEMP = OUTA;                                             \
   OUTA = STATE_VAR*DELAY_VAR*EN;                           \
   if(TEMP != OUTA) STATE_CHANGE = 1;                       \
   TEMP = OUTB;                                             \
   OUTB = (1 - STATE_VAR)*(1 - DELAY_VAR)*EN;               \
   if(TEMP != OUTB) STATE_CHANGE = 1;

struct pwm_var
{
	// declare the structure here
	double	t_prev;
	
	double	ttol_val;
	int 	state_change;
	
	double	mcu_trig;
	int		mcu_trig_count;
	int		mcu_trig_interrupt;

	double duty1; double duty2; double duty3; double duty4; double duty5; double duty6; double duty7;
	
	double p1dtime; double p1en; double p1on; double p1off; double p1out; double p1out_prev; double p1dly; double p1dt; double p1a; double p1b;
	double p2dtime; double p2en; double p2on; double p2off; double p2out; double p2out_prev; double p2dly; double p2dt; double p2a; double p2b;
	double p3dtime; double p3en; double p3on; double p3off; double p3out; double p3out_prev; double p3dly; double p3dt; double p3a; double p3b;
	double p4dtime; double p4en; double p4on; double p4off; double p4out; double p4out_prev; double p4dly; double p4dt; double p4a; double p4b;
	double p5dtime; double p5en; double p5on; double p5off; double p5out; double p5out_prev; double p5dly; double p5dt; double p5a; double p5b;
	double p6dtime; double p6en; double p6on; double p6off; double p6out; double p6out_prev; double p6dly; double p6dt; double p6a; double p6b;
	double p7dtime; double p7en; double p7on; double p7off; double p7out; double p7out_prev; double p7dly; double p7dt; double p7a; double p7b;

};

void pwm_init(struct pwm_var *a)
{
	a->p1dtime = 100*MCU_CLK_PRD;
	a->p2dtime = 100*MCU_CLK_PRD;
	a->p3dtime = 100*MCU_CLK_PRD;
	a->p4dtime = 100*MCU_CLK_PRD;
	a->p5dtime = 100*MCU_CLK_PRD;
	a->p6dtime = 100*MCU_CLK_PRD;
	a->p7dtime = 100*MCU_CLK_PRD;
	
	a->p1en = 1;
	a->p2en = 1;
	a->p3en = 1;
	a->p4en = 1;
	a->p5en = 1;
	a->p6en = 1;
	a->p7en = 1;
}

void pwm_calc(struct pwm_var *a, double t)
{
	a->state_change = 0;
	
	if((t >= a->mcu_trig)&&(a->t_prev <= a->mcu_trig))
	{
		if(a->mcu_trig_count >= 3)
		{
			a->mcu_trig_count = 0;

			PWM_A_ON_OFF_CALC(a->p1off, a->p1on, t, a->duty1, PWM_PEAK, SAMPLING_PRD, MCU_CLK_PRD)
			PWM_A_ON_OFF_CALC(a->p2off, a->p2on, t, a->duty2, PWM_PEAK, SAMPLING_PRD, MCU_CLK_PRD)
			PWM_A_ON_OFF_CALC(a->p3off, a->p3on, t, a->duty3, PWM_PEAK, SAMPLING_PRD, MCU_CLK_PRD)
			
			PWM_B_ON_OFF_CALC(a->p4off, a->p4on, t, a->duty4, PWM_PEAK, SAMPLING_PRD, MCU_CLK_PRD)
			
			PWM_A_ON_OFF_CALC(a->p5off, a->p5on, t, a->duty5, PWM_PEAK, SAMPLING_PRD, MCU_CLK_PRD)
			PWM_A_ON_OFF_CALC(a->p6off, a->p6on, t, a->duty6, PWM_PEAK, SAMPLING_PRD, MCU_CLK_PRD)
			PWM_A_ON_OFF_CALC(a->p7off, a->p7on, t, a->duty7, PWM_PEAK, SAMPLING_PRD, MCU_CLK_PRD)
		}
		else
		{
			if(a->mcu_trig_count >= 2)
			{
				
			}
			else
			{
				if(a->mcu_trig_count >= 1)
				{
					
				}
				else
				{
					
				}
			}
			a->mcu_trig_count += 1;
			a->mcu_trig_interrupt = 1 + a->mcu_trig_count;
		}

		a->mcu_trig_interrupt = 1 + a->mcu_trig_count;
		// a->state_change = 1;
		a->mcu_trig += SAMPLING_PRD/4;
	}
	
	a->p1out_prev = a->p1out;
	a->p2out_prev = a->p2out;
	a->p3out_prev = a->p3out;
	a->p4out_prev = a->p4out;
	a->p5out_prev = a->p5out;
	a->p6out_prev = a->p6out;
	a->p7out_prev = a->p7out;
	
	TRIGGER(a->t_prev, t, a->p1on, a->p1off,a->p1out)
	TRIGGER(a->t_prev, t, a->p2on, a->p2off,a->p2out)
	TRIGGER(a->t_prev, t, a->p3on, a->p3off,a->p3out)
	TRIGGER(a->t_prev, t, a->p4on, a->p4off,a->p4out)
	TRIGGER(a->t_prev, t, a->p5on, a->p5off,a->p5out)
	TRIGGER(a->t_prev, t, a->p6on, a->p6off,a->p6out)
	TRIGGER(a->t_prev, t, a->p7on, a->p7off,a->p7out)

	DEADTIME(a->p1out_prev, a->p1out, a->t_prev, t, a->p1dly, a->p1dtime, a->p1dt)
	DEADTIME(a->p2out_prev, a->p2out, a->t_prev, t, a->p2dly, a->p2dtime, a->p2dt)
	DEADTIME(a->p3out_prev, a->p3out, a->t_prev, t, a->p3dly, a->p3dtime, a->p3dt)
	DEADTIME(a->p4out_prev, a->p4out, a->t_prev, t, a->p4dly, a->p4dtime, a->p4dt)
	DEADTIME(a->p5out_prev, a->p5out, a->t_prev, t, a->p5dly, a->p5dtime, a->p5dt)
	DEADTIME(a->p6out_prev, a->p6out, a->t_prev, t, a->p6dly, a->p6dtime, a->p6dt)
	DEADTIME(a->p7out_prev, a->p7out, a->t_prev, t, a->p7dly, a->p7dtime, a->p7dt)

	double temp = 0;
	PWM_OUTPUT(a->p1a, a->p1b, a->p1out, a->p1dt, temp, a->state_change, a->p1en)
	PWM_OUTPUT(a->p2a, a->p2b, a->p2out, a->p2dt, temp, a->state_change, a->p2en)
	PWM_OUTPUT(a->p3a, a->p3b, a->p3out, a->p3dt, temp, a->state_change, a->p3en)
	PWM_OUTPUT(a->p4a, a->p4b, a->p4out, a->p4dt, temp, a->state_change, a->p4en)
	PWM_OUTPUT(a->p5a, a->p5b, a->p5out, a->p5dt, temp, a->state_change, a->p5en)
	PWM_OUTPUT(a->p6a, a->p6b, a->p6out, a->p6dt, temp, a->state_change, a->p6en)
	PWM_OUTPUT(a->p7a, a->p7b, a->p7out, a->p7dt, temp, a->state_change, a->p7en)

	a->t_prev = t;
}

#define MAXSTEP_CALC(X) if(a->t_prev < X){if(maxstep > (X - a->t_prev)){maxstep = X - a->t_prev;}}

double pwm_maxstep(struct pwm_var *a)
{
	double maxstep = 0;
	maxstep = SAMPLING_PRD;

	MAXSTEP_CALC(a->mcu_trig)

	MAXSTEP_CALC(a->p1on)
	MAXSTEP_CALC(a->p1off)
	MAXSTEP_CALC(a->p1dly)
	MAXSTEP_CALC(a->p2on)
	MAXSTEP_CALC(a->p2off)
	MAXSTEP_CALC(a->p2dly)
	MAXSTEP_CALC(a->p3on)
	MAXSTEP_CALC(a->p3off)
	MAXSTEP_CALC(a->p3dly)
	MAXSTEP_CALC(a->p4on)
	MAXSTEP_CALC(a->p4off)
	MAXSTEP_CALC(a->p4dly)
	MAXSTEP_CALC(a->p5on)
	MAXSTEP_CALC(a->p5off)
	MAXSTEP_CALC(a->p5dly)
	MAXSTEP_CALC(a->p6on)
	MAXSTEP_CALC(a->p6off)
	MAXSTEP_CALC(a->p6dly)
	MAXSTEP_CALC(a->p7on)
	MAXSTEP_CALC(a->p7off)
	MAXSTEP_CALC(a->p7dly)
	
	return maxstep;
}



double pwm_trunc(struct pwm_var *a)
{
	if(a->state_change)return TTOL_VAL;
	else return INFINITY;
}