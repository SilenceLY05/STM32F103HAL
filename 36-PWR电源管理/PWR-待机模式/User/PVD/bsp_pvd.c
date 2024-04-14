#include "bsp_pvd.h"

void PVD_Config(void)
{
	PWR_PVDTypeDef    sConfigPVD;
	
	__HAL_RCC_PWR_CLK_ENABLE();
	
	HAL_NVIC_SetPriority(PVD_IRQn,0,0);
	HAL_NVIC_EnableIRQ(PVD_IRQn);
	
	
	/* ����PVD����5 (PVD����ѹ����ֵΪ2.8V��
	   VDD��ѹ����2.8Vʱ����PVD�жϣ���������
	   �ɲ�ѯ�����ֲ��֪) ���弶������Լ���
	   ʵ��Ӧ��Ҫ������*/
	sConfigPVD.PVDLevel = PWR_PVDLEVEL_5;
	sConfigPVD.Mode = PWR_PVD_MODE_IT_RISING_FALLING;
	HAL_PWR_ConfigPVD(&sConfigPVD);
	
	/* ʹ����� */
	HAL_PWR_EnablePVD();
}

