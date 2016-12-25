/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file    blueboard.h
 * @author  Paul M. - IGREBOT
 * @date    02-January-2016
 * @version V1.0.0
 * -----------------------------------------------------------------------------
 * @brief
 *    This file contains definitions for the BlueBoard hardware.
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/firm_blueboard.git
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
 **  Firmware identification
 **
 ********************************************************************************
 */

#define YEAR_STR          "2017"
#define BUILD_VERSION_STR "v1.1"
#define ROBOT_NAME_STR    "R1"

/**
********************************************************************************
**
**  Inclusions
**
********************************************************************************
*/

/* Standard libraries */
#include <sys/stat.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <string.h>

/* Main Configuration file for STM32F7xx devices
 * This will also include the stm32f7xx_hal_conf file which defines
 * the SPL module used. */
#include "stm32f7xx.h"

/* Defines the HW mapping of the board and some useful macros */
#include "bb_io_mapping.h"

 /* Hardware (electrical & connectivity) constants */
#include "hardware_const.h"
#include "xl_320.h"

 /* Physical constants */
#include "physics_const.h"

/* Scheduler include files */
#include "FreeRTOS.h"
#include "FreeRTOS_CLI.h"
#include "task.h"
#include "semphr.h"

/* Aversive files */
#include "aversive.h"

#include "pid.h"
#include "biquad.h"
#include "quadramp.h"
#include "quadramp_derivate.h"
#include "ramp.h"
#include "angle_distance.h"

#include "blocking_detection_manager.h"
#include "control_system_manager.h"
#include "position_manager.h"
#include "robot_system.h"
#include "trajectory_manager.h"

/* User application files */
#include "motion.h"
#include "strategy.h"



/**
********************************************************************************
**
**  Applicative & High-Level Configuration
**
********************************************************************************
*/

/* OS Tasks Priorities. Higher value means higher priority */
#define OS_TASK_PRIORITY_SHELL     ( tskIDLE_PRIORITY + 1  )
#define OS_TASK_PRIORITY_LED       ( tskIDLE_PRIORITY + 2  )
#define OS_TASK_PRIORITY_ASV       ( tskIDLE_PRIORITY + 2  )
#define OS_TASK_PRIORITY_DSV       ( tskIDLE_PRIORITY + 2  )
#define OS_TASK_PRIORITY_TRAJ      ( tskIDLE_PRIORITY + 3  )
#define OS_TASK_PRIORITY_STRATEGY  ( tskIDLE_PRIORITY + 3  )
#define OS_TASK_PRIORITY_MOTION    ( tskIDLE_PRIORITY + 3  )
#define OS_TASK_PRIORITY_AVOIDANCE ( tskIDLE_PRIORITY + 4  )
#define OS_TASK_PRIORITY_AVERSIVE  ( tskIDLE_PRIORITY + 5  )

 /* Events periodicity */
#define MOTION_CONTROL_PERIOD_MS      50 /* Tests purpose */
#define AVERSIVE_PERIOD_MS            50 /* Tests purpose */

 /**
 ********************************************************************************
 **
 **  Shell Constants
 **
 ********************************************************************************
 */

/* Dimensions the buffer into which input characters are placed. */
#define SHELL_MAX_INPUT_SIZE		50

/* DEL acts as a backspace. */
#define SHELL_ASCII_DEL				( 0x7F )

/* The maximum time to wait for the mutex that guards the UART to become
available. */
#define SHELL_MAX_MUTEX_WAIT		pdMS_TO_TICKS( 300 )

/* End-Of-Line characters that should be used */
#define SHELL_EOL 						"\n\r"

/* String displayed after each output */
#define SHELL_END_OF_OUTPUT_STR 		"\n\r> "

/* Delimiter for hierarchical paths */
#define SHELL_PATH_DELIM                 "."

/* Delimiter for multiple variables output */
#define SHELL_VAR_DELIM                  ":"

/* The welcome message display at shell's startup */
#define SHELL_WELCOME_MESSAGE "\f"\
	"-----------------------------------------------------------\n\r"\
	"  IgreBot "YEAR_STR" ~ Command Shell\n\r"\
	"-----------------------------------------------------------\n\r"\
	"  Robot : "ROBOT_NAME_STR"\n\r"\
	"  Build : "BUILD_VERSION_STR"\n\r"\
	"-----------------------------------------------------------\n\r"\
	" Type 'help' for the list of available commands\n\r"\
	"-----------------------------------------------------------\n\r\n\r"\
	"> "

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
void HW_JumpToBootloader(void);

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
void HW_DBG_Init(USART_InitTypeDef * USART_InitStruct);
void HW_DBG_Put(uint8_t ch);
void HW_DBG_Puts(const char *str);
uint8_t HW_DBG_Get(void);
void OS_DebugTaskPrint( char ppcMessageToSend[] );

void OS_SHL_RegisterCommands( void );
void OS_SHL_Start( void );
void OS_SHL_OutputString( const char * const pcMessage );

BaseType_t OS_SHL_SetVariable(char* path, char* value);
BaseType_t OS_SHL_GetVariable(char* path, char* value, size_t valueLength);

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

/* FreeRTOS prototypes for the standard FreeRTOS callback/hook functions */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationTickHook( void );

/* Aversive */
bool Os_MotionTrajectoryNear(void);
bool Os_MotionTrajectoryFinished(void);
void OS_MotionSetWindow(double window_d, double window_a, double a_start);
void OS_MotionSetNearWindow(double window_d, double window_a);
void OS_MotionSetSpeed(int16_t speed_d, int16_t speed_a);
void OS_MotionTrajectoryHardStop(void);
void OS_MotionTrajectoryStop(void);
void OS_MotionSetX(int16_t pos_x);
void OS_MotionSetY(int16_t pos_y);
void OS_MotionSetA(int16_t pos_a);
int16_t OS_MotionGetX(void);
int16_t OS_MotionGetY(void);
int16_t OS_MotionGetA(void);
void OS_MotionPowerEnable(void);
void OS_MotionPowerDisable(void);
void OS_MotionMoveRelative(double d_mm, double a_deg_rel);
void OS_MotionGoToAuto(double pos_x, double pos_y);
void OS_MotionGoToFwd(double pos_x, double pos_y);
void OS_MotionGoToBwd(double pos_x, double pos_y);
void OS_MotionTurnToFront(double pos_x, double pos_y);
void OS_MotionTurnToBehind(double pos_x, double pos_y);
void motion_send_wp(wp_t *waypoint);
void motion_clear(void);

/* Mutex Handle*/
void vLockEncoderAngle(void);
void vLockEncoderDistance(void);
void vLockAngleConsign(void);
void vLockDistanceConsign(void);
void vLockRobotPosition(void);
void vUnlockEncoderAngle(void);
void vUnlockEncoderDistance(void);
void vUnlockAngleConsign(void);
void vUnlockDistanceConsign(void);
void vUnlockRobotPosition(void);

/* Main Application Tasks */
void OS_CreateDebugTask(void);
void OS_CreateLedTask(void);
void OS_CreateMotionTask(void);
void OS_CreateStrategyTask(void);
void OS_CreateAvoidanceTask(void);
void OS_CreateASVTask(void);
void OS_CreateDSVTask(void);

bool av_detection_is_valid(void);

#ifdef __cplusplus
}
#endif

#endif /* __BLUEBOARD_H */
