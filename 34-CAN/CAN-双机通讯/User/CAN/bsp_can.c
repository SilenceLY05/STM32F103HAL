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
	/* ������ռ���ȼ��ķ��� */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
	
	/*�ж����ã���ռ���ȼ�0�������ȼ�Ϊ0*/
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
	CAN_FilterInitStructure.FilterIdHigh = ((((uint32_t)0x1314<<3) | CAN_ID_EXT | CAN_RTR_DATA) & 0xFFFF0000)>>16;   //Ҫɸѡ��ID�ĸ�16λ
	CAN_FilterInitStructure.FilterIdLow = (((uint32_t)0x1314<<3) | CAN_ID_EXT | CAN_RTR_DATA) & 0xFFFF;   //Ҫɸѡ��ID�ĸ�16λ
	
	CAN_FilterInitStructure.FilterMaskIdHigh = 0xFFFF;
	CAN_FilterInitStructure.FilterMaskIdLow = 0xFFFF;   //����ɸѡ����ÿһλ������ƥ��
	
	CAN_FilterInitStructure.FilterFIFOAssignment = CAN_FilterFIFO0;   //������FIFO0
	CAN_FilterInitStructure.FilterActivation = ENABLE;   //ʹ��
	
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
	CAN_HandleTxMsg.ExtId = 0x1314;         //ʹ����չID
	CAN_HandleTxMsg.IDE = CAN_ID_EXT;       //ʹ����չģʽ
	CAN_HandleTxMsg.RTR = CAN_RTR_DATA;     //���͵�������֡
	CAN_HandleTxMsg.DLC = 8;                //���ݳ��ȣ���λ�ֽ�
	
	HAL_CAN_AddTxMessage(&CAN_Handle,&CAN_HandleTxMsg,temp,pTxmailbox);
	
}
