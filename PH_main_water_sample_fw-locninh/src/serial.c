#include "serial.h"
#include "debug.h"

ringbuf ustRbRx;
uint8_t _ustRead[1024];
SERIAL_STRUCT serialParser;
uint32_t ustTick;
uint8_t ustFlag;

static void usart_setup(void);

static void usart_setup(void)
{
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_USART3);

	nvic_enable_irq(NVIC_USART3_IRQ);

	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
          GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART3_TX);
	gpio_set_mode(GPIOB, GPIO_MODE_INPUT,
          GPIO_CNF_INPUT_FLOAT, GPIO_USART3_RX);

	usart_set_baudrate(USART3, 9600);
	usart_set_databits(USART3, 8);
	usart_set_stopbits(USART3, USART_STOPBITS_1);
	usart_set_parity(USART3, USART_PARITY_NONE);
	usart_set_flow_control(USART3, USART_FLOWCONTROL_NONE);
	usart_set_mode(USART3, USART_MODE_TX_RX);

	USART_CR1(USART3) |= USART_CR1_RXNEIE;

	usart_enable(USART3);
}

void serial_init(void)
{
	usart_setup();
	ringbuf_init(&ustRbRx, _ustRead, sizeof(_ustRead));
	ustFlag = 0;
}

void serial_StringToHex(uint8_t *source, uint16_t len, uint8_t *dest)
{
  uint16_t add;
  uint16_t hexLen = 0;
  for(add = 0; add < len; add++)
  {
    if(source[add] != '-')
    {
      dest[hexLen++] = (source[add] - '0')*10 + (source[add+1] - '0');
      add++;
    }
  }
}

void usart3_isr(void)
{
  uint8_t data;
  /* Check if we were called because of RXNE. */
  if (((USART_CR1(USART3) & USART_CR1_RXNEIE) != 0) &&
      ((USART_SR(USART3) & USART_SR_RXNE) != 0)) {
    /* Retrieve the data from the peripheral. */
    data = usart_recv(USART3);
    ringbuf_put(&ustRbRx, data);
    ustTick = tick_get();
    ustFlag = 1;
  }
}

void serial_tx(uint8_t *source, uint16_t len)
{
  uint16_t i;
  for(i = 0; i < len; i++)
  {
    usart_send_blocking(USART3, source[i]);
  }
}

void serial_parser(uint8_t *data, uint16_t len)
{

}

void serial_manager(void)
{
	uint8_t c;
	uint32_t writelen = 0, i =0;
	uint8_t getBuff[128];
	while((ringbuf_get(&ustRbRx, &c))&&(writelen<128))
	{
		writelen++;
		getBuff[i++] = c;
	}
}