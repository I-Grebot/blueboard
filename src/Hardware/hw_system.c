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

static uint16_t sysTimerCnt;

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

void HW_SYS_TimerRunTime_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_BaseStruct;

    /* Enable timer clock */
    SYS_RUNSTATS_TIM_CLK_ENABLE();

    /* Setup timer */
    TIM_BaseStruct.TIM_ClockDivision        = SYS_RUNSTATS_CKDIV;
    TIM_BaseStruct.TIM_Prescaler            = SYS_RUNSTATS_PRESCALER;
    TIM_BaseStruct.TIM_Period               = 0xFFFF;
    TIM_BaseStruct.TIM_RepetitionCounter    = 0;
    TIM_TimeBaseInit(SYS_RUNSTATS_TIM, &TIM_BaseStruct);

    /* Configure interrupt */
    TIM_ITConfig(SYS_RUNSTATS_TIM, TIM_IT_Update, ENABLE);
    NVIC_SetPriority(SYS_RUNSTATS_IRQn, OS_ISR_PRIORITY_SYS_RUNSTATS);
    NVIC_EnableIRQ(SYS_RUNSTATS_IRQn);

    /* Clear and start timer */
    sysTimerCnt = 0;
    TIM_SetCounter(SYS_RUNSTATS_TIM, 0);
    TIM_Cmd(SYS_RUNSTATS_TIM, ENABLE);

}

uint32_t HW_SYS_GetRunTimeTicks(void)
{
    uint32_t ret;
    ret = (((uint32_t) sysTimerCnt) << 16UL) + TIM_GetCounter(SYS_RUNSTATS_TIM);
    return ret;
}

/*
 * Run-Time Timer Interrupt Sub-routine
 * Required to implement a 32bits timer.
 * sysTimerCnt is used as the 16 MSB of the timer.
 */
void SYS_RUNSTATS_ISR (void)
{
    NVIC_ClearPendingIRQ(SYS_RUNSTATS_IRQn);
    TIM_ClearITPendingBit(SYS_RUNSTATS_TIM, TIM_IT_Update);
    sysTimerCnt++;
}

/* This example demonstrates how a human readable table of run time stats
information is generated from raw data provided by uxTaskGetSystemState().
The human readable table is written to pcWriteBuffer.  (see the vTaskList()
API function which actually does just this). */
void HW_SYS_GetRunTimeStats(char *pcWriteBuffer)
{
    const char * const pcHeader =
                    SHELL_SYS_PFX"Task                  Abs. Time    % Time"SHELL_EOL;
    const char * const pcLineSeparator =
                    SHELL_SYS_PFX"-----------------------------------------"SHELL_EOL;

    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x;
    uint32_t ulTotalRunTime;
    uint32_t ulStatsAsPercentage;

   /* Make sure the write buffer does not contain a string. */
   *pcWriteBuffer = 0x00;

   /* Take a snapshot of the number of tasks in case it changes while this
   function is executing. */
   uxArraySize = uxTaskGetNumberOfTasks();

   /* Allocate a TaskStatus_t structure for each task.  An array could be
   allocated statically at compile time. */
   pxTaskStatusArray = pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

   if( pxTaskStatusArray != NULL )
   {
      /* Generate raw status information about each task. */
      uxArraySize = uxTaskGetSystemState( pxTaskStatusArray,
                                 uxArraySize,
                                 &ulTotalRunTime );

      /* For percentage calculations. */
      ulTotalRunTime /= 100UL;

      /* Avoid divide by zero errors. */
      if( ulTotalRunTime > 0 )
      {
          /* Header */
          sprintf( pcWriteBuffer, pcHeader );
          pcWriteBuffer += strlen( ( char * ) pcWriteBuffer );
          sprintf( pcWriteBuffer, pcLineSeparator );
          pcWriteBuffer += strlen( ( char * ) pcWriteBuffer );

          /* For each populated position in the pxTaskStatusArray array,
          format the raw data as human readable ASCII data. */
         for( x = 0; x < uxArraySize; x++ )
         {
            /* What percentage of the total run time has the task used?
            This will always be rounded down to the nearest integer.
            ulTotalRunTimeDiv100 has already been divided by 100. */
            ulStatsAsPercentage =
                  pxTaskStatusArray[ x ].ulRunTimeCounter / ulTotalRunTime;

            if( ulStatsAsPercentage > 0UL )
            {
               sprintf( pcWriteBuffer, SHELL_SYS_PFX"%-20s %10lu   %3lu%%"SHELL_EOL,
                                 pxTaskStatusArray[ x ].pcTaskName,
                                 pxTaskStatusArray[ x ].ulRunTimeCounter,
                                 ulStatsAsPercentage );
            }
            else
            {
               /* If the percentage is zero here then the task has
               consumed less than 1% of the total run time. */
               sprintf( pcWriteBuffer, SHELL_SYS_PFX"%-20s %10lu    <1%%"SHELL_EOL,
                                 pxTaskStatusArray[ x ].pcTaskName,
                                 pxTaskStatusArray[ x ].ulRunTimeCounter );
            }

            pcWriteBuffer += strlen( ( char * ) pcWriteBuffer );
         }

         /* Footer */
         sprintf( pcWriteBuffer, pcLineSeparator );
         pcWriteBuffer += strlen( ( char * ) pcWriteBuffer );
         sprintf( pcWriteBuffer,SHELL_SYS_PFX"%-20s %10lu   %3lu%%"SHELL_EOL, "TOTAL", 100*ulTotalRunTime, 100UL);
         pcWriteBuffer += strlen( ( char * ) pcWriteBuffer );
      }

      /* The array is no longer needed, free the memory it consumes. */
      vPortFree( pxTaskStatusArray );
   }
}
