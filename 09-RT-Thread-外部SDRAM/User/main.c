/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   RT-Thread 3.0 + STM32 工程模版
  *********************************************************************
  * @attention
  *
  * 实验平台:野火 F103-霸天虎 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/ 
#include "board.h"
#include "rtthread.h"


/*
*************************************************************************
*                               变量
*************************************************************************
*/
/* 定义线程控制块 */
static rt_thread_t led1_thread = RT_NULL;


typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress

   
/*绝对定位方式访问SRAM,这种方式必须定义成全局变量*/
uint8_t testValue __attribute__((at(Bank1_SRAM4_ADDR)));

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void led1_thread_entry(void* parameter);


/*
*************************************************************************
*                             main 函数
*************************************************************************
*/
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
    /* 
	 * 开发板硬件初始化，RTT系统初始化已经在main函数之前完成，
	 * 即在component.c文件中的rtthread_startup()函数中完成了。
	 * 所以在main函数中，只需要创建线程和启动线程即可。
	 */
	
	led1_thread =                          /* 线程控制块指针 */
    rt_thread_create( "led1",              /* 线程名字 */
                      led1_thread_entry,   /* 线程入口函数 */
                      RT_NULL,             /* 线程入口函数参数 */
                      512,                 /* 线程栈大小 */
                      3,                   /* 线程的优先级 */
                      20);                 /* 线程时间片 */
                   
    /* 启动线程，开启调度 */
   if (led1_thread != RT_NULL)
        rt_thread_startup(led1_thread);
    else
        return -1;
}

/*
*************************************************************************
*                             线程定义
*************************************************************************
*/

static void led1_thread_entry(void* parameter)
{	
	rt_kprintf ( "\r\n秉火外部 SRAM 测试\r\n" );
	
  /*蓝灯亮，表示正在读写SRAM测试*/
  LED_BLUE;
  
  /*对SRAM进行读写测试，检测SRAM是否正常*/
  if(SRAM_Test()==1)
  {
		//测试正常 绿灯亮
    LED_GREEN;			  
  }
	else
	{
		//测试失败 红灯亮
		LED_RED;
	}
	
	/*指针方式访问SRAM*/
	{	
	 uint32_t temp;
	
	 rt_kprintf("\r\n指针方式访问SRAM\r\n");
	/*向SRAM写入8位数据*/
	 *( uint8_t*) (Bank1_SRAM4_ADDR ) = (uint8_t)0xAA;
	 rt_kprintf("\r\n指针访问SRAM，写入数据0xAA \r\n");

	 /*从SRAM读取数据*/
	 temp =  *( uint8_t*) (Bank1_SRAM4_ADDR );
	 rt_kprintf("读取数据：0x%X \r\n",temp);

	 /*写/读 16位数据*/
	 *( uint16_t*) (Bank1_SRAM4_ADDR+10 ) = (uint16_t)0xBBBB;
	 rt_kprintf("指针访问SRAM，写入数据0xBBBB \r\n");
	 
	 temp =  *( uint16_t*) (Bank1_SRAM4_ADDR+10 );
	 rt_kprintf("读取数据：0x%X \r\n",temp);


	 /*写/读 32位数据*/
	 *( uint32_t*) (Bank1_SRAM4_ADDR+20 ) = (uint32_t)0xCCCCCCCC;
	 rt_kprintf("指针访问SRAM，写入数据0xCCCCCCCC \r\n");	 
	 temp =  *( uint32_t*) (Bank1_SRAM4_ADDR+20 );
	 rt_kprintf("读取数据：0x%X \r\n",temp);

	}
	
	/*绝对定位方式访问SRAM,这种方式必须定义成全局变量*/
	{
		testValue = 0xDD;
		rt_kprintf("\r\n绝对定位访问SRAM，写入数据0xDD,读出数据0x%X,变量地址为%X\r\n",testValue,(uint32_t )&testValue);	 
	}
  
  while (1)
  {
    LED1_ON;
    rt_thread_delay(500);   /* 延时500个tick */
    rt_kprintf("led1_thread running,LED1_ON\r\n");
    
    LED1_OFF;     
    rt_thread_delay(500);   /* 延时500个tick */		 		
    rt_kprintf("led1_thread running,LED1_OFF\r\n");
  }
}

/********************************END OF FILE****************************/
