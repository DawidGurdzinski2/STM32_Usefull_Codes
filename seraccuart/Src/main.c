/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SERWO_MIN 500
#define SERWO_MAX 2500

#define bma_adr 0x0A<<1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
  int state = 1;
  uint8_t znak, komunikat[20], kutas, radres;
  uint16_t dlkom;
  int8_t data_rec[6];
  int odakcel;
  int kat;
  float PID;
  float Kp = 1;
  float Ki = 0.001;
  float cumError;

  int oTime;
  int nTime;
  int dTime;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
int PrzeliczKat(int kat);

void bma_write (uint8_t reg, uint8_t value){
	uint8_t data[2];
	data[0] = reg;
	data[1] = value;

	HAL_I2C_Master_Transmit(&hi2c1, bma_adr, data, 2, 10);

}


void bma_read (uint8_t reg, uint8_t numberofbytes){

	HAL_I2C_Mem_Read(&hi2c1, bma_adr, reg, 1, data_rec, numberofbytes, 100);

}

void bma_init (void){

	bma_write(0x22,0x00);

	bma_write(0x20,0x05);

	bma_write(0x1A,0x04);




}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_UART_Receive_IT(&huart2, &znak, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  	  bma_init();



  	  oTime = HAL_GetTick();

  while (1)
  {


	  bma_read(0x4,1);
  odakcel = (int)(data_rec[0] >> 2);

  	  nTime = HAL_GetTick();

  	  dTime = nTime - oTime;
  	  oTime = nTime;

  cumError += (odakcel*5.625)*dTime;

  if (cumError > 90){
	  cumError = 90;
  }
  else if (cumError < -90){
	  cumError = -90;
  }

  PID = 1*(odakcel*5.625) + 0*cumError;


/*  sprintf(komunikat,"%c", odakcel);
  			  dlkom = 1;
  HAL_UART_Transmit_IT(&huart2, komunikat, dlkom);
*/





	  kat = PrzeliczKat((int)PID);
	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, kat);
/*	  if (state == 2){
	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 2500);
	  }

	  else if (state == 1){
		  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1500);
	  }

	  else if (state == 0) {
		  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 500);
	  }

	  else if (state == 3){
		  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1750);
	  }
*/
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int PrzeliczKat(int kat){
	int wynik = 0;

	if (kat > 90){
		return 2500;
	}
	else if (kat < -90) {
		return 500;
	}

	else {
		wynik = (kat * 11.11) + 1500;
		return wynik;

	}

}





void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	if(huart->Instance == USART2){
		if(znak == 'l'){
			state = 2;

			  sprintf(komunikat,"lewo");
			  dlkom = 4;
			  HAL_UART_Transmit_IT(&huart2, komunikat, dlkom);
		}
		else if (znak == 'r'){
			state = 0;

			  sprintf(komunikat,"prawo");
			  dlkom = 5;
			  HAL_UART_Transmit_IT(&huart2, komunikat, dlkom);
		}

		else if(znak == 's'){
			state = 1;

			  sprintf(komunikat,"srodek");
			  dlkom = 6;
			  HAL_UART_Transmit_IT(&huart2, komunikat, dlkom);
		}

		else if(znak == 'm'){
			state = 3;

			  sprintf(komunikat,"moje");
			  dlkom = 4;
			  HAL_UART_Transmit_IT(&huart2, komunikat, dlkom);




		}
		HAL_UART_Receive_IT(&huart2, &znak, 1);
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
