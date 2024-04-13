#include "bsp_rs485.h"
#include <stdarg.h>


static void Delay(__IO uint32_t nCount);


UART_HandleTypeDef Uart2_Handle;


static void NVIC_Configuration(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
	
	HAL_NVIC_SetPriority(__485_INT_IRQ,1,1);
	HAL_NVIC_EnableIRQ(__485_INT_IRQ);
}


void __485_Config(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	
	__485_USART_RX_GPIO_CLK_ENABLE();
	__485_USART_TX_GPIO_CLK_ENABLE();
	__485_RE_GPIO_CLK_ENABLE();
	
	__485_USART_CLK_ENABLE();
	
	
	GPIO_InitStructure.Pin = __485_USART_TX_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(__485_USART_TX_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = __485_USART_RX_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_INPUT;
	HAL_GPIO_Init(__485_USART_RX_GPIO_PORT,&GPIO_InitStructure);
	
	
	GPIO_InitStructure.Pin = __485_RE_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(__485_RE_GPIO_PORT,&GPIO_InitStructure);
	
	
	
	Uart2_Handle.Instance = __485_USART;
	Uart2_Handle.Init.BaudRate = __485_USART_BAUDRATE;
	Uart2_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	Uart2_Handle.Init.Mode = UART_MODE_TX_RX;
	Uart2_Handle.Init.OverSampling = UART_OVERSAMPLING_16;
	Uart2_Handle.Init.Parity = UART_PARITY_NONE;
	Uart2_Handle.Init.StopBits = UART_STOPBITS_1;
	Uart2_Handle.Init.WordLength = UART_WORDLENGTH_8B;
	HAL_UART_Init(&Uart2_Handle);
	
	/* 串口1中断初始化 */
	NVIC_Configuration();
	
	/* 配置串口接收中断 */
	__HAL_UART_ENABLE_IT(&Uart2_Handle,UART_IT_RXNE);
	
	//默认进入接收模式
	HAL_GPIO_WritePin(__485_RE_GPIO_PORT,__485_RE_GPIO_PIN,GPIO_PIN_SET);
}


/* 发送一个字节 */
void __485_SendByte(uint8_t ch)
{
	HAL_UART_Transmit(&Uart2_Handle,(uint8_t*)&ch,1,0xFFFF);
}


/* 发送指定长度字节 */
void __485_SendStr_length(uint8_t *str,uint32_t strlen)
{
	unsigned int k = 0;
	
	__485_TX_EN();
	do
	{
		__485_SendByte(*(str+k));
		k++;
	}while(k<strlen);
	
	Delay(0xFFF);

	__485_RX_EN();
}


/* 发送字符串 */
void __485_SendString(uint8_t *str)
{
	unsigned int k = 0;
	
	__485_TX_EN();
	do
	{
		__485_SendByte(*(str+k));
		k++;
	}while(*(str+k) != '\0');

	Delay(0xFFF);
	__485_RX_EN();
}


//中断缓存串口数据
#define UART_BUFF_SIZE              1024
volatile uint16_t uart_p = 1;
uint8_t uart_buff[UART_BUFF_SIZE];


/* 清空缓冲区 */
void clean_rebuff(void)
{
	uint16_t i = UART_BUFF_SIZE + 1;
	uart_p = 0;
	
	while(i)
		uart_buff[--i] = 0;
}

void bsp_485_IRQHandler(void)
{
	if(uart_p < UART_BUFF_SIZE)
	{
		if(__HAL_UART_GET_FLAG(&Uart2_Handle,UART_FLAG_RXNE)!=RESET)
		{
			HAL_UART_Receive(&Uart2_Handle,(uint8_t*)(&uart_buff[uart_p]),1,1000);
			uart_p++;
		}
	}
	else
	{
		clean_rebuff();
	}
	HAL_UART_IRQHandler(&Uart2_Handle);
}


/* 获取接收到的数据和长度 */
char *get_rebuff(uint16_t *len)
{
	*len = uart_p;
	return (char*)&uart_buff;
}



static void Delay(__IO uint32_t nCount)
{
	for(;nCount!= 0;nCount--);
}

