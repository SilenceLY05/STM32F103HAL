#define GPIOB_CLK        (*(volatile unsigned int*)(0x40021000 + 0x18))
#define GPIOB_CRL        (*(volatile unsigned int*)(0x40010C00 + 0x00))
#define GPIOB_ODR        (*(volatile unsigned int*)(0x40010C00 + 0x0C))

/* 软件开发步骤：
1、使能GPIO的外设时钟。外设偏移地址：0x40021000 偏移：0x18 APB2外设时钟使能寄存器
2、通过查阅参考手册的GPIO章节，知道了要配置推挽输出模式。通过端口配置表展示的寄存器来进行配置。
3、使能GPIO的端口寄存器。外设偏移地址：0x40010C00 CRL偏移：0x00 ODR偏移：0x0C */

int main(void)
{
//	GPIOB_CLK |= (1<<3);
//	
//	GPIOB_CRL &= ~(0xF<<(4*0));   //清除低四位
//	
//	GPIOB_CRL |= (2<<0);
//	
//	GPIOB_ODR &= ~(0x1<<(1*0));   //清除低四位
//	
//	//GPIOB_ODR |= (1<<0);
	
	GPIOB_CLK |= 0x00000008;
	
	GPIOB_CRL &= 0xFFFFFFF0;   //清除低四位
	
	GPIOB_CRL |= 0x00000002;
	
	GPIOB_ODR &= 0xFFFFFFFE;   //清除低四位
	
	//GPIOB_ODR |= (1<<0);
	
	while(1)
	{
		
	}
}

