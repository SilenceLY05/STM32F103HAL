#ifndef __BSP_ILI9341_LCD_H
#define __BSP_ILI9341_LCD_H

#include "stm32f1xx.h"

#include "fonts.h"

/***************************************************************************************
2^26 =0X0400 0000 = 64MB,ÿ�� BANK ��4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

ѡ��BANK1-BORSRAM1 ���� TFT����ַ��ΧΪ0X6000 0000 ~ 0X63FF FFFF
FSMC_A16 ��LCD��DC(�Ĵ���/����ѡ��)��
�Ĵ�������ַ = 0X60000000
RAM����ַ = 0X60020000 = 0X60000000+2^16*2 = 0X60000000 + 0X20000 = 0X60020000
��ѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼���  
****************************************************************************************/

/******************************* ILI9341 ��ʾ���� FSMC �������� ***************************/
//FSMC_Bank1_NORSRAM����LCD��������ĵ�ַ
#define FSMC_Addr_ILI9341_CMD        ((uint32_t)0x60000000)

//FSMC_Bank1_NORSRAM����LCD���ݲ����ĵ�ַ
#define FSMC_Addr_ILI9341_DATA       ((uint32_t)0x60020000)

//��Ƭѡ���ž�����NOR/SRAM��
#define FSMC_Bank1_NORSRAMx          FSMC_NORSRAM_BANK1

///******************************* ILI9341 ��ʾ��8080ͨѶ���Ŷ��� ***************************/
///******�����ź���******/

///* Ƭѡ��ѡ��NOR/SRAM�� */
//#define ILI9341_CS_PORT            GPIOD
//#define ILI9341_CS_CLK             RCC_APB2Periph_GPIOD
//#define ILI9341_CS_PIN             GPIO_PIN_7

////DC���ţ�ʹ��FSMC�ĵ�ַ�źſ��ƣ������ž����˷���LCDʱʹ�õĵ�ַ
////PD11ΪFSMC_A16
//#define ILI9341_DC_PORT            GPIOD
//#define ILI9341_DC_CLK             RCC_APB2Periph_GPIOD
//#define ILI9341_DC_PIN             GPIO_PIN_11

////дʹ��
//#define ILI9341_WR_PORT            GPIOD
//#define ILI9341_WR_CLK             RCC_APB2Periph_GPIOD
//#define ILI9341_WR_PIN             GPIO_PIN_5

////��ʹ��
//#define ILI9341_RD_PORT            GPIOD
//#define ILI9341_RD_CLK             RCC_APB2Periph_GPIOD
//#define ILI9341_RD_PIN             GPIO_PIN_4

////��λ����
//#define ILI9341_RST_PORT           GPIOE
//#define ILI9341_RST_CLK            RCC_APB2Periph_GPIOE
//#define ILI9341_RST_PIN            GPIO_PIN_1

////��������
//#define ILI9341_BK_PORT            GPIOD
//#define ILI9341_BK_CLK             RCC_APB2Periph_GPIOD
//#define ILI9341_BK_PIN             GPIO_PIN_12




///* D�����ź��� */
//#define ILI9341_D0_GPIO_PORT            GPIOD
//#define ILI9341_D0_GPIO_CLK             RCC_APB2Periph_GPIOD
//#define ILI9341_D0_GPIO_PIN             GPIO_PIN_14

//#define ILI9341_D1_GPIO_PORT            GPIOD
//#define ILI9341_D1_GPIO_CLK             RCC_APB2Periph_GPIOD
//#define ILI9341_D1_GPIO_PIN             GPIO_PIN_15

//#define ILI9341_D2_GPIO_PORT            GPIOD
//#define ILI9341_D2_GPIO_CLK             RCC_APB2Periph_GPIOD
//#define ILI9341_D2_GPIO_PIN             GPIO_PIN_0

//#define ILI9341_D3_GPIO_PORT            GPIOD
//#define ILI9341_D3_GPIO_CLK             RCC_APB2Periph_GPIOD
//#define ILI9341_D3_GPIO_PIN             GPIO_PIN_1

//#define ILI9341_D4_GPIO_PORT            GPIOE
//#define ILI9341_D4_GPIO_CLK             RCC_APB2Periph_GPIOE
//#define ILI9341_D4_GPIO_PIN             GPIO_PIN_7

