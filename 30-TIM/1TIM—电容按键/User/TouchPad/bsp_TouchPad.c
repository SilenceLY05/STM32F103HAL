#include "bsp_TouchPad.h"
#include "bsp_usart.h"


//定时器最大计数值
#define TPAD_ADDR_MAX_VAL             0xFFFF

//保存没按下时定时器计数值
__IO uint16_t tpad_default_val = 0;


TIM_HandleTypeDef TIM_Handle;


//引脚与中断优先级配置
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	/* 基本定时器外设时钟使能 */
	TOUCHPAD_TIM_RCC_CLK_ENABLE();
	
	TOUCHPAD_GPIO_RCC_CLK_ENABLE();
	
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pin = TOUCHPAD_GPIO_PIN;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(TOUCHPAD_GPIO,&GPIO_InitStructure);
	
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
void TPAD_TIM_Mode_Config(void)
{
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_IC_InitTypeDef sConfigIC;
	
	TIM_Handle.Instance = TOUCHPAD_TIMx;
	// 时钟预分频数
	TIM_Handle.Init.Prescaler = TOUCHPAD_TIMx_PRESCALER;
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_Handle.Init.Period = TOUCHPAD_TIMx_PERIOD;
	// 计数器计数模式，设置为向上计数
	TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	// 时钟分频因子 ，没用到不用管
	TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	// 初始化定时器
	HAL_TIM_Base_Init(&TIM_Handle);
	
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&TIM_Handle,&sClockSourceConfig);
	
	
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	
	HAL_TIMEx_MasterConfigSynchronization(&TIM_Handle,&sMasterConfig);
	
	sConfigIC.ICFilter = 3;
	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	
	HAL_TIM_IC_ConfigChannel(& TIM_Handle, &sConfigIC, TOUCHPAD_TIM_CHANNEL);
}



/* 为电容按键放电 清楚定时器标志及计数 */
static void TPAD_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	TOUCHPAD_GPIO_RCC_CLK_ENABLE();
	
	/* 设置引脚输出为低电平 */
	HAL_GPIO_WritePin(TOUCHPAD_GPIO,TOUCHPAD_GPIO_PIN,GPIO_PIN_RESET);
	
	/* 设定电容按键对应引脚IO编号 */
	
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pin = TOUCHPAD_GPIO_PIN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(TOUCHPAD_GPIO,&GPIO_InitStructure);
	
	HAL_Delay(5);
	
	__HAL_TIM_SET_COUNTER(&TIM_Handle,0);   //清零定时器计数
	__HAL_TIM_CLEAR_FLAG(&TIM_Handle,TIM_FLAG_UPDATE | TIM_FLAG_CC2);   //清除中断标志位
	
	
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(TOUCHPAD_GPIO,&GPIO_InitStructure);
	
	
	HAL_TIM_IC_Start(&TIM_Handle,TOUCHPAD_TIM_CHANNEL);
	
}


/* 得到定时器捕获值 如果超时，则直接返回定时器的计数值 */
static uint16_t TPAD_Get_VAL(void)
{
	TPAD_Reset();
	while(__HAL_TIM_GET_FLAG(&TIM_Handle,TOUCHPAD_TIM_FLAG_CCR) == RESET)
	{
		uint16_t count;
		count = __HAL_TIM_GET_COUNTER(&TIM_Handle);
		if(count > (TOUCHPAD_TIMx_PERIOD - 500))
			return count;         //超时 直接返回CNT值
	}
	
	return HAL_TIM_ReadCapturedValue(&TIM_Handle,TOUCHPAD_TIM_CHANNEL);
}


/* 联系N次，取最大值  */
static uint16_t TPAD_Get_MaxVal(uint8_t n)
{
	uint16_t temp =0;
	uint16_t res = 0;
	
	while(n--)
	{
		temp = TPAD_Get_VAL();
		if(temp>res) res = temp;
	}
	return res;
}



/* 初始化触摸板 */
uint8_t TPAD_Init(void)
{
	uint16_t buff[10];
	uint32_t temp = 0;
	uint8_t i,j;
	
	/* 以1.5MHz的频率计数 */
	TPAD_TIM_Mode_Config();
	HAL_TIM_IC_Start(&TIM_Handle,TOUCHPAD_TIM_CHANNEL);
	
	/* 连续取值10次 */
	for(i = 0 ;i<10;i++)
	{
		buff[i] = TPAD_Get_VAL();
		HAL_Delay(10);
	}
	
	/* 排序 */
	for(i = 0;i<9;i++)
	{
		for(j = i+1;j<10;j++)
		{
			/* 升序排列 */
			if(buff[i] > buff[j])
			{
				temp = buff[i];
				buff[i] = buff[j];
				buff[j] = temp;
			}
		}
	}
	temp = 0;
	/* 取中间的6个数据进行平均值 */
	for(i = 2; i<8;i++)
	{
		temp += buff[i];
	}
	tpad_default_val = temp /6;
	printf("\r\n tpad_default_val:%d \r\n",tpad_default_val);
	
	/* 初始化遇到超过TPAD_ADDR_MAX_VAL/2的值 不正常 */
	if(tpad_default_val > TOUCHPAD_TIMx_PERIOD /2)
		return 1;
	
	return 0;
	
}


/* 扫描触摸按键 mode:0不支持连续出发，1支持连续触发 */
//阈值：不过时间必须大于（tpad_default_val + TPAD_GATE_CAL），才认为是有效触摸

#define TPAD_GATE_VAL    100

uint8_t TPAD_Scan(uint8_t mode)
{
	//0,可以开始检测;>0,还不能开始检测
	static uint8_t keyen = 0;
	
	//扫描结果
	uint8_t res = 0;
	
	//默认采样次数为3次
	uint8_t sample = 3;
	
	//捕获值
	uint16_t rval;
	
	if(mode)
	{
		/* 支持连按的时候，设置采样次数为6次 */
		sample = 6;
		//支持连按	
		keyen = 0;
	}
	
	/* 获取当前捕获值(返回sample次扫描的最大值) */
	rval = TPAD_Get_MaxVal(sample);
	
	/* 打印函数调试使用，用来确定TPAD_GATE_VAL */
	printf("\r\n scan_rval = %d \r\n",rval);
	
	/* 大于tpad_default_val + TPAD_GATE_VAL且小于10倍tpad_default_val则有效 */
	if(rval > (tpad_default_val + TPAD_GATE_VAL) && rval <(10*tpad_default_val))
	{
		if(keyen == 0)
		{
			res = 1;
		}
		keyen = 3;   //至少要再过3次之后才能按键有效   
	}
	
	
	if(keyen)
	{
		keyen--;		
	}		
	return res;
	
	
}



