#include "ad7192_test.h"
#include "bsp_key.h"
#include "bsp_ad7192.h"
#include "bsp_usart.h"


/* 连续电压转换实验 */
void continuous_conversion_voltage(void)
{
	uint32_t ad_data = 0;
	float v = 0.0;
	
	uint32_t mode = 0,cfg = 0;
	
	printf("\r\n AD7192 4路连续转换读电压实验 \r\n");
	
	/* 读 AD7192 寄存器 */
	ReadFromAD7192ViaSPI(REG_COM_STA,8,AD7192Registers,REG_COM_STA);
	
	for(int i = 0;i<8;i++)
	{
		printf("\r\n AD7192Register[%d] = 0x%06x \r\n",i+REG_COM_STA,AD7192Registers[i+REG_COM_STA]);
	}
	
	     /* 单次转换|使能状态传输|外部时钟|sinc4滤波器|禁用奇偶校验|时钟不分频|禁用单周期转换|禁用60Hz陷波|128 */
  mode = MODE_SING|DAT_STA_EN|EXT_XTAL|SINC_4|ENPAR_DIS|CLK_DIV_DIS|SINGLECYCLE_DIS|REJ60_DIS|1023;
  cfg  = CHOP_DIS|REF_IN1|AIN1_AIN2|BURN_DIS|REFDET_DIS|BUF_DIS|UB_BI|GAIN_1;
       /*禁用斩波|外部基准电压1|12差分通道(单)|禁用激励电流|禁用基准电压检测|禁用模拟输入缓冲|双极性模式|增益为128 */
  
	AD7192_ModeCfgReg(mode,cfg);
	
	/* 校准 */
	printf("\r\n 内部校准中 \r\n");
	AD7192InternalZeroScaleCalibration();
	AD7192InternalFullScaleCalibration();
	printf("\r\n 内部校准完毕 \r\n");
	
	AD7192StartContinuousConvertion(AIN1_COM | AIN2_COM | AIN3_COM |AIN4_COM);   //启动连续转换
	
	while(1)
	{
		/* 读通道1转换数据 */
	
		ad_data = AD7192ReadConvertingData();
		
		switch(ad_data&7)
		{
			case 4:
				v = ((ad_data >> 8) / 8388608.0 - 1)*3.3;
				printf("AIN1_COM = %fV\n",v);
			
			break;
			
			case 5:
				v = ((ad_data >> 8) / 8388608.0 - 1)*3.3;
				printf("AIN2_COM = %fV\n",v);
			
			break;
			
			case 6:
				v = ((ad_data >> 8) / 8388608.0 - 1)*3.3;
				printf("AIN3_COM = %fV\n",v);
			
			break;
			
			case 7:
				v = ((ad_data >> 8) / 8388608.0 - 1)*3.3;
				printf("AIN4_COM = %fV\n",v);
			
			break;
		}
		
		
	}
}

