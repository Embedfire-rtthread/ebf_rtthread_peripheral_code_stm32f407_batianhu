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

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void led1_thread_entry(void* parameter);

void LCD_Test(void);
void Printf_Charater(void);

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
  rt_kprintf("\r\n ********** 液晶屏中文显示程序（字库在外部FLASH）*********** \r\n"); 
	rt_kprintf("\r\n 若汉字显示不正常，请阅读工程中的readme.txt文件说明，根据要求给FLASH重刷字模数据\r\n"); 

  Printf_Charater();
  while (1)
  {
    LCD_Test();
  }
}

/********************************END OF FILE****************************/


/*用于测试各种液晶的函数*/
void LCD_Test(void)
{
	/*演示显示变量*/
	static uint8_t testCNT = 0;	
	char dispBuff[100];
	
	testCNT++;	
	
	LCD_SetFont(&Font16x32);
	LCD_SetColors(RED,BLACK);

  ILI9806G_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
	/********显示字符串示例*******/ 
  ILI9806G_DispStringLine_EN_CH(LINE(0),"秉火4.5寸LCD参数：");
  ILI9806G_DispStringLine_EN_CH(LINE(2),"分辨率：480x854 px");
  ILI9806G_DispStringLine_EN_CH(LINE(3),"ILI9806G液晶驱动");
  ILI9806G_DispStringLine_EN_CH(LINE(4),"GT5688触摸屏驱动");

	/********显示变量示例*******/
	LCD_SetTextColor(GREEN);

	/*使用c标准库把变量转化成字符串*/
	sprintf(dispBuff,"显示变量计数 : %d ",testCNT);
  ILI9806G_ClearLine(LINE(7));	/* 清除单行文字 */
	
	/*然后显示该字符串即可，其它变量也是这样处理*/
	ILI9806G_DispStringLine_EN(LINE(7),dispBuff);

	/*******显示图形示例******/
  /* 画直线 */
  
  ILI9806G_ClearLine(LINE(7));/* 清除单行文字 */
	LCD_SetTextColor(BLUE);

  ILI9806G_DispStringLine_EN_CH(LINE(7),"画直线:");
  
	LCD_SetTextColor(RED);
  ILI9806G_DrawLine(50,270,420,275);  
  ILI9806G_DrawLine(50,300,420,375);
  
	LCD_SetTextColor(GREEN);
  ILI9806G_DrawLine(50,370,420,475);  
  ILI9806G_DrawLine(50,400,420,475);
	
	LCD_SetTextColor(BLUE);
  ILI9806G_DrawLine(50,420,420,325);  
  ILI9806G_DrawLine(50,450,420,395);
  
  rt_thread_delay(2000);
  
  ILI9806G_Clear(0,32*7,LCD_X_LENGTH,LCD_Y_LENGTH-32*7);	/* 清屏，显示全黑 */
  
  
  /*画矩形*/

  ILI9806G_ClearLine(LINE(7));	/* 清除单行文字 */
	LCD_SetTextColor(BLUE);

  ILI9806G_DispStringLine_EN_CH(LINE(7),"画矩形:");

	LCD_SetTextColor(RED);
  ILI9806G_DrawRectangle(50,300,200,100,1);
	
	LCD_SetTextColor(GREEN);
  ILI9806G_DrawRectangle(100,300,200,120,0);
	
	LCD_SetTextColor(BLUE);
  ILI9806G_DrawRectangle(250,300,200,150,1);
  
  
  rt_thread_delay(2000);
	
	ILI9806G_Clear(0,32*7,LCD_X_LENGTH,LCD_Y_LENGTH-32*7);	/* 清屏，显示全黑 */

  /* 画圆 */
  ILI9806G_ClearLine(LINE(7));	/* 清除单行文字 */
	LCD_SetTextColor(BLUE);
	
  ILI9806G_DispStringLine_EN_CH(LINE(7),"画圆:");

	LCD_SetTextColor(RED);
	ILI9806G_DrawCircle(150,400,60,1);

	LCD_SetTextColor(GREEN);
	ILI9806G_DrawCircle(250,400,60,0);

	LCD_SetTextColor(BLUE);
	ILI9806G_DrawCircle(350,400,60,1);

  rt_thread_delay(2000);
  
  ILI9806G_Clear(0,32*7,LCD_X_LENGTH,LCD_Y_LENGTH-32*7);	/* 清屏，显示全黑 */

}





/*"当"字符的字模16x16 */ 	  
unsigned char charater_matrix[] =    
{ /*"当",0*/
0x01,0x00,0x21,0x08,0x11,0x08,0x09,0x10,0x09,0x20,0x01,0x00,0x7F,0xF8,0x00,	0x08,
0x00,0x08,0x00,0x08,0x3F,0xF8,0x00,0x08,0x00,0x08,0x00,0x08,0x7F,0xF8,0x00,0x08,

};   
  
/**
  * @brief  使用串口在上位机打印字模
	*					演示字模显示原理
  * @retval 无
  */
void Printf_Charater(void)   
{   
	int i,j;   
	unsigned char kk; 
  
	/*i用作行计数*/
  for ( i=0;i<16;i++)   
	{   
		/*j用作一字节内数据的移位计数*/
		/*一行像素的第一个字节*/
    for(j=0; j<8; j++)   
    {      
			/*一个数据位一个数据位地处理*/
      kk = charater_matrix[2*i] << j ;  //左移J位          
			if( kk & 0x80)     
			{   
				rt_kprintf("*"); //如果最高位为1，输出*号，表示笔迹
			}   
			else  
			{   
				rt_kprintf(" "); //如果最高位为0，输出空格，表示空白  
			}   
    }   
		/*一行像素的第二个字节*/
    for(j=0; j<8; j++)   
		{              
      kk = charater_matrix[2*i+1] << j ;  //左移J位   
       
			if( kk & 0x80)         
			{   
				rt_kprintf("*"); //如果最高位为1，输出*号，表示笔迹   
			}   
			else  
			{   
				rt_kprintf(" "); //如果最高位为0，输出空格，表示空白 
			}   
		}         
    rt_kprintf("\n");    //输出完一行像素，换行 
	}   
	rt_kprintf("\n\n"); 		//一个字输出完毕
}   
