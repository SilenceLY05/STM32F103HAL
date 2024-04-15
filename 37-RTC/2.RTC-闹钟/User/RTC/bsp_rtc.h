#ifndef __BSP_RTC_H__
#define __BSP_RTC_H__


#include "stm32f1xx.h"

#define USE_LCD_DISPLAY

extern RTC_HandleTypeDef Rtc_Handle;
extern __IO uint8_t Alarmflag;


//时钟源宏定义
#define RTC_CLOCK_SOURCE_LSE

#define HOURS                     0x11
#define MINUTES                   0x35
#define SECONDS                   0x00


//日期宏定义 其中WEEKDAY为无效宏定义，只要给出年月日，HAL库会自动计算出WEEKDAY
#define WEEKDAY                   4
#define DATE                      0x12
#define MONTH                     0x10
#define YEAR                      0x23



//闹钟相关宏定义
#define ALARM_HOURS               0x11
#define ALARM_MINUTES             0x35
#define ALARM_SECONDS             0x10

#define RTC_Alarm_X               RTC_ALARM_A


//备份域寄存器宏定义
#define RTC_BKP_DRX               RTC_BKP_DR1
//写入到备份寄存器的数据宏定义
#define RTC_BKP_DATA              0x32F2



void RTC_CalendarConfig(void);
void RTC_TimeAndDate_Show(void);
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc);
void RTC_AlarmSet(void);



#endif /* __BSP_RTC_H__ */

