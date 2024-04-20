#include "bsp_ov7725.h"
#include "bsp_sccb.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_usart.h"



//����ͷ��ʼ������
//ע�⣺ʹ�����ַ�ʽ��ʼ���ṹ�壬Ҫ��c/c++ѡ����ѡ�� C99 mode
OV7725_MODE_PARAM cam_mode = 
{
	.QVGA_VGA = 0,   //QVGAģʽ
	.cam_sx = 0,
	.cam_sy = 0,
	
	.cam_width = 0,
	.cam_height =0,
	
	.lcd_sx = 0,
	.lcd_sy = 0,
	.lcd_scan = 3,//LCDɨ��ģʽ�����������ÿ���1��3��5��7ģʽ
	
	//���¿ɸ����Լ�����Ҫ������������Χ���ṹ�����Ͷ���	
	.light_mode = 0,   //�Զ�����ģʽ
	.saturation = 0,
	.brightness = 0,
	.contrast = 0,
	.effect = 0,       //����ģʽ
	
	
};


typedef struct Reg
{
	uint8_t Address;          //�Ĵ�����ַ
	uint8_t Value;            //�Ĵ���ֵ
	
}Reg_Info;


/* �Ĵ����������� */
Reg_Info Sensor_Config[]=
{
	{REG_CLKRC,       0x00},   /*clock config*/
	{REG_COM7,        0x46},   /*QVGA RGB565 */
	{REG_HSTART,      0x3f},
	{REG_HSIZE,       0x50},
	{REG_VSTRT,       0x03},
	{REG_VSIZE,       0x78},
	{REG_HREF,        0x00},
	{REG_HOutSize,    0x50},
	{REG_VOutSize,    0x78},
	{REG_EXHCH,       0x00},
	
	
	/*DSP control*/
	{REG_TGT_B,     0x7f},
	{REG_FixGain,   0x09},
	{REG_AWB_Ctrl0, 0xe0},
	{REG_DSP_Ctrl1, 0xff},
	{REG_DSP_Ctrl2, 0x20},
	{REG_DSP_Ctrl3,	0x00},
	{REG_DSP_Ctrl4, 0x00},

	/*AGC AEC AWB*/
	{REG_COM8,		0xf0},
	{REG_COM4,		0x81}, /*Pll AEC CONFIG*/
	{REG_COM6,		0xc5},
	{REG_COM9,		0x21},
	{REG_BDBase,	0xFF},
	{REG_BDMStep,	0x01},
	{REG_AEW,		0x34},
	{REG_AEB,		0x3c},
	{REG_VPT,		0xa1},
	{REG_EXHCL,		0x00},
	{REG_AWBCtrl3,  0xaa},
	{REG_COM8,		0xff},
	{REG_AWBCtrl1,  0x5d},

	{REG_EDGE1,		0x0a},
	{REG_DNSOff,	0x01},
	{REG_EDGE2,		0x01},
	{REG_EDGE3,		0x01},

	{REG_MTX1,		0x5f},
	{REG_MTX2,		0x53},
	{REG_MTX3,		0x11},
	{REG_MTX4,		0x1a},
	{REG_MTX5,		0x3d},
	{REG_MTX6,		0x5a},
	{REG_MTX_Ctrl,  0x1e},

	{REG_BRIGHT,	0x00},
	{REG_CNST,		0x25},
	{REG_USAT,		0x65},
	{REG_VSAT,		0x65},
	{REG_UVADJ0,	0x81},
	//{REG_SDE,		  0x20},	//�ڰ�
	{REG_SDE,		  0x06},	//��ɫ	����SDE����Ĵ���������ʵ������Ч��
	
    /*GAMMA config*/
	{REG_GAM1,		0x0c},
	{REG_GAM2,		0x16},
	{REG_GAM3,		0x2a},
	{REG_GAM4,		0x4e},
	{REG_GAM5,		0x61},
	{REG_GAM6,		0x6f},
	{REG_GAM7,		0x7b},
	{REG_GAM8,		0x86},
	{REG_GAM9,		0x8e},
	{REG_GAM10,		0x97},
	{REG_GAM11,		0xa4},
	{REG_GAM12,		0xaf},
	{REG_GAM13,		0xc5},
	{REG_GAM14,		0xd7},
	{REG_GAM15,		0xe8},
	{REG_SLOP,		0x20},

	{REG_HUECOS,	0x80},
	{REG_HUESIN,	0x80},
	{REG_DSPAuto,	0xff},
	{REG_DM_LNL,	0x00},
	{REG_BDBase,	0x99},
	{REG_BDMStep,	0x03},
	{REG_LC_RADI,	0x00},
	{REG_LC_COEF,	0x13},
	{REG_LC_XC,		0x08},
	{REG_LC_COEFB,  0x14},
	{REG_LC_COEFR,  0x17},
	{REG_LC_CTR,	0x05},
	
	{REG_COM3,		0xd0},/*Horizontal mirror image*/

	/*night mode auto frame rate control*/
	{REG_COM5,		0xf5},	 /*��ҹ�ӻ����£��Զ�����֡�ʣ���֤���նȻ�������*/
	//{REG_COM5,		0x31},	/*ҹ�ӻ���֡�ʲ���*/
};

