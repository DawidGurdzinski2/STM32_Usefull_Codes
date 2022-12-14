#include "akcelerometr.h"


void bma_write (uint8_t reg, uint8_t value)
{
	uint8_t data[2];
	data[0] = reg;
	data[1] = value;

	HAL_I2C_Master_Transmit(&I2C_NO, bma_adr, data, 2, 10);

}


void bma_read (uint8_t reg, uint8_t numberofbytes, int8_t* data_rec)
{

	HAL_I2C_Mem_Read(&I2C_NO, bma_adr, reg, 1, data_rec, numberofbytes, 100);

}


void bma_init (void)
{

	bma_write(0x22,0x00);

	bma_write(0x20,0x05);

	bma_write(0x1A,0x04);

}


int PrzeliczKat(int kat)
{
	int wynik = 0;

	if (kat > 90)
	{
		return 2500;
	}
	else if (kat < -90)
	{
		return 500;
	}

	else
	{
		wynik = (kat * 11.11) + 1500;
		return wynik;

	}

}
