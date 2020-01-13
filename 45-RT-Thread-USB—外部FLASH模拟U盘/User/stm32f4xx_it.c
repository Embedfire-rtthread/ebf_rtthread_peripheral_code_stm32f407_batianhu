/**
  ******************************************************************************
  * @file    FMC_SDRAM/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "./Bsp/usart/bsp_debug_usart.h"

#include "rtthread.h"

#include "usb_core.h"
#include "usbd_core.h"
#include "usb_conf.h"
#include "usb_bsp.h"

extern USB_OTG_CORE_HANDLE  USB_OTG_dev;

/* Private function prototypes -----------------------------------------------*/
extern uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);

#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED 
extern uint32_t USBD_OTG_EP1IN_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
extern uint32_t USBD_OTG_EP1OUT_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
#endif


	
/**
  * @brief  This function handles OTG_HS Handler.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_OTG_HS  
void OTG_HS_IRQHandler(void)
{
  /* 进入中断 */
  rt_interrupt_enter();

  USBD_OTG_ISR_Handler (&USB_OTG_dev);
  
  /* 离开中断 */
  rt_interrupt_leave();

}
#endif

#ifdef USE_USB_OTG_FS  
void OTG_FS_IRQHandler(void)
{
  /* 进入中断 */
  rt_interrupt_enter();

  USBD_OTG_ISR_Handler (&USB_OTG_dev);
  
  /* 离开中断 */
  rt_interrupt_leave();

}
#endif
#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED 
/**
  * @brief  This function handles EP1_IN Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_EP1_IN_IRQHandler(void)
{
  /* 进入中断 */
  rt_interrupt_enter();

  USBD_OTG_EP1IN_ISR_Handler (&USB_OTG_dev);
  
  /* 离开中断 */
  rt_interrupt_leave();

}

/**
  * @brief  This function handles EP1_OUT Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_EP1_OUT_IRQHandler(void)
{
  /* 进入中断 */
  rt_interrupt_enter();

  USBD_OTG_EP1OUT_ISR_Handler (&USB_OTG_dev);
  
  /* 离开中断 */
  rt_interrupt_leave();
}
#endif
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f429_439xx.s).                         */
/******************************************************************************/

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
