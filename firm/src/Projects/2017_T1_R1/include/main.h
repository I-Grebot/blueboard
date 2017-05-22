/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file    main.h
 * @author  Paul
 * @date    2017/01/23
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

/* Project files */
#include "physics_const.h"
#include "hardware_const.h"
#include "digital_servo.h"
#include "sys_modules.h"
#include "monitoring.h"
#include "motion.h"
#include "shell.h"
#include "path.h"
#include "task_mgt.h"
#include "avoidance.h"
#include "beacons.h"
#include "strategy.h"
#include "debug.h"

 /**
 ********************************************************************************
 **
 **  The welcome message display at shell's startup
 **
 ********************************************************************************
 */

#define WELCOME_MESSAGE "\f"\
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
**  Applicative & High-Level Configuration
**
********************************************************************************
*/

/*
 * OS Tasks Priorities.
 * Higher value means higher priority
 */
#define OS_TASK_PRIORITY_SHELL        ( tskIDLE_PRIORITY + 1  )

#define OS_TASK_PRIORITY_LED          ( tskIDLE_PRIORITY + 2  )
#define OS_TASK_PRIORITY_MONITORING   ( tskIDLE_PRIORITY + 2  )
#define OS_TASK_PRIORITY_ASV          ( tskIDLE_PRIORITY + 2  )
#define OS_TASK_PRIORITY_DSV          ( tskIDLE_PRIORITY + 2  )
#define OS_TASK_PRIORITY_SYS_MODULES  ( tskIDLE_PRIORITY + 2  )

#define OS_TASK_PRIORITY_SEQUENCER    ( tskIDLE_PRIORITY + 3  )
#define OS_TASK_PRIORITY_AI_TASKS     ( tskIDLE_PRIORITY + 3  )
#define OS_TASK_PRIORITY_BEACONS      ( tskIDLE_PRIORITY + 3  )

#define OS_TASK_PRIORITY_MOTION_TRAJ  ( tskIDLE_PRIORITY + 4  )
#define OS_TASK_PRIORITY_AVS_TRAJ     ( tskIDLE_PRIORITY + 4  )

#define OS_TASK_PRIORITY_MOTION_CS    ( tskIDLE_PRIORITY + 5  )

#define OS_TASK_PRIORITY_AVOIDANCE    ( tskIDLE_PRIORITY + 6  )

/*
 * OS Tasks Stacks sizes, in bytes
 */
#define OS_TASK_STACK_SHELL             400
#define OS_TASK_STACK_LED               configMINIMAL_STACK_SIZE
#define OS_TASK_STACK_MONITORING        configMINIMAL_STACK_SIZE
#define OS_TASK_STACK_ASV               configMINIMAL_STACK_SIZE
#define OS_TASK_STACK_AVS_TRAJ          configMINIMAL_STACK_SIZE
//#define OS_TASK_STACK_DSV               200
#define OS_TASK_STACK_SEQUENCER         300
#define OS_TASK_STACK_MOTION_CS         300
#define OS_TASK_STACK_MOTION_TRAJ       200
#define OS_TASK_STACK_AI_TASKS          300
#define OS_TASK_STACK_AVOIDANCE         200
#define OS_TASK_STACK_BEACONS           configMINIMAL_STACK_SIZE
#define OS_TASK_STACK_SYS_MODULES       200

 /* NVIC Priorities. Lower value means higher priority.
  * Beware to use priorities smaller than configLIBRARY_LOWEST_INTERRUPT_PRIORITY
  * and higher than configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY when using
  * ISR Save FreeRTOS API Routines!
  */
#define OS_ISR_PRIORITY_SER             ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1 )
#define OS_ISR_PRIORITY_DSV             ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2 )

/*
 * Static durations
 */
#define OS_SHELL_BOOT_WAIT_MS            500U  // Time waited after shell is up before starting other inits

 /*
  * Events periodicity
  */
