#include "HD44780.h"

HD44780_struct hd44780;

void HD44780_InitPins(void)
{
	gpio_set_mode(HD44780_CTRL_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, HD44780_RS_PIN|HD44780_RW_PIN|HD44780_E_PIN|HD44780_LIGHT_PIN);
	gpio_set_mode(HD44780_DB_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, HD44780_DB4_PIN|HD44780_DB5_PIN|HD44780_DB6_PIN|HD44780_DB7_PIN);	
}

void HD44780_Cmd4bit(uint8_t cmd)
{
	gpio_clear(HD44780_CTRL_PORT, HD44780_E_PIN);
	(cmd & 0x08) ? gpio_set(HD44780_DB_PORT, HD44780_DB7_PIN) : gpio_clear(HD44780_DB_PORT, HD44780_DB7_PIN);
	(cmd & 0x04) ? gpio_set(HD44780_DB_PORT, HD44780_DB6_PIN) : gpio_clear(HD44780_DB_PORT, HD44780_DB6_PIN);
	(cmd & 0x02) ? gpio_set(HD44780_DB_PORT, HD44780_DB5_PIN) : gpio_clear(HD44780_DB_PORT, HD44780_DB5_PIN);
	(cmd & 0x01) ? gpio_set(HD44780_DB_PORT, HD44780_DB4_PIN) : gpio_clear(HD44780_DB_PORT, HD44780_DB4_PIN);
	gpio_set(HD44780_CTRL_PORT, HD44780_E_PIN);
	tick_wait_us(100);
	gpio_clear(HD44780_CTRL_PORT, HD44780_E_PIN);
	tick_wait_us(100);
}

void HD44780_Cmd(uint8_t cmd)
{
	gpio_clear(HD44780_CTRL_PORT, HD44780_RS_PIN); //select IR
	HD44780_Cmd4bit(cmd >> 4);
	HD44780_Cmd4bit(cmd & 0x0F);
}

void HD44780_Data(uint8_t data)
{
	gpio_set(HD44780_CTRL_PORT, HD44780_RS_PIN); //select DR
	HD44780_Cmd4bit(data >> 4);
	HD44780_Cmd4bit(data & 0x0F);
}

void HD44780_Init(uint8_t cols, uint8_t rows)
{
	HD44780_InitPins();
	gpio_clear(HD44780_CTRL_PORT, HD44780_RW_PIN);
	gpio_clear(HD44780_CTRL_PORT, HD44780_RS_PIN);
	gpio_clear(HD44780_CTRL_PORT, HD44780_E_PIN);
	tick_wait_ms(15);
	hd44780.Rows = rows;
	hd44780.Cols = cols;
	hd44780.currentX = 0;
	hd44780.currentY = 0;
	hd44780.light = 0;
	hd44780.DisplayFunction = HD44780_4BITMODE | HD44780_5x10DOTS | HD44780_1LINE;
	if(rows > 1)
		hd44780.DisplayFunction |= HD44780_2LINE;
	
	HD44780_Cmd4bit(0x03);
	tick_wait_ms(10);
	HD44780_Cmd4bit(0x03);
	tick_wait_us(150);
	HD44780_Cmd4bit(0x03);
	tick_wait_ms(10);
	HD44780_Cmd4bit(0x02);
	tick_wait_ms(10);

	HD44780_Cmd(HD44780_FUNCTIONSET | hd44780.DisplayFunction);
	hd44780.DisplayControl = HD44780_DISPLAYON;
	HD44780_DisplayOn();
	HD44780_Clear();
	hd44780.DisplayMode = HD44780_ENTRYLEFT | HD44780_ENTRYSHIFTINCREMENT;
	HD44780_Cmd(HD44780_ENTRYMODESET | hd44780.DisplayMode);
	tick_wait_ms(100);
	gpio_clear(HD44780_CTRL_PORT, HD44780_LIGHT_PIN);
}

void HD44780_DisplayOn(void)
{
	hd44780.DisplayControl |= HD44780_DISPLAYON;
	HD44780_Cmd(HD44780_DISPLAYCONTROL | hd44780.DisplayControl);
}

void HD44780_DisplayOff(void)
{
	hd44780.DisplayControl &= ~HD44780_DISPLAYON;
	HD44780_Cmd(HD44780_DISPLAYCONTROL | hd44780.DisplayControl);
}

void HD44780_Clear(void)
{
	HD44780_Cmd(0x01);
}

