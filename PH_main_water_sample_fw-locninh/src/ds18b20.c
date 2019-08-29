#include "ds18b20.h"
#include "string.h"
#include "debug.h"
#include "tick.h"

uint8_t DS18B20_SERIAL_NUMBER[MAX_SENSORS][8];

volatile uint8_t SensorCount;

#define CRC8INIT    0x00
#define CRC8POLY    0x18              //0X18 = X^8+X^5+X^4+X^0

uint8_t DS18B20_CRC(uint8_t *data, uint16_t number_of_bytes_in_data)
{
  uint8_t  crc;
  uint16_t loop_count;
  uint8_t  bit_counter;
  uint8_t  b;
  uint8_t  feedback_bit;
  crc = CRC8INIT;
  for(loop_count = 0; loop_count != number_of_bytes_in_data; loop_count++)
  {
    b = data[loop_count];
    bit_counter = 8;
    do {
          feedback_bit = (crc ^ b) & 0x01;
          if ( feedback_bit == 0x01 ) 
          {
            crc = crc^CRC8POLY;
          }
          crc = (crc >> 1) & 0x7F;
          if ( feedback_bit == 0x01 ) 
          {
            crc = crc | 0x80;
          }
          b = b >> 1;
          bit_counter--;
        } while (bit_counter > 0);
  }
  return crc;
}

void ds_input(DS18B20_STRUCT* ds)
{
	gpio_set_mode(ds->port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, ds->pin);
}

void ds_output(DS18B20_STRUCT* ds)
{
  gpio_set_mode(ds->port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, ds->pin);
}

void ds_low(DS18B20_STRUCT* ds)
{
  gpio_clear(ds->port, ds->pin);
}

void ds_high(DS18B20_STRUCT* ds)
{
  //gpio_set(ds->port, ds->pin);
}

uint16_t OneWire_GetBusLine(DS18B20_STRUCT* ds)
{
  if(gpio_get(ds->port, ds->pin) == ds->pin)
    return 1;
  else
    return 0;
}

uint8_t OneWire_ReadBit(DS18B20_STRUCT* ds)
{
  uint8_t bit = 0;
  ds_low(ds);
  ds_output(ds);
  tick_wait_us(1);
  //tick_delayUS(&ds->time, 1);
  ds_input(ds);
  //ds_high(ds);
  //tick_delayUS(&ds->time, 12);
  tick_wait_us(10);
  if(OneWire_GetBusLine(ds))
    bit = 1;
  //tick_delayUS(&ds->time, 43);
  tick_wait_us(40);
  return bit;
}

void OneWire_WriteBit(DS18B20_STRUCT* ds, uint8_t bit)
{
  ds_low(ds);
  ds_output(ds);
  if(bit){
    //tick_delayUS(&ds->time, 1);
    tick_wait_us(2);
    ds_input(ds);
    //ds_high(ds);
    //tick_delayUS(&ds->time, 50);
    //sys_tick_wait_us(50);
  }
  // else{
  //   //tick_delayUS(&ds->time, 65);
  //   sys_tick_wait_us(65);
  // }
  //tick_delayUS(&ds->time, 58);
  tick_wait_us(58);
  ds_input(ds);
  //ds_high(ds);
}

void OneWire_WriteByte(DS18B20_STRUCT* ds, uint8_t data)
{
  uint8_t _data = data;
  uint8_t i;
  for(i=0; i<8; i++) 
  {
    OneWire_WriteBit(ds, _data & 0x01);
    _data>>= 1;
  }
}

uint8_t OneWire_ReadByte(DS18B20_STRUCT* ds)
{
  uint8_t data = 0;
  uint8_t i;
  for(i=0; i<7; i++)
  {
    if(OneWire_ReadBit(ds))
      data|= 0x80;
    data>>= 1;
  }
  if(OneWire_ReadBit(ds))
    data|= 0x80;
  return data;
}         

uint8_t OneWire_Reset(DS18B20_STRUCT* ds)
{ 
  uint8_t pressence, short_circuit; 
  // do
  // {  
  //   ds_low(ds);
  //   ds_output(ds);
  //   //tick_delayUS(&ds->time, 500);
  //   sys_tick_wait_us(480);
  //   //ds_high(ds);
  //   ds_input(ds);
  //   //tick_delayUS(&ds->time, 80);
  //   sys_tick_wait_us(64);
  //   pressence = OneWire_GetBusLine(ds);
  // }
  // while(pressence);
  ds_low(ds);
  ds_output(ds);
  //tick_delayUS(&ds->time, 500);
  tick_wait_us(480);
  //ds_high(ds);
  ds_input(ds);
  //tick_delayUS(&ds->time, 80);
  tick_wait_us(64);
  pressence = OneWire_GetBusLine(ds);
  tick_wait_us(480 - 64);
  short_circuit = OneWire_GetBusLine(ds);
  //tick_delayUS(&ds->time, 180);
  return (pressence<<1)|(short_circuit);
}

void DS18B20_Init(DS18B20_STRUCT* ds, uint32_t GPIOx, uint16_t GPIO_Pin)
{
  gpio_set_mode(GPIOx, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO_Pin);

  ds->pin = GPIO_Pin;
  ds->port = GPIOx;
  ds->status = DS_INIT;
  ds->state = DS_INIT;
  memset(&ds->id[0], 0, 8);
  ds_input(ds);
  ds_high(ds);
  //OneWire_Reset(ds);
}

