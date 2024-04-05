#ifndef __FONT_H
#define __FONT_H       

#include "stm32f1xx.h"
#include "fonts.h"


#define LINE(x) ((x) * (((sFONT *)LCD_GetFont())->Height))
#define LINEY(x) ((x) * (((sFONT *)LCD_GetFont())->Width))

/** @defgroup FONTS_Exported_Types
  * @{
  */ 
typedef struct _tFont
{    
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
  
} sFONT;

typedef struct
{
	unsigned char Index[2];
	uint8_t Msk[32*32/8];
}LCD_Typedef_FONT;   //������ģ�ṹ��

extern sFONT Font24x32;
extern sFONT Font16x24;
extern sFONT Font8x16;
extern const uint8_t lcd_font[][32*32/8];
extern const LCD_Typedef_FONT lcd_font1[2];

//Ҫ֧��������Ҫʵ�ֱ��������ɲο���Һ����ʾ��Ӣ�ģ��ֿ����ⲿFLASH��������
#define      GetGBKCode( ucBuffer, usChar ) 


#endif /*end of __FONT_H    */
