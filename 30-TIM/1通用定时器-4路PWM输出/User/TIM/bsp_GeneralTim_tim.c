#include "bsp_GeneralTim_tim.h"


TIM_HandleTypeDef     TIM_TimeBase;


static void GENERAL_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef          GPIO_InitStructure;
	
	
	GENERAL_TIM_CH1_CLK_ENABLE();
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = GENERAL_TIM_CH1_GPIO_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GENERAL_TIM_CH1_GPIO,&GPIO_InitStructure);
	
	
	GENERAL_TIM_CH2_CLK_ENABLE();
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = GENERAL_TIM_CH2_GPIO_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GENERAL_TIM_CH2_GPIO,&GPIO_InitStructure);
	
	
	GENERAL_TIM_CH3_CLK_ENABLE();
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = GENERAL_TIM_CH3_GPIO_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GENERAL_TIM_CH3_GPIO,&GPIO_InitStructure);
	
	
	GENERAL_TIM_CH4_CLK_ENABLE();
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = GENERAL_TIM_CH4_GPIO_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GENERAL_TIM_CH4_GPIO,&GPIO_InitStructure);
}



///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */
void GENERAL_TIM_Mode_Config(void)
{
	GENERAL_TIM_RCC_CLK_ENABLE();
	
	TIM_TimeBase.Instance = GENERAL_TIM;
	// 时钟预分频数
	TIM_TimeBase.Init.Prescaler = GENERAL_TIM_PRESCALER;
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBase.Init.Period = GENERAL_TIM_PERIOD;
	// 计数器计数模式，设置为向上计数
	TIM_TimeBase.Init.CounterMode = TIM_COUNTERMODE_UP;
	// 时钟分频因子 ，没用到不用管
	TIM_TimeBase.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM_TimeBase.Init.RepetitionCounter = 0;
	// 初始化定时器
	HAL_TIM_Base_Init(&TIM_TimeBase);
	
	
	/*--------------------输出比较结构体初始化-------------------*/	
	// 占空比配置
	uint16_t CCR1_Val = 5;
	uint16_t CCR2_Val = 4;
	uint16_t CCR3_Val = 3;
	uint16_t CCR4_Val = 2;
	
	
	TIM_OC_InitTypeDef   TIM_OCInitStructure;
	
	//时钟分频因子 ，基本定时器没有
	TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_RESET;
	
	//配置为PWM模式1
	TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
	
	//互补输出通道空闲电平极性配置
	TIM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_SET;
	
	//互补输出通道电平极性配置
	TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	
	//输出比较通道1
	TIM_OCInitStructure.Pulse = CCR1_Val;
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBase,&TIM_OCInitStructure,TIM_CHANNEL_1);	
	HAL_TIM_PWM_Start(&TIM_TimeBase,TIM_CHANNEL_1);
	
	//输出比较通道2
	TIM_OCInitStructure.Pulse = CCR2_Val;
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBase,&TIM_OCInitStructure,TIM_CHANNEL_2);	
	HAL_TIM_PWM_Start(&TIM_TimeBase,TIM_CHANNEL_2);
	
	//输出比较通道3
	TIM_OCInitStructure.Pulse = CCR3_Val;
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBase,&TIM_OCInitStructure,TIM_CHANNEL_3);	
	HAL_TIM_PWM_Start(&TIM_TimeBase,TIM_CHANNEL_3);
	
	//输出比较通道4
	TIM_OCInitStructure.Pulse = CCR4_Val;
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBase,&TIM_OCInitStructure,TIM_CHANNEL_4);	
	HAL_TIM_PWM_Start(&TIM_TimeBase,TIM_CHANNEL_4);
	
	
}


void GENERAL_TIM_Init(void)
{
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_Mode_Config();
}


