/**
  ******************************************************************************
  * @file    bsp_touchpad.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   电容按键应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F407 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "board.h"
#include "rtthread.h"

// 电容按键空载的时候充电时间
volatile uint16_t tpad_default_val=0;


static void TPAD_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	// 使能通道引脚时钟	
	RCC_AHB1PeriphClockCmd(TPAD_TIM_GPIO_CLK, ENABLE); 	
	// 指定引脚复用
	GPIO_PinAFConfig(TPAD_TIM_CH_PORT,TPAD_TIM_SOURCE,TPAD_TIM_AF); 
	
	// 端口配置
	GPIO_InitStructure.GPIO_Pin = TPAD_TIM_CH_PIN;	
	// 复用功能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//不带上下拉 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(TPAD_TIM_CH_PORT, &GPIO_InitStructure);
}
/***********************************
 *
 * 定时器输入捕获配置
 *
 ***********************************/
static void TPAD_TIM_Mode_Config(void)
{ 
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
	// 使能TIM时钟,TIMx_CLK = 2 * PCLK1 = 84M
	RCC_APB1PeriphClockCmd(TPAD_TIM_CLK,ENABLE);
/*--------------------时基结构体初始化----------------------*/
  // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period = TPAD_TIM_PERIOD;
	// 驱动CNT计数器的时钟 = TIMx_CLK/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler = TPAD_TIM_PSC;
	// 时钟分频因子 ，配置死区时间时需要用到，这里不需要	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	// 计数器计数模式	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	// 初始化定时器
	TIM_TimeBaseInit(TPAD_TIMx, &TIM_TimeBaseStructure); 
/*--------------------输入捕获结构体初始化-------------------*/	
	// 配置输入捕获的通道，需要根据具体的GPIO来配置
	TIM_ICInitStructure.TIM_Channel = TPAD_TIM_Channel_X; 
	// 输入捕获信号的极性配置
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	
	// 输入通道和捕获通道的映射关系，有直连和非直连两种
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
	//配置输入分频,不分频 
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	
	//配置输入滤波器 不滤波
	TIM_ICInitStructure.TIM_ICFilter = 0x00;
	// 定时器输入捕获初始化
	TIM_ICInit(TPAD_TIMx, &TIM_ICInitStructure);
	
	// 使能计数器
	TIM_Cmd ( TPAD_TIMx, ENABLE ); 
}

/*===========================================================================================*/
/**
  * @brief  复位电容按键，放电，重新充电
  * @param  无
  * @retval 无
  * 说明：
  * 开发板上电之后，电容按键默认已经充满了电，要想测得电容按键的充电时间
  * 就必须先把电容按键的电放掉，方法为让接电容按键的IO输出低电平即可
  * 放电完毕之后，再把连接电容按键的IO配置为输入，然后通过输入捕获的方法
  * 测量电容按键的充电时间，这个充电时间是没有手指触摸的情况下的充电时间
  * 而且这个空载的充电时间非常稳定，因为电路板的硬件已经确定了
  * 
  * 当有手指触摸的情况下，充电时间会变长，我们只需要对比这两个时间就可以
  * 知道电容按键是否有手指触摸
  */
static void TPAD_Reset(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	// 使能通道引脚时钟	
	RCC_AHB1PeriphClockCmd(TPAD_TIM_GPIO_CLK, ENABLE); 
	
	//配置引脚为普通推挽输出
 	GPIO_InitStructure.GPIO_Pin = TPAD_TIM_CH_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
 	GPIO_Init(TPAD_TIM_CH_PORT, &GPIO_InitStructure);
	
	// 连接TPAD的IO配置为输出，然后输出低电平，延时一会，确保电容按键放电完毕
 	GPIO_ResetBits ( TPAD_TIM_CH_PORT, TPAD_TIM_CH_PIN );						 
  // 放电是很快的，一般是us级别
	//Delay_ms(5);
	rt_thread_delay(5);	
	
	// 连接TPAD的IO配置为输入，用于输入捕获
	GPIO_InitStructure.GPIO_Pin = TPAD_TIM_CH_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(TPAD_TIM_CH_PORT,&GPIO_InitStructure);
}

/**
  * @brief  获取定时器捕获值
  * @param  无
  * @retval 定时器捕获值。如果超时,则直接返回定时器的计数值。
  */
