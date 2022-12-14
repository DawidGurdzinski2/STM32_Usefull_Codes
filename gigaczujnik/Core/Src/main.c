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
#include <stdio.h>
#include <string.h>
#include "servo360.h"
#include "akcelerometr.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SERWO_MIN 500
#define SERWO_MAX 2500

//#define bma_adr 0x0A<<1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
char msg[64];
float changed_value;
float avg_changed_value;

//sternvarablrs
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
//sternvariablesend
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//int PrzeliczKat(int kat);
//
//void bma_write (uint8_t reg, uint8_t value){
//	uint8_t data[2];
//	data[0] = reg;
//	data[1] = value;
//
//	HAL_I2C_Master_Transmit(&hi2c1, bma_adr, data, 2, 10);
//
//}
//
//
//void bma_read (uint8_t reg, uint8_t numberofbytes){
//
//	HAL_I2C_Mem_Read(&hi2c1, bma_adr, reg, 1, data_rec, numberofbytes, 100);
//
//}
//
//void bma_init (void){
//
//	bma_write(0x22,0x00);
//
//	bma_write(0x20,0x05);
//
//	bma_write(0x1A,0x04);
//
//
//
//
//}
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
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  	  HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_UART_Receive_IT(&huart2, &znak, 1);
    HAL_Delay(1000);

	 // __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1400);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    bma_init();



    oTime = HAL_GetTick();
  while (1)
  {

	  //stern
	  bma_read(0x4,1,data_rec);
	  //bma_read(0x0,1);
	  //sprintf(komunikat,"%c", data_rec[0]);
	   odakcel = (int)(data_rec[0] >> 2);

	  // sprintf(komunikat,"%c", odakcel);
	     //			  dlkom = 1;
	    // HAL_UART_Transmit(&huart2, komunikat, 1, 1000);

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
	   //kat = PrzeliczKat((int)PID);
	 // sprintf(komunikat,"%c", (int)PID);
	  // HAL_UART_Transmit(&huart2, komunikat, 1, 1000);
 	   //HAL_UART_Transmit(&huart2, (uint8_t*)komunikat, strlen(komunikat),1000);

	  //stern

	  uint32_t start = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);
	  uint32_t stop = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_2);
	  	 // sprintf((char*)msg,"Odczyt: %lu\n",start-2236);

	  	   changed_value = (stop-start)/58.0f;

	  	  	sprintf((char*)msg,"ODL %.1f cm\n",(stop-start) / 58.0f);
	  	  	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg),1000);

	  	  	HAL_Delay(100);
	  	  	//set_ang(0);
	  	//  	HAL_Delay(1000);
	  	//  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1530);//stoi jak chuj
	  	 // set_ang(1800);
	  	  //HAL_Delay(1000);
	  	//__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 830);
	  	  if ( PID <-20)
	  	  {
	  	  	//__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1530);//stoi jak chuj

	  		 set_move(changed_value-25, 0);
	  	  }
	  	  else if ( PID > 20)
	  	  {
	  	  	  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1530);//stoi jak chuj

	  		set_move((25 - changed_value),1);
	  	  }

	  	  else{
	  		  	  if(changed_value<=25)
	  		  	  {
	  		  		  set_move((25 - changed_value),1);
	  		  	  }
	  		  	  else
	  		  	  {
	  		  		  set_move(changed_value-25, 0);
	  		  	  }
	  	  	  }



	  	//set_move(3,1);

	  	//  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1530);//stoi jak chuj
	  //	HAL_Delay(1000);
	  	//stop_servo();
	  	//HAL_Delay(1000);
	  	//stop_servo();
	  		  	  //HAL_Delay(1000);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_6;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_3;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//int PrzeliczKat(int kat){
//	int wynik = 0;
//
//	if (kat > 90){
//		return 2500;
//	}
//	else if (kat < -90) {
//		return 500;
//	}
//
//	else {
//		wynik = (kat * 11.11) + 1500;
//		return wynik;
//
//	}
//
//}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
//
//	if(huart->Instance == USART2){
//		if(znak == 'l'){
//			state = 2;
//
//			  sprintf(komunikat,"lewo");
//			  dlkom = 4;
//			  HAL_UART_Transmit_IT(&huart2, komunikat, dlkom);
//		}
//		else if (znak == 'r'){
//			state = 0;
//
//			  sprintf(komunikat,"prawo");
//			  dlkom = 5;
//			  HAL_UART_Transmit_IT(&huart2, komunikat, dlkom);
//		}
//
//		else if(znak == 's'){
//			state = 1;
//
//			  sprintf(komunikat,"srodek");
//			  dlkom = 6;
//			  HAL_UART_Transmit_IT(&huart2, komunikat, dlkom);
//		}
//
//		else if(znak == 'm'){
//			state = 3;
//
//			  sprintf(komunikat,"moje");
//			  dlkom = 4;
//			  HAL_UART_Transmit_IT(&huart2, komunikat, dlkom);
//
//
//
//
//		}
//		HAL_UART_Receive_IT(&huart2, &znak, 1);
//	}
//}
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
