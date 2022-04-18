# include <ultrasonic.h>
# include <led.h>
# include <beep.h>
# include <delay.h>
# include <usart.h>
# include "ds18b20.h"
int main(void)
{
	float length = 0 ;
	float temp = 0 ;
	float v = 0 ;
	delay_init();
	uart_init(115200); // init_usrt
	EXTI4_PB4_init();  // 中断初始化函数  完成  接收
	
	DS18B20_Init();
	temp = DS18B20_Get_Temp()* 0.1;
	v = 331.4 + 0.607 * temp;
	printf("dis = %fC\r\n",temp);
	printf("dis = %fm/s\r\n",v);
	
	tim4_pwm_init(25,72);    //40k
	tim3_counter_init();
	
	PWM_DRIVER_INIT();  //开启pb7,使能定时器  发送 
	delay_us(300);
//	while(1);
	OpenTimerForHc();  // 开启计时 time3   计时
	
	tim4_pwm_init(1000,12);   //  6k
	BEEP_Init();
	led_init();
	
	while(1)
	{		
		printf("T = %fC\r\n",temp);
		if(over == 1){
			length = GetLength(v);
			printf("dis = %fcm\r\n",length);//串口打印距离
			GPIO_SetBits(GPIOD,GPIO_Pin_2);
			over = 0;
			if(length > 50){
				BEEP_ON();		
			}
		}
	}
	
}
