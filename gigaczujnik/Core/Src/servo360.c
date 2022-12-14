
#include "servo360.h"
#include "tim.h"



void set_move(float speed_level, uint8_t direction )
{
	if(direction == 0 &&speed_level <101 && speed_level>=0){		// krecenie w lewo
		float servo_speed = PWM_STOP-(speed_level*STEP_SPEED);
		__HAL_TIM_SET_COMPARE(&TIM_NO, TIM_CH_NO, servo_speed);
	}
	else if(direction == 1 && speed_level <101 && speed_level>=0){		// krecenie w prawo
			float servo_speed = PWM_STOP+(speed_level*STEP_SPEED);
				__HAL_TIM_SET_COMPARE(&TIM_NO, TIM_CH_NO, servo_speed);
			}
	else{
		return;
	}
	}

void stop_servo()
{
	__HAL_TIM_SET_COMPARE(&TIM_NO, TIM_CH_NO, 1530);
}
