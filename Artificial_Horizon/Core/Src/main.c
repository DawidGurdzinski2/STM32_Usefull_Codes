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
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include"QMC5883.h"
#include "st7735.h"
#include "fonts.h"
#include "testimg.h"
#include "MPU6050.h"
#include "artificial_horizon.h"
#include <math.h>
#include<stdlib.h>
#include "MY_Keypad4x4.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define h 80-height_point
#define minus_h 80+height_point
#define w 60+width_point
#define minus_w 60-width_point
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
char tab [4];

//magnetometer variables
QMC_t pusula_sensor;
float Compas_Value;
float 				CalibratedXaxis;
float 				CalibratedYaxis;
float 				CalibratedZaxis;
float biasaxisX = (XMAX+XMIN)/2;
float biasaxisY = (YMAX+YMIN)/2;
float biasaxisZ = (ZMAX+ZMIN)/2;

//float deltaaxisX = (XMAX-XMIN)/2;
//float deltaaxisY = (YMAX-YMIN)/2;
//float deltaaxisZ = (ZMAX-ZMIN)/2;
//
//float deltaavg = ((XMAX-XMIN) + (YMAX-YMIN) + (ZMAX-ZMIN))*0.66;

float scaleaxisX = ((((XMAX-XMIN) + (YMAX-YMIN) + (ZMAX-ZMIN))/6)/((XMAX-XMIN)/2));
float scaleaxisY = (((XMAX-XMIN) + (YMAX-YMIN) + (ZMAX-ZMIN))/6)/((YMAX-YMIN)/2);
float scaleaxisZ = (((XMAX-XMIN) + (YMAX-YMIN) + (ZMAX-ZMIN))/6)/((ZMAX-ZMIN)/2);
float Compas;
float Calibrated_heading;
int Direction;
//acc/gyro variables
float Ax, Ay, Az, Gx, Gy, Gz;
float pitch_point; // od 1 do 160 na srodku ekranu punkt horyzontu w osi X
float width_point;
float height_point;
float triangle_point_w;
float triangle_point_h;
float alfay;
float alfax;

//keyboard variables

volatile uint8_t row = 0;
volatile uint8_t column = 0;
bool mySwitches[16];
uint16_t counter_button;
volatile uint8_t magnetometer_value_index;
volatile uint16_t magnetometer_value;
volatile bool heading_set = false;
uint8_t LastButton = 20;
char Keypad_keys[16] =
{
    '1',
    '2',
    '3',
    'B',
    '4',
    '5',
    '6',
    'R',
    '7',
    '8',
    '9',
    'C',
    '*',
    '0',
    '#',
    'E'
};
int calibrationData[3][2] = { { 32767, -32768 },
                              { 32767, -32768 },
                              { 32767, -32768 }  }; // Initialization added!
bool changed = false;
bool done = false;
unsigned long t = 0; // Changed from int to unsigned long!
unsigned long c = 0; // Changed from int to unsigned long!


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void setDirectionArrow(float heading);
void drawDirectionArrow(int Direction);
void drawArrowLeft();
void drawArrowRight();

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
  MX_SPI1_Init();
  MX_I2C3_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  ST7735_Init();
  QMC_init(&pusula_sensor, &hi2c3, 200);
  HAL_Delay(1000);
  MPU6050_Init();
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_FillRectangleFast(0, 0, 128, 80, ST7735_BLUE);
  ST7735_FillRectangleFast(0, 80, 128, 80, ST7735_BROWN);
  HAL_Delay(1000);
  HAL_TIM_Base_Start_IT(&htim3);

