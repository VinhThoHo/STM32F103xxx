#include "led.h"

void gpio_init(void){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = atsh_led1_pin|atsh_led2_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
void led_init(void){
    atsh_set_led1();
    atsh_set_led2();
}
void time(){
    HAL_Delay(1000);
}