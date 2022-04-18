# include "ultrasonic.h"


void tim4_pwm_init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;    //timer_init
	// NVIC_InitTypeDef NVIC_InitStructure;  // NVIC_init
	TIM_OCInitTypeDef TIM_OCInitStruct;  // tim_channel_init
	
	//1.开启tim4的时钟->APB1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	//2.开启GPIO的时钟->GPIOB ->APB2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_Period = arr-1;		// 计数40KHz   ?????2k
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc-1;  //预分频
	//4.初始化tim4定时器!!!!!
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	
		//5.初始化输出通道1  io pb 6
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse =(int)(arr / 2);   // 通过寄存器设置占空比
	TIM_OC1Init(TIM4, &TIM_OCInitStruct);
	
	// 互补 io pb 7
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = (int)(arr / 2);   // 通过寄存器设置占空比	
	TIM_OC2Init(TIM4, &TIM_OCInitStruct);
	
	//6.使能比较寄存器预装载功能
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	//7.使能自动重装载功能
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	//8.使能定时器的计数功能
	TIM_Cmd(TIM4, DISABLE);
}

void PWM_DRIVER_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;   // io_init
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//3.初始化GPIOB 6 7
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
	
	// 初始化接收信号 引脚 GPIOB 4
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 ;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;   // input_floating   GPIO_Mode_IPD 下拉输入
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);  // io与中断映射关系
	
	//exit_init 
	EXTI_InitStruct.EXTI_Line = EXTI_Line4; //中断线4 PB4
	EXTI_InitStruct.EXTI_Mode= EXTI_Mode_Interrupt  ; // 中断事件
	EXTI_InitStruct.EXTI_Trigger= EXTI_Trigger_Rising ; //上升沿和下降沿触发
	EXTI_InitStruct.EXTI_LineCmd= ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	EXTI_ClearITPendingBit(EXTI_Line4);
	
	//NVIC_INIT
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    // 响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
		
}

void tim3_counter_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;     //生成用于定时器设置的结构体
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   //使能TIM3时钟
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //psc,设置用来作为TIMx时钟频率除数的预分频值  1M的计数频率 1US计数
  TIM_TimeBaseStructure.TIM_Period = (1000-1); //arr,设置在下一个更新事件装入活动的自动重装载寄存器周期的值         计数到1000为1ms  
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位         
        
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);   //清除更新中断，免得一打开中断立即产生中断
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);    //使能定时器更新中断
       
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;             //选择TIM3中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //抢占式中断优先级设置为0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //响应式中断优先级设置为0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        //使能中断
  NVIC_Init(&NVIC_InitStructure);

  TIM_Cmd(TIM3,DISABLE);     //失能TIM3
	
}

void OpenTimerForHc(void)        //打开定时器
{
        TIM_SetCounter(TIM3,0);//清除计数
        msHcCount = 0;
        TIM_Cmd(TIM3, ENABLE);  //使能TIM3外设
}
 
 
 
//获取定时器时间
u32 GetEchoTimer(void)
{
        u32 t = 0;
        t = msHcCount*1000;//ms→us
        t += TIM_GetCounter(TIM3);//得到总的us
          TIM3->CNT = 0;  //将TIM3计数寄存器的计数值清零
          // delay_ms(80);
        return t;
}
 

float GetLength(float v)
{
	  int t=0;
    float lengthTemp = 0;
    t = GetEchoTimer();        //获取时间,分辨率为1US
    lengthTemp = ((float)t*0.0001* v /2.0);//得到距离（cm）	
    return lengthTemp;
}
