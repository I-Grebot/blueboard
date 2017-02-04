/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2017
 * -----------------------------------------------------------------------------
 * @file    main.h
 * @author  Paul
 * @date    2017/01/23
 * @version V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *    This file is the main header of IgreBot's 2017 Team 1 Robot 1 firmware
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#ifndef __MAIN_H
#define __MAIN_H

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
#define ROBOT_NAME_STR    "R1"
#define BUILD_VERSION_STR __DATE__" @ "__TIME__

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

/* Board-Support Package Drivers */
#include "blueboard.h"
#include "xl_320.h"

/* Middleware: FreeRTOS + FreeRTOS-Plus */
#include "FreeRTOS.h"
#include "FreeRTOS_CLI.h"
#include "task.h"
#include "semphr.h"

/* Middleware: Aversive */
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

/* Project files */
#include "motion.h"
#include "strategy.h"
#include "shell.h"
#include "physics_const.h"
#include "hardware_const.h"


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
#define OS_TASK_PRIORITY_AVERSIVE  ( tskIDLE_PRIORITY + 4  )

 /* NVIC Priorities. Lower value means higher priority.
  * Beware to use priorities smaller than configLIBRARY_LOWEST_INTERRUPT_PRIORITY
  * and higher than configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY when using
  * ISR Save FreeRTOS API Routines!
  */
#define OS_ISR_PRIORITY_SER             ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1 )
#define OS_ISR_PRIORITY_SYS_RUNSTATS    ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY )

 /* Events periodicity */
#define MOTION_CONTROL_PERIOD_MS      50
#define AVERSIVE_PERIOD_MS            50


/**
********************************************************************************
**
**  Prototypes
**
********************************************************************************
*/

/* FreeRTOS prototypes for the standard FreeRTOS callback/hook functions */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationTickHook( void );

/* OS handlers */
void HW_SYS_GetRunTimeStats(char *pcWriteBuffer);

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

/* Analog Servos */
void ASV_DeployLeftArm(void);
void ASV_DeployRightArm(void);
void ASV_IdleLeftArm(void);
void ASV_IdleRightArm(void);
void ASV_MoveIndex(uint16_t position);
void ASV_DeployParasol(uint16_t position);

/* Digital Servos */
void dsv_init(void);
void DSV_SetServo1(uint16_t position);
void DSV_SetServo2(uint16_t position);

bool av_detection_is_valid(void);

/* LEDs */
//void LedSetMode(BB_LED_ModeTypeDef mode);
//void LedSetColor(BB_LED_ColorTypeDef color);

/* Serial Interface */
BaseType_t serial_init(void);
BaseType_t serial_put(char ch);
BaseType_t serial_puts(const char* str);
BaseType_t serial_get(const char* str);

void OS_DebugTaskPrint( char ppcMessageToSend[] );

/* Shell */

void OS_SHL_RegisterCommands( void );
void OS_SHL_Start( void );
void OS_SHL_OutputString( const char * const pcMessage );

const char* OS_SHL_GetTypeAsString(const OS_SHL_VarTypeEnum type);
size_t OS_SHL_GetTypeSize(const OS_SHL_VarTypeEnum type);
const char* OS_SHL_GetAccessAsString(const OS_SHL_VarAccessEnum acc);
BaseType_t OS_SHL_GetVariablesList(char* ret, size_t retLength);
BaseType_t OS_SHL_FindVariableByName(char* name, const OS_SHL_VarItemTypeDef** var);
BaseType_t OS_SHL_FindVariableById(size_t id, const OS_SHL_VarItemTypeDef** var);
BaseType_t OS_SHL_SetVariable(OS_SHL_VarItemTypeDef const* var, char* value);
BaseType_t OS_SHL_GetVariable(OS_SHL_VarItemTypeDef const* var, char* ret, size_t retLength);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
