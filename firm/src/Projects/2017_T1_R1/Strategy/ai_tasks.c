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
**  AI Tasks
**
********************************************************************************
*/

// -----------------------------------------------------------------------------
// Start task
//   Description: called once at the beginning of the match, exit the starting
//                area and do nothing more.
// -----------------------------------------------------------------------------


void ai_task_start(void *params)
{

  task_t* self;

  // A pointer to corresponding task-structure (self) is passed
  self = (task_t*) params;

  for(;;)
  {

  }

}