#define OS_MOTION_CONTROL_PERIOD_MS       50
#define OS_AVERSIVE_PERIOD_MS             50
#define OS_MONITORING_PERIOD_MS          100
#define OS_SEQUENCER_PERIOD_MS           100
#define OS_AI_TASKS_PERIOD_MS            100
#define OS_BEACONS_PERIOD_MS             100
#define OS_AVOIDANCE_PERIOD_MS            10
#define OS_SYS_MODULES_PERIOD_MS         100

/*
 * Software task 32 bits notifiers
 */

// Main sequencer notifiers
#define OS_NOTIFY_AVOIDANCE_EVT       0x00000001    // Avoidance event: must be assess quickly
#define OS_NOTIFY_AVOIDANCE_CLR       0x00000002    // Avoidance clear flag
// ...
#define OS_NOTIFY_INIT_START          0x00000100    // Software start of the initialization phase
#define OS_NOTIFY_MATCH_START         0x00000200    // Software start of the match notification
#define OS_NOTIFY_MATCH_PAUSE         0x00000400    // Software pause of the match (freeze everything)
#define OS_NOTIFY_MATCH_RESUME        0x00000800    // Software resume of the match (continues)
#define OS_NOTIFY_MATCH_ABORT         0x00001000    // Software abort of the match (clean end, no reset)

// Modules system notifiers
#define OS_NOTIFY_SYS_MOD_INIT        0x00000001    // Initialize the modules system
#define OS_NOTIFY_SYS_MOD_SELF_TEST   0x00000002    // Launch self-test procedure
#define OS_NOTIFY_SYS_MOD_FOLD        0x00000010    // Initiate procedure to fold state
#define OS_NOTIFY_SYS_MOD_GRAB        0x00000020    // Initiate procedure to grab state
#define OS_NOTIFY_SYS_MOD_LAND        0x00000040    // Initiate procedure to land state


/**
********************************************************************************
**
**  Useful constants & macros
**
********************************************************************************
*/

#ifndef PI
#define PI 3.141592653589
#endif

// Min, max and abs macros for 2 values
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// Min, Max and abs macros for 3 values
#define MAX3(x, y, z) (MAX(MAX(x, y), z))
#define MIN3(x, y, z) (MIN(MIN(x, y), z))

// Degrees to Radians and vice-versa
#define DEG_TO_RAD(a) (((double) (a)) * PI / 180.0)
#define RAD_TO_DEG(a) (((double) (a)) * 180.0 / PI)


/**
********************************************************************************
**
**  Prototypes
**
********************************************************************************
*/

// -----------------------------------------------------------------------------
// FreeRTOS
// -----------------------------------------------------------------------------

void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationTickHook( void );

BaseType_t sys_create_task(TaskFunction_t pxTaskCode,
                           const char * const pcName,
                           const uint16_t usStackDepth,
                           void * const pvParameters,
                           UBaseType_t uxPriority,
                           TaskHandle_t * const pxCreatedTask);

void sys_get_run_time_stats(char *pcWriteBuffer);

// -----------------------------------------------------------------------------
// Analog Servos
// -----------------------------------------------------------------------------

BaseType_t asv_start(void);

// -----------------------------------------------------------------------------
// Digital Servos
// -----------------------------------------------------------------------------

BaseType_t dsv_start(void);
void dsv_init(void);
void dsv_update_config(void);
uint8_t dsv_put(uint8_t chan_idx, uint8_t tx_data);
uint8_t dsv_get(uint8_t chan_idx, const uint8_t* data);
uint8_t dsv_flush(uint8_t chan_idx);
BaseType_t dsv_dump_servo(dxl_servo_t* servo, char* ret, size_t retLength);

// -----------------------------------------------------------------------------
// Monitoring
// -----------------------------------------------------------------------------

BaseType_t monitoring_start(void);

// -----------------------------------------------------------------------------
// Analog Servos
// -----------------------------------------------------------------------------

