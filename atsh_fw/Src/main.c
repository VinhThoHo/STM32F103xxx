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
#include "button.h"

int _write(int file, char *data, int len);

#define LCD_RST_1       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,  GPIO_PIN_SET)
#define LCD_RST_0       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,  GPIO_PIN_RESET)
#define LCD_RS_1        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define LCD_RS_0        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define LCD_SCLK_1      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
#define LCD_SCLK_0      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)
#define LCD_SID_1       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET)
#define LCD_SID_0       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET)

const unsigned char logo [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0xFF, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x1F, 0xE0, 0x00, 0xFF, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x3F, 0xE0, 0x01, 0xFF, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x3F, 0xE0, 0x01, 0xFF, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x3F, 0xE0, 0x01, 0xFF, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0xFE, 0x00, 0x00, 0x7F, 0xE0, 0x03, 0xFF, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0xFE, 0x00, 0x00, 0x7F, 0xE0, 0x03, 0xFF, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xF8, 0x00, 0x7F, 0xE0, 0x03, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFE, 0x00, 0x7F, 0xE0, 0x03, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0x80, 0xFF, 0xE0, 0x03, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xC0, 0xFF, 0xE0, 0x07, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xE0, 0xFF, 0xC0, 0x07, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xE0, 0xFF, 0xC0, 0x07, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xF0, 0xFF, 0xC0, 0x0F, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xF0, 0xFF, 0xC1, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7F, 0xFE, 0x1F, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xFF, 0xFC, 0x0F, 0xF9, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xFF, 0xF8, 0x07, 0xF9, 0xFF, 0xF8, 0x0F, 0xFC, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7F, 0xF0, 0x07, 0xF9, 0xFF, 0xC0, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7F, 0xF0, 0x03, 0xF9, 0xFF, 0x80, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7F, 0xE0, 0x07, 0xF9, 0xFF, 0x80, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7F, 0xF0, 0x07, 0xF9, 0xFF, 0x00, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7F, 0xF0, 0x1F, 0xF9, 0xFF, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xFF, 0xFC, 0x7F, 0xF9, 0xFF, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xF1, 0xFE, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xF1, 0xFE, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xE1, 0xFE, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xC1, 0xFC, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0x81, 0xFC, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x01, 0xFE, 0xFF, 0xFF, 0x01, 0xFC, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x01, 0xFE, 0x1F, 0xFC, 0x01, 0xFC, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x01, 0xFC, 0x01, 0xC0, 0x01, 0xF8, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x07, 0xF0, 0x0F, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x03, 0xF8, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x03, 0xF0, 0x38, 0xE0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x03, 0xF8, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x03, 0xF0, 0x30, 0x60, 0x00, 0x00,
0x00, 0x00, 0x00, 0x03, 0xF8, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0xE0, 0x60, 0x4C, 0x00, 0x00,
0x00, 0x00, 0x00, 0x03, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x0C, 0x00, 0x00,
0x00, 0x00, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00,
0x06, 0x00, 0x1E, 0x07, 0xF0, 0x18, 0x00, 0x03, 0x00, 0x3C, 0x80, 0x00, 0xF0, 0x00, 0x00, 0x38,
0x3F, 0x80, 0xF7, 0x87, 0xF0, 0xE7, 0x03, 0xFF, 0xE0, 0xFF, 0xC7, 0x83, 0xFF, 0x3C, 0x03, 0xFE,
0x60, 0xC1, 0xC1, 0x87, 0xE1, 0x83, 0x81, 0xF0, 0xF0, 0x38, 0x01, 0x80, 0xF0, 0x0C, 0x07, 0x07,
0xE0, 0xC3, 0x80, 0x87, 0xE3, 0x81, 0xC1, 0xE0, 0x70, 0x38, 0x01, 0x80, 0x60, 0x0C, 0x0E, 0x03,
0xE0, 0x07, 0x00, 0x07, 0xE7, 0x81, 0xC1, 0xC0, 0x70, 0x38, 0x01, 0x80, 0x60, 0x0C, 0x0E, 0x00,
0x78, 0x07, 0x00, 0x07, 0xE7, 0xFF, 0xC1, 0xC0, 0x30, 0x38, 0x01, 0x80, 0x60, 0x0C, 0x1C, 0x00,
0x7F, 0x07, 0x00, 0x07, 0xE7, 0xFF, 0x81, 0xC0, 0x30, 0x38, 0x01, 0x80, 0x60, 0x0C, 0x1C, 0x00,
0x0F, 0xC7, 0x00, 0x07, 0xE7, 0x00, 0x01, 0xC0, 0x30, 0x38, 0x01, 0x80, 0x60, 0x0C, 0x1C, 0x00,
0x03, 0xE7, 0x00, 0x07, 0xE7, 0x00, 0x01, 0xC0, 0x30, 0x38, 0x01, 0x80, 0x60, 0x0C, 0x1C, 0x00,
0x00, 0xE7, 0x00, 0x0F, 0xC7, 0x80, 0x01, 0xC0, 0x30, 0x38, 0x01, 0x80, 0x60, 0x0C, 0x0E, 0x00,
0x40, 0xE3, 0x80, 0x07, 0x83, 0x80, 0x01, 0xC0, 0x30, 0x38, 0x01, 0x80, 0x60, 0x0C, 0x0E, 0x00,
0x60, 0xC3, 0xC0, 0x87, 0x83, 0xC0, 0xC1, 0xE0, 0x78, 0x1C, 0x41, 0x80, 0xF0, 0x0E, 0x0F, 0x83,
0x7F, 0x81, 0xFF, 0x83, 0x00, 0xFF, 0x87, 0xF0, 0xFC, 0x0F, 0xCF, 0xE2, 0x04, 0x3F, 0x83, 0xFE,
0x00, 0x00, 0x08, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20
};

