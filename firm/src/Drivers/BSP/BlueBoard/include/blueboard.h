/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file    blueboard.h
 * @author  Paul
 * @date    2017/01/23
 * @version V2.0
 * -----------------------------------------------------------------------------
 * @brief
 *    This file is the main header of the blueboard BSP
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#ifndef __BLUEBOARD_H
#define __BLUEBOARD_H

#ifdef __cplusplus
 extern "C" {
#endif

/**
********************************************************************************
**
**  Inclusions
**
********************************************************************************
*/

/* Main Configuration file for STM32F7xx devices
 * This will also include the stm32f7xx_hal_conf file which defines
 * the SPL module used. */
#include <stm32f7xx.h>

/* Defines the HW mapping of the board and some useful macros */
#include "bb_io_mapping.h"

/* External configuration of the BlueBoard */
#include "bb_config.h"

/* Components libraries */
#include "dynamixel.h"
#include "xl_320.h"

 /**
 ********************************************************************************
 **
 **  Static Peripherals Definitions
 **
 ********************************************************************************
 */

 /* Timer to be used for run-time statistics */
 #define SYS_RUNSTATS_TIM                    TIM6
 #define SYS_RUNSTATS_TIM_CLK_ENABLE()       RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE)
 #define SYS_RUNSTATS_TIM_CLK_DISABLE()      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, DISABLE)
 #define SYS_RUNSTATS_IRQn                   TIM6_DAC_IRQn
 #define SYS_RUNSTATS_ISR                    TIM6_DAC_IRQHandler

/**
********************************************************************************
**
**  Enumerations
**
********************************************************************************
*/

/* List of available power enables */
typedef enum {
    BB_PWR_VP1 = 0,
    BB_PWR_VP2 = 1,
    BB_PWR_VP3 = 2
} BB_PWR_TypeDef;

/* List of available Analog Servo channels */
typedef enum {
    BB_ASV_CHANNEL1 = 0,
    BB_ASV_CHANNEL2 = 1,
    BB_ASV_CHANNEL3 = 2,
    BB_ASV_CHANNEL4 = 3,
    BB_ASV_CHANNEL5 = 4,
    BB_ASV_CHANNEL6 = 5,
    BB_ASV_CHANNEL7 = 6,
    BB_ASV_CHANNEL8 = 7
} BB_ASV_ChannelTypeDef;

typedef enum {
    BB_MOT_CHANNEL1 = 0,
    BB_MOT_CHANNEL2 = 1
} BB_MOT_ChannelTypeDef;

typedef enum {
    BB_ENC_CHANNEL1 = 0,
    BB_ENC_CHANNEL2 = 1,
} BB_ENC_ChannelTypeDef;

/* Bridge Current definition (%age), adjusted through I[1:0] */
typedef enum {
    BB_MOT_CURRENT_100,     /* 100 % */
    BB_MOT_CURRENT_71,      /*  71 % */
    BB_MOT_CURRENT_38,      /*  38 % */
    BB_MOT_CURRENT_0        /*   0 % */
} BB_MOT_CurrentTypeDef;

/* List of available monitoring channels */
typedef enum {
    BB_MON_CEL1     = 0,
    BB_MON_CEL2     = 1,
    BB_MON_CEL3     = 2,
    BB_MON_CEL4     = 3,
    BB_MON_IBAT     = 4,
    BB_MON_IP1      = 5,
    BB_MON_IP2      = 6,
    BB_MON_IP3      = 7,
    BB_MON_IMOT1    = 8,
    BB_MON_IMOT2    = 9,
    BB_MON_VTEMP    = 10,
    BB_MON_VREF     = 11,
} BB_MON_TypeDef;

/* List of LED colors */
typedef enum {
    BB_LED_OFF      = 0, /* All OFF */
    BB_LED_RED      = 1,
    BB_LED_GREEN    = 2,
    BB_LED_BLUE     = 3,
    BB_LED_CYAN     = 4, /* Blue + Green */
    BB_LED_YELLOW   = 5, /* Red + Green */
    BB_LED_MAGENTA  = 7, /* Blue + Red */
    BB_LED_WHITE    = 8, /* Red + Green + Blue */
} BB_LED_ColorTypeDef;

/* List of LED modes */
typedef enum {
    BB_LED_STATIC       = 0,
    BB_LED_BLINK_SLOW   = 1,
    BB_LED_BLINK_FAST   = 2
} BB_LED_ModeTypeDef;


/* List of HMI frame states */
/* SPI CSn is Active Low when actively transmitting a frame */
typedef enum {
    BB_HMI_FRAME_IDLE   = (Bit_SET),
    BB_HMI_FRAME_ACTIVE = (Bit_RESET)
} BB_HMI_FrameTypeDef;

/**
********************************************************************************
**
**  Prototypes
**
********************************************************************************
*/

/* Hardware Top */
void bb_init(void);
void bb_power_up(void);
void bb_power_down(void);

/* System */
void bb_system_clock_config(void);
void bb_sys_cpu_cache_enable(void);
void bb_sys_timer_run_time_config();
uint32_t bb_sys_timer_get_run_time_ticks(void);

/* Power modules */
void bb_pwr_init(void);
void bb_pwr_enable(BB_PWR_TypeDef power);
void bb_pwr_disable(BB_PWR_TypeDef power);

/* Quadrature Encoders */
void bb_enc_init(void);
int32_t bb_enc_get_channel(BB_ENC_ChannelTypeDef channel);
void bb_enc_reset_channels(void);

/* Main Motors */
void bb_mot_init(void);
void bb_mot_set_bridge_current(BB_MOT_CurrentTypeDef current);
void bb_mot_set_motor_speed(BB_MOT_ChannelTypeDef channel, int16_t speed, FunctionalState fastDecay);
void bb_mot_set_motor_speed_fast_decay(BB_MOT_ChannelTypeDef channel, int32_t speed);

/* Analog Servos */
void bb_asv_init(void);
void bb_asv_set_pwm_pulse_length(BB_ASV_ChannelTypeDef ASV_Channel, uint16_t pulseLength);

/* Digital Servo */
void bb_dsv_init(uint8_t chan_idx, USART_InitTypeDef * USART_InitStruct);
void bb_dsv_switch(uint8_t chan_idx, dxl_switch_mode_e mode);
void bb_dsv_put(uint8_t chan_idx, uint8_t ch);

/* Analog Monitoring */
void bb_mon_init(void);
uint16_t bb_mon_read_channel(uint8_t channel);
uint32_t bb_mon_convert_raw_value_to_mv(const uint16_t rawValue);
int32_t bb_mon_convert_temp_value_to_degree(const uint32_t vsense);

/* RGB LED */
void bb_led_init(void);
void bb_led_set_color(BB_LED_ColorTypeDef color);
/*void LedSetMode(BB_LED_ModeTypeDef mode);
void LedSetColor(BB_LED_ColorTypeDef color);*/

/* Digital I/Os */
void bb_dio_init(void);

/* Human-Machine Interface (HMI) */
void bb_hmi_init(void);
uint16_t bb_hmi_tx_rx(uint16_t value);

/* CAN Interface */
// TODO

/* RS-485 Interface */
// TODO

/* Debug Interface */
void bb_dbg_init(USART_InitTypeDef * USART_InitStruct);
void bb_dbg_enable(uint32_t nvic_priority);
void bb_dbg_disable(void);

#ifdef __cplusplus
}
#endif

#endif /* __BLUEBOARD_H */
