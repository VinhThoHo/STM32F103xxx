#include "sensor.h"
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/timer.h>
#include "kalman_filter.h"

uint8_t channel_arr[8] = {ADC_CHANNEL2, ADC_CHANNEL3, ADC_CHANNEL4, ADC_CHANNEL5, ADC_CHANNEL8, ADC_CHANNEL9, ADC_CHANNEL16, ADC_CHANNEL17};
//ADC_CHANNEL18 : VREFIN
//ADC_CHANNEL17: TEMPERATURE

uint16_t dmaBuffer[160];

ADC_STRUCT adcGet;
uint8_t adcFlag = 0;
uint8_t cnt=0;

void adc_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);

	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, 
		GPIO_CNF_INPUT_ANALOG, GPIO2|GPIO3|GPIO4|GPIO5);

	gpio_set_mode(GPIOB, GPIO_MODE_INPUT, 
		GPIO_CNF_INPUT_ANALOG, GPIO0|GPIO1);

	rcc_periph_clock_enable(RCC_ADC1);

	adc_power_off(ADC1);
	adc_enable_scan_mode(ADC1); //enable scan mode
	//adc_disable_scan_mode(ADC1);
	//adc_set_single_conversion_mode(ADC1);
	//adc_set_continuous_conversion_mode(ADC1); //continuous conversion mode
	adc_disable_external_trigger_regular(ADC1); //disable external trigger
	adc_set_right_aligned(ADC1); //12 bit data alignment righ
	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_28DOT5CYC); //28.5 cycle
	adc_enable_temperature_sensor();
	adc_enable_eoc_interrupt(ADC1); // enable complete conversion
	nvic_enable_irq(NVIC_ADC1_2_IRQ); // enable vecter ADC interrupt
	nvic_set_priority(NVIC_ADC1_2_IRQ, ADC_TRIG_PRIORITY); // set priority
	adc_enable_dma(ADC1); // enable DMA trigger
	adc_power_on(ADC1);
	tick_wait_ms(50);
	adc_reset_calibration(ADC1);
	adc_calibrate(ADC1);
	while(adc_is_calibrating(ADC1));
	adc_set_regular_sequence(ADC1, ADC_CHANNEL, channel_arr); 
}

void dma_setup(void)
{
	rcc_periph_clock_enable(RCC_DMA1);
	dma_disable_channel(DMA1, DMA_CHANNEL1);

	nvic_set_priority(NVIC_DMA1_CHANNEL1_IRQ, DMA_PRIORITY);
	nvic_enable_irq(NVIC_DMA1_CHANNEL1_IRQ);

	dma_set_priority(DMA1, DMA_CHANNEL1, DMA_CCR_PL_HIGH);

	dma_channel_reset(DMA1, DMA_CHANNEL1);

	dma_set_peripheral_address(DMA1, DMA_CHANNEL1, (uint32_t)&ADC1_DR);
	dma_set_peripheral_size(DMA1, DMA_CHANNEL1, DMA_CCR_PSIZE_16BIT);

	dma_set_number_of_data(DMA1, DMA_CHANNEL1, ADC_SAMPLE*ADC_CHANNEL);

	dma_set_memory_address(DMA1, DMA_CHANNEL1, (uint32_t)&adcGet.adcValue);
	//dma_set_memory_address(DMA1, DMA_CHANNEL1, (uint32_t)&dmaBuffer);
	dma_set_memory_size(DMA1, DMA_CHANNEL1, DMA_CCR_MSIZE_16BIT);

	dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL1);
	dma_enable_circular_mode(DMA1, DMA_CHANNEL1);
	dma_set_read_from_peripheral(DMA1, DMA_CHANNEL1);

	dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL1);
	dma_clear_interrupt_flags(DMA1, DMA_CHANNEL1, DMA_GIF|DMA_TCIF);
	dma_enable_channel(DMA1, DMA_CHANNEL1);
}

void sensor_tick_setup(void)
{
	rcc_periph_clock_enable(RCC_TIM2);

	rcc_periph_reset_pulse(RST_TIM2);

	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT,
					TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	// timer_set_prescaler(TIM2, 71);
	timer_set_prescaler(TIM2, 31);

	timer_disable_preload(TIM2);
	timer_continuous_mode(TIM2);


	timer_set_period(TIM2, 5000);
	timer_set_counter(TIM2, 0);

	
	nvic_enable_irq(NVIC_TIM2_IRQ);
	nvic_set_priority(NVIC_TIM2_IRQ, TIMER_PRIORIY);
	timer_enable_update_event(TIM2);
	timer_enable_irq(TIM2, TIM_DIER_UIE);
	timer_clear_flag(TIM2, TIM_SR_UIF);
	timer_enable_counter(TIM2);
}

