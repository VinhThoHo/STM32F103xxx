#include "logo.h"
#include "u8g2.h"
#include "user_bitmap.h"

u8g2_t u8g2;

void Logo(void)
{
  u8g2_DrawBitmap(&u8g2, 0, 0, bmp_logo_ph.width/8, bmp_logo_ph.height, bmp_logo_ph.data);
  u8g2_SendBuffer(&u8g2);
}