//KALIBRACJA MAGNETOMETRU
//  while(!done){
// 	  c = HAL_GetTick();
// 	  int x, y, z;
// 	  if(HAL_GPIO_ReadPin(Mag_Data_Ready_GPIO_Port, Mag_Data_Ready_Pin) == GPIO_PIN_SET){
// 	  		  if(QMC_read(&pusula_sensor)==0 )
// 	  			{
// 	  				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
//
// 	  				 x = pusula_sensor.Xaxis;
// 	  				 y = pusula_sensor.Yaxis;
// 	  				 z = pusula_sensor.Zaxis;
// 	  				 changed = false;
// 	  				if(x < calibrationData[0][0]) {
// 	  				    calibrationData[0][0] = x;
// 	  				    changed = true;
// 	  				  }
// 	  				  if(x > calibrationData[0][1]) {
// 	  				    calibrationData[0][1] = x;
// 	  				    changed = true;
// 	  				  }
//
// 	  				  if(y < calibrationData[1][0]) {
// 	  				    calibrationData[1][0] = y;
// 	  				    changed = true;
// 	  				  }
// 	  				  if(y > calibrationData[1][1]) {
// 	  				    calibrationData[1][1] = y;
// 	  				    changed = true;
// 	  				  }
//
// 	  				  if(z < calibrationData[2][0]) {
// 	  				    calibrationData[2][0] = z;
// 	  				    changed = true;
// 	  				  }
// 	  				  if(z > calibrationData[2][1]) {
// 	  				    calibrationData[2][1] = z;
// 	  				    changed = true;
// 	  				  }
// 	  				 if (changed && !done) {
// 	  				    c = HAL_GetTick();
// 	  				  }
// 	  				    t = HAL_GetTick();
// 	  				  if ( (t - c > 10000) && !done) {
// 	  				      done = true;
// 	  				//sprintf(tab ,"kat %f\n X %d\n Y %d\n Z %d\n"  , pusula_sensor.heading,pusula_sensor.Xaxis,pusula_sensor.Yaxis,pusula_sensor.Zaxis);
// 	  			}
// 	  			else
// 	  			{
// 	  				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
// 	  			}
// 	  	  }
//   }
//  }
//KONIEC KALIBRACJI MAGNETOMETRU
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  ST7735_DrawLine((60+width_point),80-height_point,60-width_point,80+height_point, ST7735_BLACK); //narysowac samolocik w stosunku do wyznaczonych punktow na okregu
		  ST7735_DrawLine((60+width_point),80-height_point,60-width_point,80+height_point, ST7735_BLACK); //narysowac samolocik w stosunku do wyznaczonych punktow na okregu
		  ST7735_DrawLine(60-triangle_point_w, 80+triangle_point_h, w/5,h/5, ST7735_BLACK);
		  ST7735_DrawLine(60-triangle_point_w, 80+triangle_point_h, minus_w/5,minus_h/5, ST7735_BLACK);
//	  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
//	  HAL_Delay(1000);
		  for(uint8_t i=0; i<16;i++){

		  	  		if(mySwitches[i]){
		  	  			if(Keypad_keys[i] == 82){
		  	  				heading_set = false;
		  	  				mySwitches[i]=false;
		  	  				continue;
		  	  			}
		  	  			if(!heading_set){
		  	  			if(LastButton == i){
		  	  				  if(counter_button > 7){
		  	  				  		LastButton = i;
		  	  				  		counter_button = 0;
		  	  				  		if ( Keypad_keys[i] == 66){
		  	  				  			if(magnetometer_value_index!= 0){
		  	  				  		  		tab[magnetometer_value_index-1] =0;
		  	  				  			  	mySwitches[i]=false;
		  	  				  			  	magnetometer_value_index--;
		  	  				  			  	ST7735_FillRectangleFast(0, 0, 128, 30, ST7735_BLUE);

		  	  				  			}
		  	  				  		}
		  	  				  		else{
		  	  				  			tab[magnetometer_value_index] = Keypad_keys[i];
		  	  				  			magnetometer_value_index++;
		  	  				  		//ST7735_WriteString(0, 0, tab, Font_11x18, ST7735_WHITE, ST7735_BLACK);

		  	  				  			mySwitches[i]=false;
		  	  				  		}
		  	  				  		}
		  	  				  		counter_button++;
		  	  					  	mySwitches[i]=false;
		  	  				  		}

		  	  				  	else{
		  	  				  		LastButton = i;
		  	  				  		counter_button = 0;
		  	  				  		if (Keypad_keys[i] == 66 ){
		  	  				  			if(magnetometer_value_index!= 0){
		  	  				  				tab[magnetometer_value_index-1] =0;
		  	  				  				mySwitches[i]=false;
		  	  				  				magnetometer_value_index--;
		  	  				  			  	ST7735_FillRectangleFast(0, 0, 128, 30, ST7735_BLUE);

		  	  				  			}
		  	  				  		}
		  	  				  		else{
		  	  				  			tab[magnetometer_value_index]= Keypad_keys[i];
		  	  							magnetometer_value_index++;
		  	  				  			mySwitches[i]=false;
		  	  				  		}
		  	  				  		}
		  	  				  	if( magnetometer_value_index == 4){
		  	  				  		if((tab[3]== 69) & (tab[0] >= 48) & (tab[0]<=57)
		  	  				  				       & (tab[1] >= 48) & (tab[1]<=57)
		  										   & (tab[2] >= 48) & (tab[2]<=57)){

		  	  				  			uint16_t magnetometer_value_temp = 100*(tab[0]-48)+10*(tab[1]-48)+(tab[2]-48);
		  	  				  			if((magnetometer_value_temp<=360)& (magnetometer_value_temp >=0)){
		  	  				  				magnetometer_value = magnetometer_value_temp;
		  	  				  				heading_set = true;
		  	  				  			}

		  	  				  		}


		  	  				  		for (uint8_t z = 0; z<4; z++){
		  	  				  			tab[z]=0;
		  	  				  			}
		  	  				  		magnetometer_value_index=0;
  	  				  			  	ST7735_FillRectangleFast(0, 0, 128, 30, ST7735_BLUE);
		  	  				  		}
		  	  		  			ST7735_WriteString(0, 0, tab, Font_11x18, ST7735_WHITE, ST7735_BLACK);

		  	  				  	}
		  	  			else if(heading_set){
		  			  		mySwitches[i]=false;

		  	  			}
		  	  		}

		  	  	}
