#include "akcelerometr.h"

/**@brief
 * Funkcja sluzaca do wpisywania do rejestrow BMA
 * @param reg uint8_t adres rejestru w akcelerometrze, do ktorego chcemy wpisac wartosc
 * @param value uint8_t wartosc ktora chcemy wpisac do reg
 */
void bma_write (uint8_t reg, uint8_t value)
{
	uint8_t data[2];
	data[0] = reg;
	data[1] = value;

	HAL_I2C_Master_Transmit(&I2C_NO, bma_adr, data, 2, 10);

}

/**@brief
 * Funkcja sluzaca do odczytywania z rejestrow BMA
 * @param reg uint8_t adres rejestru w akcelerometrze, z ktorego chcemy zczytac wartosc
 * @param numberofbytes uint8_t ilosc bajtow, ktora chcemy odczytac
 * @retval int8_t odczytana wartosc z rejestru
 */
int8_t bma_read (uint8_t reg, uint8_t numberofbytes)
{
	int8_t odakcel;

	int8_t temp_data_rec[1];
	temp_data_rec[0] = 0;
	HAL_I2C_Mem_Read(&I2C_NO, bma_adr, reg, 1, temp_data_rec, numberofbytes, 100);
	odakcel = (temp_data_rec[0] >> 2);
	return odakcel;
}

/**@brief
 * Funckja inicjalizujaca czujnik, poprzez ustawienie bazowych rejestrow
 *
 */
void bma_init (void)
{

	bma_write(0x22,0x00);

	bma_write(0x20,0x05);

	bma_write(0x1A,0x04);

}