void asv_init_servo(asv_servo_t* servo, BB_ASV_ChannelTypeDef channel, uint16_t min_pos, uint16_t max_pos);

// -----------------------------------------------------------------------------
// Avoidance
// -----------------------------------------------------------------------------

void avoidance_start(void);
void avd_enable(void);
void avd_disable(void);
void avd_mask_all(bool value);
void avd_mask_front(bool value);
void avd_mask_back(bool value);
bool avd_detection_is_valid(void);

// -----------------------------------------------------------------------------
// Beacons
// -----------------------------------------------------------------------------

BaseType_t beacons_init(void);
BaseType_t beacons_start(void);
void beacons_write_reg(uint8_t add, int16_t data);
int16_t beacons_read_reg(uint8_t add);

// -----------------------------------------------------------------------------
// Motion Control System
// -----------------------------------------------------------------------------

BaseType_t motion_cs_start(void);
void motion_set_x(int16_t pos_x);
void motion_set_y(int16_t pos_y);
void motion_set_a(int16_t pos_a);
int16_t motion_get_x(void);
int16_t motion_get_y(void);
int16_t motion_get_a(void);
void motion_power_enable(void);
void motion_power_disable(void);
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

// -----------------------------------------------------------------------------
// Motion Trajectory
// -----------------------------------------------------------------------------

BaseType_t motion_traj_start(void);
bool motion_is_traj_near(void);
bool motion_is_traj_finished(void);

void motion_traj_hard_stop(void);

void motion_set_window(double window_d, double window_a, double a_start);
void motion_set_near_window(double window_d, double window_a);
void motion_set_speed(int16_t speed_d, int16_t speed_a);

void motion_traj_stop(void);

void motion_move_relative(double d_mm, double a_deg_rel);
void motion_goto_auto(double pos_x, double pos_y);
void motion_goto_forward(double pos_x, double pos_y);
void motion_goto_backward(double pos_x, double pos_y);
void motion_turnto_front(double pos_x, double pos_y);
void motion_turnto_behind(double pos_x, double pos_y);

void motion_clear_all_wp(void);
BaseType_t motion_add_new_wp(wp_t *waypoint);
bool motion_is_traj_done(wp_t *waypoint);
void motion_execute_wp(wp_t *waypoint);

// -----------------------------------------------------------------------------
// Sequencer
// -----------------------------------------------------------------------------

BaseType_t sequencer_start(void);
void sequencer_print_match(void);
const char* match_state_to_str(match_state_e state);
const char* match_color_to_str(match_color_e color);

// -----------------------------------------------------------------------------
// Task manager
// -----------------------------------------------------------------------------

void tasks_init(void);
bool task_is_valid(task_t* task);
task_t* task_get_next(void);
void task_add_dep(task_t* task, task_t* dep);
void task_print(task_t* task);
BaseType_t task_print_list(char* ret, size_t retLength);
void task_to_str(task_t* task, char* ret, size_t retLength);
const char* task_state_to_str(task_state_e state);
void task_remove_dep(task_t* task, task_t* dep);
void task_remove_dep_from_all(task_t* dep);
task_elt_t* task_new_elt(task_t* task);
void task_add_elt(task_t* task, task_elt_t* elt);

// -----------------------------------------------------------------------------
// Physical engine
// -----------------------------------------------------------------------------

void phys_init(void);
uint8_t phys_is_north_left(void);
void phys_update_color_pois(void);
void phys_update_color_polys(void);
void phys_update_with_color(poi_t* poi);
void phys_update_with_color_xya(int16_t* x, int16_t* y, int16_t* a);
void phys_update_with_color_xy(int16_t* x, int16_t* y);
void phys_update_with_color_poly(path_poly_t* poly);
void phys_apply_offset(poi_t* src, poi_t* dest, const poi_t* offset);
void phys_apply_polar_offset(int16_t* x, int16_t* y, int16_t d, int16_t a_deg);
void phys_set_obstacle_positions(void);
void phys_set_opponent_position(uint8_t robot_idx, int16_t x, int16_t y);

