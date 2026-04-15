/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint16_t adc_value[100];
uint16_t adc_data[5]={0};

uint16_t sin_data[340] = {
	2066,2104,2142,2179,2217,2255,2292,2330,2367,2405,2442,2479,2516,2552,2589,2625,2662,
2698,2733,2769,2804,2839,2874,2908,2942,2976,3010,3043,3076,3108,3141,3172,3204,
3235,3265,3296,3325,3355,3384,3412,3440,3467,3494,3521,3547,3572,3597,3622,3646,
3669,3692,3714,3736,3757,3777,3797,3817,3835,3853,3871,3888,3904,3920,3935,3949,
3963,3976,3988,4000,4011,4021,4031,4040,4048,4056,4063,4069,4074,4079,4083,4087,
4090,4092,4093,4094,4094,4093,4092,4090,4087,4083,4079,4074,4069,4063,4056,4048,
4040,4031,4021,4011,4000,3988,3976,3963,3949,3935,3920,3904,3888,3871,3853,3835,
3817,3797,3777,3757,3736,3714,3692,3669,3646,3622,3597,3572,3547,3521,3494,3467,
3440,3412,3384,3355,3325,3296,3265,3235,3204,3172,3141,3108,3076,3043,3010,2976,
2942,2908,2874,2839,2804,2769,2733,2698,2662,2625,2589,2552,2516,2479,2442,2405,
2367,2330,2292,2255,2217,2179,2142,2104,2066,2028,1990,1952,1915,1877,1839,1802,
1764,1727,1689,1652,1615,1578,1542,1505,1469,1432,1396,1361,1325,1290,1255,1220,
1186,1152,1118,1084,1051,1018,986,953,922,890,859,829,798,769,739,710,
682,654,627,600,573,547,522,497,472,448,425,402,380,358,337,317,
297,277,259,241,223,206,190,174,159,145,131,118,106,94,83,73,
63,54,46,38,31,25,20,15,11,7,4,2,1,0,0,1,
2,4,7,11,15,20,25,31,38,46,54,63,73,83,94,106,
118,131,145,159,174,190,206,223,241,259,277,297,317,337,358,380,
402,425,448,472,497,522,547,573,600,627,654,682,710,739,769,798,
829,859,890,922,953,986,1018,1051,1084,1118,1152,1186,1220,1255,1290,1325,
1361,1396,1432,1469,1505,1542,1578,1615,1652,1689,1727,1764,1802,1839,1877,1915,
1952,1990,2028};

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
  MX_DMA_Init();
  MX_ADC2_Init();
  MX_DAC2_Init();
  MX_TIM6_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
	
	HAL_TIM_Base_Start(&htim6);
	HAL_DAC_Start_DMA(&hdac2,DAC2_CHANNEL_1,(uint32_t*)sin_data,340,DAC_ALIGN_12B_R);
	
	HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_Start(&hadc2,ADC_SINGLE_ENDED);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc_value,100);
	HAL_ADC_Start_DMA(&hadc2,(uint32_t*)adc_data,5);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) 
{
	if (hadc == &hadc1) {
		HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc_value,100);
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
