#include "bsp_TouchPad.h"
#include "bsp_usart.h"


//��ʱ��������ֵ
#define TPAD_ADDR_MAX_VAL             0xFFFF

//����û����ʱ��ʱ������ֵ
__IO uint16_t tpad_default_val = 0;


TIM_HandleTypeDef TIM_Handle;


//�������ж����ȼ�����
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	/* ������ʱ������ʱ��ʹ�� */
	TOUCHPAD_TIM_RCC_CLK_ENABLE();
	
	TOUCHPAD_GPIO_RCC_CLK_ENABLE();
	
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pin = TOUCHPAD_GPIO_PIN;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(TOUCHPAD_GPIO,&GPIO_InitStructure);
	
}



/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
void TPAD_TIM_Mode_Config(void)
{
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_IC_InitTypeDef sConfigIC;
	
	TIM_Handle.Instance = TOUCHPAD_TIMx;
	// ʱ��Ԥ��Ƶ��
	TIM_Handle.Init.Prescaler = TOUCHPAD_TIMx_PRESCALER;
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_Handle.Init.Period = TOUCHPAD_TIMx_PERIOD;
	// ����������ģʽ������Ϊ���ϼ���
	TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	// ʱ�ӷ�Ƶ���� ��û�õ����ù�
	TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	// ��ʼ����ʱ��
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



/* Ϊ���ݰ����ŵ� �����ʱ����־������ */
static void TPAD_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	TOUCHPAD_GPIO_RCC_CLK_ENABLE();
	
	/* �����������Ϊ�͵�ƽ */
	HAL_GPIO_WritePin(TOUCHPAD_GPIO,TOUCHPAD_GPIO_PIN,GPIO_PIN_RESET);
	
	/* �趨���ݰ�����Ӧ����IO��� */
	
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pin = TOUCHPAD_GPIO_PIN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(TOUCHPAD_GPIO,&GPIO_InitStructure);
	
	HAL_Delay(5);
	
	__HAL_TIM_SET_COUNTER(&TIM_Handle,0);   //���㶨ʱ������
	__HAL_TIM_CLEAR_FLAG(&TIM_Handle,TIM_FLAG_UPDATE | TIM_FLAG_CC2);   //����жϱ�־λ
	
	
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(TOUCHPAD_GPIO,&GPIO_InitStructure);
	
	
	HAL_TIM_IC_Start(&TIM_Handle,TOUCHPAD_TIM_CHANNEL);
	
}


/* �õ���ʱ������ֵ �����ʱ����ֱ�ӷ��ض�ʱ���ļ���ֵ */
static uint16_t TPAD_Get_VAL(void)
{
	TPAD_Reset();
	while(__HAL_TIM_GET_FLAG(&TIM_Handle,TOUCHPAD_TIM_FLAG_CCR) == RESET)
	{
		uint16_t count;
		count = __HAL_TIM_GET_COUNTER(&TIM_Handle);
		if(count > (TOUCHPAD_TIMx_PERIOD - 500))
			return count;         //��ʱ ֱ�ӷ���CNTֵ
	}
	
	return HAL_TIM_ReadCapturedValue(&TIM_Handle,TOUCHPAD_TIM_CHANNEL);
}


/* ��ϵN�Σ�ȡ���ֵ  */
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



/* ��ʼ�������� */
uint8_t TPAD_Init(void)
{
	uint16_t buff[10];
	uint32_t temp = 0;
	uint8_t i,j;
	
	/* ��1.5MHz��Ƶ�ʼ��� */
	TPAD_TIM_Mode_Config();
	HAL_TIM_IC_Start(&TIM_Handle,TOUCHPAD_TIM_CHANNEL);
	
	/* ����ȡֵ10�� */
	for(i = 0 ;i<10;i++)
	{
		buff[i] = TPAD_Get_VAL();
		HAL_Delay(10);
	}
	
	/* ���� */
	for(i = 0;i<9;i++)
	{
		for(j = i+1;j<10;j++)
		{
			/* �������� */
			if(buff[i] > buff[j])
			{
				temp = buff[i];
				buff[i] = buff[j];
				buff[j] = temp;
			}
		}
	}
	temp = 0;
	/* ȡ�м��6�����ݽ���ƽ��ֵ */
	for(i = 2; i<8;i++)
	{
		temp += buff[i];
	}
	tpad_default_val = temp /6;
	printf("\r\n tpad_default_val:%d \r\n",tpad_default_val);
	
	/* ��ʼ����������TPAD_ADDR_MAX_VAL/2��ֵ ������ */
	if(tpad_default_val > TOUCHPAD_TIMx_PERIOD /2)
		return 1;
	
	return 0;
	
}


/* ɨ�败������ mode:0��֧������������1֧���������� */
//��ֵ������ʱ�������ڣ�tpad_default_val + TPAD_GATE_CAL��������Ϊ����Ч����

#define TPAD_GATE_VAL    100

uint8_t TPAD_Scan(uint8_t mode)
{
	//0,���Կ�ʼ���;>0,�����ܿ�ʼ���
	static uint8_t keyen = 0;
	
	//ɨ����
	uint8_t res = 0;
	
	//Ĭ�ϲ�������Ϊ3��
	uint8_t sample = 3;
	
	//����ֵ
	uint16_t rval;
	
	if(mode)
	{
		/* ֧��������ʱ�����ò�������Ϊ6�� */
		sample = 6;
		//֧������	
		keyen = 0;
	}
	
	/* ��ȡ��ǰ����ֵ(����sample��ɨ������ֵ) */
	rval = TPAD_Get_MaxVal(sample);
	
	/* ��ӡ��������ʹ�ã�����ȷ��TPAD_GATE_VAL */
	printf("\r\n scan_rval = %d \r\n",rval);
	
	/* ����tpad_default_val + TPAD_GATE_VAL��С��10��tpad_default_val����Ч */
	if(rval > (tpad_default_val + TPAD_GATE_VAL) && rval <(10*tpad_default_val))
	{
		if(keyen == 0)
		{
			res = 1;
		}
		keyen = 3;   //����Ҫ�ٹ�3��֮����ܰ�����Ч   
	}
	
	
	if(keyen)
	{
		keyen--;		
	}		
	return res;
	
	
}



