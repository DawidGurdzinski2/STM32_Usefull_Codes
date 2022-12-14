#ifndef DIODE_H_
#define DIODE_H_

#include "stm32f3xx_hal.h"
#define D1_Pin GPIO_PIN_7
#define D1_GPIO_Port GPIOA
#define D4_Pin GPIO_PIN_7
#define D4_GPIO_Port GPIOC
#define D2_Pin GPIO_PIN_9
#define D2_GPIO_Port GPIOA
#define D3_Pin GPIO_PIN_6
#define D3_GPIO_Port GPIOB


void light_diode(uint8_t );
void turn_off_diodes(void);


#endif /* DIODE_H_ */
