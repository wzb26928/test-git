/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2021 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#define MOTOR_A_EN_GPIO GPIOA
#define MOTOR_A_EN_PIN GPIO_PIN_0

#define MOTOR_A_CON1_GPIO GPIOA
#define MOTOR_A_CON1_PIN GPIO_PIN_2
	 
#define MOTOR_A_CON2_GPIO GPIOB
#define MOTOR_A_CON2_PIN GPIO_PIN_9


	 
#define MOTOR_B_CON1_GPIO GPIOA
#define MOTOR_B_CON1_PIN GPIO_PIN_1
	 
#define MOTOR_B_CON2_GPIO GPIOC
#define MOTOR_B_CON2_PIN GPIO_PIN_9
	 
#define MOTOR_B_EN_GPIO GPIOA
#define MOTOR_B_EN_PIN GPIO_PIN_3
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void Change_Pulse(uint16_t left,uint16_t right);
void go_left(void);
void go_right(void);
void Straight(void);
void StraightB(void);
void Stop(void);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
uint8_t c[2] = {0};
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */
		HAL_UART_Receive_IT(&huart1, c, 2);
  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

/* TIM2 init function */
void MX_TIM2_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 800;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_PWM_Init(&htim2);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 999;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);

  HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4);

}

/* USART1 init function */
void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PA1 PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB6 PB7 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart ==&huart1)
	{
		
		
		if(c[0] == 'q')
		{
			Straight();
			
		}
		else if(c[0] == 'h')
		{
			StraightB();
			
		}
		else if(c[0] == 'l')
		{
			go_left();
		
		}
		else if(c[0] == 'r')
		{
			go_right();
		
		}
		else if(c[0] == 's')
		{
			Stop();
		
		}
			
	}
}
void Change_Pulse(uint16_t left,uint16_t right)
{
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, left);
  __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, right);
	
}
//-----------------------------------------------
void go_left(void)//向左矫正
 {
	Change_Pulse(0,200);
  HAL_GPIO_WritePin(MOTOR_A_CON1_GPIO, MOTOR_A_CON1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_A_CON2_GPIO, MOTOR_A_CON2_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MOTOR_B_CON1_GPIO, MOTOR_B_CON1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_B_CON2_GPIO, MOTOR_B_CON2_PIN, GPIO_PIN_SET);
 }
//------------------------------------------------
 void go_right(void)//向右矫正
 {
	Change_Pulse(200,0);
  HAL_GPIO_WritePin(MOTOR_A_CON1_GPIO, MOTOR_A_CON1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_A_CON2_GPIO, MOTOR_A_CON2_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MOTOR_B_CON1_GPIO, MOTOR_B_CON1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_B_CON2_GPIO, MOTOR_B_CON2_PIN, GPIO_PIN_SET);
 }
//-----------------------------------------------------
void Straight(void)	//直行
{
	Change_Pulse(200,200);
	HAL_GPIO_WritePin(MOTOR_A_CON1_GPIO, MOTOR_A_CON1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_A_CON2_GPIO, MOTOR_A_CON2_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MOTOR_B_CON1_GPIO, MOTOR_B_CON1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_B_CON2_GPIO, MOTOR_B_CON2_PIN, GPIO_PIN_SET); 
}
void StraightB(void)	//直行往后
{
	Change_Pulse(200,200);
	HAL_GPIO_WritePin(MOTOR_A_CON1_GPIO, MOTOR_A_CON1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MOTOR_A_CON2_GPIO, MOTOR_A_CON2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_B_CON1_GPIO, MOTOR_B_CON1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MOTOR_B_CON2_GPIO, MOTOR_B_CON2_PIN, GPIO_PIN_RESET); 
}
void Stop(void)	//停止
{
	Change_Pulse(0,0);
	HAL_GPIO_WritePin(MOTOR_A_CON1_GPIO, MOTOR_A_CON1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_A_CON2_GPIO, MOTOR_A_CON2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_B_CON1_GPIO, MOTOR_B_CON1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_B_CON2_GPIO, MOTOR_B_CON2_PIN, GPIO_PIN_RESET);
}

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
