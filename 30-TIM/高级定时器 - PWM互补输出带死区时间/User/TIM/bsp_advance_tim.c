#include "bsp_advance_tim.h"


TIM_HandleTypeDef     TIM_TimeBaseStructure;
TIM_OC_InitTypeDef    TIM_OCInitStructure;

__IO uint16_t ChannelPulse = 500;


static void ADVANCE_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	
	ADVANCE_TIM_CH1_GPIO_CLK();
	ADVANCE_TIM_CH1N_GPIO_CLK();
	
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = ADVANCE_TIM_CH1_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ADVANCE_TIM_CH1_PORT,&GPIO_InitStructure);
	
	ADVANCE_TIM_CH1N_GPIO_CLK();
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = ADVANCE_TIM_CH1N_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ADVANCE_TIM_CH1N_PORT,&GPIO_InitStructure);
	
}




static void ADVANCE_TIM_Mode_Config(void)
{
	TIM_BreakDeadTimeConfigTypeDef    TIM_BDTRInitStructure;
	
	ADVANCE_TIM_RCC_CLK_ENABLE();
	
	TIM_TimeBaseStructure.Instance = ADVANCE_TIMx;
	
	//时钟分频因子 ，基本定时器没有
	TIM_TimeBaseStructure.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	
	//计数器计数模式，基本定时器只能向上计数，没有计数模式的设置
	TIM_TimeBaseStructure.Init.CounterMode = TIM_COUNTERMODE_UP;
	
	//自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.Init.Period = 1000-1;
	
	//驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.Init.Prescaler = 72-1;
	
	//重复计数器的值，基本定时器没有
	TIM_TimeBaseStructure.Init.RepetitionCounter = 0;
	
	// 初始化定时器
	HAL_TIM_PWM_Init(&TIM_TimeBaseStructure);
	
	/* ------------------ 输出结构体初始化 --------------- */
	TIM_OC_InitTypeDef        TIM_OCInitStructure;
	
	//时钟分频因子 ，基本定时器没有
	TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_RESET;
	
	//配置为PWM模式1
	TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
	
	//互补输出通道空闲电平极性配置
	TIM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_SET;
	
	TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_SET;
	
	//互补输出通道电平极性配置
	TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_HIGH;;
	
	TIM_OCInitStructure.Pulse = ChannelPulse;
	
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	//初始化通道1输出PWM 
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure,&TIM_OCInitStructure,TIM_CHANNEL_1);
	
	/* 自动输出使能，断路，死区时间和锁定配置 */
	TIM_BDTRInitStructure.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;
	TIM_BDTRInitStructure.BreakPolarity = TIM_BREAKPOLARITY_LOW;
	TIM_BDTRInitStructure.BreakState = TIM_BREAK_ENABLE;
	TIM_BDTRInitStructure.DeadTime = 11;
	TIM_BDTRInitStructure.LockLevel = TIM_LOCKLEVEL_1;
	TIM_BDTRInitStructure.OffStateIDLEMode = TIM_OSSI_ENABLE;
	TIM_BDTRInitStructure.OffStateRunMode = TIM_OSSR_ENABLE;
	
	HAL_TIMEx_ConfigBreakDeadTime(&TIM_TimeBaseStructure,&TIM_BDTRInitStructure);
	
	/* 定时器通道1输出PWM */
	HAL_TIM_PWM_Start(&TIM_TimeBaseStructure,TIM_CHANNEL_1);
	/* 定时器通道1互补输出PWM */
	HAL_TIMEx_PWMN_Start(&TIM_TimeBaseStructure,TIM_CHANNEL_1);
	
}




void TIM__Config(void)
{
	ADVANCE_TIM_GPIO_Config();
	
	ADVANCE_TIM_Mode_Config();
	
	
}





