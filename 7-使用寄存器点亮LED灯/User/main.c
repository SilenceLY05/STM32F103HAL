#define GPIOB_CLK        (*(volatile unsigned int*)(0x40021000 + 0x18))
#define GPIOB_CRL        (*(volatile unsigned int*)(0x40010C00 + 0x00))
#define GPIOB_ODR        (*(volatile unsigned int*)(0x40010C00 + 0x0C))

/* ����������裺
1��ʹ��GPIO������ʱ�ӡ�����ƫ�Ƶ�ַ��0x40021000 ƫ�ƣ�0x18 APB2����ʱ��ʹ�ܼĴ���
2��ͨ�����Ĳο��ֲ��GPIO�½ڣ�֪����Ҫ�����������ģʽ��ͨ���˿����ñ�չʾ�ļĴ������������á�
3��ʹ��GPIO�Ķ˿ڼĴ���������ƫ�Ƶ�ַ��0x40010C00 CRLƫ�ƣ�0x00 ODRƫ�ƣ�0x0C */

int main(void)
{
//	GPIOB_CLK |= (1<<3);
//	
//	GPIOB_CRL &= ~(0xF<<(4*0));   //�������λ
//	
//	GPIOB_CRL |= (2<<0);
//	
//	GPIOB_ODR &= ~(0x1<<(1*0));   //�������λ
//	
//	//GPIOB_ODR |= (1<<0);
	
	GPIOB_CLK |= 0x00000008;
	
	GPIOB_CRL &= 0xFFFFFFF0;   //�������λ
	
	GPIOB_CRL |= 0x00000002;
	
	GPIOB_ODR &= 0xFFFFFFFE;   //�������λ
	
	//GPIOB_ODR |= (1<<0);
	
	while(1)
	{
		
	}
}

