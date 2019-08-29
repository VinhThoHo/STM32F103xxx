#include "eeprom.h"
#include "debug.h"
#include "clock_rtc.h"

EEPROM_STRUCT eeprom;
EEPROM_STORAGE store[25];

// static void i2c_mem_write(uint8_t devAdd, uint16_t memAdd, uint8_t *data, uint8_t length);
// static void i2c_mem_read(uint8_t devAdd, uint16_t memAdd, uint8_t *data, uint8_t length);

// bool eeprom_write_byte(uint16_t addr, uint8_t data);
// bool eeprom_write_page(uint16_t addr, uint8_t *byte_ptr, uint16_t data_length);
// bool eeprom_read_byte(uint16_t addr, uint8_t *byte_ptr);
// bool eeprom_read_page(uint16_t addr, uint8_t *byte_ptr, uint16_t data_length);

void log_save(uint8_t bot)
{	
	uint8_t i;
	i = bot - 1;
	store[i].bottle = bot;
	store[i].logtime = localTime;
	store[i].buff[0] = store[i].bottle;
	store[i].buff[1] = store[i].logtime.hour;
	store[i].buff[2] = store[i].logtime.min;
	store[i].buff[3] = store[i].logtime.sec;
	store[i].buff[4] = store[i].logtime.mday;
	store[i].buff[5] = store[i].logtime.month;
	store[i].buff[6] = store[i].logtime.year >> 8;
	store[i].buff[7] = store[i].logtime.year;

	i2c_mem_write(eeprom.devAddr, (i*8), store[i].buff , sizeof(store[i].buff));
	tick_wait_ms(5);
}

void log_load(uint8_t bot, uint8_t buff[8])
{
	uint8_t i;
	i = bot - 1;
	// INFO("size: %d \n", sizeof(buff));
	i2c_mem_read(eeprom.devAddr, (i*8), buff, 8);
	tick_wait_ms(5);

}

void log_clear(void)
{
	uint8_t buff[200];
	memset(buff, 0x00, sizeof(buff));
	i2c_mem_write(eeprom.devAddr, 0x00, buff, sizeof(buff));
	tick_wait_ms(5);
}

void eeprom_init(void)
{
	eeprom.devAddr = 0xA0 >> 1;
	rcc_periph_clock_enable(RCC_I2C1);
	rcc_periph_clock_enable(RCC_AFIO);
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, 
					GPIO_I2C1_SCL | GPIO_I2C1_SDA);
	i2c_peripheral_disable(I2C1);
	//i2c_reset(I2C1);
	i2c_set_clock_frequency(I2C1, I2C_CR2_FREQ_36MHZ);
	i2c_set_standard_mode(I2C1);
	
	i2c_set_ccr(I2C1, 0xB4);
	i2c_set_trise(I2C1, 37);
	//i2c_enable_interrupt(I2C1, I2C_CR2_ITERREN);
	i2c_peripheral_enable(I2C1);
	//nvic_enable_irq(NVIC_I2C1_EV_IRQ);
}


void i2c_mem_write(uint8_t devAddr, uint16_t memAddr, uint8_t* buf, uint16_t length)
{
	volatile uint32_t timeout;
	uint8_t memAddrLow, memAddrHigh;
	memAddrHigh = memAddr >> 8;
	memAddrLow = memAddr & 0xFF;
	i2c_send_start(I2C1);
	timeout = 0xFFFF;
	while((!((I2C_SR1(I2C1) & I2C_SR1_SB)
		& (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY)))) && (timeout--));
	i2c_send_7bit_address(I2C1, devAddr, I2C_WRITE);
	timeout = 0xFFFF;
	while((!(I2C_SR1(I2C1) & I2C_SR1_ADDR)) && (timeout--));
	(void)I2C_SR2(I2C1);
	timeout = 0xFFFF;
	i2c_send_data(I2C1, memAddrHigh);
	while((!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE))) && (timeout--));
	(void)I2C_SR2(I2C1);
	i2c_send_data(I2C1, memAddrLow);
	// // // timeout = 0xFFFF;
	while((!(I2C_SR1(I2C1) & I2C_SR1_BTF)) && (timeout--));
	// // // timeout = 0xFFFF;
	for(uint8_t i = 0; i < length; i++)
	{
		i2c_send_data(I2C1, buf[i]);
		while(!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE)));
	}
	i2c_send_stop(I2C1);
}

void i2c_mem_read(uint8_t devAddr, uint16_t memAddr, uint8_t* buf, uint16_t length)
{
	volatile uint32_t timeout;
	uint8_t memAddrLow, memAddrHigh;
	uint8_t i;
	memAddrHigh = memAddr >> 8;
	memAddrLow = memAddr & 0xFF;
	i2c_send_start(I2C1);
	timeout = 0xFFFF;
	while((!((I2C_SR1(I2C1) & I2C_SR1_SB)
		& (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY))))&&(timeout--));
	i2c_send_7bit_address(I2C1, devAddr, I2C_WRITE);
	timeout = 0xFFFF;
	while((!(I2C_SR1(I2C1) & I2C_SR1_ADDR)) && (timeout--));
	(void)I2C_SR2(I2C1);
	timeout = 0xFFFF;
	i2c_send_data(I2C1, memAddrHigh);
	while((!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE))) && (timeout--));
	(void)I2C_SR2(I2C1);
	i2c_send_data(I2C1, memAddrLow);
	timeout = 0xFFFF;
	while((!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE))) && (timeout--));
	
	i2c_send_start(I2C1);
	timeout = 0xFFFF;
	while((!((I2C_SR1(I2C1) & I2C_SR1_SB)
		& (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY)))) && (timeout--));
	i2c_send_7bit_address(I2C1, devAddr, I2C_READ);
	i2c_enable_ack(I2C1);
	timeout = 0xFFFF;
	while ((!(I2C_SR1(I2C1) & I2C_SR1_ADDR)) && (timeout--));
	(void)I2C_SR2(I2C1);
	// if(I2C_SR2(I2C1) & I2C_SR2_BUSY)
	// {
	// 	i2c_enable_ack(I2C1);
	// 	while(length > 0)
	// 	{
	// 		while ((!(I2C_SR1(I2C1) & I2C_SR1_RxNE)));
	// 		*buf = i2c_get_data(I2C1);
	// 		buf++;
	// 		length--;
	// 		if(length == 1)
	// 		{
	// 			i2c_disable_ack(I2C1);
	// 		}
	// 	}
	// }
	for(i = 0; i < length; ++i)
	{
		if(i == length - 1)
			i2c_disable_ack(I2C1);
		while ((!(I2C_SR1(I2C1) & I2C_SR1_RxNE)));
		buf[i] = i2c_get_data(I2C1);
	}
	i2c_send_stop(I2C1);
}

void eeprom_read(uint16_t memAddr, uint8_t* buf, uint16_t length)
{
	i2c_mem_read(eeprom.devAddr, memAddr, buf, length);
}

void eeprom_write(uint16_t memAddr, uint8_t* buf, uint16_t length)
{
	i2c_mem_write(eeprom.devAddr, memAddr, buf, length);
}