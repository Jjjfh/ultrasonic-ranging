#include "beep.h"
#include "ultrasonic.h"


void BEEP_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��GPIOB�˿�ʱ��
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //BEEP-->PB.6 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�����������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //�ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	 //���ݲ�����ʼ��GPIOB.6
 GPIO_ResetBits(GPIOB,GPIO_Pin_6);//���0���رշ��������
}


void BEEP_ON(void)
{
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4

}

void BEEP_OFF(void)
{
TIM_Cmd(TIM4, DISABLE);    //ʧ��TIM4
}
