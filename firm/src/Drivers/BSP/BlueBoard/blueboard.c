/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       blueboard.c
 * @author     Paul
 * @date       Jan 4, 2016
 * -----------------------------------------------------------------------------
 * @brief
 *   This is the top-level module of the Board Support Package (BSP) functions.
 *
 *   Hardware informations:
 *      o External Oscillator Clock :   8 MHz
 *      o Internal System Clock     : 192 MHz
 *
 *   Hardware peripherals resources usage:
 *      o TIM1                  for [MOT] Main Motors DIR and PWM channels 1 to 4
 *      o TIM3 / TIM4           for [QUA] Quadrature Encoders channels A (1) and B (2)
 *      o TIM5 / TIM8           for [ASV] Analog Servos PWM channels 1 to 8
 *      o TIM6                  for [SYS] Run-Time statistics
 *      o SPI4                  for [HMI] Human Machine Interface
 *      o CAN1                  for [CAN] CAN bus Interface
 *      o USART1                for [DBG] Debug USART
 *      o USART2                for [RS4] RS485 bus Interface
 *      o USART3                for [DSV] Digital Servo bus Interface
 *      o ADC1 / TIM2 / DMA2    for [MON] Analog monitoring (Scan-mode with DMA, auto)
 *      o USB_OTG               for [USB] USB OTG High-Speed 2.0 Interface
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

/* Inclusions */
#include "blueboard.h"

/**
  * @brief  Main initializations for the blueboard modules.
  * @param  None
  * @retval None
  */
void bb_init(void)
{
    /* System Config */
    bb_sys_cpu_cache_enable();
    bb_system_clock_config();

    /* Modules without custom-configuration */
    bb_pwr_init();
    bb_led_init();
    bb_enc_init();
    bb_mot_init();
    bb_asv_init();
    bb_mon_init();
    bb_hmi_init();
    bb_dio_init();

    /* Set Interrupt group priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
}

/**
  * @brief  Apply the power-up sequence
  * @param  None
  * @retval None
  */
void bb_power_up(void)
{

    //MOT_SLEEP_WRITE(MOT_SLEEP_OFF);
    //MOT_RESET_WRITE(MOT_RESET_ON);

    /* Enable powers */
    bb_pwr_enable(BB_PWR_VP1);
    bb_pwr_enable(BB_PWR_VP2);
    bb_pwr_enable(BB_PWR_VP3);

    /* Enable H-bridges power */
  //  MOT_RESET_WRITE(MOT_RESET_OFF);

}

/**
  * @brief  Apply the power-down sequence
  * @param  None
  * @retval None
  */
void bb_power_down(void)
{
    /* Disable H-bridges */
    MOT_SLEEP_WRITE(MOT_SLEEP_ON);
    MOT_RESET_WRITE(MOT_RESET_ON);

    /* Disable Power */
    bb_pwr_disable(BB_PWR_VP1);
    bb_pwr_disable(BB_PWR_VP2);
    bb_pwr_disable(BB_PWR_VP3);
}

