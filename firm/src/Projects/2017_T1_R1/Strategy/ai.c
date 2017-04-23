/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       ai.c
 * @author     Bebop35
 * @date       2017/04/23
 * -----------------------------------------------------------------------------
 * @brief
 *   Artificial Intelligence main module
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

extern wp_t checkpoints[20]; // TEMP

/**
********************************************************************************
**
**  AI Management helpers
**
********************************************************************************
*/

BaseType_t ai_task_launch(task_t* task)
{
  BaseType_t ret;

  // Create the task, pass its own containing pointer
  ret = xTaskCreate(task->function,
                    task->name,
                    OS_TASK_STACK_AI_TASKS,
                    (void*) task,
                    OS_TASK_PRIORITY_AI_TASKS,
                    task->handle);

  if(ret != pdPASS)
  {
    DEBUG_CRITICAL("Could not start AI task %s"DEBUG_EOL, task->name);

  } else {
    DEBUG_INFO("Starting AI task %s"DEBUG_EOL, task->name);
  }

  return ret;
}


// Main holder for inits
void ai_task_def(void)
{

/*
  task_elt_t* elt;
  uint8_t task_id;

  // Initialize the Start task (entry point)
  // ---------------------------------------------------------------------------
  task_id = TASK_ID_START;
  tasks[task_id].id = task_id;
  tasks[task_id].value = TASK_INIT_VALUE_START;
  tasks[task_id].state = TASK_STATE_START; // unique! only to be used with this task

  // Grab the north spot-builder bulb and wait because
  // the secondary robot is still on the way
  elt = task_new_elt(&tasks[task_id]);
  elt->action = TASK_ACTUATOR;
  elt->params.act.flags = ACT_SEND_SPOTS_CMD;
  elt->params.act.spots.bull_bar = CAN_SPOTS_CMD_BULL_IN;
  elt->params.act.spots.north_builder = CAN_SPOTS_CMD_NONE;
  elt->params.act.spots.south_builder = CAN_SPOTS_CMD_GRAB_BULB;
  elt->timeout_ms = TASK_NO_TIMEOUT;
  elt->timer_ms = WAIT_BEFORE_START_MSEC;

  */
    checkpoints[STRAT_EXIT].coord.abs.x = phys.exit_start.x;
    checkpoints[STRAT_EXIT].coord.abs.y = phys.exit_start.y;
    checkpoints[STRAT_EXIT].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_EXIT].trajectory_must_finish=true;
    checkpoints[STRAT_EXIT].type=WP_GOTO_AUTO;

    checkpoints[STRAT_TURN_TO_CUBE1].coord.abs.x = phys.cube[PHYS_ID_CUBE_1].x;
    checkpoints[STRAT_TURN_TO_CUBE1].coord.abs.y = phys.cube[PHYS_ID_CUBE_1].y;
    checkpoints[STRAT_TURN_TO_CUBE1].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_TURN_TO_CUBE1].trajectory_must_finish = true;
    checkpoints[STRAT_TURN_TO_CUBE1].type = WP_ORIENT_FRONT;

    checkpoints[STRAT_PUSH_CUBE1].coord.abs.x = phys.drop.x;
    checkpoints[STRAT_PUSH_CUBE1].coord.abs.y = phys.drop.y;
    checkpoints[STRAT_PUSH_CUBE1].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_PUSH_CUBE1].trajectory_must_finish=true;
    checkpoints[STRAT_PUSH_CUBE1].type = WP_GOTO_AUTO;

    checkpoints[STRAT_RETURN_HUT_2].coord.abs.x = phys.huts[PHYS_ID_HUT_2].x;
    checkpoints[STRAT_RETURN_HUT_2].coord.abs.y = phys.cube[PHYS_ID_CUBE_1].y;
    checkpoints[STRAT_RETURN_HUT_2].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_RETURN_HUT_2].trajectory_must_finish=true;
    checkpoints[STRAT_RETURN_HUT_2].type=WP_GOTO_BWD;

    checkpoints[STRAT_TURN_TO_HUT_2].coord.abs.x = phys.huts[PHYS_ID_HUT_2].x;
    checkpoints[STRAT_TURN_TO_HUT_2].coord.abs.y = phys.huts[PHYS_ID_HUT_2].y;
    checkpoints[STRAT_TURN_TO_HUT_2].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_TURN_TO_HUT_2].trajectory_must_finish=true;
    checkpoints[STRAT_TURN_TO_HUT_2].type = WP_ORIENT_FRONT;

    checkpoints[STRAT_CLOSE_HUT_2].coord.abs.x = phys.huts[PHYS_ID_HUT_2].x;
    checkpoints[STRAT_CLOSE_HUT_2].coord.abs.y = phys.huts[PHYS_ID_HUT_2].y;
    checkpoints[STRAT_CLOSE_HUT_2].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_CLOSE_HUT_2].trajectory_must_finish=true;
    checkpoints[STRAT_CLOSE_HUT_2].type = WP_GOTO_FWD;

    checkpoints[STRAT_RETURN_HUT_1].coord.abs.x = phys.huts[PHYS_ID_HUT_1].x;
    checkpoints[STRAT_RETURN_HUT_1].coord.abs.y = 500;
    checkpoints[STRAT_RETURN_HUT_1].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_RETURN_HUT_1].trajectory_must_finish=true;
    checkpoints[STRAT_RETURN_HUT_1].type=WP_GOTO_BWD;

    checkpoints[STRAT_TURN_TO_HUT_1].coord.abs.x = phys.huts[PHYS_ID_HUT_1].x;
    checkpoints[STRAT_TURN_TO_HUT_1].coord.abs.y = phys.huts[PHYS_ID_HUT_1].y;
    checkpoints[STRAT_TURN_TO_HUT_1].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_TURN_TO_HUT_1].trajectory_must_finish=true;
    checkpoints[STRAT_TURN_TO_HUT_1].type = WP_ORIENT_FRONT;

    checkpoints[STRAT_CLOSE_HUT_1].coord.abs.x = phys.huts[PHYS_ID_HUT_1].x;
    checkpoints[STRAT_CLOSE_HUT_1].coord.abs.y = phys.huts[PHYS_ID_HUT_1].y;
    checkpoints[STRAT_CLOSE_HUT_1].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_CLOSE_HUT_1].trajectory_must_finish=true;
    checkpoints[STRAT_CLOSE_HUT_1].type=WP_GOTO_AUTO;

    checkpoints[STRAT_CLOSE_HUT_1].coord.abs.x = phys.huts[PHYS_ID_HUT_1].x;
    checkpoints[STRAT_CLOSE_HUT_1].coord.abs.y = phys.huts[PHYS_ID_HUT_1].y;
    checkpoints[STRAT_CLOSE_HUT_1].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_CLOSE_HUT_1].trajectory_must_finish=true;
    checkpoints[STRAT_CLOSE_HUT_1].type=WP_GOTO_AUTO;

    checkpoints[STRAT_PREPARE_TO_CUBE2].coord.abs.x = phys.huts[PHYS_ID_HUT_1].x;
    checkpoints[STRAT_PREPARE_TO_CUBE2].coord.abs.y = 500;
    checkpoints[STRAT_PREPARE_TO_CUBE2].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_PREPARE_TO_CUBE2].trajectory_must_finish=true;
    checkpoints[STRAT_PREPARE_TO_CUBE2].type=WP_GOTO_BWD;

    checkpoints[STRAT_TURN_TO_FACE_SHELLS].coord.abs.x = phys.shells[PHYS_ID_SHELL_SW].x;
    checkpoints[STRAT_TURN_TO_FACE_SHELLS].coord.abs.y = phys.shells[PHYS_ID_SHELL_SW].y;
    checkpoints[STRAT_TURN_TO_FACE_SHELLS].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_TURN_TO_FACE_SHELLS].trajectory_must_finish=true;
    checkpoints[STRAT_TURN_TO_FACE_SHELLS].type=WP_ORIENT_FRONT;

    checkpoints[STRAT_GOTO_SHELL_SW].coord.abs.x = phys.shells[PHYS_ID_SHELL_SW].x;
    checkpoints[STRAT_GOTO_SHELL_SW].coord.abs.y = phys.shells[PHYS_ID_SHELL_SW].y;
    checkpoints[STRAT_GOTO_SHELL_SW].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_GOTO_SHELL_SW].trajectory_must_finish=true;
    checkpoints[STRAT_GOTO_SHELL_SW].type=WP_GOTO_FWD;

    checkpoints[STRAT_GOTO_SHELL_SE].coord.abs.x = phys.shells[PHYS_ID_SHELL_SE].x;
    checkpoints[STRAT_GOTO_SHELL_SE].coord.abs.y = phys.shells[PHYS_ID_SHELL_SE].y;
    checkpoints[STRAT_GOTO_SHELL_SE].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_GOTO_SHELL_SE].trajectory_must_finish=true;
    checkpoints[STRAT_GOTO_SHELL_SE].type=WP_GOTO_FWD;

    checkpoints[STRAT_GOTO_SHELL_NE].coord.abs.x = phys.shells[PHYS_ID_SHELL_NE].x;
    checkpoints[STRAT_GOTO_SHELL_NE].coord.abs.y = phys.shells[PHYS_ID_SHELL_NE].y;
    checkpoints[STRAT_GOTO_SHELL_NE].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_GOTO_SHELL_NE].trajectory_must_finish=true;
    checkpoints[STRAT_GOTO_SHELL_NE].type=WP_GOTO_FWD;

    checkpoints[STRAT_GOTO_START].coord.abs.x = 300;
    checkpoints[STRAT_GOTO_START].coord.abs.y = TABLE_Y_MAX/2;
    checkpoints[STRAT_GOTO_START].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_GOTO_START].trajectory_must_finish=true;
    checkpoints[STRAT_GOTO_START].type=WP_GOTO_FWD;

}


