#ifndef __BSP_RTC_H__
#define __BSP_RTC_H__


#include "stm32f1xx.h"

#define USE_LCD_DISPLAY

extern RTC_HandleTypeDef Rtc_Handle;


//ʱ��Դ�궨��
#define RTC_CLOCK_SOURCE_LSE

#define HOURS                     0x10
#define MINUTES                   0x10
#define SECONDS                   0x12


//���ں궨�� ����WEEKDAYΪ��Ч�궨�壬ֻҪ���������գ�HAL����Զ������WEEKDAY
#define WEEKDAY                   4
#define DATE                      0x12
#define MONTH                     0x10
#define YEAR                      0x23


//������Ĵ����궨��
#define RTC_BKP_DRX               RTC_BKP_DR1
//д�뵽���ݼĴ��������ݺ궨��
#define RTC_BKP_DATA              0x32F2



void RTC_CalendarConfig(void);
void RTC_TimeAndDate_Show(void);
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc);




#endif /* __BSP_RTC_H__ */

