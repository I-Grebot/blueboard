/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       hw_system.c
 * @author     Paul
 * @date       Jan 2, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   Contains top-level system functions implementation
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev:: 1431                                                                 $
 * $LastChangedBy:: paul.m                                                     $
 * $LastChangedDate:: 2016-01-19 22:06:16 +0100 (mar., 19 janv. 2016)          $
 * -----------------------------------------------------------------------------
 */

#include "blueboard.h"

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 192000000
  *            HCLK(Hz)                       = 192000000
  *            USBCLK(Hz)                     = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 4
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 384
  *            PLL_P                          = 2
  *            PLL_Q                          = 8
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
void HW_SystemClock_Config(void)
{
  RCC_HSEConfig(RCC_HSE_ON);

  if ( RCC_WaitForHSEStartUp() != SUCCESS )
  {
    while(1);
  }

  RCC_PLLConfig(RCC_PLLSource_HSE, 8, 384, 2, 8);
  RCC_PLLCmd(ENABLE);

  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

  /* Activate the OverDrive to reach the 196 MHz Frequency */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  PWR_OverDriveCmd( ENABLE );

  while( PWR_GetFlagStatus( PWR_FLAG_ODRDY ) == RESET );

  while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

  PWR_OverDriveSWCmd(ENABLE);

  while( PWR_GetFlagStatus( PWR_FLAG_ODSWRDY ) == RESET );

  FLASH_SetLatency(FLASH_Latency_7);
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

  RCC_HCLKConfig(RCC_SYSCLK_Div1);
  RCC_PCLK1Config(RCC_HCLK_Div4);
  RCC_PCLK2Config(RCC_HCLK_Div4);

  SystemCoreClockUpdate();
}


/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
void HW_CPU_CACHE_Enable(void)
{
  /* Enable branch prediction */
  SCB->CCR |= (1 <<18);
  __DSB();

  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

