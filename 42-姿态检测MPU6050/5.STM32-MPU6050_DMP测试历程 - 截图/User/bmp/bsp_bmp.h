#ifndef __BSP_BMP_H__
#define __BSP_BMP_H__

#include "stm32f1xx.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//�ĸ��ֽڶ��� ��1�ƴ���
#define WIDTHBYTES(bits)    (((bits) + 31)/ 32 *4)    //����24λ���ɫ ÿһ�е����ؿ�ȱ�����4�ı��� ����0����

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;


//λͼ�ļ�ͷ��Ϣ�ṹ����
//���в������ļ�������Ϣ�����ڽṹ����ڴ�ṹ������Ҫ�Ǽ��˵Ļ���������ȷ��ȡ�ļ���Ϣ��
typedef struct tagBITMAPFILEHEADER
{
	DWORD bfSize;                          //�ļ���С
	WORD bfReserved1;                      //�����֣�������
	WORD bfReserved2;                      //�����֣�ͬ��
	DWORD bfOffBits;                       //ʵ��λͼ���ݵ�ƫ���ֽ�������ǰ�������ֳ���֮��
}BITMAPFILEHEADER,tagBITMAPFILEHEADER;


typedef struct tagBITMAPINFOHEADER
{
	DWORD biSize;                           //ָ���˽ṹ��ĳ��ȣ�Ϊ40
	LONG biWidth;                           //λͼ��
	LONG biHeight;                          //λͼ��
	WORD biPlanes;                          //ƽ������Ϊ1
	WORD biBitCount;                        //������ɫλ����������1,2,4,6,16,24�µĿ�����32
	DWORD biCompression;                    //ѹ����ʽ��������0,1,2������0��ʾ��ѹ��
	DWORD biSizeImage;                      //ʵ��λͼ����ռ�õ��ֽ���
	LONG biXPelsPerMeter;                   //X����ķֱ���
	LONG biYPelsPerMeter;                   //Y����ķֱ���
	DWORD biClrUsed;                        //ʹ�õ���ɫ�������Ϊ0�����ʾĬ��ֵ(2^��ɫλ��)
	DWORD biClrImportant;                   //��Ҫ��ɫ�������Ϊ0�����ʾ������ɫ������Ҫ��
}BITMAPINFOHEADER,tagBITMAPINFOHEADER;


typedef  struct tagRGBQUAD
{
	BYTE rgbBlue;             //����ɫ����ɫ����
	BYTE rgbGrees;            //����ɫ����ɫ����
	BYTE rgbRed;              //����ɫ�ĺ�ɫ����
	BYTE rgbReserves;         //����ֵ
}RGBQUAD,tagRGBQUAD;


typedef struct RGB_PIXEL
{
	//���ص���������
	unsigned char rgbBlue;                 
	unsigned char rgbGreen;                
	unsigned char rgbRed;                    
}RGB_PIXEL;


#define GETR_FROM_RGB16(RGB565)   ((unsigned char)((((unsigned short int)RGB565)>>11)<<3))             //����8λ R
#define GETG_FROM_RGB16(RGB565)   ((unsigned char)((((unsigned short int)(RGB565&0x7FF))>>5)<<2))      //����8λ G
#define GETB_FROM_RGB16(RGB565)   ((unsigned char)((((unsigned short int)(RGB565&0x1F))<<3)))      //����8λ R
#pragma diag_suppress 870    //ʹ������֧�ֶ��ֽ��ַ����������invalid multibyte character sequence����




void Lcd_show_bmp(uint16_t x,uint16_t y,uint8_t *pic_name);
int Screen_shot(uint16_t x,uint16_t y,uint16_t Width,uint16_t Height,uint8_t* filename);



#endif /* __BSP_BMP_H__ */


