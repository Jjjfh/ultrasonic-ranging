#ifndef __ULTRASONIC_H  
#define __ULTRASONIC_H  

#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>
#include <misc.h>
#include <stm32f10x_exti.h>
#include <stm32f10x.h>
#include "stm32f10x_it.h" 

void tim4_pwm_init(u16 arr,u16 psc);
void EXTI4_PB4_init(void);
void tim3_counter_init(void);
void OpenTimerForHc(void);
float GetLength(float v);
void PWM_DRIVER_INIT(void);


#endif 
