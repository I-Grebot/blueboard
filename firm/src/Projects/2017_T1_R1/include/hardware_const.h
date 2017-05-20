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
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#ifndef __HARDWARE_CONST_H
#define __HARDWARE_CONST_H



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
**  Input switches
**
********************************************************************************
*/

/* Start switch */
#define SW_START            SW2_VALUE
#define SW_START_INSERTED   1U
#define SW_START_REMOVED    0U

/* Color switch */
#define SW_COLOR            SW1_VALUE
#define SW_COLOR_TEAM1      0U
#define SW_COLOR_TEAM2      1U

/**
********************************************************************************
**
**  Industrial Sensors
**
********************************************************************************
*/

// Avoidance sensors
#define SW_AVD_FRONT_LEFT_VALUE      IND4_VALUE
#define SW_AVD_FRONT_CENTER_VALUE    IND7_VALUE
#define SW_AVD_FRONT_RIGHT_VALUE     IND6_VALUE
#define SW_AVD_BACK_LEFT_VALUE       IND1_VALUE
#define SW_AVD_BACK_CENTER_VALUE     IND2_VALUE
#define SW_AVD_BACK_RIGHT_VALUE      IND3_VALUE
#define SW_AVD_ON                    true

// Modules System
#define SW_SYS_MOD_DETECT_VALUE      IND8_VALUE
#define SW_SYS_MOD_DETECT_ON         true

/**
********************************************************************************
**
**  Analog Servos
**
********************************************************************************
*/

// Channels definition
#define ASV_CHANNEL_ROTATOR   BB_ASV_CHANNEL1
#define ASV_CHANNEL_TROLLET   BB_ASV_CHANNEL2

// Positions
#define ASV_ROTATOR_MIN   0x0000
#define ASV_ROTATOR_MAX   0xFFFF

#define ASV_TROLLET_MIN   0x0000
#define ASV_TROLLET_MAX   0xFFFF


/**
********************************************************************************
**
**  Digital Servos
**
********************************************************************************
*/

/* Queues size for each channel, in Bytes */
#define DSV_RX_QUEUE_SIZE   64
#define DSV_TX_QUEUE_SIZE   64

/* Timeouts */
#define DSV_RX_TIMEOUT      pdMS_TO_TICKS( 10 ) // Must be at least longest TX + Return-time delay
#define DSV_TX_TIMEOUT      pdMS_TO_TICKS(  5 )

/**
********************************************************************************
**
**  Serial Interface
**
********************************************************************************
*/

#define SERIAL_BAUDRATE         115200

#define SERIAL_ISR              DBG_ISR
#define SERIAL_COM              DBG_COM

#define SERIAL_RX_QUEUE_LEN     64
#define SERIAL_TX_QUEUE_LEN    256 // That's only because we like to transmit

#define SERIAL_RX_TIMEOUT      pdMS_TO_TICKS( 10 )
#define SERIAL_TX_TIMEOUT      pdMS_TO_TICKS( 10 )

#endif /* __HARDWARE_CONST_H */
