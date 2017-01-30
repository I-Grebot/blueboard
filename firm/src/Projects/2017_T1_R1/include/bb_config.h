/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file    bb_config.h
 * @author  Paul
 * @date    2017/01/30
 * @version V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *    External configuration-file for the blueboard BSP
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#ifndef __BB_CONFIG_H
#define __BB_CONFIG_H

#include "main.h"

/**
 ********************************************************************************
 **
 **  Analog Servos
 **
 ********************************************************************************
 */

 /* Prescaler for TIM5/TIM8 of Analog Servos
  * Both timers have a 96 MHz input clock with no divider
  * Setup a 2 MHz tick frequency and a 50 Hz PWM frequency
  * Pulse length resolution is then 50 ns (40000 steps)
  */
 #define ASV_TIMER_PRESCALER         47
 #define ASV_TIMER_PERIOD            39999

/**
 ********************************************************************************
 **
 **  Main Motors & Quadrature Encoders
 **
 ********************************************************************************
 */

 /* Prescaler for TIM1 of Main Motors
  * The timer is fed with a 96 MHz input clock with no divider
  * Setup a 24kHz PWM frequency (4000 steps for duty-cycle adjustment)
  */
 #define MOT_TIMER_PRESCALER         0
 #define MOT_TIMER_PERIOD            3999


 /* Right and Left motors PWM channels */
 #define MOT_CHANNEL_LEFT            HW_MOT_CHANNEL1
 #define MOT_CHANNEL_RIGHT           HW_MOT_CHANNEL2

 /* Right and Left encoders channels */
 #define ENC_CHANNEL_LEFT            HW_ENC_CHANNEL1
 #define ENC_CHANNEL_RIGHT           HW_ENC_CHANNEL2

 /**
 ********************************************************************************
 **
 **  ADC
 **
 ********************************************************************************
 */

 /* External VREF value in mV */
 #define ADC_VREF_VALUE              3000

 /* Number of steps per V for a 12 bits conversion */
 #define ADC_STEPS_PER_VOLT          (4096000/ADC_VREF_VALUE)

 /* Temperature sensor offset at 25°C in mV */
 #define ADC_TEMPERATURE_V25         760

 /* Temperature sensor slope in mV/C */
 #define ADC_TEMPERATURE_AVG_SLOPE   2500

/**
 ********************************************************************************
 **
 ** System Run-Stats
 **
 ********************************************************************************
 */

 /* Clock-divider and prescaler config to adjust the run-time timer frequency
  * APB1 Timers Running at   96000 kHz
  * System ticks running at      1 kHz
  */
#define SYS_RUNSTATS_CKDIV          TIM_CKD_DIV4
#define SYS_RUNSTATS_PRESCALER      (1199) // 20 kHz

/* NVIC priority of the system runstats timer */
#define BB_PRIORITY_SYS_RUNSTATS    OS_ISR_PRIORITY_SYS_RUNSTATS



#endif /* __BB_CONFIG_H */
