
#include "servo.h"
#include "tim.h"

/*
 * ang - kat obrotu walu serwomechanizmu
 * mode - tryb obrotu zgodnie/przeciwnie do wskazowek zegara
 */
void set_ang(uint16_t ang)
{
	uint16_t val;

	if(ang > ANGLE_MAX)
	{
		ang = ANGLE_MAX;
	}
	else if (ang < ANGLE_MIN)
	{
		ang = ANGLE_MIN;
	}
	val = PWM_MIN + (ang * STEP) / 1000;



	__HAL_TIM_SET_COMPARE(&TIM_NO, TIM_CH_NO, val);
}
