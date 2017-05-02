/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       ai_tasks.c
 * @author     Bebop35
 * @date       2017/04/23
 * -----------------------------------------------------------------------------
 * @brief
 *   Contains the AI tasks (AI sub-routines)
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "main.h"

/**
********************************************************************************
**
**  Globals
**
********************************************************************************
*/

// External definitions
extern task_t tasks[TASKS_NB];
extern match_t match;
extern robot_t robot;
extern phys_t phys;
extern path_t pf;

/**
********************************************************************************
**
**  Common and useful functions for the tasks
**    Warning: This functions must absolutely be reentrant!!
**
********************************************************************************
*/

// TBD

/**
********************************************************************************
**
**  AI Tasks
**
********************************************************************************
*/

// -----------------------------------------------------------------------------
// IDLE task
//   Description: This task must be left and is used when there is nothing to do.
//                Also, it does nothing, but waiting!
//                It is always running.
// -----------------------------------------------------------------------------

void ai_task_idle(void *params)
{
  // A pointer to corresponding task-structure (self) is passed
  task_t* self = (task_t*) params;

  // Tick timer
  TickType_t new_wake_time = xTaskGetTickCount();

  for(;;)
  {

    // Block until next iteration
    vTaskDelayUntil( &new_wake_time, pdMS_TO_TICKS(OS_AI_TASKS_PERIOD_MS));
  }

}

// -----------------------------------------------------------------------------
// START task
//   Description: called once at the beginning of the match, exit the starting
//                area and do nothing more.
// -----------------------------------------------------------------------------


void ai_task_start(void *params)
{
  // A pointer to corresponding task-structure (self) is passed
  task_t* self = (task_t*) params;

  // Tick timer
  TickType_t new_wake_time = xTaskGetTickCount();

  for(;;)
  {

    // Test
    if(match.timer_msec >= 10000) {
      self->state = TASK_STATE_SUCCESS;
    }

    // Block until next iteration
    vTaskDelayUntil( &new_wake_time, pdMS_TO_TICKS(OS_AI_TASKS_PERIOD_MS));
  }

}

// GET_ORES_SMALL_1A
// GET_ORES_SMALL_1B
// GET_ORES_BIG_1

// THROW_ORES

// GET_MODULES_MONO_SHIP
// GET_MODULES_MONO_TABLE_A
// GET_MODULES_MONO_TABLE_B
// GET_MODULES_MONO_TABLE_C


// BUILD_MODULES_SIDE
// BUILD_MODULES_CENTRAL

// -----------------------------------------------------------------------------
// FUNNY task
//   Description: called once at the end, triggers the funny-action
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// FUNNY task
//   Description: called once at the end, triggers the funny-action
// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
// WON'T DO List

// GET_MODULES_POLY_SHIP
// GET_MODULES_POLY_TABLE_A/B/C

// GET_ORES_SMALL_2A
// GET_ORES_SMALL_2B
// GET_ORES_BIG_2
