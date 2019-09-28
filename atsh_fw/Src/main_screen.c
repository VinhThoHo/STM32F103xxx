#include "main_screen.h"
#include "u8g2.h"
#include "key.h"


u8g2_t u8g2;

MAIN_SCREEN_STR main_scr;

void main_screen_init(void)
{
    main_scr.lampStatus = 0;
    main_scr.uvStatus = 0;
    main_scr.fanStatus = 0;
    main_scr.socketStatus = 0;
    main_scr.fanRotate = 0;
    u8g2_ClearBuffer(&u8g2);
    main_scr.tick = HAL_GetTick();
}

void Lamp_Status(uint8_t lampStatus)
{
    if(lampStatus)
    {
        // u8g2_ClearBuffer(&u8g2);
        u8g2_DrawBitmap(&u8g2, 0, 33, bmp_neon_on.width/8, bmp_neon_on.height, bmp_neon_on.data);
        u8g2_SetFont(&u8g2, u8g2_font_micro_tr );
        // u8g2_SetDrawColor(&u8g2, ~lampStatus);
        u8g2_DrawStr(&u8g2, 9, 33, "NEON");
    }
    else
    {
        // u8g2_ClearBuffer(&u8g2);
        u8g2_DrawBitmap(&u8g2, 0, 33, bmp_neon_off.width/8, bmp_neon_off.height, bmp_neon_off.data);
        u8g2_SetFont(&u8g2, u8g2_font_micro_tr );
        // u8g2_SetDrawColor(&u8g2, ~lampStatus);
        u8g2_DrawStr(&u8g2, 9, 33, "NEON");
    }  
}
void UV_Status(uint8_t uvStatus)
{
    if(uvStatus)
    {
        u8g2_DrawBitmap(&u8g2, 32, 33, bmp_uv_on.width/8, bmp_uv_on.height, bmp_uv_on.data);
        u8g2_SetFont(&u8g2, u8g2_font_micro_tr );
        // u8g2_SetDrawColor(&u8g2, ~uvStatus);
        u8g2_DrawStr(&u8g2, 45, 33, "UV");
    }
    else
    {
        u8g2_DrawBitmap(&u8g2, 32, 33, bmp_uv_off.width/8, bmp_uv_off.height, bmp_uv_off.data);
        u8g2_SetFont(&u8g2, u8g2_font_micro_tr );
        // u8g2_SetDrawColor(&u8g2, ~uvStatus);
        u8g2_DrawStr(&u8g2, 45, 33, "UV");
    }  
}

void Fan_Status(uint8_t fanStatus)
{
    if((HAL_GetTick() - main_scr.tick > 200) && main_scr.fanStatus)
    {
      main_scr.fanRotate = ~main_scr.fanRotate;
      main_scr.tick = HAL_GetTick();
    }
    if(main_scr.fanRotate)
    {
        u8g2_SetFont(&u8g2, u8g2_font_micro_tr );
        // u8g2_SetDrawColor(&u8g2, ~fanStatus);
        u8g2_DrawStr(&u8g2, 75, 33, "FAN");
        u8g2_DrawBitmap(&u8g2, 68, 38, bmp_fan_origin.width/8, bmp_fan_rotate.height, bmp_fan_rotate.data);
    }
    else
    {
        u8g2_SetFont(&u8g2, u8g2_font_micro_tr );
        // u8g2_SetDrawColor(&u8g2, ~fanStatus);
        u8g2_DrawStr(&u8g2, 75, 33, "FAN");
        u8g2_DrawBitmap(&u8g2, 68, 38, bmp_fan_origin.width/8, bmp_fan_origin.height, bmp_fan_origin.data);
    }  
}
void Socket_Status(uint8_t sStatus)
{
    if(sStatus)
    {
        u8g2_DrawBitmap(&u8g2, 98, 34, bmp_socket_on.width/8, bmp_socket_on.height, bmp_socket_on.data);
        u8g2_SetFont(&u8g2, u8g2_font_u8glib_4_tf );
        u8g2_DrawStr(&u8g2, 108, 62, "ON");
        u8g2_SetFont(&u8g2, u8g2_font_micro_tr );
        // u8g2_SetDrawColor(&u8g2, ~sStatus);
        u8g2_DrawStr(&u8g2, 101, 33, "SOCKET");
    }
    else
    {
        u8g2_DrawBitmap(&u8g2, 98, 34, bmp_socket_off.width/8, bmp_socket_off.height, bmp_socket_off.data);
        u8g2_SetFont(&u8g2, u8g2_font_u8glib_4_tf );
        u8g2_DrawStr(&u8g2, 105, 62, "OFF");
        u8g2_SetFont(&u8g2, u8g2_font_micro_tr );
        // u8g2_SetDrawColor(&u8g2, ~sStatus);
        u8g2_DrawStr(&u8g2, 101, 33, "SOCKET");
    }  
}
void Filter(uint8_t FStatus)
{
    switch(FStatus)
    {
        case 1:
            u8g2_SetFont(&u8g2, u8g2_font_5x7_tf);//
            u8g2_DrawStr(&u8g2, 2, 19, "Please change the filter!");// When Filter < 30%, show string char.
            break;
        case 0:
            u8g2_SetDrawColor(&u8g2, ~FStatus);
            u8g2_SetFont(&u8g2, u8g2_font_5x7_tf);//
            u8g2_DrawStr(&u8g2, 2, 19, "Please change the filter!");// When Filter < 30%, show string char.
            u8g2_DrawStr(&u8g2, 7, 10, "Filter:");
            u8g2_DrawStr(&u8g2, 98, 10, "100"); //variable x
            u8g2_DrawStr(&u8g2, 115, 10, "%");
            u8g2_DrawBox(&u8g2, 45, 4, 50, 6); //Value of filter
            break;
    }
}
void Horizontal(void)
{
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawFrame(&u8g2, 0, 24, 128, 40);
    u8g2_DrawHLine(&u8g2, 0, 36, 128);
    u8g2_DrawHVLine(&u8g2, 32, 24, 40, 1);
    u8g2_DrawHVLine(&u8g2, 64, 24, 40, 1);
    u8g2_DrawHVLine(&u8g2, 96, 24, 40, 1);
}
void Main_Screen_Manage(void)
{
    //KeyManage();
    Lamp_Status(main_scr.lampStatus);
    UV_Status(main_scr.uvStatus);
    Fan_Status(main_scr.fanStatus);
    Socket_Status(main_scr.socketStatus);
    Filter(0);
    Horizontal();
}
