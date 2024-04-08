#include "bsp_basci_tim.h"


extern volatile uint32_t time;    //ms计时变量
TIM_HandleTypeDef     htimx;


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	/* 基本定时器外设时钟使能 */
	BASIC_TIM_RCC_CLK_ENABLE();
	
	/* 外设中断设置 */
	HAL_NVIC_SetPriority(BASIC_TIM_IRQ,1,0);
	HAL_NVIC_EnableIRQ(BASIC_TIM_IRQ);
}



/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */
void BASIC_TIMx_Init(void)
{
	TIM_MasterConfigTypeDef sMasterConfig;
	
	htimx.Instance = BASIC_TIMx;
	htimx.Init.Prescaler = BASIC_TIMx_PRESCALER;
	htimx.Init.Period = BASIC_TIMx_PERIOD;
	htimx.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&htimx);
	
	
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	
	HAL_TIMEx_MasterConfigSynchronization(&htimx,&sMasterConfig);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	time++;
}

