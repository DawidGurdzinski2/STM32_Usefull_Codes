#ifndef INC_SERVO_H_
#define INC_SERVO_H_
#include <stdint.h>

/* konfiguracja timera do generowania PWM */
#define TIM_NO htim16
#define TIM_CH_NO TIM_CHANNEL_1

/* zakresy katowe pracy serwomechanizmu */
#define ANGLE_MIN 0
#define ANGLE_MAX 1800
/* zakres PWM */
#define PWM_MIN 500
#define PWM_MAX 2400

#define STEP 1000*(3500/1800)

void set_ang(uint16_t ang);

#endif /* INC_SERVO_H_ */
