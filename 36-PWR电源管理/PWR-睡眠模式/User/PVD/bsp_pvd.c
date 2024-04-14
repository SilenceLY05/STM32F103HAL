#include "bsp_pvd.h"

void PVD_Config(void)
{
	PWR_PVDTypeDef    sConfigPVD;
	
	__HAL_RCC_PWR_CLK_ENABLE();
	
	HAL_NVIC_SetPriority(PVD_IRQn,0,0);
	HAL_NVIC_EnableIRQ(PVD_IRQn);
	
	
	/* 配置PVD级别5 (PVD检测电压的阈值为2.8V，
	   VDD电压低于2.8V时产生PVD中断，具体数据
	   可查询数据手册获知) 具体级别根据自己的
	   实际应用要求配置*/
	sConfigPVD.PVDLevel = PWR_PVDLEVEL_5;
	sConfigPVD.Mode = PWR_PVD_MODE_IT_RISING_FALLING;
	HAL_PWR_ConfigPVD(&sConfigPVD);
	
	/* 使能输出 */
	HAL_PWR_EnablePVD();
}