void tim2_isr(void)
{
	if (timer_get_flag(TIM2, TIM_SR_UIF))
	{
		timer_clear_flag(TIM2, TIM_SR_UIF);
		timer_set_counter(TIM2, 0);
		//adc_start_conversion_direct(ADC1);
		adc_trigger();
		//INFO("trigger \n");
	}
}

void dma1_channel1_isr(void)
{
	//INFO("DMA ISR\n");

	if(dma_get_interrupt_flag(DMA1, DMA_CHANNEL1, DMA_TCIF))
	{
		cnt = 0;
		//INFO("Complete DMA transfer \n");
		dma_clear_interrupt_flags(DMA1, DMA_CHANNEL1, DMA_GIF|DMA_TCIF);
		adcGet.flag = 1;
		adcFlag = 1;
	}
	// dma_disable_transfer_complete_interrupt(DMA1, DMA_CHANNEL1);
}

void adc1_2_isr(void)
{
	// if(adc_eoc())
	++cnt;
	//INFO("Complete conversion: %d\n", cnt);
}

uint8_t sensor_get_flag(void)
{
	return adcFlag;
}

void sensor_init(void)
{
	dma_setup();
	adc_setup();
	adcGet.sample = 0;
	sensor_tick_setup();
	tick_initTime(&adcGet.time);
}

void sensor_start(void)
{
	nvic_enable_irq(NVIC_TIM2_IRQ);
	timer_enable_counter(TIM2);
	adcGet.sample = 0;
	adcFlag = 0;
}

void sensor_stop(void)
{
	timer_disable_counter(TIM2);
	nvic_disable_irq(NVIC_TIM2_IRQ);
}

void adc_trigger(void)
{
	adc_start_conversion_direct(ADC1);
}

uint16_t adc_get_buff(uint8_t index, uint16_t *buff, uint8_t sample)
{
	uint16_t i, min = 4095, max = 0;
	long retval = 0;

	for(i = index; i < sample*ADC_CHANNEL; i+=ADC_CHANNEL)
	{
		//INFO("buff: %d\n",*(buff+i));
		if(*(buff+i) < min)
		{
			min = *(buff+i);
			//INFO("min: %d\n",min);
		}
		if(*(adcGet.adcValue + i) > max)
		{
			max = *(buff+i);
			//INFO("max: %d\n",max);
		}
	}
	for(i = index; i < sample*ADC_CHANNEL; i+=ADC_CHANNEL)
	{
		retval += buff[i];
	}
	retval= retval - (max+min);
	retval=retval/(sample-2);
	//INFO("retval average: %d\n", retval);
	return (uint16_t)retval;
}

float temperature_cal(uint32_t measure)
{
	int32_t temp;
	temp = (float)((V25 - measure)/Avg_Slope + 25); 
	return temp; 
}

void get_samples(void)
{
	uint16_t ADC1_buff[8];
	uint8_t i = 0;
	//INFO("VREF: %d \n", adcGet.port[7]);
	INFO("Vref: %f \n", sensor_get(SS_VREF));
	//INFO("SS_LM1: %f \n", sensor_get(SS_LM1));
	INFO("SS_MT1: %f \n", sensor_get(SS_MT1));
	//INFO("SS_LM2: %f \n", sensor_get(SS_LM2));
	INFO("SS_MT2: %f \n", sensor_get(SS_MT2));
	INFO("SS_LM3: %f \n", sensor_get(SS_LM3));
	INFO("SS_MT3: %f \n", sensor_get(SS_MT3));
	INFO("TEMP: %f \n", sensor_get(SS_TEMP));
}


