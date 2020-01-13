#ifndef __TOUCHPAD_H_
#define	__TOUCHPAD_H_

#include "stm32f4xx.h"
 
/******************** TPAD 引脚配置参数定义 **************************/
#define TPAD_TIMx                   TIM2
#define TPAD_TIM_CLK                RCC_APB1Periph_TIM2

#define TPAD_TIM_Channel_X          TIM_Channel_1
#define TPAD_TIM_IT_CCx             TIM_IT_CC1
#define TPAD_TIM_GetCaptureX        TIM_GetCapture1
#define TPAD_TIM_PERIOD             0XFFFF
#define TPAD_TIM_PSC                (84-1)

//定时器最大计数值
#define TPAD_ARR_MAX_VAL 	0XFFFF	

// TIM 输入捕获通道GPIO相关宏定义
#define TPAD_TIM_GPIO_CLK           RCC_AHB1Periph_GPIOA
#define TPAD_TIM_CH_PORT            GPIOA
#define TPAD_TIM_CH_PIN             GPIO_Pin_5
#define TPAD_TIM_AF                 GPIO_AF_TIM2
#define TPAD_TIM_SOURCE             GPIO_PinSource5

// 电容按键被按下的时候门限值，需要根据不同的硬件实际测试，
// 减小这个门限值可以提高响应速度
#define            TPAD_GATE_VAL              30

// 电容按键空载的时候的最大和最小的充电时间，不同的硬件不一样，
// F407霸天虎稳定在32
#define            TPAD_DEFAULT_VAL_MIN       30
#define            TPAD_DEFAULT_VAL_MAX       35

#define            TPAD_ON                     1
#define            TPAD_OFF                    0

/************************** TPAD 函数声明********************************/
void TPAD_TIM_Init(void);
uint8_t TPAD_Init(void);
uint16_t TPAD_Get_Val(void);
uint8_t TPAD_Scan(void);

#endif /* __TOUCHPAD_H_ */

