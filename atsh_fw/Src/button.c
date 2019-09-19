#include "button.h"
#include "u8g2.h"

extern u8g2_t u8g2;
uint32_t t;

void button_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pins : PC13 PC10 PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  t = HAL_GetTick();
}

uint8_t get_button(void)
{
  if(key1 == 0)
    return 1;
  if(key2 == 0)
    return 2;
  return 0;
}
void button_test(void)
{
  if(HAL_GetTick()-t >500)
  {
    uint8_t key = 0;
    key = get_button();
    if(key == 1)
      printf("Button 1 Press\n");
     if(key == 2)
      printf("Button 2 Press\n");
    t = HAL_GetTick();
  }
}

// void Display(void)
// {
//     if(SW1 == 0){
//       if(HAL_GetTick()-t >500){
//         u8g2_SetFont(&u8g2, u8g2_font_6x10_tf);
//         u8g2_SetFontRefHeightAll(&u8g2);   /* this will add some extra space for the text inside the buttons */
//         u8g2_UserInterfaceMessage(&u8g2, "UV", "Light", "Fan", " Ok \n Cancel ");
//         u8g2_SendBuffer(&u8g2);
//         t = HAL_GetTick();
//       }
//     }
//     else{
//       t= HAL_GetTick();
//     }
// }