// Compute a task priority.
// This function is one of the core of the decision algorithm.
// For the given task, it'll look up on other tasks achievement state in order
// to dynamically affect the importance of the task.
// The priority can also depends on other factors such as remaining time,
// opponent location and/or avoidance states.
void ai_compute_task_priorities(void)
{

  task_t* task;

  for(task = tasks; task < tasks + TASKS_NB; task++) {
    if(task_is_valid(task)) {

      // Check for near end of match actions to perform.
      // Basically we have time to do only one last thing.
      if(match.timer_msec > 80000) {

        // Check to see if there is at least one spot in each builder.
        // If so, remove the dependencies so the land task can operate immediatly.
        /*if(robot.nb_spot_south > 0) {
          task_remove_dep(&tasks[TASK_ID_SPOT_S_LAND], &tasks[TASK_ID_SPOT_S4]);
        }

        if(robot.nb_spot_north > 0) {
          task_remove_dep(&tasks[TASK_ID_SPOT_N_LAND], &tasks[TASK_ID_SPOT_N4]);
        }*/

        // Remove various dependencies
        //task_remove_dep(&tasks[TASK_ID_SPOT_N_LAND], &tasks[TASK_ID_POP_GET]);
        //task_remove_dep(&tasks[TASK_ID_CLAP_1], &tasks[TASK_ID_SPOT_S3]);

        // Finally define a super-strong priorities for important tasks
        /*switch(task->id) {

        case TASK_ID_SPOT_S_LAND:
        case TASK_ID_SPOT_N_LAND:
          task->priority = PRIORITY_MAX;
          break;

        case TASK_ID_POP_FLUSH:
          task->priority = PRIORITY_MAX-1;
          break;

        case TASK_ID_CUP_N_SET:
        case TASK_ID_CUP_S_SET:
        case TASK_ID_CUP_C_SET:
          task->priority = PRIORITY_MAX-2;
          break;

        case TASK_ID_CLAP_1:
        case TASK_ID_CLAP_2:
        case TASK_ID_CLAP_3:
          task->priority = PRIORITY_MAX-3;
          break;

        default:
          task->priority = task->value;
          break;
      }
*/
      // There is still some time...
      // Only affect the priority for tasks that have already tried a couple of
      // times to succeed (but get SUSPENDED)
      } else {
        task->priority = (task->value) /* (task->trials + 1)*/; // avoid dividing by 0
      }

    // Task is not valid
    } else {
      task->priority = PRIORITY_MIN;
    }

  }

}



