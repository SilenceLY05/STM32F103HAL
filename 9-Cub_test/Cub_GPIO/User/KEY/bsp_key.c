#include "bsp_key.h"


void KEY_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;



    /*��������GPIO�ڵ�ʱ��*/

    KEY1_GPIO_CLK_ENABLE();

    KEY2_GPIO_CLK_ENABLE();

    /*ѡ�񰴼�������*/	

    GPIO_InitStructure.Pin = KEY1_PIN; 



    /*��������Ϊ����ģʽ*/

    GPIO_InitStructure.Mode = GPIO_MODE_INPUT; 


    /*�������Ų�����Ҳ������*/

    GPIO_InitStructure.Pull = GPIO_NOPULL;

    /*ʹ������Ľṹ���ʼ������*/

    HAL_GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);

    /*ѡ�񰴼�������*/

    GPIO_InitStructure.Pin = KEY2_PIN; 


    /*ʹ������Ľṹ���ʼ������*/

    HAL_GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);

}


uint8_t KEY_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_PIN)
{
	/* �жϰ����Ƿ��� */
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_PIN) == GPIO_PIN_SET)
	{
		/* �������� */
		while(HAL_GPIO_ReadPin(GPIOx,GPIO_PIN) == GPIO_PIN_SET);
		return KEY_ON;
	}
	else
	{
		/* ����δ������ */
		return KEY_OFF;
	}
}

