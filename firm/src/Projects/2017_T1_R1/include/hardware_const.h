/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       hardware_const.h
 * @author     Paul
 * @date       Jan 4, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   This file contains some hardware constant definitions
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev: 1464 $
 * $LastChangedBy: Pierrick_Boissard $
 * $LastChangedDate: 2016-05-02 15:38:42 +0200 (lun., 02 mai 2016) $
 * -----------------------------------------------------------------------------
 */

#ifndef __HARDWARE_CONST_H
#define __HARDWARE_CONST_H

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
**  LED
**
********************************************************************************
*/

/* LED PWM Period and Brightness value (global), ticks unit*/
#define LED_PWM_PERIOD_TICK      20
#define LED_PWM_DUTY_TICK         1

/* Slow blinking periods (ticks x PWM_PERIOD unit) */
#define LED_BLINK_SLOW          100
#define LED_BLINK_FAST           10


/**
********************************************************************************
**
**  Human-Machine Interface
**
********************************************************************************
*/

/* SPI CSn is Active Low when actively transmitting a frame */
#define HMI_FRAME_IDLE                      (Bit_SET)
#define HMI_FRAME_ACTIVE                    (Bit_RESET)


/**
********************************************************************************
**
**  Industrial Sensors
**
********************************************************************************
*/

/* IND reading scanning time */
#define IND_SCAN_PERIOD_TICK	100

/**
********************************************************************************
**
**  Debug Interface
**
********************************************************************************
*/

#define DBG_RX_QUEUE_LEN     64
#define DBG_TX_QUEUE_LEN    256 // That's only because we like to transmit

#define DBG_RX_TIMEOUT      pdMS_TO_TICKS( 10 )
#define DBG_TX_TIMEOUT      pdMS_TO_TICKS( 10 )

#endif /* __HARDWARE_CONST_H */
