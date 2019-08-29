#include "motor.h"
#include <libopencm3/stm32/timer.h>
#include "debug.h"

MOTOR_STRUCT motor[3];

void tim1_setup(void)
{
  rcc_periph_clock_enable(RCC_TIM1);
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_AFIO);
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                GPIO8 | GPIO9 | GPIO10);
  //timer_reset(TIM1);
  timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_CENTER_1, TIM_CR1_DIR_UP);
  timer_set_prescaler(TIM1, 17);

  timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM2);
  // timer_enable_oc_output(TIM1, TIM_OC1);
  timer_disable_oc_output(TIM1, TIM_OC1);
  timer_disable_oc_output(TIM1, TIM_OC1N);

  timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM2);
  // timer_enable_oc_output(TIM1, TIM_OC2);
  timer_disable_oc_output(TIM1, TIM_OC2);
  timer_disable_oc_output(TIM1, TIM_OC2N);

  timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM2);
  // timer_enable_oc_output(TIM1, TIM_OC3);
  timer_disable_oc_output(TIM1, TIM_OC3);
  timer_disable_oc_output(TIM1, TIM_OC3N);

  timer_enable_break_main_output(TIM1);
  //timer_set_oc_polarity_high(TIM1, TIM_OC2N);
  timer_enable_preload(TIM1);
  timer_set_period(TIM1, 100);

  timer_disable_oc_preload(TIM1, TIM_OC1);
  timer_set_oc_value(TIM1, TIM_OC1, (100*50)/100);
  timer_set_oc_value(TIM1, TIM_OC1, 0);

  timer_disable_oc_preload(TIM1, TIM_OC2);
  timer_set_oc_value(TIM1, TIM_OC2, (100*50)/100);
  timer_set_oc_value(TIM1, TIM_OC2, 0);

  timer_disable_oc_preload(TIM1, TIM_OC3);
  timer_set_oc_value(TIM1, TIM_OC3, (100*50)/100);
  timer_set_oc_value(TIM1, TIM_OC3, 0);

  timer_generate_event(TIM1, TIM_EGR_UG);
  //timer_disable_counter(TIM1);
  timer_enable_counter(TIM1);
}

void motor_init(void) /*STOP*/
{
	uint8_t i;
	motor[MOTORX].portPWM = portPWM_MT;
	motor[MOTORX].portDIR = portDIR_MT;
	motor[MOTORX].pinPWM = pinPWM_MT1;
	motor[MOTORX].pinDIR = pinDIR_MT1;
	motor[MOTORY].portPWM = portPWM_MT;
	motor[MOTORY].portDIR = portDIR_MT;
	motor[MOTORY].pinPWM = pinPWM_MT2;
	motor[MOTORY].pinDIR = pinDIR_MT2;
	motor[MOTORZ].portPWM = portPWM_MT;
	motor[MOTORZ].portDIR = portDIR_MT;
	motor[MOTORZ].pinPWM = pinPWM_MT3;
	motor[MOTORZ].pinDIR = pinDIR_MT3;
	rcc_periph_clock_enable(RCC_GPIOB);
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
			GPIO_CNF_OUTPUT_PUSHPULL, pinDIR_MT1 | pinDIR_MT2 | pinDIR_MT3);
	gpio_clear(GPIOB, motor[MOTORX].pinDIR);
	gpio_clear(GPIOB, motor[MOTORY].pinDIR);
	gpio_clear(GPIOB, motor[MOTORX].pinDIR);
	for(i = 0; i<sizeof(motor); i++)
	{
		motor[i].mode = STOP;
	}
	tim1_setup();
}

void motor_setup_mode(MOTOR_ID motor_id, MOTOR_MODE mode)
{
	switch(motor_id)
	{
		case MOTORX:
			motor[MOTORX].mode = mode;
			break;
		case MOTORY:
			motor[MOTORY].mode = mode;
			break;
		case MOTORZ:
			motor[MOTORZ].mode = mode;
			break;
		default:
			break;
	}
}

