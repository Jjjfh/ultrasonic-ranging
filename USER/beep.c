#include "beep.h"
#include "ultrasonic.h"


void BEEP_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能GPIOB端口时钟
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //BEEP-->PB.6 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //复用推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	 //根据参数初始化GPIOB.6
 GPIO_ResetBits(GPIOB,GPIO_Pin_6);//输出0，关闭蜂鸣器输出
}


void BEEP_ON(void)
{
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4

}

void BEEP_OFF(void)
{
TIM_Cmd(TIM4, DISABLE);    //失能TIM4
}
