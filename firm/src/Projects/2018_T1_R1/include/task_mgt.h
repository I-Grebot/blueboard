/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       task_mgt.h
 * @author     Paul
 * @date       2017/04/23
 * -----------------------------------------------------------------------------
 * @brief
 *   Definitions for the task management
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#ifndef _TASK_MGT_H_
#define _TASK_MGT_H_

#include <stdint.h>

// FreeRTOS Task module
#include "FreeRTOS.h"
#include "task.h"

#include "../../2018_T1_R1/include/motion.h"

/**
********************************************************************************
**
**  Constants
**
********************************************************************************
*/

// IDs of the different tasks
// --------------------------

// Special tasks
#define TASK_ID_SWITCH_PUSH			0   // Go to push automatique switch
#define TASK_ID_LAUCH_BEE	  		1   // Go to kick ass the bee
#define TASK_ID_EMPTY_OUR_WATER 	2   // Go to empty our water recuperator
#define TASK_ID_EMPTY_MIXED_WATER 	3   // Go to empty mixed water recuperator
#define TASK_ID_RECOVER_CUBES		4   // Go to get some cubes

// Total Number of tasks
#define TASKS_NB                 	4   // Must equal exactly the number of tasks!

// Initial values of the different tasks
// -------------------------------------

#define TASK_INIT_VALUE_START   50000

// Priorities
#define TASK_PRIORITY_MIN        0x00
#define TASK_PRIORITY_DEFAULT    0x10
#define TASK_PRIORITY_MAX        0xFF

// Maximum amount of dependencies that a given task can have.
// Dependences are inherited from dependant tasks.
// For example:
// Task A (TA) can be executed freely
// Task B (TB) requires that TA is finished
// Task C (TC) requires that TB is finished
// Then TC also depends on TA, so the dependencies array size for this example
// is 0 for TA, 1 for TB and 1 for TC.
#define TASK_MAX_DEPENDENCIES   8

// Value used when we don't want to set a timeout for a task element
#define TASK_NO_TIMEOUT 0xFFFF

// Default timer value
#define TASK_DEFAULT_TIMER_MS   1000


/**
********************************************************************************
**
**  Types & Enumerations
**
********************************************************************************
*/

// Define a task structure:
// A task is a sequential chain of motion and/or action. These elements
// are stored in a task element structure and are organized in a chain pointer
// fashion.

typedef struct task     task_t;
typedef struct task_elt task_elt_t; // Forward declaration

/* Task state enumeration.
 * This is not related to FreeRTOS task system, as an AI task
 * exist even when the FreeRTOS task is not created.
 * Instead, FreeRTOS task exists in "RUNNING" or "SUSPENDED" states.
 */
typedef enum
{
  TASK_STATE_INACTIVE  = 0xFF, // Task is not active
  TASK_STATE_RUNNING   = 0x10, // Task is running, elements are being executed
  TASK_STATE_SUSPENDED = 0x20, // The last task element could not finished
  TASK_STATE_FAILED    = 0x21, // The task stopped due to an error
  TASK_STATE_SUCCESS   = 0x30  // Task finished with no error
} task_state_e;

struct task
{
  uint8_t id;            // Unique ID of the task (value affected from TASK_ID_xxx)
  char name[configMAX_TASK_NAME_LEN];      // Name of the task (unique)

  task_state_e state;       // Current state of the task
  TaskFunction_t function;  // FreeRTOS Task function
  TaskHandle_t handle;      // FreeRTOS Task handle

  uint8_t nb_dependencies;                        // Number of dependencies
  task_t* dependencies[TASK_MAX_DEPENDENCIES];    // Array of pointers on dependencies
  uint16_t value;     // Value given to a task so it'll affect its priority
  uint8_t priority;  // Task priority calculated after the current task is finished
  uint8_t trials;     // Counter to measure the number of attempts for a task

  // TBC: probablye not useful anymore
  /*uint8_t nb_elt;        // Number of elements in the chain
  uint8_t nb_elt_done;   // Number of elements already performed
  task_elt_t* first_elt; // Pointer to the 1st element in the chain
  task_elt_t* last_elt;  // Pointer to the last element in the chain
  task_elt_t* current_elt;    // Current task element being executed
  */

};

/*
 * Task management structure: used to store all necessary informations for
 * the tasks execution handling. Only one object is used and managed by the
 * top-level strategy and AI algorithms.
 */

typedef struct {

  // Current task being executed
  // This must be valid. In case nothing interesting is performed, this
  // points on the IDLE task
  task_t* active_task;



} task_mgt_t;


// Defines the various actions that a task element can launch
typedef enum {
  TASK_MOTION,
  TASK_ACTUATOR,
  TASK_WAIT       // A dummy kind of task which can be used to wait
} task_action_e;

typedef struct {

  // Flags containing the actuators to be executed
  uint8_t flags;

  // Commands for spots
  struct {
    uint8_t north_builder;
    uint8_t south_builder;
    uint8_t bull_bar;
  } spots;

  // Commands for pop-corn
  struct {
    uint8_t north_pop;
    uint8_t south_pop;
  } pops;

  // Commands for claps
  struct {
    uint8_t north_clap;
    uint8_t south_clap;
  } claps;

  // Commands for cups
  struct {
    uint8_t north_cup;
    uint8_t south_cup;
  } cups;

} act_t;

// Define a task element structure.
struct task_elt {

  // Parent of the task element
  task_t* parent;

  // Next task element in the chain
  task_elt_t* next;

  // Type of action to do for this element
  task_action_e action;

  // Parameters for the action to be executed
  union {
    struct {
      wp_t dest;
      bool use_pf;
    } mot;
    act_t act;
  } params ;

  // Timer: used for handling task elements transition.
  // After an action is launched, the timer will be started
  // When it reaches 0, then the task element is considered as done.
  uint32_t timer_ms;

  // Timeout will lead to task's state = FAIL if reached
  // Set to TASK_NO_TIMEOUT for no timeout
  uint32_t timeout_ms;


  bool launched; // TODO: check
};

#endif // _TASK_MGT_H_
