#ifndef INC_AKCELEROMETR_H_
#define INC_AKCELEROMETR_H_
#define I2C_NO hi2c1

#include <stdint.h>
#include "i2c.h"



#define bma_adr 0x0A<<1  // address of IR distance sensor
#define bma_x 0x4



void bma_write (uint8_t, uint8_t );

int8_t bma_read (uint8_t, uint8_t);

void bma_init (void);

int PrzeliczKat(int);

#endif /* INC_AKCELEROMETR_H_ */
