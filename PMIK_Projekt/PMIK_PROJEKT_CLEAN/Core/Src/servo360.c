
#include "servo360.h"
#include "tim.h"

//speed_level from 55 to 355
//stop_speed_level = 205
#define Skala 4.66 // 4.66 od szyny do szyny


void set_move(float PID_value, uint8_t direction)
{
	if(direction == 0 &&PID_value <150 && PID_value>=0){		// krecenie w lewo
		float servo_speed = PWM_STOP - (Skala * PID_value) ;
		__HAL_TIM_SET_COMPARE(&TIM_NO, TIM_CH_NO, servo_speed);
	}
	else if(direction == 1 && PID_value <150 && PID_value>=0){		// krecenie w prawo
			float servo_speed = PWM_STOP +(Skala * PID_value) ;
				__HAL_TIM_SET_COMPARE(&TIM_NO, TIM_CH_NO, servo_speed);
			}
	else{
		//stop_servo();
		return;
	}
	}

void stop_servo()
{
	__HAL_TIM_SET_COMPARE(&TIM_NO, TIM_CH_NO, 1510);
}