void motor_setup_speed(MOTOR_ID motor_id, MOTOR_SPEED speed)
{
	switch(motor_id)
	{
		case MOTORX:
			motor[MOTORX].speed = speed;
			break;
		case MOTORY:
			motor[MOTORY].speed = speed;
			break;
		case MOTORZ:
			motor[MOTORZ].speed = speed;
			break;
		default:
			break;
	}
}

void motor_run_forward(MOTOR_ID motor_id)
{	
	switch(motor_id)
	{
		case MOTORX:
			timer_enable_oc_output(TIM1, TIM_OC1);
			timer_enable_oc_preload(TIM1, TIM_OC1);
			timer_set_oc_value(TIM1, TIM_OC1, motor[MOTORX].speed);
			gpio_clear(motor[MOTORX].portDIR, motor[MOTORX].pinDIR);

			// timer_disable_oc_output(TIM1, TIM_OC1);
			// timer_disable_oc_preload(TIM1, TIM_OC1);
			break;

		case MOTORY:
			timer_enable_oc_output(TIM1, TIM_OC2);
			timer_enable_oc_preload(TIM1, TIM_OC2);
			timer_set_oc_value(TIM1, TIM_OC2, motor[MOTORY].speed);
			gpio_clear(motor[MOTORY].portDIR, motor[MOTORY].pinDIR);

			// timer_disable_oc_output(TIM1, TIM_OC2);
			// timer_disable_oc_preload(TIM1, TIM_OC2);

			break;

		case MOTORZ:
			timer_enable_oc_output(TIM1, TIM_OC3);
			timer_enable_oc_preload(TIM1, TIM_OC3);
			timer_set_oc_value(TIM1, TIM_OC3, motor[MOTORZ].speed);
			gpio_clear(motor[MOTORZ].portDIR, motor[MOTORZ].pinDIR);

			// timer_disable_oc_output(TIM1, TIM_OC3);
			// timer_disable_oc_preload(TIM1, TIM_OC3);
			break;
		default:
			break;
	}
}

void motor_run_reverse(MOTOR_ID motor_id)
{	
	switch(motor_id)
	{
		case MOTORX:
			
			// timer_disable_oc_output(TIM1, TIM_OC1);
			// timer_disable_oc_preload(TIM1, TIM_OC1);
			timer_enable_oc_output(TIM1, TIM_OC1);
			timer_enable_oc_preload(TIM1, TIM_OC1);
			timer_set_oc_value(TIM1, TIM_OC1, motor[MOTORX].speed);
			gpio_set(motor[MOTORX].portDIR, motor[MOTORX].pinDIR);
			break;

		case MOTORY:
			// timer_disable_oc_output(TIM1, TIM_OC2);
			// timer_disable_oc_preload(TIM1, TIM_OC2);
			timer_enable_oc_output(TIM1, TIM_OC2);
			timer_enable_oc_preload(TIM1, TIM_OC2);
			timer_set_oc_value(TIM1, TIM_OC2, motor[MOTORY].speed);
			gpio_set(motor[MOTORY].portDIR, motor[MOTORY].pinDIR);

			break;
		case MOTORZ:
			// timer_disable_oc_output(TIM1, TIM_OC3);
			// timer_disable_oc_preload(TIM1, TIM_OC3);
			timer_enable_oc_output(TIM1, TIM_OC3);
			timer_enable_oc_preload(TIM1, TIM_OC3);
			timer_set_oc_value(TIM1, TIM_OC3, motor[MOTORZ].speed);
			gpio_set(motor[MOTORZ].portDIR, motor[MOTORZ].pinDIR);
			break;
		default:
			break;
	}
}

void motor_stop(MOTOR_ID motor_id)
{
	switch(motor_id)
	{
		case MOTORX:
			timer_disable_oc_output(TIM1, TIM_OC1);
			timer_disable_oc_preload(TIM1, TIM_OC1);
			//gpio_set(motor[MOTORX].portDIR, motor[MOTORX].pinDIR);
			break;

		case MOTORY:
			timer_disable_oc_output(TIM1, TIM_OC2);
			timer_disable_oc_preload(TIM1, TIM_OC2);
			//gpio_set(motor[MOTORY].portDIR, motor[MOTORY].pinDIR);
			break;

		case MOTORZ:
			timer_disable_oc_output(TIM1, TIM_OC3);
			timer_disable_oc_preload(TIM1, TIM_OC3);
			//gpio_set(motor[MOTORZ].portDIR, motor[MOTORZ].pinDIR);

			break;
		default:
			break;
	}
}

