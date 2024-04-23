#include "ad7192_test.h"
#include "bsp_key.h"
#include "bsp_ad7192.h"
#include "bsp_usart.h"


/* ���ε�ѹת��ʵ�� */
void single_conversion_voltage(void)
{
	uint32_t ad_data = 0;
	float v = 0.0;
	
	uint32_t mode = 0,cfg = 0;
	
	printf("\r\n AD7192 4·����ת������ѹʵ�� \r\n");
	
	/* �� AD7192 �Ĵ��� */
	ReadFromAD7192ViaSPI(REG_COM_STA,8,AD7192Registers,REG_COM_STA);
	
	for(int i = 0;i<8;i++)
	{
		printf("\r\n AD7192Register[%d] = 0x%06x \r\n",i+REG_COM_STA,AD7192Registers[i+REG_COM_STA]);
	}
	
	     /* ����ת��|����״̬����|�ⲿʱ��|sinc4�˲���|������żУ��|ʱ�Ӳ���Ƶ|���õ�����ת��|����60Hz�ݲ�|128 */
  mode = MODE_SING|DAT_STA_DIS|EXT_XTAL|SINC_4|ENPAR_DIS|CLK_DIV_DIS|SINGLECYCLE_DIS|REJ60_DIS|1023;
  cfg  = CHOP_DIS|REF_IN1|AIN1_AIN2|BURN_DIS|REFDET_DIS|BUF_DIS|UB_BI|GAIN_1;
       /*����ն��|�ⲿ��׼��ѹ1|12���ͨ��(��)|���ü�������|���û�׼��ѹ���|����ģ�����뻺��|˫����ģʽ|����Ϊ128 */
  
	AD7192_ModeCfgReg(mode,cfg);
	
	/* У׼ */
	printf("\r\n �ڲ�У׼�� \r\n");
	AD7192InternalZeroScaleCalibration();
	AD7192InternalFullScaleCalibration();
	printf("\r\n �ڲ�У׼��� \r\n");
	
	while(1)
	{
		/* ��ͨ��1ת������ */
		AD7192StartSingleConvertion(AIN1_COM);
		ad_data = AD7192ReadConvertingData();
		v= (ad_data / 8388608.0)*3.3;
		printf("AIN1_COM = %fV\n",v);
		
		/* ��ͨ��2ת������ */
		AD7192StartSingleConvertion(AIN2_COM);
		ad_data = AD7192ReadConvertingData();
		v= (ad_data / 8388608.0)*3.3;
		printf("AIN2_COM = %fV\n",v);
		
		/* ��ͨ��3ת������ */
		AD7192StartSingleConvertion(AIN3_COM);
		ad_data = AD7192ReadConvertingData();
		v= (ad_data / 8388608.0)*3.3;
		printf("AIN1_COM = %fV\n",v);
		
		/* ��ͨ��4ת������ */
		AD7192StartSingleConvertion(AIN4_COM);
		ad_data = AD7192ReadConvertingData();
		v= (ad_data / 8388608.0)*3.3;
		printf("AIN4_COM = %fV\n",v);
	}
}

