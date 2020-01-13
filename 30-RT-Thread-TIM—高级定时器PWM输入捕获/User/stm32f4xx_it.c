/* 该文件统一用于存放中断服务函数 */
#include "stm32f4xx.h"
#include "board.h" 

__IO uint16_t IC2Value = 0;
__IO uint16_t IC1Value = 0;
__IO float DutyCycle = 0;
__IO float Frequency = 0;

void  ADVANCE_TIM_IRQHandler (void)
{
  /* 清除定时器捕获/比较1中断 */
  TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);

  /* 获取输入捕获值 */
  IC1Value = TIM_GetCapture1(ADVANCE_TIM);
  IC2Value = TIM_GetCapture2(ADVANCE_TIM);	
  //printf("IC1Value = %d  IC2Value = %d ",IC1Value,IC2Value);
	
  // 注意：捕获寄存器CCR1和CCR2的值在计算占空比和频率的时候必须加1
	if (IC1Value != 0)
  {
    /* 占空比计算 */
    DutyCycle = (float)((IC2Value+1) * 100) / (IC1Value+1);

    /* 频率计算 */
    Frequency = 168000000/(ADVANCE_TIM_PSC+1)/(float)(IC1Value+1);
		printf("占空比：%0.2f%%   频率：%0.2fHz\n",DutyCycle,Frequency);
  }
  else
  {
    DutyCycle = 0;
    Frequency = 0;
  }
}