void motor_fast_stop(MOTOR_ID motor_id)
{
	switch(motor_id)
	{
		case MOTORX:
			
			timer_disable_oc_output(TIM1, TIM_OC1);
			timer_disable_oc_preload(TIM1, TIM_OC1);
			gpio_clear(motor[MOTORX].portDIR, motor[MOTORX].pinDIR);
			break;

		case MOTORY:

			timer_disable_oc_output(TIM1, TIM_OC2);
			timer_disable_oc_preload(TIM1, TIM_OC2);
			gpio_clear(motor[MOTORY].portDIR, motor[MOTORY].pinDIR);

			break;

		case MOTORZ:
			timer_disable_oc_output(TIM1, TIM_OC3);
			timer_disable_oc_preload(TIM1, TIM_OC3);
			gpio_clear(motor[MOTORZ].portDIR, motor[MOTORZ].pinDIR);
			break;
		default:
			break;
	}
}

void motor_run(MOTOR_ID motor_id, MOTOR_MODE mode)
{
	motor_setup_mode(motor_id, mode);
	switch(motor[motor_id].mode)
	{
		case STOP:
			motor_fast_stop(motor_id);
			break;
		case FORWARD:
			motor_run_forward(motor_id);
			break;
		case REVERSE:
			motor_run_reverse(motor_id);
			break;
		default:
			break;
	}
}

// void tim3_setup(void)
// {
//   //rcc_periph_clock_enable(RCC_TIM3);
//   // rcc_periph_clock_enable(RCC_GPIOA);
//   // rcc_periph_clock_enable(RCC_GPIOB);
//   // rcc_periph_clock_enable(RCC_AFIO);
//   // gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
//   //               GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
//   //               GPIO6 | GPIO7);
//   // gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
//   //               GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
//   //               GPIO0);
//   timer_reset(TIM3);
//   timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_CENTER_1, TIM_CR1_DIR_UP);
//   timer_set_prescaler(TIM3, 17);

//   timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_PWM1);
//   timer_disable_oc_output(TIM3, TIM_OC1);
//   timer_disable_oc_output(TIM3, TIM_OC1N);

//   timer_set_oc_mode(TIM3, TIM_OC2, TIM_OCM_PWM1);
//   timer_disable_oc_output(TIM3, TIM_OC2);
//   timer_disable_oc_output(TIM3, TIM_OC2N);

//   timer_set_oc_mode(TIM3, TIM_OC3, TIM_OCM_PWM1);
//   timer_disable_oc_output(TIM3, TIM_OC3);
//   timer_disable_oc_output(TIM3, TIM_OC3N);

//    timer_enable_break_main_output(TIM3);
//   //timer_set_oc_polarity_high(TIM1, TIM_OC2N);
//   timer_enable_preload(TIM3);
//   timer_set_period(TIM3, 100);

//   timer_disable_oc_preload(TIM3, TIM_OC1);
//   timer_set_oc_value(TIM3, TIM_OC1, 0);
//   timer_disable_oc_preload(TIM3, TIM_OC2);
//   timer_set_oc_value(TIM3, TIM_OC2, 0);
//   timer_disable_oc_preload(TIM3, TIM_OC3);
//   timer_set_oc_value(TIM3, TIM_OC3, 0);

//   timer_generate_event(TIM3, TIM_EGR_UG);
//   timer_enable_counter(TIM3);
// }

// void tim1_setup(void)
// {
//   //rcc_periph_clock_enable(RCC_TIM1);
//   // rcc_periph_clock_enable(RCC_GPIOA);
//   // rcc_periph_clock_enable(RCC_AFIO);
//   // gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
//   //               GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
//   //               GPIO8 | GPIO9 | GPIO10);
//   timer_reset(TIM1);
//   timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_CENTER_1, TIM_CR1_DIR_UP);
//   timer_set_prescaler(TIM1, 17);

