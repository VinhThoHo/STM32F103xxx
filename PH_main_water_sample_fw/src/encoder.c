#include "encoder.h"
#include "debug.h"
#include "motor.h"
#include <libopencm3/cm3/nvic.h>


// ENCODER_STRUCT ENCX;
// ENCODER_STRUCT ENCY;

volatile uint16_t cntx = 0;
volatile uint16_t cnty = 0;
volatile uint16_t cntz = 0;


/*Setup tim3 encoder1*/
void tim3_setup(void)
{
	rcc_periph_clock_enable(RCC_TIM3);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_AFIO);
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO6 | GPIO7);

	//timer_reset(TIM3);
	timer_set_period(TIM3, 500);
	timer_slave_set_mode(TIM3, 0x3); // encoder
	timer_ic_set_input(TIM3, TIM_IC1, TIM_IC_IN_TI1);
	timer_ic_set_input(TIM3, TIM_IC2, TIM_IC_IN_TI2);
	timer_ic_set_filter(TIM3, TIM_IC1, TIM_IC_CK_INT_N_4);
	timer_ic_set_filter(TIM3, TIM_IC2, TIM_IC_CK_INT_N_4);
	// timer_ic_set_filter(TIM3, TIM_IC1, TIM_IC_DTF_DIV_32_N_8);
	// timer_ic_set_filter(TIM3, TIM_IC2, TIM_IC_DTF_DIV_32_N_8);
	timer_set_counter(TIM3, 0);
	nvic_enable_irq(NVIC_TIM3_IRQ);
	timer_enable_counter(TIM3);
	timer_enable_irq(TIM3, TIM_DIER_UIE);
}

/*setup tim4 encoder2*/
void tim4_setup(void)
{
	rcc_periph_clock_enable(RCC_TIM4);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_AFIO);

	gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO6|GPIO7);

	//timer_reset(TIM4);
	timer_set_period(TIM4, 500);
	timer_slave_set_mode(TIM4, 0x3);
	timer_ic_set_input(TIM4, TIM_IC1, TIM_IC_IN_TI1);
	timer_ic_set_input(TIM4, TIM_IC1, TIM_IC_IN_TI2);

	timer_ic_set_filter(TIM4, TIM_IC1, TIM_IC_CK_INT_N_4);
	timer_ic_set_filter(TIM4, TIM_IC2, TIM_IC_CK_INT_N_4);
	// timer_ic_set_filter(TIM4, TIM_IC1, TIM_IC_DTF_DIV_32_N_8);
	// timer_ic_set_filter(TIM4, TIM_IC2, TIM_IC_DTF_DIV_32_N_8);

	timer_set_counter(TIM4, 0);

	nvic_enable_irq(NVIC_TIM4_IRQ);
	timer_enable_counter(TIM4);
	timer_enable_irq(TIM4, TIM_DIER_UIE);
}


/*setup tim5 encoder3 */
void tim5_setup(void)
{
	rcc_periph_clock_enable(RCC_TIM5);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_AFIO);

	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO0|GPIO1);

	//timer_reset(TIM5);
	timer_set_period(TIM5, 500);
	timer_slave_set_mode(TIM5, 0x3);
	timer_ic_set_input(TIM5, TIM_IC1, TIM_IC_IN_TI1);
	timer_ic_set_input(TIM5, TIM_IC1, TIM_IC_IN_TI2);

	timer_ic_set_filter(TIM5, TIM_IC1, TIM_IC_CK_INT_N_4);
	timer_ic_set_filter(TIM5, TIM_IC2, TIM_IC_CK_INT_N_4);

	timer_set_counter(TIM5, 0);

	nvic_enable_irq(NVIC_TIM5_IRQ);
	timer_enable_counter(TIM5);
	timer_enable_irq(TIM5, TIM_DIER_UIE);
}

void encoder_init(void)
{
	tim3_setup(); //encoder motorX
	tim4_setup(); //encoder motorY
	tim5_setup();
}

// void cal_turn(uint8_t motor)
// {
// 	if(motor == 0)
// 	{
// 		ENCX.turn = (uint32_t)cntx/27;
// 	}
// 	if(motor == 1)
// 	{
// 		ENCY.turn = (uint32_t)cnty/27;
// 	}
// }

// void encodersDir(void)
// {
// 	cnt_current = timer_get_counter(TIM3);
// 	if((cnt_current - cnt_old) > 0) INFO("THUAN\n");
// 	if((cnt_old - cnt_current) > 0) INFO("NGHICH\n");
// }

void tim3_isr(void)
{
	if(timer_get_flag(TIM3, TIM_SR_UIF))
	{
		timer_clear_flag(TIM3, TIM_SR_UIF);
		//ENCX.dir = (TIM3_CR1 & (TIM3_CR1<<4))?1:0;
		//INFO("X_dir: %d\n", ENCX.dir);
		if(++cntx >= 1400) cntx = 0;
	}
}

void tim4_isr(void)
{
	if(timer_get_flag(TIM4, TIM_SR_UIF))
	{
		timer_clear_flag(TIM4, TIM_SR_UIF);
		//ENCY.dir = TIM4_CR1 & (TIM4_CR1<<4);
		//INFO("Y_dir: %d\n", ENCY.dir);
		//INFO("cnty: %d \n", cnty);
		if(++cnty >= 1400) cnty = 0;
	}
}

void tim5_isr(void)
{
	if(timer_get_flag(TIM5, TIM_SR_UIF))
	{
		timer_clear_flag(TIM5, TIM_SR_UIF);
		//ENCY.dir = TIM5_CR1 & (TIM5_CR1<<4);
		//INFO("Y_dir: %d\n", ENCY.dir);
		if(++cntz >= 1400) cntz = 0;
	}
}


// void init_timer_encode(void)
// {
// 	/* init encoder 1*/
// 	rcc_periph_clock_enable(RCC_TIM3);
// 	rcc_periph_clock_enable(RCC_GPIOA);
// 	rcc_periph_clock_enable(RCC_AFIO);
// 	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO6 | GPIO7);

// 	timer_reset(TIM3);
// 	timer_set_period(TIM3, 500);
// 	timer_slave_set_mode(TIM3, 0x3); // encoder
// 	timer_ic_set_input(TIM3, TIM_IC1, TIM_IC_IN_TI1);
// 	timer_ic_set_input(TIM3, TIM_IC2, TIM_IC_IN_TI2);
// 	// timer_ic_set_filter(TIM3, TIM_IC1, );
// 	// timer_ic_set_filter(TIM3, TIM_IC2, );
// 	nvic_enable_irq(NVIC_TIM3_IRQ);
// 	timer_enable_counter(TIM3);
// 	timer_enable_irq(TIM3, TIM_DIER_UIE);

// 	/* init encoder 2*/
// 	rcc_periph_clock_enable(RCC_TIM4);
// 	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO0|GPIO1);

// 	timer_reset(TIM4);
// 	timer_set_period(TIM4, 500);
// 	timer_slave_set_mode(TIM4, 0x3);
// 	timer_ic_set_input(TIM4, TIM_IC1, TIM_IC_IN_TI1);
// 	timer_ic_set_input(TIM4, TIM_IC1, TIM_IC_IN_TI2);

// 	nvic_enable_irq(NVIC_TIM4_IRQ);
// 	timer_enable_counter(TIM4);
// 	timer_enable_irq(TIM4, TIM_DIER_UIE);
// }

