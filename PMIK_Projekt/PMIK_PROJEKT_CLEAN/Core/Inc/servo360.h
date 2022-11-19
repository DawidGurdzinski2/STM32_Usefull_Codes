#ifndef INC_SERVO360_H_
#define INC_SERVO360_H_
#include <stdint.h>

/* konfiguracja timera do generowania PWM */
#define TIM_NO htim3
#define TIM_CH_NO TIM_CHANNEL_1

/* zakres PWM */
#define PWM_MAX_LEFT 830
#define PWM_STOP 1530
#define PWM_MAX_RIGHT 2230
#define STEP_SPEED 7
#define SPEED_LEVEL_MIN 0
#define SPEED_LEVEL_MAX 100


void set_move(float ,uint8_t);

void stop_servo();


#endif /* INC_SERVO360_H_ */