//#define ILI9341_D5_GPIO_PORT            GPIOE
//#define ILI9341_D5_GPIO_CLK             RCC_APB2Periph_GPIOE
//#define ILI9341_D5_GPIO_PIN             GPIO_PIN_8

//#define ILI9341_D6_GPIO_PORT            GPIOE
//#define ILI9341_D6_GPIO_CLK             RCC_APB2Periph_GPIOE
//#define ILI9341_D6_GPIO_PIN             GPIO_PIN_9

//#define ILI9341_D7_GPIO_PORT            GPIOE
//#define ILI9341_D7_GPIO_CLK             RCC_APB2Periph_GPIOE
//#define ILI9341_D7_GPIO_PIN             GPIO_PIN_10

//#define ILI9341_D8_GPIO_PORT            GPIOE
//#define ILI9341_D8_GPIO_CLK             RCC_APB2Periph_GPIOE
//#define ILI9341_D8_GPIO_PIN             GPIO_PIN_11

//#define ILI9341_D9_GPIO_PORT            GPIOE
//#define ILI9341_D9_GPIO_CLK             RCC_APB2Periph_GPIOE
//#define ILI9341_D9_GPIO_PIN             GPIO_PIN_12

//#define ILI9341_D10_GPIO_PORT           GPIOE
//#define ILI9341_D10_GPIO_CLK            RCC_APB2Periph_GPIOE
//#define ILI9341_D10_GPIO_PIN            GPIO_PIN_13

//#define ILI9341_D11_GPIO_PORT           GPIOE
//#define ILI9341_D11_GPIO_CLK            RCC_APB2Periph_GPIOE
//#define ILI9341_D11_GPIO_PIN            GPIO_PIN_14

//#define ILI9341_D12_GPIO_PORT           GPIOE
//#define ILI9341_D12_GPIO_CLK            RCC_APB2Periph_GPIOE
//#define ILI9341_D12_GPIO_PIN            GPIO_PIN_15

//#define ILI9341_D13_GPIO_PORT           GPIOD
//#define ILI9341_D13_GPIO_CLK            RCC_APB2Periph_GPIOD
//#define ILI9341_D13_GPIO_PIN            GPIO_PIN_8

//#define ILI9341_D14_GPIO_PORT           GPIOD
//#define ILI9341_D14_GPIO_CLK            RCC_APB2Periph_GPIOD
//#define ILI9341_D14_GPIO_PIN            GPIO_PIN_9

//#define ILI9341_D15_GPIO_PORT           GPIOD
//#define ILI9341_D15_GPIO_CLK            RCC_APB2Periph_GPIOD
//#define ILI9341_D15_GPIO_PIN            GPIO_PIN_10

/* ����Ԥ�� */
#define DEBUG_DELAY()

/* ILI9341��ʾ�������ʼ������������� */
#define ILI9341_DispWindow_X_Star     0  //��ʼ���X����
#define ILI9341_DispWindow_Y_Star     0  //��ʼ���Y����

#define ILI9341_LESS_PIXEL            240//Һ�����϶̷�������ؿ��
#define ILI9341_MORE_PIXEL            320//Һ�����϶̷�������ؿ��

//����Һ��ɨ�跽����仯��XY���ؿ��
//����ILI9341_GramScan�������÷���ʱ���Զ�����
extern uint16_t LCD_X_LENGTH,LCD_Y_LENGTH;

//Һ����ɨ��ģʽ
//������ѡֵΪ0-7
extern uint8_t LCD_SCAN_MODE;

/* ���� ILI9341 ��ʾ��������ɫ */
#define BACKGROUND              BLACK  //Ĭ�ϱ�����ɫ

#define WHITE                   0xFFFF//��ɫ
#define BLACK                   0x0000//��ɫ
#define GREY                    0xF7DE//��ɫ
#define BLUE                    0x001F//��ɫ
#define BLUE2                   0x051F//ǳ��ɫ
#define RED                     0xF800//��ɫ
#define MAGENTA                 0xF81F//����ɫ
#define GREEN                   0x07E0//��ɫ
#define CYAN                    0x7FFF//����ɫ
#define YELLOW                  0xFFE0//��ɫ
#define BRED                    0xF81F//��ɫ
#define GRED                    0xFFE0//��ɫ
#define GBLUE                   0x07FF//��ɫ