if(heading_set){
	  if(HAL_GPIO_ReadPin(Mag_Data_Ready_GPIO_Port, Mag_Data_Ready_Pin) == GPIO_PIN_SET){
		  if(QMC_read(&pusula_sensor)==0 )
			{
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
				Compas_Value=pusula_sensor.heading;
				//sprintf(tab ,"kat %f\n X %d\n Y %d\n Z %d\n"  , pusula_sensor.heading,pusula_sensor.Xaxis,pusula_sensor.Yaxis,pusula_sensor.Zaxis);
				CalibratedXaxis = scaleaxisX*((float)pusula_sensor.Xaxis - biasaxisX);
				CalibratedYaxis = scaleaxisY*((float)pusula_sensor.Yaxis - biasaxisY);
				CalibratedZaxis = scaleaxisZ*((float)pusula_sensor.Zaxis - biasaxisZ);
				Compas = atan2f(CalibratedYaxis,CalibratedXaxis)*180.00/M_PI;

						  if(Compas>0)
						  {
							  Calibrated_heading= Compas;
						  }
						  else
						  {
							  Calibrated_heading=360+Compas;
						  }

				ST7735_WriteString(0, 0, tab, Font_11x18, ST7735_WHITE, ST7735_BLACK);
			}
			else
			{
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
			}
	  }


	  MPU6050_Read_Accel();
	  MPU6050_Read_Gyro();
	  setBankLine(Az, Ay);
	  setPitchAngle(Ax, Az);
	  setPitchPoint(alfax);
	  ST7735_FillRectangleFast(0, 0, 128, 160-pitch_point, ST7735_BLUE);
	  ST7735_FillRectangleFast(0, 160-pitch_point, 128, pitch_point, ST7735_BROWN);
	  ST7735_DrawLine((60+width_point),80-height_point,60-width_point,80+height_point, ST7735_BLACK); //narysowac samolocik w stosunku do wyznaczonych punktow na okregu
	  ST7735_DrawLine((60+width_point),80-height_point,60-width_point,80+height_point, ST7735_BLACK); //narysowac samolocik w stosunku do wyznaczonych punktow na okregu
	  ST7735_DrawLine(60-triangle_point_w, 80+triangle_point_h, w/5,h/5, ST7735_BLACK);
	  ST7735_DrawLine(60-triangle_point_w, 80+triangle_point_h, minus_w/5,minus_h/5, ST7735_BLACK);
	  ST7735_DrawLine((60+width_point),80-height_point,60-width_point,80+height_point, ST7735_BLACK); //narysowac samolocik w stosunku do wyznaczonych punktow na okregu
		  ST7735_DrawLine((60+width_point),80-height_point,60-width_point,80+height_point, ST7735_BLACK); //narysowac samolocik w stosunku do wyznaczonych punktow na okregu
		  ST7735_DrawLine(60-triangle_point_w, 80+triangle_point_h, w/5,h/5, ST7735_BLACK);
		  ST7735_DrawLine(60-triangle_point_w, 80+triangle_point_h, minus_w/5,minus_h/5, ST7735_BLACK);
		  ST7735_DrawLine((60+width_point),80-height_point,60-width_point,80+height_point, ST7735_BLACK); //narysowac samolocik w stosunku do wyznaczonych punktow na okregu
			  ST7735_DrawLine((60+width_point),80-height_point,60-width_point,80+height_point, ST7735_BLACK); //narysowac samolocik w stosunku do wyznaczonych punktow na okregu
			  ST7735_DrawLine(60-triangle_point_w, 80+triangle_point_h, w/5,h/5, ST7735_BLACK);
			  ST7735_DrawLine(60-triangle_point_w, 80+triangle_point_h, minus_w/5,minus_h/5, ST7735_BLACK);
			  ST7735_DrawLine((60+width_point),80-height_point,60-width_point,80+height_point, ST7735_BLACK); //narysowac samolocik w stosunku do wyznaczonych punktow na okregu
				  ST7735_DrawLine((60+width_point),80-height_point,60-width_point,80+height_point, ST7735_BLACK); //narysowac samolocik w stosunku do wyznaczonych punktow na okregu
				  ST7735_DrawLine(60-triangle_point_w, 80+triangle_point_h, w/5,h/5, ST7735_BLACK);
				  ST7735_DrawLine(60-triangle_point_w, 80+triangle_point_h, minus_w/5,minus_h/5, ST7735_BLACK);
	setDirectionArrow(Calibrated_heading);
	drawDirectionArrow(Direction);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		}
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void setDirectionArrow(float heading){
if(magnetometer_value >=5){
	if(heading < magnetometer_value-5){
			Direction = 1; // strzalka w prawo
		}
		else if (heading > magnetometer_value+5){
			Direction = -1; // strzalka w lewo
		}
		else
			Direction = 0; // nie rysuj strzalki jak jest +-5 stopni od kierunku
		if( abs(heading - magnetometer_value) >180){
			Direction = -Direction;
		}
}


}
void drawDirectionArrow(int Direction){
	switch(Direction){
		case 0:
			break;
		case 1: drawArrowRight();
			break;
		case -1: drawArrowLeft();
			break;
	}
}
void drawArrowRight(){
	ST7735_DrawLine(80, 20, 110, 20, ST7735_RED);
	ST7735_DrawLine(110, 20,95 , 10, ST7735_RED);
	ST7735_DrawLine(110, 20,95, 30, ST7735_RED);

}
void drawArrowLeft(){
	ST7735_DrawLine(80, 20, 110, 20, ST7735_RED);
	ST7735_DrawLine(80, 20,95,10, ST7735_RED);
	ST7735_DrawLine(80, 20,95,30, ST7735_RED);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim -> Instance == TIM3){
		if(HAL_GPIO_ReadPin(R1_GPIO_Port, R1_Pin)){ ///pb5 1 wiersz
			HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, GPIO_PIN_SET);
			row = 1;
		}
		else if(HAL_GPIO_ReadPin(R2_GPIO_Port, R2_Pin)){
			HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, GPIO_PIN_SET);
			row = 2;
		}
		else if(HAL_GPIO_ReadPin(R3_GPIO_Port, R3_Pin)){
			HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, GPIO_PIN_SET);
			row = 3;
		}
		else {
			HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, GPIO_PIN_SET);
			row = 0;
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == C1_Pin){
		column = 0;
		switch(row){
			case 0 : mySwitches[column] = true; break;
			case 1 : mySwitches[4+column] = true; break;
			case 2 : mySwitches[8+column] = true; break;
			case 3 : mySwitches[12+column] = true; break;
		}
	}
	else if(GPIO_Pin == C2_Pin){
		column = 1;
		switch(row){
			case 0 : mySwitches[column] = true; break;
			case 1 : mySwitches[4+column] = true; break;
			case 2 : mySwitches[8+column] = true; break;
			case 3 : mySwitches[12+column] = true; break;
		}
	}
	else if(GPIO_Pin == C3_Pin){
		column = 2;
		switch(row){
			case 0 : mySwitches[column] = true; break;
			case 1 : mySwitches[4+column] = true; break;
			case 2 : mySwitches[8+column] = true; break;
			case 3 : mySwitches[12+column] = true; break;
		}
	}
	else if (GPIO_Pin == C4_Pin){
		column = 3;
		switch(row){
			case 0 : mySwitches[column] = true; break;
			case 1 : mySwitches[4+column] = true; break;
			case 2 : mySwitches[8+column] = true; break;
			case 3 : mySwitches[12+column] = true; break;
		}
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
