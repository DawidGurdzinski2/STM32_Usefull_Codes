#ifndef INC_DISTANCE_SENSOR_H_
#define INC_DISTANCE_SENSOR_H_
#include <stdint.h>


#define I2C_NO hi2c1

uint16_t IR_Get_Distance();

void IR_Init();


#endif /* INC_DISTANCE_SENSOR_H_ */
