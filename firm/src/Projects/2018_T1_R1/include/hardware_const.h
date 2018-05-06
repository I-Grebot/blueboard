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
#define SW_START_INSERTED   0U
#define SW_START_REMOVED    1U

/* Color switch */
#define SW_COLOR            SW1_VALUE
#define SW_COLOR_TEAM1      0U
#define SW_COLOR_TEAM2      1U

/**
********************************************************************************
**
**  Output commands
**
********************************************************************************
*/

// Command for the trollet
#define SW_SHOOTER_CMD0   END8_WRITE
#define SW_SHOOTER_CMD1   END7_WRITE

// Commands decoding
#define SW_SHOOTER_WAIT         3U // matches default GPIO config with a pull-down
#define SW_SHOOTER_INIT         2U
#define SW_SHOOTER_SHOOT_HIGH   1U
#define SW_SHOOTER_SHOOT_LOW    0U

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

/**
********************************************************************************
**
**  Analog Servos
**
********************************************************************************
*/

// Channels definition
#define ASV_CHANNEL_FUNNY     BB_ASV_CHANNEL1
#define ASV_CHANNEL_ROTATOR   BB_ASV_CHANNEL2
#define ASV_CHANNEL_TROLLET   BB_ASV_CHANNEL3

// Positions
#define ASV_FUNNY_OFF       (ASV_TIMER_PERIOD/10)
#define ASV_FUNNY_TRIGGER   (ASV_TIMER_PERIOD/20)

#define ASV_ROTATOR_MIN   (ASV_TIMER_PERIOD/10)
#define ASV_ROTATOR_MAX   (ASV_TIMER_PERIOD/20)
#define ASV_ROTATOR_0     (ASV_TIMER_PERIOD/20)
#define ASV_ROTATOR_90    (3800)

#define ASV_TROLLET_MIN    0x0000
#define ASV_TROLLET_MAX    ASV_TIMER_PERIOD
#define ASV_TROLLET_LEFT   0
#define ASV_TROLLET_RIGHT  ASV_TIMER_PERIOD

/**
********************************************************************************
**
**  Digital Servos
**
********************************************************************************
*/

// Queues size for each channel, in Bytes
#define DSV_RX_QUEUE_SIZE   64
#define DSV_TX_QUEUE_SIZE   64

// Timeouts
#define DSV_RX_TIMEOUT      pdMS_TO_TICKS( 50) // Must be at least longest TX + Return-time delay
#define DSV_TX_TIMEOUT      pdMS_TO_TICKS( 50 )

// Servos IDs
#define DSV_LEFT_ARM_ID     30
#define DSV_RIGHT_ARM_ID    40
#define DSV_OPENER_ID       50
#define DSV_PUSHER_ID       10
#define DSV_INDEX_ID        22

// Positions
#define DSV_LEFT_ARM_POS_DOWN                    100
#define DSV_LEFT_ARM_POS_UP                      360

#define DSV_RIGHT_ARM_POS_DOWN                   735
#define DSV_RIGHT_ARM_POS_UP                     475

#define DSV_OPENER_POS_RIGHT                     650
#define DSV_OPENER_POS_CENTER                    400
#define DSV_OPENER_POS_LEFT                      150

#define DSV_PUSHER_OUT						 	 540
#define DSV_PUSHER_IN						 	 350

#define DSV_INDEX_POS_GET					     350
#define DSV_INDEX_POS_SET				         650


// Speeds
#define DSV_ARMS_SPEED_FAST                     1023
#define DSV_ARMS_SPEED_SLOW                      300
#define DSV_OPENER_SPEED_FAST                   1023
#define DSV_OPENER_SPEED_SLOW                    300
#define DSV_PUSHER_SPEED                         300
#define DSV_INDEX_SPEED                          500

// Torques
#define DSV_ARMS_TORQUE                          500
#define DSV_OPENER_TORQUE                        500
#define DSV_PUSHER_TORQUE                        250
#define DSV_INDEX_TORQUE                         500

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
