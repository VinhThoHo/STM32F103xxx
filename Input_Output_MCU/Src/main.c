#include "main.h"
#include "hd44780.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
// HD44780_Puts(0, 0, "TEST LCD");

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  
  
  while (1)
  {
    Input_Output();
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_AFIO_REMAP_SWJ_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Ctrl1_Pin|Ctrl2_Pin|Ctrl3_Pin|Ctrl4_Pin 
                          |Ctrl5_Pin|Ctrl6_Pin|Ctrl7_Pin|Ctrl8_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : In3_Pin In4_Pin In5_Pin In6_Pin 
                           In1_Pin In2_Pin */
  GPIO_InitStruct.Pin = In3_Pin|In4_Pin|In5_Pin|In6_Pin 
                          |In1_Pin|In2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : Led_Pin */
  GPIO_InitStruct.Pin = Led_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(Led_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Ctrl1_Pin Ctrl2_Pin Ctrl3_Pin Ctrl4_Pin 
                           Ctrl5_Pin Ctrl6_Pin Ctrl7_Pin Ctrl8_Pin */
  GPIO_InitStruct.Pin = Ctrl1_Pin|Ctrl2_Pin|Ctrl3_Pin|Ctrl4_Pin 
                          |Ctrl5_Pin|Ctrl6_Pin|Ctrl7_Pin|Ctrl8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void Input_Output(){
  switch(HAL_GPIO_ReadPin(In1_GPIO_Port, In1_Pin)){
    case 1:
      HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(Ctrl1_GPIO_Port, Ctrl1_Pin, GPIO_PIN_SET);
      break;
    case 0:
      HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(Ctrl1_GPIO_Port, Ctrl1_Pin, GPIO_PIN_RESET);
      break;
  }
  switch(HAL_GPIO_ReadPin(In2_GPIO_Port, In2_Pin)){
    case 1:
      HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(Ctrl2_GPIO_Port, Ctrl2_Pin, GPIO_PIN_SET);
      break;
    case 0:
      HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(Ctrl2_GPIO_Port, Ctrl2_Pin, GPIO_PIN_RESET);
      break;
  }
  switch(HAL_GPIO_ReadPin(In3_GPIO_Port, In3_Pin)){
    case 1:
      HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(Ctrl3_GPIO_Port, Ctrl3_Pin, GPIO_PIN_SET);
      break;
    case 0:
      HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(Ctrl3_GPIO_Port, Ctrl3_Pin, GPIO_PIN_RESET);
      break;
  }
  switch(HAL_GPIO_ReadPin(In4_GPIO_Port, In4_Pin)){
    case 1:
      HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(Ctrl4_GPIO_Port, Ctrl4_Pin, GPIO_PIN_SET);
      break;
    case 0:
      HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(Ctrl4_GPIO_Port, Ctrl4_Pin, GPIO_PIN_RESET);
      break;
  }
  switch(HAL_GPIO_ReadPin(In5_GPIO_Port, In5_Pin)){
    case 1:
      HAL_GPIO_WritePin(Ctrl5_GPIO_Port, Ctrl5_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(Ctrl7_GPIO_Port, Ctrl7_Pin, GPIO_PIN_SET);
      break;
    case 0:
      HAL_GPIO_WritePin(Ctrl5_GPIO_Port, Ctrl5_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(Ctrl7_GPIO_Port, Ctrl7_Pin, GPIO_PIN_RESET);
      break;
  }
  switch(HAL_GPIO_ReadPin(In6_GPIO_Port, In6_Pin)){
    case 1:
      HAL_GPIO_WritePin(Ctrl6_GPIO_Port, Ctrl6_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(Ctrl8_GPIO_Port, Ctrl8_Pin, GPIO_PIN_SET);
      break;
    case 0:
      HAL_GPIO_WritePin(Ctrl6_GPIO_Port, Ctrl6_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(Ctrl8_GPIO_Port, Ctrl8_Pin, GPIO_PIN_RESET);
      break;
  }
}

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
