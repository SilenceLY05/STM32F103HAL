#include "bsp_advance_tim.h"


TIM_HandleTypeDef     TIM_TimeBaseStructure;

__IO uint16_t IC2Value = 0;
__IO uint16_t IC1Value = 0;
__IO float DutyCycle = 0;
__IO float Frequency = 0;


static void ADVANCE_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	
	GENERAL_TIM_RCC_CLK_ENABLE();
	ADVANCE_TIM_RCC_CLK_ENABLE();
	
	GENERAL_TIM_CH1_GPIO_CLK();
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = GENERAL_TIM_CH1_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GENERAL_TIM_CH1_PORT,&GPIO_InitStructure);
	
	ADVANCE_TIM_CH1_GPIO_CLK();
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = ADVANCE_TIM_CH1_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ADVANCE_TIM_CH1_PORT,&GPIO_InitStructure);
	
}

 /**
  * @brief  高级控制定时器 TIMx,x[1,8]中断优先级配置
  * @param  无
  * @retval 无
  */
static void TIMx_NVIC_Configuration(void)
{
	//设置抢占优先级，子优先级
	HAL_NVIC_SetPriority(ADVANCE_TIM_IRQn,0,3);
	// 设置中断来源
	HAL_NVIC_EnableIRQ(ADVANCE_TIM_IRQn);
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

static void ADVANCE_TIM_PWMOUTPUT_Config(void)
{
	ADVANCE_TIM_RCC_CLK_ENABLE();
	
	TIM_TimeBaseStructure.Instance = GENERAL_TIMx;
	
	GENERAL_TIM_RCC_CLK_ENABLE();
	//时钟分频因子 ，基本定时器没有
	TIM_TimeBaseStructure.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	
	//计数器计数模式，基本定时器只能向上计数，没有计数模式的设置
	TIM_TimeBaseStructure.Init.CounterMode = TIM_COUNTERMODE_UP;
	
	//自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.Init.Period = GENERAL_TIMx_PERIOD;
	
	//驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.Init.Prescaler = GENERAL_TIMx_PRESCALER;
	
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
	
	//互补输出通道电平极性配置
	TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_HIGH;;
	
	TIM_OCInitStructure.Pulse = GENERAL_TIMx_PULSE;
	
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure,&TIM_OCInitStructure,TIM_CHANNEL_1);
	
	HAL_TIM_PWM_Start(&TIM_TimeBaseStructure,TIM_CHANNEL_1);
	
}




static void TIM_PWMINPUT_Config(void)
{
	TIM_IC_InitTypeDef    TIM_ICInitStructure;
	
	TIM_SlaveConfigTypeDef   TIM_SlaveConfigStructure;
	// 开启TIMx_CLK,x[1,8] 
	ADVANCE_TIM_RCC_CLK_ENABLE();
	/* 定义定时器的句柄即确定定时器寄存器的基地址*/
	TIM_TimeBaseStructure.Instance = ADVANCE_TIMx;
	TIM_TimeBaseStructure.Init.Period = ADVANCE_TIMx_PERIOD;
	TIM_TimeBaseStructure.Init.Prescaler 	= ADVANCE_TIMx_PRESCALER;
	TIM_TimeBaseStructure.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM_TimeBaseStructure.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_IC_Init(&TIM_TimeBaseStructure);
	
	/* IC1捕获：上升沿触发 TI1FP1 */
	TIM_ICInitStructure.ICPolarity = TIM_ICPOLARITY_RISING;
	TIM_ICInitStructure.ICSelection = TIM_ICSELECTION_DIRECTTI;
	TIM_ICInitStructure.ICFilter = 0x0;
	TIM_ICInitStructure.ICPrescaler = TIM_ICPSC_DIV1;
	HAL_TIM_IC_ConfigChannel(&TIM_TimeBaseStructure,&TIM_ICInitStructure,TIM_CHANNEL_1);
	
	
	/* IC2捕获：下降沿触发 TI1FP2 */
	TIM_ICInitStructure.ICPolarity = TIM_ICPOLARITY_FALLING;
	TIM_ICInitStructure.ICSelection = TIM_ICSELECTION_INDIRECTTI;
	TIM_ICInitStructure.ICFilter = 0x0;
	TIM_ICInitStructure.ICPrescaler = TIM_ICPSC_DIV1;
	HAL_TIM_IC_ConfigChannel(&TIM_TimeBaseStructure,&TIM_ICInitStructure,TIM_CHANNEL_2);
	
	
	TIM_SlaveConfigStructure.SlaveMode = TIM_SLAVEMODE_RESET;
	TIM_SlaveConfigStructure.InputTrigger = TIM_TS_TI1FP1;
	HAL_TIM_SlaveConfigSynchronization(&TIM_TimeBaseStructure,&TIM_SlaveConfigStructure);
	
	
	/* 使能捕获/比较2中断请求 */
	HAL_TIM_IC_Start_IT(&TIM_TimeBaseStructure,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&TIM_TimeBaseStructure,TIM_CHANNEL_2);
	
}


/**
  * @brief  初始化高级控制定时器定时，1ms产生一次中断
  * @param  无
  * @retval 无
  */
void TIMx_Configuration(void)
{
	ADVANCE_TIM_GPIO_Config();
	TIMx_NVIC_Configuration();
	ADVANCE_TIM_PWMOUTPUT_Config();
	TIM_PWMINPUT_Config();
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		/* 获取输入捕获值 */
		IC1Value = HAL_TIM_ReadCapturedValue(&TIM_TimeBaseStructure,TIM_CHANNEL_1);
		IC2Value = HAL_TIM_ReadCapturedValue(&TIM_TimeBaseStructure,TIM_CHANNEL_2);
		if(IC1Value != 0)
		{
			DutyCycle = (float)((IC2Value+1)*100) / (IC1Value+1);
			
			/* 频率计算 */
		Frequency = 7200000/72/(float)(IC1Value+1);
		
	  }
	  else
	  {
		DutyCycle = 0;
		Frequency = 0;
	  }
	}
}






