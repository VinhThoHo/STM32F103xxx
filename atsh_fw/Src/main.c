/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "iwdg.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include "u8g2.h"
#include "key.h"
#include "logo.h"
#include "main_screen.h"
#include "info_screen.h"
#include "spi.h"

int _write(int file, char *data, int len);

#define LCD_RST_1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET)
#define LCD_RST_0 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET)
#define LCD_RS_1 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define LCD_RS_0 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)

void SystemClock_Config(void);
uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);
uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void MX_GPIO_Init(void);
/*Variable for interface monitor*/

u8g2_t u8g2;
volatile uint8_t toggle = 0;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI1_Init();
  // MX_I2C1_Init();
  // MX_I2C2_Init();
  // MX_IWDG_Init();
  // MX_USART1_UART_Init();
  // MX_USART2_UART_Init();

  MX_USB_DEVICE_Init();
  KeyInit();
  u8g2_Setup_st7920_s_128x64_f(&u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi, u8g2_gpio_and_delay_stm32); // init u8g2 structure
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0); // wake up display

  LCD_RST_0;
  HAL_Delay(10);
  LCD_RST_1;

  Logo();
  HAL_Delay(3000);
  main_screen_init();
  Info_Screen_Init();
  uint32_t timeRefesh = HAL_GetTick();
  uint32_t tick = HAL_GetTick();
  // uint8_t timeSend;
  while (1)
  {
    /* timeSend = HAL_GetTick();
    u8g2_SendBuffer(&u8g2);
    printf("Time Send Buff: %d \n", HAL_GetTick() - timeSend);*/

    if (HAL_GetTick() - tick > 10000)
    {
      toggle = !toggle;
      tick = HAL_GetTick();
    }
    if (toggle)
    {
      u8g2_ClearBuffer(&u8g2);
      Main_Screen_Manage();
    }
    else
    {
      u8g2_ClearBuffer(&u8g2);
      Info_Screen_Manage();
    }
    KeyManage();

    if (HAL_GetTick() - timeRefesh > 100)
    {
      u8g2_SendBuffer(&u8g2);
      timeRefesh = HAL_GetTick();
    }
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */
uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
  //   GPIO_InitTypeDef  gpioinitstruct;

  //   switch(msg){
  //   //Function to define the logic level of the RESET line
  // case U8X8_MSG_GPIO_RESET:
  //   if (arg_int) LCD_RST_1;
  //   else LCD_RST_0;

  // break;

  //   default:
  //     return 0; //A message was received which is not implemented, return 0 to indicate an error
  // }

  return 1; // command processed successfully.
}

// uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
// {
//     GPIO_InitTypeDef  gpioinitstruct;

//     switch(msg){

//     //Function which implements a delay, arg_int contains the amount of ms
//     case U8X8_MSG_GPIO_AND_DELAY_INIT:

//     __HAL_RCC_GPIOA_CLK_ENABLE();
//     /* Configure the GPIO_LED pin */
//     gpioinitstruct.Pin    = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7;
//     gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
//     gpioinitstruct.Pull   = GPIO_NOPULL;
//     gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
//     HAL_GPIO_Init(GPIOA, &gpioinitstruct);
//     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7, GPIO_PIN_SET);

//     break;
//     //Function which implements a delay, arg_int contains the amount of ms
//     case U8X8_MSG_DELAY_MILLI:
//     HAL_Delay(arg_int);
//     break;
//     //Function which delays 10us
//     case U8X8_MSG_DELAY_10MICRO:
//     delay_us(10);
//     break;
//     //Function which delays 100ns
//     case U8X8_MSG_DELAY_100NANO:
//     __NOP();

//     break;
//     //Function to define the logic level of the clockline
//     case U8X8_MSG_GPIO_SPI_CLOCK:
//       if (arg_int) LCD_SCLK_1;
//       else LCD_SCLK_0;

//     break;
//     //Function to define the logic level of the data line to the display
//     case U8X8_MSG_GPIO_SPI_DATA:
//       if (arg_int) LCD_SID_1;
//       else LCD_SID_0;

//     break;

//     // Function to define the logic level of the CS line
//     case U8X8_MSG_GPIO_CS1:
//       if (arg_int) LCD_RS_1 ;
//       else LCD_RS_0;

//     break;
//     //Function to define the logic level of the Data/ Command line
//     case U8X8_MSG_GPIO_DC:

//     break;
//     //Function to define the logic level of the RESET line
//     case U8X8_MSG_GPIO_RESET:
//       if (arg_int) LCD_RST_1;
//       else LCD_RST_0;

//     break;

//     default:
//       return 0; //A message was received which is not implemented, return 0 to indicate an error
//   }

//   return 1; // command processed successfully.
// }

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t byte;
  uint8_t *data;

  switch (msg)
  {
  case U8X8_MSG_BYTE_SEND:
    data = (uint8_t *)arg_ptr;
    while (arg_int > 0)
    {
      byte = *data;
      data++;
      arg_int--;
      HAL_SPI_Transmit(&hspi1, &byte, 1, 100);
    }
    break;
  case U8X8_MSG_BYTE_START_TRANSFER:
    //HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
    LCD_RS_1;
    __NOP(); // 21 ns
    break;
  case U8X8_MSG_BYTE_END_TRANSFER:
    __NOP(); // 21 ns
    //HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
    LCD_RS_0;
    break;
  default:
    return 0;
  }
  return 1;
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

#ifdef USE_FULL_ASSERT
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

int _write(int file, char *data, int len)
{
  if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
  {
    errno = EBADF;
    return -1;
  }
  uint8_t status = CDC_Transmit_FS((uint8_t *)data, len);
  return status;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