uint8_t OV7725_REG_NUM = sizeof(Sensor_Config)/ sizeof(Sensor_Config[0]);     /* �ṹ�������Ա��Ŀ */

volatile uint8_t Ov7725_vsync;    /* ֡ͬ���źű�־�����жϺ�����main��������ʹ�� */



/* FIFO GPIO���� */
static void FIFO_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pin = OV7725_OE_GPIO_PIN;
	HAL_GPIO_Init(OV7725_OE_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = OV7725_WRST_GPIO_PIN;
	HAL_GPIO_Init(OV7725_WRST_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = OV7725_RRST_GPIO_PIN;
	HAL_GPIO_Init(OV7725_RRST_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = OV7725_RCLK_GPIO_PIN;
	HAL_GPIO_Init(OV7725_RCLK_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = OV7725_WE_GPIO_PIN;
	HAL_GPIO_Init(OV7725_WE_GPIO_PORT,&GPIO_InitStructure);
	
	
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = OV7725_DATA_0_GPIO_PIN | OV7725_DATA_1_GPIO_PIN | OV7725_DATA_2_GPIO_PIN | 
													 OV7725_DATA_3_GPIO_PIN | OV7725_DATA_4_GPIO_PIN | OV7725_DATA_5_GPIO_PIN | 
													 OV7725_DATA_6_GPIO_PIN | OV7725_DATA_7_GPIO_PIN;
	HAL_GPIO_Init(OV7725_DATA_GPIO_PORT,&GPIO_InitStructure);
	
	
	FIFO_OE_L();                  //����ʹFIFO���ʹ��
	FIFO_WE_H();                  //����ʹFIFOд����
	
	
}



/* VSYNC�ж����� */
static void VSYNC_GPIO_Config(void)
{
	GPIO_InitTypeDef         GPIO_InitStructure;
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = OV7725_VSYNC_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(OV7725_VSYNC_GPIO_PORT,&GPIO_InitStructure);
	
	HAL_NVIC_SetPriority(OV7725_VSYNC_EXTI_IRQ,0,3);
	HAL_NVIC_EnableIRQ(OV7725_VSYNC_EXTI_IRQ);
}



/* ��ʼ����������ͷ������IO */
void OV7725_GPIO_Config(void)
{
	SCCB_GPIO_Config();
	FIFO_GPIO_Config();
	VSYNC_GPIO_Config();
}


/* Sensor ��ʼ�� */
ErrorStatus OV7725_Init(void)
{
	uint16_t i = 0;
	uint8_t Sensor_IDCode = 0;
	
	if(SCCB_WriteByte(0x12,0x80) == 0)
	{
		return ERROR;
	}
	
	if(SCCB_ReadByte(&Sensor_IDCode,1,0x0b) == 0)
	{
		return ERROR;
	}
	
	if(Sensor_IDCode == OV7725_ID)
	{
		for(i=0;i<OV7725_REG_NUM;i++)
		{
			if(SCCB_WriteByte(Sensor_Config[i].Address,Sensor_Config[i].Value) == 0)
			{
				return ERROR;
			}
		}
	}
	else
	{
		return ERROR;
	}
	return SUCCESS;
}



/* ���ù���ģʽ mode������ģʽ 0-5 0���Զ� 1������ 2������ 3���칫�� 4������ 5��ҹ�� */
void OV7725_Light_Mode(uint8_t mode)
{
	switch(mode)
	{
		case 0:	//Auto���Զ�ģʽ
			SCCB_WriteByte(0x13, 0xff); //AWB on 
			SCCB_WriteByte(0x0e, 0x65);
			SCCB_WriteByte(0x2d, 0x00);
			SCCB_WriteByte(0x2e, 0x00);
			break;
		case 1://sunny������
			SCCB_WriteByte(0x13, 0xfd); //AWB off
			SCCB_WriteByte(0x01, 0x5a);
			SCCB_WriteByte(0x02, 0x5c);
			SCCB_WriteByte(0x0e, 0x65);
			SCCB_WriteByte(0x2d, 0x00);
			SCCB_WriteByte(0x2e, 0x00);
			break;	
		case 2://cloudy������
			SCCB_WriteByte(0x13, 0xfd); //AWB off
			SCCB_WriteByte(0x01, 0x58);
			SCCB_WriteByte(0x02, 0x60);
			SCCB_WriteByte(0x0e, 0x65);
			SCCB_WriteByte(0x2d, 0x00);
			SCCB_WriteByte(0x2e, 0x00);
			break;	
		case 3://office���칫��
			SCCB_WriteByte(0x13, 0xfd); //AWB off
			SCCB_WriteByte(0x01, 0x84);
			SCCB_WriteByte(0x02, 0x4c);
			SCCB_WriteByte(0x0e, 0x65);
			SCCB_WriteByte(0x2d, 0x00);
			SCCB_WriteByte(0x2e, 0x00);
			break;	
		case 4://home������
			SCCB_WriteByte(0x13, 0xfd); //AWB off
			SCCB_WriteByte(0x01, 0x96);
			SCCB_WriteByte(0x02, 0x40);
			SCCB_WriteByte(0x0e, 0x65);
			SCCB_WriteByte(0x2d, 0x00);
			SCCB_WriteByte(0x2e, 0x00);
			break;	
		
		case 5://night��ҹ��
			SCCB_WriteByte(0x13, 0xff); //AWB on
			SCCB_WriteByte(0x0e, 0xe5);
			break;	
		
		default:
			 OV7725_DEBUG("Light Mode parameter error!"); 

			break;
	}
}


/* ���ñ��Ͷ� sat:���Ͷ� �ο���Χ-4~+4 */
void OV7725_Color_Saturation(int8_t sat)
{
	if(sat>=-4 && sat<=4)
	{
		SCCB_WriteByte(REG_USAT,(sat+4)<<4);
		SCCB_WriteByte(REG_VSAT,(sat+4)<<4);
	}
	else
	{
		OV7725_DEBUG("Color Saturation parameter error!");
	}
}


/* ���ù��ն� bri:���նȣ��ο���Χ[-4~+4] */
void OV7725_Brightness(int8_t bri)
{
	uint8_t BRIGHT_Value,SIGN_Value;
	
	switch(bri)
	{
		case 4:
			BRIGHT_Value = 0x48;
			SIGN_Value = 0x06;
		break;
		
		case 3:
			BRIGHT_Value = 0x38;
			SIGN_Value = 0x06;
		break;
		
		case 2:
			BRIGHT_Value = 0x28;
			SIGN_Value = 0x06;
		break;
		
		case 1:
			BRIGHT_Value = 0x18;
			SIGN_Value = 0x06;
		break;
		
		case 0:
			BRIGHT_Value = 0x08;
			SIGN_Value = 0x06;
		break;
		
		case -1:
			BRIGHT_Value = 0x08;
			SIGN_Value = 0x0E;
		break;
		
		case -2:
			BRIGHT_Value = 0x18;
			SIGN_Value = 0x0E;
		break;
		
		case -3:
			BRIGHT_Value = 0x28;
			SIGN_Value = 0x0E;
		break;
		
		case -4:
			BRIGHT_Value = 0x38;
			SIGN_Value = 0x0E;
		break;
		
		default:
			OV7725_DEBUG("Brightness parameter error!");
		break;
	}
	SCCB_WriteByte(REG_BRIGHT,BRIGHT_Value);
	SCCB_WriteByte(REG_SIGN,SIGN_Value);
}



/* ���öԱȶ� cnst:�Աȶȣ��ο���Χ[-4~+4]*/
void OV7725_Contrast(int8_t cnst)
{
	if(cnst>=-4&&cnst<=4)
	{
		SCCB_WriteByte(REG_CNST,(0x30-(4-cnst)*4));
	}
	else
	{
		OV7725_DEBUG("Contrast parameter error!");
	}
}



/* ��������Ч�� eff:����Ч����������Χ[0~6] 0:���� 1:�ڰ� 2:ƫ�� 3:���� 4:ƫ�� 5:ƫ�� 6:���� */
void OV7725_Special_Effect(uint8_t eff)
{
	switch(eff)
	{
		case 0://����
			SCCB_WriteByte(0xa6, 0x06);
			SCCB_WriteByte(0x60, 0x80);
			SCCB_WriteByte(0x61, 0x80);
		break;
		
		case 1://�ڰ�
			SCCB_WriteByte(0xa6, 0x26);
			SCCB_WriteByte(0x60, 0x80);
			SCCB_WriteByte(0x61, 0x80);
		break;	
		
		case 2://ƫ��
			SCCB_WriteByte(0xa6, 0x1e);
			SCCB_WriteByte(0x60, 0xa0);
			SCCB_WriteByte(0x61, 0x40);	
		break;	
		
		case 3://����
			SCCB_WriteByte(0xa6, 0x1e);
			SCCB_WriteByte(0x60, 0x40);
			SCCB_WriteByte(0x61, 0xa0);	
		break;	
		
		case 4://ƫ��
			SCCB_WriteByte(0xa6, 0x1e);
			SCCB_WriteByte(0x60, 0x80);
			SCCB_WriteByte(0x61, 0xc0);		
		break;	
		
		case 5://ƫ��
			SCCB_WriteByte(0xa6, 0x1e);
			SCCB_WriteByte(0x60, 0x60);
			SCCB_WriteByte(0x61, 0x60);		
		break;	
		
		case 6://����
			SCCB_WriteByte(0xa6, 0x46);
		break;	
				
		default:
			OV7725_DEBUG("Special Effect error!");
			break;
	}
}		


/**
  * @brief  ����ͼ��������ڣ��ֱ��ʣ�QVGA
	* @param  sx:����x��ʼλ��
	* @param  sy:����y��ʼλ��
	* @param  width:���ڿ��
	* @param  height:���ڸ߶�
	* @param QVGA_VGA:0,QVGAģʽ��1��VGAģʽ
  *
	* @note 	QVGAģʽ ����Ҫ��sx + width <= 320 ,sy+height <= 240
	* 			 	VGAģʽ����Ҫ��sx + width <= 640 ,sy+height <= 480
	*					������ Һ�����ֱ��� �� FIFO�ռ� �����ƣ������̲������ڳ���320*240������
	*         ʹ��VGAģʽ��Ҫ����ΪOV7725�޷�ֱ�ӽ���XY����QVGA������ʹ����ƽ��ʾ��
	*					���ó�VGAģʽ������ʹ��������ʾ��
	*					���QVGAģʽ��ͬ���ֱ����� VGAģʽ ͼ�����֡������
  * @retval ��
  */
void OV7725_Window_Set(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height,uint8_t QVGA_VGA)
{
	uint8_t reg_raw,cal_temp;
	
	if(QVGA_VGA == 0)
	{
		SCCB_WriteByte(REG_COM7,0x46);
	}
	else
	{
		SCCB_WriteByte(REG_COM7,0x06);
	}
	
	/***************HSTART*********************/
	/* ��ȡ�Ĵ�����ԭ���ݣ�HStart����ƫ��ֵ����ԭʼƫ��ֵ�Ļ����ϼ��ϴ���ƫ�� */
	SCCB_ReadByte(&reg_raw,1,REG_HSTART);
	
	//sxΪ����ƫ�ƣ���8λ�洢��HSTART����2λ��HREF
	cal_temp = (reg_raw+(sx>>2));
	SCCB_WriteByte(REG_HSTART,cal_temp);
	
	/***************HSIZE*********************/
	/* ˮƽ��ȣ���8λ�洢��HSIZE����2λ�洢��HREF */
	SCCB_WriteByte(REG_HSIZE,width>>2);   //HSIZE������λ
	
	
	/***************VSTART*********************/
	/* ��ȡ�Ĵ������ݣ�VStart����ƫ��ֵ����ԭʼƫ��ֵ�Ļ����ϼ��ϴ���ƫ�� */
	SCCB_ReadByte(&reg_raw,1,REG_VSTRT);
	//syΪ����ƫ�ƣ���8λ�洢��HSTART����1λ��HREF
	cal_temp = (reg_raw+(sy>>1));
	SCCB_WriteByte(REG_VSTRT,cal_temp);
	
	/***************VSIZE*********************/
	/* ��ֱ�߶ȣ���8λ�洢��VSIZE����1λ�洢��HREF */
	SCCB_WriteByte(REG_VSIZE,height>>1);
	
	/***************VSTART*********************/
	/* ��ȡ�Ĵ������ݣ�VStart����ƫ��ֵ����ԭʼƫ��ֵ�Ļ����ϼ��ϴ���ƫ�� */
	SCCB_ReadByte(&reg_raw,1,REG_HREF);
	//��ˮƽ��ȵĵ�2λ����ֱ�߶ȵĵ�1λ��ˮƽƫ�Ƶĵ�2λ����ֱƫ�Ƶĵ�1λ��������ӵ�HREF
	cal_temp = (reg_raw |(width&0x03) | ((height&0x01)<<2) | ((sx&0x03)<<4) | ((sy&0x01)<<6));
	
	SCCB_WriteByte(REG_HREF,cal_temp);
	
	/* ��ֱ�߶ȣ���8λ�洢��VSIZE����1λ�洢��HREF */
	SCCB_WriteByte(REG_HOutSize,width>>2);
	SCCB_WriteByte(REG_VOutSize,height>>1);
	
	
	/* ��ȡ�Ĵ�����ԭ���� */
	SCCB_ReadByte(&reg_raw,1,REG_EXHCH);
	cal_temp = (reg_raw|(width&0x03)|((height&0x01)<<2));
	
	SCCB_WriteByte(REG_EXHCH,cal_temp);
	

}



/**
  * @brief  ����ͼ��������ڣ��ֱ��ʣ�VGA
	* @param  sx:����x��ʼλ��
	* @param  sy:����y��ʼλ��
	* @param  width:���ڿ��
	* @param  height:���ڸ߶�
	* @note 	����������Ҫ��sx + width <= 640 ,sy+height <= 480
	*					������ Һ�����ֱ��� �� FIFO�ռ� �����ƣ������̲������ڳ���320*240������
	*         ʹ�ñ�������Ҫ����ΪOV7725�޷�ֱ�ӽ���XY����QVGA������ʹ����ƽ��ʾ��
	*					ʹ�ñ��������ó�VGAģʽ������ʹ��������ʾ��
	*					���QVGAģʽ��ͬ���ֱ����� VGAģʽ ͼ�����֡������
  * @retval ��
  */
void OV7725_Window_VGA_Set(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
	uint8_t reg_raw,cal_temp;
	
	/***********QVGA or VGA *************/
	/*VGA RGB565 */
	SCCB_WriteByte(REG_COM7,0x06);
	
	
	/***************HSTART*********************/
	/* ��ȡ�Ĵ�����ԭ���ݣ�HStart����ƫ��ֵ����ԭʼƫ��ֵ�Ļ����ϼ��ϴ���ƫ�� */
	SCCB_ReadByte(&reg_raw,1,REG_HSTART);
	
	//sxΪ����ƫ�ƣ���8λ�洢��HSTART����2λ��HREF
	cal_temp = (reg_raw+(sx>>2));
	SCCB_WriteByte(REG_HSTART,cal_temp);
	
	/***************HSIZE*********************/
	/* ˮƽ��ȣ���8λ�洢��HSIZE����2λ�洢��HREF */
	SCCB_WriteByte(REG_HSIZE,width>>2);   //HSIZE������λ
	
	
	/***************VSTART*********************/
	/* ��ȡ�Ĵ������ݣ�VStart����ƫ��ֵ����ԭʼƫ��ֵ�Ļ����ϼ��ϴ���ƫ�� */
	SCCB_ReadByte(&reg_raw,1,REG_VSTRT);
	//syΪ����ƫ�ƣ���8λ�洢��HSTART����1λ��HREF
	cal_temp = (reg_raw+(sy>>1));
	SCCB_WriteByte(REG_VSTRT,cal_temp);
	
	/***************VSIZE*********************/
	/* ��ֱ�߶ȣ���8λ�洢��VSIZE����1λ�洢��HREF */
	SCCB_WriteByte(REG_VSIZE,height>>1);
	
	/***************VSTART*********************/
	/* ��ȡ�Ĵ������ݣ�VStart����ƫ��ֵ����ԭʼƫ��ֵ�Ļ����ϼ��ϴ���ƫ�� */
	SCCB_ReadByte(&reg_raw,1,REG_HREF);
	//��ˮƽ��ȵĵ�2λ����ֱ�߶ȵĵ�1λ��ˮƽƫ�Ƶĵ�2λ����ֱƫ�Ƶĵ�1λ��������ӵ�HREF
	cal_temp = (reg_raw |(width&0x03) | ((height&0x01)<<2) | ((sx&0x03)<<4) | ((sy&0x01)<<6));
	
	SCCB_WriteByte(REG_HREF,cal_temp);
	
	/* ��ֱ�߶ȣ���8λ�洢��VSIZE����1λ�洢��HREF */
	SCCB_WriteByte(REG_HOutSize,width>>2);
	SCCB_WriteByte(REG_VOutSize,height>>1);
	
	
	/* ��ȡ�Ĵ�����ԭ���� */
	SCCB_ReadByte(&reg_raw,1,REG_EXHCH);
	cal_temp = (reg_raw|(width&0x03)|((height&0x01)<<2));
	
	SCCB_WriteByte(REG_EXHCH,cal_temp);
	

}



/**
  * @brief  ������ʾλ��
	* @param  sx:x��ʼ��ʾλ��
	* @param  sy:y��ʼ��ʾλ��
	* @param  width:��ʾ���ڿ��,Ҫ���OV7725_Window_Set�����е�widthһ��
	* @param  height:��ʾ���ڸ߶ȣ�Ҫ���OV7725_Window_Set�����е�heightһ��
  * @retval ��
  */
void ImageDisp(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
	uint16_t i,j;
	uint16_t Camera_Data;
	
	ILI9341_OpenWindow(sx,sy,width,height);
	ILI9341_Write_Cmd(CMD_SetPixel);
	
	for(i=0;i<width;i++)
	{
		for(j=0;j<height;j++)
		{
			READ_FIFO_PIXEL(Camera_Data);         /* ��FIFO����һ��rgb565���ص�Camera_Data���� */
			ILI9341_Write_Data(Camera_Data);
		}
	}
}




