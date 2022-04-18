# include "ultrasonic.h"


void tim4_pwm_init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;    //timer_init
	// NVIC_InitTypeDef NVIC_InitStructure;  // NVIC_init
	TIM_OCInitTypeDef TIM_OCInitStruct;  // tim_channel_init
	
	//1.����tim4��ʱ��->APB1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	//2.����GPIO��ʱ��->GPIOB ->APB2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_Period = arr-1;		// ����40KHz   ?????2k
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc-1;  //Ԥ��Ƶ
	//4.��ʼ��tim4��ʱ��!!!!!
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	
		//5.��ʼ�����ͨ��1  io pb 6
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse =(int)(arr / 2);   // ͨ���Ĵ�������ռ�ձ�
	TIM_OC1Init(TIM4, &TIM_OCInitStruct);
	
	// ���� io pb 7
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = (int)(arr / 2);   // ͨ���Ĵ�������ռ�ձ�	
	TIM_OC2Init(TIM4, &TIM_OCInitStruct);
	
	//6.ʹ�ܱȽϼĴ���Ԥװ�ع���
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	//7.ʹ���Զ���װ�ع���
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	//8.ʹ�ܶ�ʱ���ļ�������
	TIM_Cmd(TIM4, DISABLE);
}

void PWM_DRIVER_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;   // io_init
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//3.��ʼ��GPIOB 6 7
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 ;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	TIM_Cmd(TIM4, ENABLE);
}


void EXTI4_PB4_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;   // io_init
	EXTI_InitTypeDef EXTI_InitStruct;   // exit_init
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO,ENABLE);  //enable_clock
	
	// ��ʼ�������ź� ���� GPIOB 4
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 ;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;   // input_floating   GPIO_Mode_IPD ��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);  // io���ж�ӳ���ϵ
	
	//exit_init 
	EXTI_InitStruct.EXTI_Line = EXTI_Line4; //�ж���4 PB4
	EXTI_InitStruct.EXTI_Mode= EXTI_Mode_Interrupt  ; // �ж��¼�
	EXTI_InitStruct.EXTI_Trigger= EXTI_Trigger_Rising ; //�����غ��½��ش���
	EXTI_InitStruct.EXTI_LineCmd= ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	EXTI_ClearITPendingBit(EXTI_Line4);
	
	//NVIC_INIT
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    // ��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
		
}

void tim3_counter_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;     //�������ڶ�ʱ�����õĽṹ��
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   //ʹ��TIM3ʱ��
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //psc,����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  1M�ļ���Ƶ�� 1US����
  TIM_TimeBaseStructure.TIM_Period = (1000-1); //arr,��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ         ������1000Ϊ1ms  
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//����Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ         
        
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);   //��������жϣ����һ���ж����������ж�
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);    //ʹ�ܶ�ʱ�������ж�
       
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;             //ѡ��TIM3�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռʽ�ж����ȼ�����Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //��Ӧʽ�ж����ȼ�����Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        //ʹ���ж�
  NVIC_Init(&NVIC_InitStructure);

  TIM_Cmd(TIM3,DISABLE);     //ʧ��TIM3
	
}

void OpenTimerForHc(void)        //�򿪶�ʱ��
{
        TIM_SetCounter(TIM3,0);//�������
        msHcCount = 0;
        TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3����
}
 
 
 
//��ȡ��ʱ��ʱ��
u32 GetEchoTimer(void)
{
        u32 t = 0;
        t = msHcCount*1000;//ms��us
        t += TIM_GetCounter(TIM3);//�õ��ܵ�us
          TIM3->CNT = 0;  //��TIM3�����Ĵ����ļ���ֵ����
          // delay_ms(80);
        return t;
}
 

float GetLength(float v)
{
	  int t=0;
    float lengthTemp = 0;
    t = GetEchoTimer();        //��ȡʱ��,�ֱ���Ϊ1US
    lengthTemp = ((float)t*0.0001* v /2.0);//�õ����루cm��	
    return lengthTemp;
}
