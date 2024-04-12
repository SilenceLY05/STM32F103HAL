#include "bsp_can.h"


CAN_HandleTypeDef CAN_Handle;
CAN_TxHeaderTypeDef  CAN_HandleTxMsg;
CAN_RxHeaderTypeDef  CAN_HandleRxMsg;


static void CAN_GPIO_Config(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;

	CAN_RX_GPIO_CLK_ENABLE();

	CAN_TX_GPIO_CLK_ENABLE();
	
	
	GPIO_InitStructure.Pin = CAN_RX_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(CAN_RX_GPIO_PORT, &GPIO_InitStructure);


	GPIO_InitStructure.Pin = CAN_TX_PIN;
	HAL_GPIO_Init(CAN_TX_GPIO_PORT, &GPIO_InitStructure);
}

static void CAN_NVIC_Config(void)
{
	/* 配置抢占优先级的分组 */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
	
	/*中断设置，抢占优先级0，子优先级为0*/
	HAL_NVIC_SetPriority(CAN_RX_IRQ,0,0);
	HAL_NVIC_EnableIRQ(CAN_RX_IRQ);
}


static void CAN_Mode_Config(void)
{
	CAN_CLK_ENABLE();

  CAN_Handle.Instance = CANx;
  CAN_Handle.Init.Prescaler = 4;
  CAN_Handle.Init.Mode = CAN_MODE_LOOPBACK;
  CAN_Handle.Init.SyncJumpWidth = CAN_SJW_2TQ;
  CAN_Handle.Init.TimeSeg1 = CAN_BS1_5TQ;
  CAN_Handle.Init.TimeSeg2 = CAN_BS2_3TQ;
  CAN_Handle.Init.TimeTriggeredMode = DISABLE;
  CAN_Handle.Init.AutoBusOff = ENABLE;
  CAN_Handle.Init.AutoWakeUp = ENABLE;
  CAN_Handle.Init.AutoRetransmission = DISABLE;
  CAN_Handle.Init.ReceiveFifoLocked = DISABLE;
  CAN_Handle.Init.TransmitFifoPriority = DISABLE;

	HAL_CAN_Init(&CAN_Handle);
}




static void CAN_Filter_Config(void)
{
	CAN_FilterTypeDef CAN_FilterInitStructure;
	
	CAN_FilterInitStructure.FilterBank = 0;
	CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_FilterInitStructure.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN_FilterInitStructure.FilterIdHigh = ((((uint32_t)0x1314<<3) | CAN_ID_EXT | CAN_RTR_DATA) & 0xFFFF0000)>>16;   //要筛选的ID的高16位
	CAN_FilterInitStructure.FilterIdLow = (((uint32_t)0x1314<<3) | CAN_ID_EXT | CAN_RTR_DATA) & 0xFFFF;   //要筛选的ID的高16位
	
	CAN_FilterInitStructure.FilterMaskIdHigh = 0xFFFF;
	CAN_FilterInitStructure.FilterMaskIdLow = 0xFFFF;   //设置筛选器的每一位都必须匹配
	
	CAN_FilterInitStructure.FilterFIFOAssignment = CAN_FilterFIFO0;   //关联到FIFO0
	CAN_FilterInitStructure.FilterActivation = ENABLE;   //使能
	
	HAL_CAN_ConfigFilter(&CAN_Handle,&CAN_FilterInitStructure);
}

void CAN_Config(void)
{
	CAN_GPIO_Config();
  CAN_NVIC_Config();
  CAN_Mode_Config();
  CAN_Filter_Config();
  Init_RxMes(); 

}

void Init_RxMes(void)
{
	uint8_t temp [8] = {0};
	
	CAN_HandleRxMsg.StdId = 0x00;
	CAN_HandleRxMsg.ExtId = 0x00;
	CAN_HandleRxMsg.IDE = CAN_ID_STD;
	CAN_HandleRxMsg.DLC = 0;
	CAN_HandleRxMsg.FilterMatchIndex = 0;
	HAL_CAN_GetRxMessage(&CAN_Handle,CAN_RX_FIFO0,&CAN_HandleRxMsg,temp);
}


void CAN_SetMsg(void)
{

	
	uint32_t *pTxmailbox;
	uint8_t temp [8] = {1,2,3,4,5,6,7,8};
	
	CAN_HandleTxMsg.StdId = 0x00;
	CAN_HandleTxMsg.ExtId = 0x1314;         //使用扩展ID
	CAN_HandleTxMsg.IDE = CAN_ID_EXT;       //使用扩展模式
	CAN_HandleTxMsg.RTR = CAN_RTR_DATA;     //发送的是数据帧
	CAN_HandleTxMsg.DLC = 8;                //数据长度，单位字节
	
	HAL_CAN_AddTxMessage(&CAN_Handle,&CAN_HandleTxMsg,temp,pTxmailbox);
	
}