// -----------------------------------------------------------------------------
// TASKS POLICIES
// -----------------------------------------------------------------------------

// Apply a specific policy for the task after it became suspended.
// We need to handle it and decide if we do something else or simply
// mark it as FAILED.
// Here we can do some actual A.I. For instance:
// - remove dependencies on a task when we know its not doable
// - increase the value of another task to balance the decision
// ...
void ai_on_suspend_policy(task_t* task)
{

  // Increment the number of trials
  task->trials++;

  // After 5 trials, a task is considered as failed.
  // Also, LANDING task cannot fail, because they value too much points.
  // (we will try to achieve them endlessly if needed).
  /*if((task->trials > 100) &&  // TBC
     (task->id != TASK_ID_SPOT_S_LAND) &&
     (task->id != TASK_ID_SPOT_N_LAND)) {
    task->state = TASK_STATE_FAILED;
    ai_on_failure_policy(task);
  }*/

  // Remove some dependencies immediatly when a task becomes SUSPENDED
  // This ensures a nice dynamic adaptation of the robot.
/*switch(task->id) {

    case TASK_ID_START:
      task_remove_dep(&tasks[TASK_ID_SPOT_S1], task);
      break;

    // No dependance
    case TASK_ID_SPOT_S1:
      break;

    // Woaw... opponent really fast! already in the S3/S4/Claps zone!
    // Leave it like that if we have not already processed the upper part, we
    // will try to come back to it later to do the north zone
    case TASK_ID_SPOT_S2:

      // Always allows to get immediately the S4 spot
      task_remove_dep(&tasks[TASK_ID_SPOT_S4], &tasks[TASK_ID_SPOT_S3]);

      // Allows to land only a dual spot light with S4
      // S2, S3 are considered lost
      // We can still try to do the claps, so we also remove dependencies
      if(tasks[TASK_ID_SPOT_N_LAND].state == TASK_STATE_SUCCESS) {
        task_remove_dep(&tasks[TASK_ID_SPOT_S_LAND], &tasks[TASK_ID_SPOT_S2]);
        task_remove_dep(&tasks[TASK_ID_SPOT_S_LAND], &tasks[TASK_ID_SPOT_S3]);
        task_remove_dep(&tasks[TASK_ID_CLAP_1], &tasks[TASK_ID_SPOT_S3]);
      }
      break;

    // This is unlikely, change nothing
    case TASK_ID_SPOT_S3:
      break;

    // TBC
    case TASK_ID_SPOT_S4:
      break;

    // TODO
    default:
      break;


  }
*/

  // Safely back actuators
  // TODO

}


// Dedicated policy for failed tasks. A failed task is a task that is considered
// lost, so we will never look back to it.
void ai_on_failure_policy(task_t* task)
{

  // Cleanup all dependencies of the other tasks
  task_remove_dep_from_all(task);

}

