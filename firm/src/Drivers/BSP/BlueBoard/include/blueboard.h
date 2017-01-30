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
    HW_PWR_VP1 = 0,
    HW_PWR_VP2 = 1,
    HW_PWR_VP3 = 2
} HW_PWR_TypeDef;

/* List of available Analog Servo channels */
typedef enum {
    HW_ASV_CHANNEL1 = 0,
    HW_ASV_CHANNEL2 = 1,
    HW_ASV_CHANNEL3 = 2,
    HW_ASV_CHANNEL4 = 3,
    HW_ASV_CHANNEL5 = 4,
    HW_ASV_CHANNEL6 = 5,
    HW_ASV_CHANNEL7 = 6,
    HW_ASV_CHANNEL8 = 7
} HW_ASV_ChannelTypeDef;

typedef enum {
    HW_MOT_CHANNEL1 = 0,
    HW_MOT_CHANNEL2 = 1
} HW_MOT_ChannelTypeDef;

typedef enum {
    HW_ENC_CHANNEL1 = 0,
    HW_ENC_CHANNEL2 = 1,
} HW_ENC_ChannelTypeDef;

/* Bridge Current definition (%age), adjusted through I[1:0] */
typedef enum {
    HW_MOT_CURRENT_100,     /* 100 % */
    HW_MOT_CURRENT_71,      /*  71 % */
    HW_MOT_CURRENT_38,      /*  38 % */
    HW_MOT_CURRENT_0        /*   0 % */
} HW_MOT_CurrentTypeDef;

/* List of available monitoring channels */
typedef enum {
    HW_MON_CEL1     = 0,
    HW_MON_CEL2     = 1,
    HW_MON_CEL3     = 2,
    HW_MON_CEL4     = 3,
    HW_MON_IBAT     = 4,
    HW_MON_IP1      = 5,
    HW_MON_IP2      = 6,
    HW_MON_IP3      = 7,
    HW_MON_IMOT1    = 8,
    HW_MON_IMOT2    = 9,
    HW_MON_VTEMP    = 10,
    HW_MON_VREF     = 11,
} HW_MON_TypeDef;

/* List of LED colors */
typedef enum {
    HW_LED_OFF      = 0, /* All OFF */
    HW_LED_RED      = 1,
    HW_LED_GREEN    = 2,
    HW_LED_BLUE     = 3,
    HW_LED_CYAN     = 4, /* Blue + Green */
    HW_LED_YELLOW   = 5, /* Red + Green */
    HW_LED_MAGENTA  = 7, /* Blue + Red */
    HW_LED_WHITE    = 8, /* Red + Green + Blue */
} HW_LED_ColorTypeDef;

/* List of LED modes */
typedef enum {
    HW_LED_STATIC       = 0,
    HW_LED_BLINK_SLOW   = 1,
    HW_LED_BLINK_FAST   = 2
} HW_LED_ModeTypeDef;

/* List of HMI frame states */
/* SPI CSn is Active Low when actively transmitting a frame */
typedef enum {
    HMI_FRAME_IDLE   = (Bit_SET),
    HMI_FRAME_ACTIVE = (Bit_RESET)
} BB_HMI_FrameTypeDef;

/**
********************************************************************************
**
**  Prototypes
**
********************************************************************************
*/

/* Hardware Top */
void HW_InitAll(void);
void HW_PowerUp(void);
void HW_PowerDown(void);

/* System */
void HW_SystemClock_Config(void);
void HW_CPU_CACHE_Enable(void);
void HW_SYS_TimerRunTime_Config();
uint32_t HW_SYS_GetRunTimeTicks(void);

/* Power modules */
void HW_PWR_Init(void);
void HW_PWR_Enable(HW_PWR_TypeDef power);
void HW_PWR_Disable(HW_PWR_TypeDef power);

/* Quadrature Encoders */
void HW_ENC_Init(void);
int32_t HW_ENC_GetChannel(HW_ENC_ChannelTypeDef channel);
void HW_ENC_ResetChannels(void);

/* Main Motors */
void HW_MOT_Init(void);
void HW_MOT_SetBridgeCurrent(HW_MOT_CurrentTypeDef current);
void HW_MOT_SetMotorSpeed(HW_MOT_ChannelTypeDef channel, int16_t speed, FunctionalState fastDecay);
void HW_MOT_SetMotorSpeedFastDecay(HW_MOT_ChannelTypeDef channel, int32_t speed);

/* Analog Servos */
void HW_ASV_Init(void);
void HW_ASV_SetPwmPulseLength(HW_ASV_ChannelTypeDef ASV_Channel, uint16_t pulseLength);
void ASV_DeployLeftArm(void);
void ASV_DeployRightArm(void);
void ASV_IdleLeftArm(void);
void ASV_IdleRightArm(void);
void ASV_MoveIndex(uint16_t position);
void ASV_DeployParasol(uint16_t position);

/* RGB LEDs */
void HW_LED_Init(void);
void HW_LED_SetColor(HW_LED_ColorTypeDef color);
void LedSetMode(HW_LED_ModeTypeDef mode);
void LedSetColor(HW_LED_ColorTypeDef color);

/* Analog Monitoring */
void HW_MON_Init(void);
uint16_t HW_MON_ReadChannel(uint8_t channel);
uint32_t HW_MON_ConvertRawValueToMv(const uint16_t rawValue);
int32_t HW_MON_ConvertTempValueToDegree(const uint32_t vsense);

/* Debug */
void bb_dbg_Init(USART_InitTypeDef * USART_InitStruct);
void bb_dbg_enable(uint32_t nvic_priority);
void bb_dbg_disable(void);

void OS_SHL_RegisterCommands( void );
void OS_SHL_Start( void );
void OS_SHL_OutputString( const char * const pcMessage );


/* Digital Servo */
void HW_DSV_Init(USART_InitTypeDef * USART_InitStruct);
void HW_DSV_Put(uint8_t ch);
void DSV_SetServo1(uint16_t position);
void DSV_SetServo2(uint16_t position);

/* Human-Machine Interface (HMI) */
void HW_HMI_Init(void);
uint16_t HW_HMI_TxRx(uint16_t value);

/* Digital Inputs */
void HW_Digital_Input_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __BLUEBOARD_H */
