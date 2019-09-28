#include "info_screen.h"
#include "u8g2.h"

u8g2_t u8g2;

INFO_SCREEN_STR Info_Scr;

void Info_Screen_Init(void)
{
    Info_Scr.SysTime = 0;
    Info_Scr.SysWork = 0;
    Info_Scr.Filter_Pa = 0;
    Info_Scr.Filter_Time = 0;
    Info_Scr.Flow = 0;
    Info_Scr.UV = 0;
    Info_Scr.Door = 2;
    Info_Scr.tick = HAL_GetTick();
}
void SysTime(uint8_t TimeStatus)
{
    switch (TimeStatus)
    {
    case 0:
        u8g2_SetFont(&u8g2, u8g2_font_inr21_mf);
        u8g2_DrawStr(&u8g2, 19, 24, "23"); //Variable
        u8g2_DrawStr(&u8g2, 55, 24, ":");  //Blink for second
        u8g2_DrawStr(&u8g2, 73, 24, "59"); //Variable
        u8g2_SetFont(&u8g2, u8g2_font_5x7_tr);
        u8g2_DrawStr(&u8g2, 40, 36, "31"); //Variable
        u8g2_DrawStr(&u8g2, 50, 36, ".");
        u8g2_DrawStr(&u8g2, 54, 36, "12"); //Variable
        u8g2_DrawStr(&u8g2, 64, 36, ".");
        u8g2_DrawStr(&u8g2, 69, 36, "2019"); //Variable
        break;
    case 1:
        break;
    default:
        break;
    }
}
void Filter_Pa(uint8_t PaStatus)
{
    u8g2_SetFont(&u8g2, u8g2_font_5x8_tr);
    u8g2_DrawStr(&u8g2, 3, 51, "Filter:");
    u8g2_DrawStr(&u8g2, 77, 51, "Pa");
    switch (PaStatus)
    {
    case 0:
        u8g2_SetFont(&u8g2, u8g2_font_5x7_tr);
        u8g2_DrawStr(&u8g2, 55, 51, "000"); //Variable
        break;
    case 1:
        break;
    default:
        break;
    }
}
void Flow(uint8_t FlowStatus)
{
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mf);
    u8g2_DrawStr(&u8g2, 3, 61, "Flow  :");
    u8g2_SetFont(&u8g2, u8g2_font_5x7_mf);
    u8g2_DrawStr(&u8g2, 77, 61, "m/s");
    switch (FlowStatus)
    {
    case 0:
        u8g2_SetFont(&u8g2, u8g2_font_5x7_tr);
        u8g2_DrawStr(&u8g2, 46, 61, "00"); //Variable
        u8g2_DrawStr(&u8g2, 56, 61, ".");
        u8g2_DrawStr(&u8g2, 60, 61, "00"); //Variable
        break;
    case 1:
        break;
    default:
        break;
    }
}
void Door(uint8_t DStatus)
{
    u8g2_SetFont(&u8g2, u8g2_font_5x8_mf);
    u8g2_DrawStr(&u8g2, 108, 61, "Door");
    switch (DStatus)
    {
    case 0:
        u8g2_DrawBox(&u8g2, 115, 50, 5, 3); //0
        break;
    case 1:
        u8g2_DrawBox(&u8g2, 115, 50, 5, 3); //0
        u8g2_DrawBox(&u8g2, 113, 46, 9, 3); //1
        break;
    case 2:
        u8g2_DrawBox(&u8g2, 115, 50, 5, 3);  //0
        u8g2_DrawBox(&u8g2, 113, 46, 9, 3);  //1
        u8g2_DrawBox(&u8g2, 111, 42, 13, 3); //2
        break;
    default:
        break;
    }
}
void Info_Screen_Manage(void)
{
    SysTime(Info_Scr.SysTime);
    Filter_Pa(Info_Scr.Filter_Pa);
    Flow(Info_Scr.Flow);
    Door(Info_Scr.Door);
    u8g2_DrawHLine(&u8g2, 29, 40, 70);
}
// void SysWork(uint8_t WorkStatus)
// {
//     switch(WorkStatus)
//     {
//         case 0:
//             u8g2_SetFont(&u8g2, u8g2_font_5x8_mf  );
//             u8g2_DrawStr(&u8g2, 7, 19, "System Work:");
//             u8g2_SetFont(&u8g2, u8g2_font_5x7_tr);
//             u8g2_DrawStr(&u8g2, 93, 19, "999999");
//             u8g2_DrawStr(&u8g2, 124, 19, "h");
//             break;
//         case 1:
//             break;
//         default:
//             break;
//     }
// }

// void Filter_Time(uint8_t FTStatus)
// {
//     switch (FTStatus)
//     {
//         case 0:
//             u8g2_SetFont(&u8g2, u8g2_font_5x7_tr);
//             u8g2_DrawStr(&u8g2, 7, 39, "Filter Time:");
//             u8g2_DrawStr(&u8g2, 96, 39, "0000");
//             u8g2_DrawStr(&u8g2, 124, 39, "h");
//             break;
//         case 1:
//             break;
//         default:
//             break;
//     }
// }

// void UV_Time(uint8_t UvStatus)
// {
//     switch(UvStatus)
//     {
//         case 0:
//             u8g2_SetFont(&u8g2, u8g2_font_5x7_tr);
//             u8g2_DrawStr(&u8g2, 7, 59, "UV Time:");
//             u8g2_DrawStr(&u8g2, 90, 59, "0000");
//             u8g2_DrawStr(&u8g2, 124, 59, "h");
//             break;
//         case 1:
//             break;
//         default:
//             break;
//     }
// }