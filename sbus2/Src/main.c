/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//SBUS variable
#define SBUS_BUF_MAX	32
#define SBUS_BUF_NUM	25
#define SBUS_CH_NUM	16
//#define SBUS_CH_NUM	4

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

//SBUS variable
uint8_t SBUS_TEMP;
int32_t head_sbus_ptr;
int32_t tail_sbus_ptr;
uint8_t sbus_buff[SBUS_BUF_NUM]; // implemented as a circular buffer
uint8_t channel_data[SBUS_BUF_NUM];
int16_t channels[SBUS_CH_NUM]  = {0,};
//int16_t channels[SBUS_CH_NUM]  = {1023,1023,1023,0};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void FUTABA_Receive(void);

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

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */

	HAL_UART_Receive_IT(&huart2, &SBUS_TEMP, 1);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		FUTABA_Receive();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 100000;
  huart2.Init.WordLength = UART_WORDLENGTH_9B;
  huart2.Init.StopBits = UART_STOPBITS_2;
  huart2.Init.Parity = UART_PARITY_EVEN;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_TXINVERT_INIT|UART_ADVFEATURE_RXINVERT_INIT;
  huart2.AdvancedInit.TxPinLevelInvert = UART_ADVFEATURE_TXINV_ENABLE;
  huart2.AdvancedInit.RxPinLevelInvert = UART_ADVFEATURE_RXINV_ENABLE;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	if(huart->Instance == USART2)
	{
		register uint8_t S_T_TEMP = SBUS_TEMP;
		register int32_t head_sbus_ptr_temp = head_sbus_ptr;
		
		if((S_T_TEMP == 0x0F)&&(sbus_buff[0] != 0x0F))
		{
			head_sbus_ptr_temp=0;
			sbus_buff[head_sbus_ptr_temp]=S_T_TEMP;
		}
		
		if(sbus_buff[0] == 0x0F )
		{
			sbus_buff[head_sbus_ptr_temp] = S_T_TEMP;
			if(++head_sbus_ptr_temp>sizeof(sbus_buff))
			{
				head_sbus_ptr_temp = 0;
			}
		}
		
		head_sbus_ptr = head_sbus_ptr_temp;
		HAL_UART_Receive_IT(&huart2, &SBUS_TEMP, 1);
	}
}

void FUTABA_Receive(void)
{
	int32_t h_s_p = head_sbus_ptr;
	int32_t t_s_p = tail_sbus_ptr;
	
	if((h_s_p - t_s_p) > SBUS_BUF_NUM-1 )
		 {
			   if( (sbus_buff[t_s_p] == 0x0F) && (
                        (sbus_buff[t_s_p + SBUS_BUF_NUM-1] == 0x00) ||  //00 Non-Telemetry
                        (sbus_buff[t_s_p + SBUS_BUF_NUM-1] == 0x04) ||  //04 First Main
                        (sbus_buff[t_s_p + SBUS_BUF_NUM-1] == 0x14) ||  //14 Second Main
                        (sbus_buff[t_s_p + SBUS_BUF_NUM-1] == 0x24) ||  //24 Third Main
                        (sbus_buff[t_s_p + SBUS_BUF_NUM-1] == 0x34) ))   //34 Forth Main
				 {
					 
					 for (uint32_t i=0; i<SBUS_BUF_NUM; i++)
           {
             channel_data[i] = sbus_buff[i];
             sbus_buff[i] = 0;
           }
					 
				 }
				 
				 for (uint32_t i=0; i<SBUS_CH_NUM; i++)
						channels[i] = 0;
				 
				//FUTABA_Trans
			 
				channels[ 0] = ((int16_t)channel_data[ 1] >> 0 | ((int16_t)channel_data[ 2] << 8 )) & 0x07FF;	
				channels[ 1] = ((int16_t)channel_data[ 2] >> 3 | ((int16_t)channel_data[ 3] << 5 )) & 0x07FF;	
				channels[ 2] = ((int16_t)channel_data[ 3] >> 6 | ((int16_t)channel_data[ 4] << 2 )  | (int16_t)channel_data[ 5] << 10 ) & 0x07FF;	
				channels[ 3] = ((int16_t)channel_data[ 5] >> 1 | ((int16_t)channel_data[ 6] << 7 )) & 0x07FF;	
				channels[ 4] = ((int16_t)channel_data[ 6] >> 4 | ((int16_t)channel_data[ 7] << 4 )) & 0x07FF;
				channels[ 5] = ((int16_t)channel_data[ 7] >> 7 | ((int16_t)channel_data[ 8] << 1 )  | (int16_t)channel_data[9] <<  9 ) & 0x07FF;
				channels[ 6] = ((int16_t)channel_data[ 9] >> 2 | ((int16_t)channel_data[10] << 6 )) & 0x07FF;
				channels[ 7] = ((int16_t)channel_data[10] >> 5 | ((int16_t)channel_data[11] << 3 )) & 0x07FF;
				channels[ 8] = ((int16_t)channel_data[12] << 0 | ((int16_t)channel_data[13] << 8 )) & 0x07FF;
				channels[ 9] = ((int16_t)channel_data[13] >> 3 | ((int16_t)channel_data[14] << 5 )) & 0x07FF;
				channels[10] = ((int16_t)channel_data[14] >> 6 | ((int16_t)channel_data[15] << 2 )  | (int16_t)channel_data[16] << 10 ) & 0x07FF;
				channels[11] = ((int16_t)channel_data[16] >> 1 | ((int16_t)channel_data[17] << 7 )) & 0x07FF;
				channels[12] = ((int16_t)channel_data[17] >> 4 | ((int16_t)channel_data[18] << 4 )) & 0x07FF;
				channels[13] = ((int16_t)channel_data[18] >> 7 | ((int16_t)channel_data[19] << 1 )  | (int16_t)channel_data[20] <<  9 ) & 0x07FF;
				channels[14] = ((int16_t)channel_data[20] >> 2 | ((int16_t)channel_data[21] << 6 )) & 0x07FF;
				channels[15] = ((int16_t)channel_data[21] >> 5 | ((int16_t)channel_data[22] << 3 )) & 0x07FF;


				//---------------------------------------------------------------------------------------------------------------------------
				 
				 
				 head_sbus_ptr = 0;
         tail_sbus_ptr = 0;
         for (uint32_t i=0; i<SBUS_BUF_MAX-1; i++)
					sbus_buff[i] = 0;
				 
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
