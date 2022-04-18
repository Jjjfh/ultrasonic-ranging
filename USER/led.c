#include <led.h>


void led_init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);	 //使能PA,PD端口时钟
	
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_8|GPIO_Pin_11;				 //LED0 LED2 LED3-->PA.1 8 11 端口配置
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //input_floating		GPIO_Mode_IN_FLOATING		
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
   GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.1/8/11
   GPIO_ResetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_8|GPIO_Pin_11);//PA.1.8.11全部拉低
	
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //LED1-->PD.2端口配置
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
   GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化GPIOD
   GPIO_ResetBits(GPIOD,GPIO_Pin_2);						 //PD.2 输出

}
