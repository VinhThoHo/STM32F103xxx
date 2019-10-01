#include "BlinkLed.h"

uint32_t tick;
void Led_Init(void)
{
      GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

  tick = HAL_GetTick();
}

void Blink(void)
{
    if(HAL_GetTick() - tick >1000)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
        tick = HAL_GetTick();
    }
}