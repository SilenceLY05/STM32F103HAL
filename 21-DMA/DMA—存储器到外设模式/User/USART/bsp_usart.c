#include "bsp_usart.h"

UART_HandleTypeDef UartHandle;
uint8_t SendBuff[SENDBUFF_SIZE];
//extern uint8_t ucTemp;  

/**
  * @brief  USART GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */
void Debug_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
      
  DEBUG_USART_RX_GPIO_CLK_ENABLE();
  DEBUG_USART_TX_GPIO_CLK_ENABLE();
  /* ʹ�� UART ʱ�� */
  DEBUG_USART_CLK_ENABLE();
  
   /* ����Tx����Ϊ���ù���  */
  GPIO_InitStruct.Pin = DEBUG_USART_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStruct.Pin = DEBUG_USART_RX_PIN;
    GPIO_InitStruct.Mode=GPIO_MODE_AF_INPUT;	//ģʽҪ����Ϊ��������ģʽ��	
  HAL_GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct);
  
  
  UartHandle.Instance          = DEBUG_USART;
  UartHandle.Init.BaudRate     = DEBUG_USART_BAUDRATE;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  
  HAL_UART_Init(&UartHandle); 
}

///�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ�����DEBUG_USART */
	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}

///�ض���c�⺯��scanf������DEBUG_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		
	int ch;
	HAL_UART_Receive(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	return (ch);
}

/* USER CODE END 1 */