float DS18B20_GetTemp(DS18B20_STRUCT* ds, uint8_t SensorNum)
{
  uint8_t TL, TH;
  uint8_t i;
  uint8_t buff[10];
  uint8_t checksum;

  //__disable_irq();
  if(OneWire_Reset(ds) != 0x01) 
  {
    INFO("\r\nDS18B20 error reset...");
    return DS_ERR;
  }
  if(SensorNum==0)
  {
    OneWire_WriteByte(ds, 0xCC);
  }
  else
  {
    OneWire_WriteByte(ds, DS18B20_MATCHROM_CMD);
    for(i=0; i<8; i++)
    {
      OneWire_WriteByte(ds, DS18B20_SERIAL_NUMBER[SensorNum-1][i]);
    }
  }
  OneWire_WriteByte(ds, 0xBE);
  for(i=0;i<10;i++)
  {
    buff[i] = OneWire_ReadByte(ds);
  }
  //__enable_irq();
  checksum = DS18B20_CRC(&buff[0], 8);
  TL = buff[0];
  TH = buff[1];
  if((checksum == buff[8])&&(buff[8] != 0)&&(TL != 0x50)&&(TH != 0x05))
  {
    ds->temp = (int16_t)(TL)|((int16_t)TH<<8);
    ds->temp*= 0.0625;
		ds->state = DS_OK;
    return DS_OK;
  }
  //INFO("\r\nDS18B20 error checkSum: %02x %02x %02x", checksum, TH, TL);
	ds->state = DS_ERR;
  return DS_ERR;
}

void DS18B20_StartConversion(DS18B20_STRUCT* ds, uint8_t SensorNum)
{
  uint8_t i;
  OneWire_Reset(ds);
  if(SensorNum==0)
  {
    OneWire_WriteByte(ds, 0xCC);
  }
  else
  {
    OneWire_WriteByte(ds, DS18B20_MATCHROM_CMD);    
    for(i=0; i<8; i++)
    {
      OneWire_WriteByte(ds, DS18B20_SERIAL_NUMBER[SensorNum-1][i]);
    }
  }
    OneWire_WriteByte(ds, 0x44);    
}

uint8_t OneWire_SearchROM(DS18B20_STRUCT* ds, uint8_t diff, uint8_t *id)
{   
  uint8_t DataBit, DataByte, next_diff, bit;
  //asm volatile ("cpsid i");
  //INFO("\r\nDS18B20 reset: %02x", OneWire_Reset(ds));
  if(OneWire_Reset(ds) != 0x01) 
  {
    //INFO("\r\nDS18B20 error reset...");
    //tick_delayMS(&ds->time, 10);
    return 0xFF;
  }
  OneWire_WriteByte(ds, DS18B20_SEARCH_ROM_CMD);
  //asm volatile ("cpsie i");
  next_diff = 0x00; 
  DataBit = 64;
  do 
  {   
    DataByte = 8;
    do 
    { 
      //asm volatile ("cpsid i");
      bit = OneWire_ReadBit(ds);
      if(OneWire_ReadBit(ds)) 
      {
        if(bit){
          INFO("\r\nDS18B20 error read bit 0xFE: %d %d", DataBit, DataByte);
          return 0xFE;
        }
      }
      else 
      { 
        if(!bit) 
        {
          if(diff>DataBit || ((*id&1) && diff!=DataBit)) 
          { 
            bit = 1; 
            next_diff = DataBit; 
          }
        }
      }
      //asm volatile ("cpsie i");
      OneWire_WriteBit(ds, bit);
      *id>>= 1;
      if(bit)
        *id |= 0x80;
      DataBit--;
    } 
    while(--DataByte);
    id++;
  } 
  while(DataBit);
  return next_diff;
}

void OneWire_FindROM(DS18B20_STRUCT* ds, uint8_t *diff, uint8_t id[])
{
  while(1)
  {
    *diff = OneWire_SearchROM(ds, *diff, &id[0]);
    if(*diff==0xFF || *diff==0xFE || *diff==0x00) return;
    return;
  }
}

void DS18B20_GetROM(DS18B20_STRUCT* ds, uint8_t SensorNum, uint8_t *buffer)
{
  uint8_t i;
  if(SensorNum>SensorCount || SensorNum==0)return;
  for(i=0; i<8; i++)
    buffer[i] = DS18B20_SERIAL_NUMBER[SensorNum-1][i];      
}

uint8_t DS18B20_ScanBus(DS18B20_STRUCT* ds)
{ 
  uint8_t i;
  uint8_t id[8];
  uint8_t diff;
  SensorCount = 0;
  for(diff=0xFF; diff!=0 && (SensorCount<MAX_SENSORS);)
  { 
    OneWire_FindROM(ds, &diff, &id[0]);
    if(diff==0xFF || diff==0xFE)break;
    for(i=0;i<8;i++)
      DS18B20_SERIAL_NUMBER[SensorCount][i] = id[i];
    SensorCount++;
  }
  return SensorCount;
}

uint8_t DS18B20_GetState(DS18B20_STRUCT* ds)
{
	return ds->state;
}

