#include "ad7192_test.h"
#include "bsp_key.h"
#include "bsp_ad7192.h"
#include "bsp_usart.h"


/* ������ѹת��ʵ�� */
void electronic_scale_test(void)
{
	static float coefficient = 173.6f;        //ת��ϵ��
	int32_t ad_data_init = 0;
	int32_t ad_data = 0;
	
	uint32_t mode = 0,cfg = 0;
	
	printf("\r\n AD7192 ���ӳ�ʵ�� \r\n");
	
	/* �� AD7192 �Ĵ��� */
	ReadFromAD7192ViaSPI(REG_COM_STA,8,AD7192Registers,REG_COM_STA);
	
	for(int i = 0;i<8;i++)
	{
		printf("\r\n AD7192Register[%d] = 0x%06x \r\n",i+REG_COM_STA,AD7192Registers[i+REG_COM_STA]);
	}
	
	     /* ����ת��|ʹ��״̬����|�ⲿʱ��|sinc4�˲���|������żУ��|ʱ�Ӳ���Ƶ|���õ�����ת��|����60Hz�ݲ�|128 */
  mode = MODE_SING|DAT_STA_EN|EXT_XTAL|SINC_4|ENPAR_DIS|CLK_DIV_DIS|SINGLECYCLE_DIS|REJ60_EN|0x3FF;
  cfg  = CHOP_EN|REF_IN1|AIN1_AIN2|BURN_DIS|REFDET_DIS|BUF_DIS|UB_BI|GAIN_128;
       /*����ն��|�ⲿ��׼��ѹ1|12���ͨ��(��)|���ü�������|���û�׼��ѹ���|����ģ�����뻺��|˫����ģʽ|����Ϊ128 */
  
	AD7192_ModeCfgReg(mode,cfg);
	
	/* У׼ */
	printf("\r\n �ڲ�У׼�� \r\n");
	AD7192InternalZeroScaleCalibration();
	AD7192InternalFullScaleCalibration();
	printf("\r\n �ڲ�У׼��� \r\n");
	
	AD7192StartContinuousConvertion(AIN1_AIN2);   //��������ת��
	//��¼��ʼֵ
	ad_data_init = AD7192ReadConvertingData();
	
	printf("\r\n ad_data - ad_data_init = %d \r\n",ad_data - ad_data_init);
	printf("\r\n ����Ϊ��%.2fg \r\n",(ad_data - ad_data_init) / coefficient);
	
	if(Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON)
	{
		/* ȥƤ ���� */
		ad_data_init = AD7192ReadConvertingData();
	}
	if(Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON)
	{
		/* У׼�����ڲ�ͬ�Ĵ������ı仯�ʲ�һ��������ʱ������Ҫ����У׼������50g�������Key2 */
		ad_data = AD7192ReadConvertingData();
		coefficient = (ad_data - ad_data_init) / 50.0;
		printf("\r\n coefficient = %f \r\n",coefficient);
	}
		
	HAL_Delay(100);
}