void sensor_manager(void)
{
	uint8_t add;
	if(adcGet.flag == 1)
	{
		//printf("Complete Flag!\n");
		adcGet.flag = 0;
		//INFO("%d\n", adcGet.sample);
		if(++adcGet.sample >= ADC_SAMPLE)
		{
			adcGet.sample = 0;
			adcFlag = 1;
		}
		for(add = 0; add < ADC_CHANNEL; add++)
		{
			//INFO("ADC: %d: %d\n", add, adc_get_buff(add, &adcGet.adcValue[0], ADC_SAMPLE));
			// adcGet.adcValueMean[adcGet.sample*ADC_CHANNEL +add] = adc_get_buff(add, &adcGet.adcValue[0], ADC_SAMPLE);
			adcGet.adcValueMean[((uint16_t)adcGet.sample*ADC_CHANNEL)+add] = adc_get_buff(add, &adcGet.adcValue[0], ADC_SAMPLE);
		}

		// adcGet.port[0] = adc_get_buff(SS_LM1, &adcGet.adcValueMean[0], ADC_SAMPLE);
		// adcGet.port[1] = adc_get_buff(SS_LM2, &adcGet.adcValueMean[0], ADC_SAMPLE);
		// adcGet.port[2] = adc_get_buff(SS_LM3, &adcGet.adcValueMean[0], ADC_SAMPLE);
		// adcGet.port[3] = adc_get_buff(SS_MT1, &adcGet.adcValueMean[0], ADC_SAMPLE);
		// adcGet.port[4] = adc_get_buff(SS_MT2, &adcGet.adcValueMean[0], ADC_SAMPLE);
		// adcGet.port[5] = adc_get_buff(SS_MT3, &adcGet.adcValueMean[0], ADC_SAMPLE);
		// // adcGet.port[6] = adc_get_buff(SS_TEMP, &adcGet.adcValueMean[0], ADC_SAMPLE);
		// // adcGet.port[7] = adc_get_buff(SS_VREF, &adcGet.adcValueMean[0], ADC_SAMPLE);
		// adcGet.temp = adc_get_buff(SS_TEMP, &adcGet.adcValueMean[0], ADC_SAMPLE);
		// adcGet.vref = adc_get_buff(SS_VREF, &adcGet.adcValueMean[0], ADC_SAMPLE);

		adcGet.port[0] = adc_get_buff(SS_LM1, &adcGet.adcValueMean[0], 4);
		adcGet.port[1] = adc_get_buff(SS_LM2, &adcGet.adcValueMean[0], 4);
		adcGet.port[2] = adc_get_buff(SS_LM3, &adcGet.adcValueMean[0], 4);
		adcGet.port[3] = adc_get_buff(SS_MT1, &adcGet.adcValueMean[0], 4);
		adcGet.port[4] = adc_get_buff(SS_MT2, &adcGet.adcValueMean[0], 4);
		adcGet.port[5] = adc_get_buff(SS_MT3, &adcGet.adcValueMean[0], 4);
		// adcGet.port[6] = adc_get_buff(SS_TEMP, &adcGet.adcValueMean[0], ADC_SAMPLE);
		// adcGet.port[7] = adc_get_buff(SS_VREF, &adcGet.adcValueMean[0], ADC_SAMPLE);
		adcGet.temp = adc_get_buff(SS_TEMP, &adcGet.adcValueMean[0], 4);
		adcGet.vref = adc_get_buff(SS_VREF, &adcGet.adcValueMean[0], 4);

	}
}

float sensor_get(SENSOR_ID sensor)
{
	float vrefAdc;
	float value;
	switch(sensor)
	{
		case SS_LM1:
			vrefAdc = (((float)4096*1.2)/adcGet.vref);
			value = (((float)adcGet.port[0]*vrefAdc)/4096);
			return value;
			break;
		case SS_LM2:
			vrefAdc = (((float)4096*1.2)/adcGet.vref);
			value = (((float)adcGet.port[1]*vrefAdc)/4096);
			return value;
			break;
		case SS_LM3:
			vrefAdc = (((float)4096*1.2)/adcGet.vref);
			value = (((float)adcGet.port[2]*vrefAdc)/4096);
			return value;
			break;
		case SS_MT1:
			vrefAdc = (((float)4096*1.2)/adcGet.vref);
			value = (((float)adcGet.port[3]*vrefAdc)/4096);
			return value;
			break;
		case SS_MT2:
			vrefAdc = (((float)4096*1.2)/adcGet.vref);
			value = (((float)adcGet.port[4]*vrefAdc)/4096);
			return value;
			break;
		case SS_MT3:
			vrefAdc = (((float)4096*1.2)/adcGet.vref);
			value = (((float)adcGet.port[5]*vrefAdc)/4096);
			return value;
			break;
		case SS_TEMP:
			value = temperature_cal(adcGet.temp);
			return value;
			break;
		case SS_VREF:
			value = vrefAdc = (((float)4096*1.2)/adcGet.vref);
			return vrefAdc;
			break;
	}
}

/*Read ADC single channel, single conversion*/
// uint16_t read_adc_naiive(uint8_t channel)
// {
// 	uint8_t channel_array[16];
// 	channel_array[0] = channel;
// 	adc_set_regular_sequence(ADC1, 1, channel_array);
// 	adc_start_conversion_direct(ADC1);
// 	while(!adc_eoc(ADC1));
// 	uint16_t reg16 = adc_read_regular(ADC1);
// 	return reg16;
// }

