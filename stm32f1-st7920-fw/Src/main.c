#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include "main.h"
#include "stm32f1xx_hal.h"
#include "usb_device.h"
#include "u8g2.h"
#include "display.h"

#define LCD_RST_1       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,  GPIO_PIN_SET)
#define LCD_RST_0       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,  GPIO_PIN_RESET)
#define LCD_RS_1        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define LCD_RS_0        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define LCD_SCLK_1      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
#define LCD_SCLK_0      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)
#define LCD_SID_1       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET)
#define LCD_SID_0       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET)

void delay_us(uint32_t time)
{
  uint32_t i = 0;
  while(time--)
  {
    i = 10;
    while(i--);
  }
}

void delay_ms(uint32_t time)
{
  uint32_t i = 0;
  while(time--)
  {
    i = 12000;
    while(i--);
  }
}

void SystemClock_Config(void);
uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);
static void MX_GPIO_Init(void);
void display_logo();

static u8g2_t *u8g2;
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  //MX_USB_DEVICE_Init();
  u8g2_Setup_st7920_s_128x64_f(&u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8g2_gpio_and_delay_stm32); // init u8g2 structure
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0); // wake up display
  display_logo();
  HAL_Delay(10000);
  uint32_t timeRefesh = HAL_GetTick();
  while (1)
  {
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFontMode(&u8g2, 1);
    u8g2_SetFontDirection(&u8g2, 0);
    // u8g2_SetFont(&u8g2, u8g2_font_helvB18_te);
    // u8g2_DrawStr(&u8g2,  0, 24, "HelloWorld");
    // u8g2_DrawStr(&u8g2,  0, 53, "i'm vinhtho");
    // display_menu();
    // u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);
    // u8g2_SetFontRefHeightAll(&u8g2);   /* this will add some extra space for the text inside the buttons */
    // u8g2_UserInterfaceMessage(&u8g2, "Title1", "Title2", "Title3", " Ok \n Cancel ");
    //u8g2_DrawHorizontalBitmap(&u8g2, 32, 32, 64, logo);

    //Refresh display
    if(HAL_GetTick() - timeRefesh > 100)
    {
      u8g2_SendBuffer(&u8g2);
      timeRefesh = HAL_GetTick();
    }
  }
}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();



  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
  __HAL_AFIO_REMAP_SWJ_ENABLE();

}

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


void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */