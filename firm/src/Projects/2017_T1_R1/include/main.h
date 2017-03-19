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

// TEMP
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
#include "digital_servo.h"
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
#define OS_ISR_PRIORITY_DSV             ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2 )

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

/*
 * -----------------------------------------------------------------------------
 * Core & Middlewares
 * -----------------------------------------------------------------------------
 */

/* FreeRTOS prototypes for the standard FreeRTOS callback/hook functions */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationTickHook( void );

/* OS handlers */
void sys_get_run_time_stats(char *pcWriteBuffer);

/*
 * -----------------------------------------------------------------------------
 * Hardware Management
 * -----------------------------------------------------------------------------
 */

/* Analog Servos */
BaseType_t asv_start(void);

/* Digital Servos */
void dsv_init(void);
void dsv_update_config(void);
uint8_t dsv_put(uint8_t chan_idx, uint8_t tx_data);
uint8_t dsv_get(uint8_t chan_idx, const uint8_t* data);
uint8_t dsv_flush(uint8_t chan_idx);

BaseType_t dsv_start(void);

void dsv_test_pos(uint8_t id, uint16_t pos); // TEMP

/* RGB LED */
BaseType_t led_start(void);
void led_set_mode(BB_LED_ModeTypeDef mode);
void led_set_color(BB_LED_ColorTypeDef color);

/* Serial Interface */
BaseType_t serial_init(void);
BaseType_t serial_put(char ch);
BaseType_t serial_puts(const char* str);
BaseType_t serial_get(const char* str);

/* Avoidance */
void avoidance_start(void);
bool avoidance_detection_is_valid(void);

/*
 * -----------------------------------------------------------------------------
 * Sub-Systems
 * -----------------------------------------------------------------------------
 */

/* Motion */
BaseType_t motion_start(void);
bool motion_is_traj_near(void);
bool motion_is_traj_finished(void);
void motion_set_window(double window_d, double window_a, double a_start);
void motion_set_near_window(double window_d, double window_a);
void motion_set_speed(int16_t speed_d, int16_t speed_a);
void motion_traj_hard_stop(void);
void motion_traj_stop(void);
void motion_set_x(int16_t pos_x);
void motion_set_y(int16_t pos_y);
void motion_set_a(int16_t pos_a);
int16_t motion_get_x(void);
int16_t motion_get_y(void);
int16_t motion_get_a(void);
void motion_power_enable(void);
void motion_power_disable(void);
void OS_MotionMoveRelative(double d_mm, double a_deg_rel);
void OS_MotionGoToAuto(double pos_x, double pos_y);
void OS_MotionGoToFwd(double pos_x, double pos_y);
void OS_MotionGoToBwd(double pos_x, double pos_y);
void OS_MotionTurnToFront(double pos_x, double pos_y);
void OS_MotionTurnToBehind(double pos_x, double pos_y);
void motion_send_wp(wp_t *waypoint);
void motion_clear(void);

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

/* Strategy */
BaseType_t strategy_start(void);

/* Shell */
void shell_register_commands(void);
BaseType_t shell_start(void);
const char* shell_get_type_as_string(const OS_SHL_VarTypeEnum type);
size_t shell_get_type_size(const OS_SHL_VarTypeEnum type);
const char* shell_get_access_as_string(const OS_SHL_VarAccessEnum acc);
BaseType_t shell_get_variables_list(char* ret, size_t retLength);
BaseType_t shell_find_variable_by_name(const char* name, const OS_SHL_VarItemTypeDef** var);
BaseType_t shell_find_variable_by_id(size_t id, const OS_SHL_VarItemTypeDef** var);
BaseType_t shell_set_variable(OS_SHL_VarItemTypeDef const* var, char* value);
BaseType_t shell_get_variable(OS_SHL_VarItemTypeDef const* var, char* ret, size_t retLength);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