uint16_t TPAD_Get_Val(void)
{		
  // 每次捕获的时候，必须先复位放电	
	TPAD_Reset();
	
	// 当电容按键复位放电之后，计数器清0开始计数
	TIM_SetCounter(TPAD_TIMx,0);
	
	// 清除相关的标志位
	TIM_ClearITPendingBit(TPAD_TIMx, TPAD_TIM_IT_CCx|TIM_IT_Update);
	
	// 等待捕获上升沿，当电容按键充电到1.8V左右的时候，就会被认为是上升沿
	while(TIM_GetFlagStatus ( TPAD_TIMx, TPAD_TIM_IT_CCx ) == RESET)
	{
		// 如果超时了,直接返回CNT的值
		// 一般充电时间都是在ms级别以内，很少会超过定时器的最大计数值
		if(TIM_GetCounter(TPAD_TIMx)>TPAD_ARR_MAX_VAL-500)
		{
			return TIM_GetCounter(TPAD_TIMx);
		}
			
	}
	// 获取捕获比较寄存器的值
	return TPAD_TIM_GetCaptureX(TPAD_TIMx );	  
} 	

void TPAD_TIM_Init(void)
{
	TPAD_TIM_GPIO_Config();
	TPAD_TIM_Mode_Config();		
}

/**
  * @brief  初始化触摸按键，获得空载的时候触摸按键的充电时间
  * @param  无
  * @retval 0：成功，1：失败
  * @note   空载值一般很稳定，由硬件电路决定，该函数只需要调用一次即可
  *         而且这个空载的充电时间每个硬件都不一样，最好实际测试下
  */
uint8_t TPAD_Init(void)
{
	uint16_t temp;
	
	// 电容按键用到的输入捕获的IO和捕获模式参数初始化
	TPAD_TIM_Init();
	
	temp = TPAD_Get_Val();
	
	// 调试的时候可以把捕获的值打印出来，看看默认的充电时间是多少
	printf("电容按键默认充电时间为: %d us\n",temp);
	
	// 电容按键空载的充电时间非常稳定，不同的硬件充电时间不一样
	// 需要实际测试所得，F407-霸天虎上的电容按键空载充电时间稳定在32us
	// 如果你觉得单次测量不准确，你可以多次测量然后取个平均值
	if( (TPAD_DEFAULT_VAL_MIN<temp) && (temp<TPAD_DEFAULT_VAL_MAX) )
	{
		tpad_default_val = temp;
		return 0;  // 成功
	}
	else
  {
		return 1; // 失败
  }
}
 
/**
  * @brief  读取若干次定时器捕获值，并返回最大值。
  * @param  num ：读取次数
  * @retval 读取到的最大定时器捕获值
  */
uint16_t TPAD_Get_MaxVal( uint8_t num )
{
	uint16_t temp=0, res=0;
	
	while(num--)
	{
		temp = TPAD_Get_Val();
		if( temp > res ) 
			res = temp;
	}	
	return res;	
}


/**
  * @brief  按键扫描函数
  * @param  无
  * @retval 1：按键有效，0：按键无效
  */
uint8_t TPAD_Scan(void)
{
	// keyen：按键检测使能标志
	// 0：可以开始检测
	// >0：还不能开始检测，表示按键一直被按下
	// 注意：keytn 这个变量由 static 修饰，相当于一个全局变量，但是因为是在函数内部定义，
	// 所以是相当于这个函数的全局变量，每次修改之前保留的是上一次的值
	static uint8_t keyen=0;	

	uint8_t res=0,sample=3; 
	uint16_t scan_val;		

	// 根据sample值采样多次，并取最大值，小的一般是干扰或者是误触摸
	scan_val = TPAD_Get_MaxVal(sample); 
	
  // 当扫描的值大于空载值加上默认的门限值之后，表示按键按下
	// 这个TPAD_GATE_VAL根据硬件决定，需要实际测试
	if(scan_val > (tpad_default_val+TPAD_GATE_VAL))
	{						
    // 再次检测，类似于机械按键的去抖
		scan_val = TPAD_Get_MaxVal(sample);		
		if( ( keyen == 0 )&& (scan_val > (tpad_default_val+TPAD_GATE_VAL)))
				res = 1;	// 有效的按键

		// 如果按键一直被按下，keyen的值会一直在keyen的初始值和keyen-1之间循环，永远不会等于0
		keyen = 2;
	}
	
	// 当按键没有被按下或者keyen>0时，会执行keyen--
	if( keyen > 0)
		keyen--;
  
	return res;
}


/*********************************************END OF FILE**********************/
