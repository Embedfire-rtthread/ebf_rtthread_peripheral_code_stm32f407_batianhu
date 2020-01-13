/**
  ******************************************************************************
  * @file    bsp_advance_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   通用定时器定时范例
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F407 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./tim/bsp_advance_tim.h"


 /**
  * @brief  高级控制定时器 TIMx,x[1,8]中断优先级配置
  * @param  无
  * @retval 无
  */
void TIMx_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM_IRQn; 	
		// 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */
void TIM_PWMOUTPUT_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*--------------------GPIO结构体初始化-------------------------*/
	// 开始GPIO端口时钟
	RCC_AHB1PeriphClockCmd (GENERAL_OCPWM_GPIO_CLK, ENABLE); 	

	// 通用定时器复用引脚
	GPIO_PinAFConfig(GENERAL_OCPWM_GPIO_PORT,GENERAL_OCPWM_PINSOURCE,GENERAL_OCPWM_AF);
	
	// 通用定时器PWM输出引脚															   
	GPIO_InitStructure.GPIO_Pin = GENERAL_OCPWM_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(GENERAL_OCPWM_GPIO_PORT, &GPIO_InitStructure);
	
	/*--------------------时基结构体初始化-------------------------*/
	// 配置周期，这里配置为100K
	
	// 开启TIMx_CLK
	// 通用定时器时钟源TIMxCLK = 2*PCLK1=84MHz 
  RCC_APB1PeriphClockCmd(GENERAL_TIM_CLK, ENABLE); 

  // 累计 TIM_Period个后产生一个更新或者中断*/
  TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM_PERIOD;
	// 驱动CNT计数器的时钟 = TIMxCLK/(psc+1)
  TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM_PSC;	
  // 采样时钟分频
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // 计数方式
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	
	// 初始化定时器TIMx
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
	
	/*--------------------输出比较结构体初始化-------------------*/
	// 配置为PWM模式1
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	
  // 输出使能	
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 输出通道电平极性配置
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// 输出比较值配置
  TIM_OCInitStructure.TIM_Pulse = GENERAL_TIM_CCR1;
	// 使能通道
  TIM_OC1Init(GENERAL_TIM, &TIM_OCInitStructure);	
	// 使能通道重载
	TIM_OC1PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	// 使能定时器
	TIM_Cmd(GENERAL_TIM, ENABLE);	
}

void TIM_PWMINPUT_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*--------------------GPIO结构体初始化-------------------------*/
	// 开始GPIO端口时钟
	RCC_AHB1PeriphClockCmd (ADVANCE_ICPWM_GPIO_CLK, ENABLE); 	

	// 定时器复用引脚
	GPIO_PinAFConfig(ADVANCE_ICPWM_GPIO_PORT,ADVANCE_ICPWM_PINSOURCE,ADVANCE_ICPWM_AF); 
	
	// 高级控制定时器PWM输入捕获引脚															   
	GPIO_InitStructure.GPIO_Pin = ADVANCE_ICPWM_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(ADVANCE_ICPWM_GPIO_PORT, &GPIO_InitStructure);
	
	// 开启TIMx_CLK
	// 高级定时器时钟源TIMxCLK = 2*PCLK2=168MHz 
  RCC_APB2PeriphClockCmd(ADVANCE_TIM_CLK, ENABLE); 
	
  /*--------------------时基结构体初始化----------------------*/
	// 累计 TIM_Period个后产生一个更新或者中断*/
  TIM_TimeBaseStructure.TIM_Period = ADVANCE_TIM_PERIOD;
  // 驱动CNT计数器的时钟 = TIMxCLK/(psc+1)
  TIM_TimeBaseStructure.TIM_Prescaler = ADVANCE_TIM_PSC;
	// 时钟分频因子 ，配置死区时间时需要用到
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;	
  // 计数方式
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	// 初始化定时器
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);
	
  /*--------------------输入捕获结构体初始化-------------------*/
	// 使用PWM输入模式时，需要占用两个捕获寄存器，一个测周期，另外一个测占空比
	// 捕获通道IC1配置
	// 选择捕获通道
  TIM_ICInitStructure.TIM_Channel = ADVANCE_IC1PWM_CHANNEL;
	// 设置捕获的边沿
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	// 设置捕获通道的信号来自于哪个输入通道，有直连和非直连两种
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// 1分频，即捕获信号的每个有效边沿都捕获
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// 不滤波
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
	// 初始化PWM输入模式
  TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);
	
  // 当工作做PWM输入模式时,只需要设置触发信号的那一路即可（用于测量周期）
  // 另外一路（用于测量占空比）会由硬件自带设置，不需要再配置
	
	// 选择输入捕获的触发信号
  TIM_SelectInputTrigger(ADVANCE_TIM, TIM_TS_TI1FP1);		

  // 选择从模式: 复位模式
	// PWM输入模式时,从模式必须工作在复位模式，当捕获开始时,计数器CNT会被复位
  TIM_SelectSlaveMode(ADVANCE_TIM, TIM_SlaveMode_Reset);
  TIM_SelectMasterSlaveMode(ADVANCE_TIM,TIM_MasterSlaveMode_Enable);
	
	// 使能捕获中断,这个中断针对的是主捕获通道（测量周期那个）
	TIM_ITConfig(ADVANCE_TIM, TIM_IT_CC1, ENABLE);
	TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);
	
  // 使能高级控制定时器，计数器开始计数
  TIM_Cmd(ADVANCE_TIM, ENABLE);  
}


/*********************************************END OF FILE**********************/