/* ����ILI9341�������� */
#define CMD_SetCoordinateX      0x2A    //����X����
#define CMD_SetCoordinateY      0x2B    //����Y����
#define CMD_SetPixel            0x2C    //�������


//#define RGB888_2_RGB565(R,G,B)		 (uint16_t)(((R & 0x1F) << 11) | ((G & 0x3F) << 5) | (B & 0x1F) )

/* ���� LCD ����оƬ ID */
#define     LCDID_UNKNOWN             0
#define     LCDID_ILI9341             0x9341
#define     LCDID_ST7789V             0x8552


/********************************** ���� ILI934 ���� ***************************************/
void                     ILI9341_Init                    ( void );
uint16_t                 ILI9341_ReadID                 ( void );
void                     ILI9341_Rst                     ( void );
void                     ILI9341_BackLed_Control         ( FunctionalState enumState );
void                     ILI9341_GramScan                ( uint8_t ucOtion );
void                     ILI9341_OpenWindow              ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight );
void                     ILI9341_Clear                   ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight );
void                     ILI9341_SetPointPixel           ( uint16_t usX, uint16_t usY );
uint16_t                 ILI9341_GetPointPixel           ( uint16_t usX , uint16_t usY );
void                     ILI9341_DrawLine                ( uint16_t usX1, uint16_t usY1, uint16_t usX2, uint16_t usY2);
void                     ILI9341_DrawRectangle           ( uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight,uint8_t ucFilled );
void                     ILI9341_DrawCircle              ( uint16_t usX_Center, uint16_t usY_Center, uint16_t usRadius, uint8_t ucFilled );
void                     ILI9341_DispChar_EN             ( uint16_t usX, uint16_t usY, const char cChar );
void                     ILI9341_DispStringLine_EN      ( uint16_t line, char * pStr );
void                     ILI9341_DispString_EN      			( uint16_t usX, uint16_t usY, char * pStr );
void 					 ILI9341_DispString_EN_YDir 		(   uint16_t usX,uint16_t usY ,  char * pStr );
void ILI9341_DispChar_CH(uint16_t usX,uint16_t usY,uint16_t usChar);
void ILI9341_DispString_CH(uint16_t usX,uint16_t usY,char *pStr);
void ILI9341_DispStringLine_EN_CH(uint16_t line,char *pStr);
void ILI9341_DispString_EN_CH(uint16_t usX,uint16_t usY,char *pStr);
void ILI9341_DispString_EN_CH_YDir(uint16_t usX,uint16_t usY,char *pStr);
void ILI9341_zoomChar(uint16_t in_width,uint16_t in_heig,uint16_t out_width,uint16_t out_heig,uint8_t *in_ptr,uint8_t *out_ptr,uint8_t en_cn);
void ILI9341_DrawChar_Ex(uint16_t usX,uint16_t usY,uint16_t Font_width,uint16_t Font_Height,uint8_t *c,uint16_t DrawModel);
void ILI9341_DisplayStringEx(uint16_t x,uint16_t y,uint16_t Font_width,uint16_t Font_Height,uint8_t *ptr,uint16_t DrawModel);
void ILI9341_DisplayStringEx_YDir(uint16_t x,uint16_t y,uint16_t Font_width,uint16_t Font_Height,uint8_t *ptr,uint16_t DrawModel);


void 											LCD_SetFont											(sFONT *fonts);
sFONT 										*LCD_GetFont											(void);
void 											LCD_ClearLine										(uint16_t Line);
void 											LCD_SetBackColor								(uint16_t Color);
void 											LCD_SetTextColor								(uint16_t Color)	;
void 											LCD_SetColors										(uint16_t TextColor, uint16_t BackColor);
void 											LCD_GetColors										(uint16_t *TextColor, uint16_t *BackColor);


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define digitalH(p,i)      {p->BSRR=i;}           //����Ϊ�ߵ�ƽ
#define digitalL(p,i)      {p->BSRR=(uint32_t)i<<16;}  //����͵�ƽ




																				

#endif /* __BSP_ILI9341_LCD_H */