void phys_poly_to_str(path_poly_t* poly, uint8_t idx_poly, char* str, size_t len);
BaseType_t phys_print_pf_polys(char* ret, size_t len);

// -----------------------------------------------------------------------------
// AI
// -----------------------------------------------------------------------------

void ai_init(void);
void ai_stop(void);
void ai_manage(bool notified, uint32_t sw_notification);
void ai_tasks_def(void);
BaseType_t ai_task_launch(task_t* task);
void ai_compute_task_priorities(void);
void ai_on_suspend_policy(task_t* task);
void ai_on_failure_policy(task_t* task);

// AI tasks
void ai_task_start(void *params);

// -----------------------------------------------------------------------------
// Path-finder
// -----------------------------------------------------------------------------

// Initializer
void path_init(void);

// Inputs definitions
void path_set_objective(int32_t src_x, int32_t src_y, int32_t dst_x, int32_t dst_y);
path_poly_t* path_add_new_poly(uint8_t nb_points);
void path_poly_set_points(path_poly_t* poly, uint8_t idx, int32_t x, int32_t y);

// Core functions
void path_point_to_line(const path_proc_pt_t* p1, const path_proc_pt_t* p2, path_line_t* l);
path_line_cross_e path_interesect_line(const path_line_t* l1,
                                       const path_line_t* l2,
                                        path_proc_pt_t* p);
path_seg_cross_e path_intersect_segment(path_proc_pt_t* s1, path_proc_pt_t* s2,
                                        path_proc_pt_t* t1, path_proc_pt_t* t2,
                                        path_proc_pt_t* p);
path_pt_in_poly_e path_pt_is_in_poly(const path_proc_pt_t* p, const path_poly_t* poly);
path_seg_cross_poly_e path_seg_is_crossing_poly(path_proc_pt_t p1,
                                                path_proc_pt_t p2,
                                                const path_poly_t* poly);

// Main processing functions
uint16_t path_compute_rays(path_poly_t* polys, uint8_t n_polys, uint8_t* rays);
void path_compute_rays_weight(const path_poly_t* polys, const uint8_t* rays, uint16_t ray_n, uint16_t* weight);
void path_compute_dijkstra(uint8_t start_poly, uint8_t start_pt);
int8_t path_get_result(path_poly_t* polys, uint8_t* rays);
int8_t path_process(void);

// TODO: result

// -----------------------------------------------------------------------------
// Systems
// -----------------------------------------------------------------------------

BaseType_t sys_modules_start(void);

void sys_mod_do_init(TaskHandle_t* caller);
void sys_mod_do_self_test(TaskHandle_t* caller);
void sys_mod_do_grab(TaskHandle_t* caller, uint16_t grab_pos);
void sys_mod_do_land(TaskHandle_t* caller, uint16_t land_pos, uint16_t land_angle);
void sys_mod_do_fold(TaskHandle_t* caller);

// -----------------------------------------------------------------------------
// RGB LED
// -----------------------------------------------------------------------------

BaseType_t led_start(void);
void led_set_mode(BB_LED_ModeTypeDef mode);
void led_set_color(BB_LED_ColorTypeDef color);

// -----------------------------------------------------------------------------
// Serial Interface
// -----------------------------------------------------------------------------

BaseType_t serial_init(void);
BaseType_t serial_put(char ch);
BaseType_t serial_puts(const char* str);
BaseType_t serial_get(const char* str);
int serial_printf(const char * restrict format, ... );

// -----------------------------------------------------------------------------
// Shell
// -----------------------------------------------------------------------------

void shell_register_commands(void);
BaseType_t shell_start(void);

BaseType_t shell_sem_take(void);
void shell_sem_give(void);
void shell_print( const char * const pcMessage );

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
