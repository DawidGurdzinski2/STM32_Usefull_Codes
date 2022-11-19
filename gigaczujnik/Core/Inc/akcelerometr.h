#ifndef INC_AKCELEROMETR_H_
#define INC_AKCELEROMETR_H_
#define I2C_NO hi2c1

#include <stdint.h>



#define bma_adr 0x0A<<1
extern int8_t data_rec[6];


void bma_write (uint8_t, uint8_t );

void bma_read (uint8_t, uint8_t, int8_t*);

void bma_init (void);

int PrzeliczKat(int);

#endif /* INC_AKCELEROMETR_H_ */