void SystemClock_Config(void);
uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);
void MX_GPIO_Init(void);
static void delay_us(uint32_t time);
static void delay_ms(uint32_t time);
u8g2_t u8g2;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  // MX_I2C1_Init();
  // MX_I2C2_Init();
  // MX_IWDG_Init();
  // MX_USART1_UART_Init();
  // MX_USART2_UART_Init();
  MX_USB_DEVICE_Init();
  u8g2_Setup_st7920_s_128x64_f(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8g2_gpio_and_delay_stm32); // init u8g2 structure
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0); // wake up display
  u8g2_DrawBitmap(&u8g2, 0, 0, 16, 64, logo);
  u8g2_SendBuffer(&u8g2);
  //HAL_Delay(5000);
  button_init();
  uint32_t lastTime = HAL_GetTick();
  uint32_t timeRefesh = HAL_GetTick();
  uint32_t timeSend;
  while (1)
  {
    if(HAL_GetTick() - lastTime > 1000)
    {
      printf("Hello World %.2f\n", 2.5);
      lastTime = HAL_GetTick();
    }
    button_test();
    if(HAL_GetTick() - timeRefesh > 100)
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
    GPIO_InitTypeDef  gpioinitstruct;
  
    switch(msg){

    //Function which implements a delay, arg_int contains the amount of ms
    case U8X8_MSG_GPIO_AND_DELAY_INIT:

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /* Configure the GPIO_LED pin */
    gpioinitstruct.Pin    = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7;
    gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
    gpioinitstruct.Pull   = GPIO_NOPULL;
    gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpioinitstruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7, GPIO_PIN_SET);

    break;
    //Function which implements a delay, arg_int contains the amount of ms
    case U8X8_MSG_DELAY_MILLI:
    HAL_Delay(arg_int);
    break;
    //Function which delays 10us
    case U8X8_MSG_DELAY_10MICRO:
    delay_us(10);
    break;
    //Function which delays 100ns
    case U8X8_MSG_DELAY_100NANO:
    __NOP();

    break;
    //Function to define the logic level of the clockline
    case U8X8_MSG_GPIO_SPI_CLOCK:
      if (arg_int) LCD_SCLK_1;
      else LCD_SCLK_0;

    break;
    //Function to define the logic level of the data line to the display
    case U8X8_MSG_GPIO_SPI_DATA:
      if (arg_int) LCD_SID_1;
      else LCD_SID_0;

    break;

    // Function to define the logic level of the CS line
    case U8X8_MSG_GPIO_CS1:
      if (arg_int) LCD_RS_1 ;
      else LCD_RS_0;

    break;
    //Function to define the logic level of the Data/ Command line
    case U8X8_MSG_GPIO_DC:

    break;
    //Function to define the logic level of the RESET line
    case U8X8_MSG_GPIO_RESET:
      if (arg_int) LCD_RST_1;
      else LCD_RST_0;

    break;
    
    default:
      return 0; //A message was received which is not implemented, return 0 to indicate an error
  }

  return 1; // command processed successfully.
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

int _write(int file, char *data, int len)
{
  if((file != STDOUT_FILENO) && (file != STDERR_FILENO))
  {
    errno = EBADF;
    return -1;
  }
  uint8_t status = CDC_Transmit_FS((uint8_t*)data, len);
  return status; 
}

static void delay_us(uint32_t time)
{
  asm volatile("nop");
  // uint32_t i = 0;
  // while(time--)
  // {
  //   i = 10;
  //   while(i--);
  // }
}

static void delay_ms(uint32_t time)
{
  uint32_t i = 0;
  while(time--)
  {
    i = 12000;
    while(i--);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
