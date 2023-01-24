#ifndef INC_SERVO_H_
#define INC_SERVO_H_
#include <stdint.h>

/* konfiguracja timera do generowania PWM */
#define TIM_NO htim3
#define TIM_CH_NO TIM_CHANNEL_1

/* zakresy katowe pracy serwomechanizmu */
#define ANGLE_MIN 0
#define ANGLE_MAX 1200
#define ANGLE_MID 860

/* zakres PWM */
#define PWM_MIN 400
#define PWM_MAX 2600

#define STEP ((1000 * (PWM_MAX-PWM_MIN)) / (ANGLE_MAX-ANGLE_MIN))

void set_ang(uint16_t );

#endif /* INC_SERVO_H_ */