void HD44780_CursorSet(uint8_t col, uint8_t row)
{
	uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
	if(row >= hd44780.Rows) row = 0;
	hd44780.currentX = col;
	hd44780.currentY = row;
	HD44780_Cmd(HD44780_SETDDRAMADDR | (col + row_offsets[row]));
	tick_wait_ms(10);
}

void HD44780_Puts(uint8_t x, uint8_t y, char* str)
{
	HD44780_CursorSet(x, y);
	while(*str)
	{
		if(hd44780.currentX >= hd44780.Cols)
		{
			hd44780.currentX = 0;
			hd44780.currentY++;
			HD44780_CursorSet(hd44780.currentX, hd44780.currentY);
		}
		if(*str == '\n')
		{
			hd44780.currentY++;
			HD44780_CursorSet(hd44780.currentX, hd44780.currentY);
		}
		else if(*str == '\r')
		{
			HD44780_CursorSet(0, hd44780.currentY);
		}
		else
		{
			HD44780_Data(*str);
			hd44780.currentX++;
		}
		str++;
	}
}

void HD44780_BlinkOn(void)
{
	hd44780.DisplayControl |= HD44780_DISPLAYON;
	HD44780_Cmd(HD44780_DISPLAYCONTROL | hd44780.DisplayControl);
}

void HD44780_BlinkOff(void)
{
	hd44780.DisplayControl &= ~HD44780_DISPLAYON;
	HD44780_Cmd(HD44780_DISPLAYCONTROL | hd44780.DisplayControl);
}

void HD44780_CursorOn(void)
{
	hd44780.DisplayControl |= HD44780_CURSORON;
	HD44780_Cmd(HD44780_DISPLAYCONTROL | hd44780.DisplayControl);
}

void HD44780_CursorOff(void)
{
	hd44780.DisplayControl &= ~HD44780_CURSORON;
	HD44780_Cmd(HD44780_DISPLAYCONTROL | hd44780.DisplayControl);
}

void HD44780_ScrollLeft(void)
{
	HD44780_Cmd(HD44780_CURSORSHIFT | HD44780_DISPLAYMOVE | HD44780_MOVELEFT);
}

void HD44780_ScrollRight(void)
{
	HD44780_Cmd(HD44780_CURSORSHIFT | HD44780_DISPLAYMOVE | HD44780_MOVERIGHT);
}

void HD44780_CreateChar(uint8_t location, uint8_t *data)
{
	uint8_t i;
	location &= 0x07; 
	HD44780_Cmd(HD44780_SETCGRAMADDR | (location <<3));
	for(i = 0; i < 8; i++)
		HD44780_Data(data[i]);
}

void HD44780_PutCustom(uint8_t x, uint8_t y, uint8_t location)
{
	HD44780_CursorSet(x, y);
	HD44780_Data(location);
}

void LCD_DisplayCGRAM0(uint8_t address, uint8_t *ptrTable)
{

  uint8_t u;

  /* Set CGRAM Address */
  HD44780_Cmd((uint8_t)0x40);

  u = 32; /* Nb byte in the table */
  while (u)
  {
    HD44780_Data(ptrTable[32 - u]);
    u--;
  }

  /* Setup Display Address */
  HD44780_Cmd(address);
  HD44780_Data((uint8_t)0x00);
  HD44780_Data((uint8_t)0x00);
}

void LCD_DisplayCGRAM1(uint8_t address, uint8_t *ptrTable)
{

  uint8_t u;

 /* Set CGRAM Address */
  HD44780_Cmd((uint8_t)((uint8_t)0x40 | (uint8_t)0x10));

  u = 32; /* Nb byte in the table */
  while (u)
  {
    HD44780_Data(ptrTable[32 - u]);
    u--;
  }

  /* Setup Display Address */
  HD44780_Cmd((uint8_t)(address + 1));
  HD44780_Data((uint8_t)0x00);
  HD44780_Data((uint8_t)0x02);
}

bool get_lcd_light(void)
{
	return hd44780.light;
}

void LCD_LIGHT(bool en)
{
	if(en) 
	{
		gpio_set(HD44780_CTRL_PORT, HD44780_LIGHT_PIN);
		hd44780.light = 1;
	}
	else
	{
		gpio_clear(HD44780_CTRL_PORT, HD44780_LIGHT_PIN);
		hd44780.light = 0;
	} 
}
