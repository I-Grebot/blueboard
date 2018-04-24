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
 #define MOT_CHANNEL_LEFT            BB_MOT_CHANNEL2
 #define MOT_CHANNEL_RIGHT           BB_MOT_CHANNEL1

 /* Right and Left encoders channels */
 #define ENC_CHANNEL_LEFT            BB_ENC_CHANNEL2
 #define ENC_CHANNEL_RIGHT           BB_ENC_CHANNEL1

/**
 ********************************************************************************
 **
 **  Digital Servos
 **
 ********************************************************************************
 */

/* This must be defined if the RS485 interface is used as the 2nd digital servo channel */
#define BB_USE_RS485_DSV_CHAN2


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

 /* Temperature sensor offset and gain (default) */
#define ADC_TEMPERATURE_V25_MV                 760L
#define ADC_TEMPERATURE_AVG_SLOPE_UV_PER_C    2400L


/* Shunt resistors values (default) for current measurement, in milliohms*/
#define ADC_SHUNT_IBAT_MOHM   5L
#define ADC_SHUNT_IP1_MOHM    10L
#define ADC_SHUNT_IP2_MOHM    10L
#define ADC_SHUNT_IP3_MOHM    10L


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
#define BB_PRIORITY_SYS_RUNSTATS    (15) // configLIBRARY_LOWEST_INTERRUPT_PRIORITY



#endif /* __BB_CONFIG_H */