//   timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
//   // timer_enable_oc_output(TIM1, TIM_OC1);
//   timer_disable_oc_output(TIM1, TIM_OC1);
//   timer_disable_oc_output(TIM1, TIM_OC1N);

//   timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
//   // timer_enable_oc_output(TIM1, TIM_OC2);
//   timer_disable_oc_output(TIM1, TIM_OC2);
//   timer_disable_oc_output(TIM1, TIM_OC2N);

//   timer_set_oc_mode(TIM1, TIM_OC3, TIM_OCM_PWM1);
//   // timer_enable_oc_output(TIM1, TIM_OC3);
//   timer_disable_oc_output(TIM1, TIM_OC3);
//   timer_disable_oc_output(TIM1, TIM_OC3N);

//   timer_enable_break_main_output(TIM1);
//   //timer_set_oc_polarity_high(TIM1, TIM_OC2N);
//   timer_enable_preload(TIM1);
//   timer_set_period(TIM1, 100);

//   timer_disable_oc_preload(TIM1, TIM_OC1);
//   // timer_set_oc_value(TIM1, TIM_OC1, (100*20)/100);
//   timer_set_oc_value(TIM1, TIM_OC1, 0);
//   timer_disable_oc_preload(TIM1, TIM_OC2);
//   // timer_set_oc_value(TIM1, TIM_OC2, (100*30)/100);
//   timer_set_oc_value(TIM1, TIM_OC2, 0);
//   timer_disable_oc_preload(TIM1, TIM_OC3);
//   // timer_set_oc_value(TIM1, TIM_OC3, (100*80)/100);
//   timer_set_oc_value(TIM1, TIM_OC3, 0);

//   timer_generate_event(TIM1, TIM_EGR_UG);
//   timer_enable_counter(TIM1);
// }

// static void tim3_setup(void)
// {
//     /* Clock division and mode */
//     TIM3_CR1 = TIM_CR1_CKD_CK_INT | TIM_CR1_CMS_EDGE;
//     /* Period */
//     TIM3_ARR = 5000;
//     /* Prescaler */
//     TIM3_PSC = 71;
//     TIM3_EGR = TIM_EGR_UG;
//     /* ---- */
//     /* Output compare 1 mode and preload */
//     //TIM3_CCMR1 |= TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE;
//     TIM3_CCMR1 |= TIM_CCMR1_OC1M_PWM2 | TIM_CCMR1_OC1PE;
//     /* Polarity and state */
//     TIM3_CCER |= TIM_CCER_CC1P | TIM_CCER_CC1E;
//     //TIM3_CCER |= TIM_CCER_CC1E;
//     /* Capture compare value */
//     TIM3_CCR1 = 0;
//     /* ---- */
//     /* Output compare 2 mode and preload */
//    ///TIM3_CCMR1 |= TIM_CCMR1_OC2M_PWM1 | TIM_CCMR1_OC2PE;
//     TIM3_CCMR1 |= TIM_CCMR1_OC2M_PWM2 | TIM_CCMR1_OC2PE;
//     /* Polarity and state */
//     TIM3_CCER |= TIM_CCER_CC2P | TIM_CCER_CC2E;
//     //TIM3_CCER |= TIM_CCER_CC2E;
//     /* Capture compare value */
//     TIM3_CCR2 = 0;
//     /* Output compare 2 mode and preload */
//     //TIM3_CCMR2 |= TIM_CCMR2_OC3M_PWM1 | TIM_CCMR2_OC3PE;
//     TIM3_CCMR2 |= TIM_CCMR2_OC3M_PWM2 | TIM_CCMR2_OC3PE;
//     /* Polarity and state */
//     TIM3_CCER |= TIM_CCER_CC3P | TIM_CCER_CC3E;
//     //TIM3_CCER |= TIM_CCER_CC2E;
//     /* Capture compare value */
//     TIM3_CCR3 = 0;
//     /* ---- */
//     /* ARR reload enable */
//     TIM3_CR1 |= TIM_CR1_ARPE;
//     /* Counter enable */
//     TIM3_CR1 |= TIM_CR1_CEN;
// }

