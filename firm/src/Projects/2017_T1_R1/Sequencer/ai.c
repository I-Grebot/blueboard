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
extern task_mgt_t task_mgt;
extern match_t match;
extern robot_t robot;
extern phys_t phys;
extern path_t pf;
extern av_t av;


extern TaskHandle_t handle_task_avoidance;

/**
********************************************************************************
**
**  AI Management functions
**
********************************************************************************
*/

// AI Initialization, called at the beginning of the match
// Color selection is already done and stored in the match structure.
void ai_init(void)
{

  // Define static obstacles positions
  phys_set_obstacle_positions();

  // Opponent position: probably in starting zone but we don't know
  robot.opp_pos.x = OPPONENT_POS_INIT_X;
  robot.opp_pos.y = OPPONENT_POS_INIT_Y;
  robot.opp_pos.a = 0;

  // Update POIs and Path-Finder polygons depending on color
  phys_update_color_pois();
  phys_update_color_polys();

  // Initialize robot position with correct color
  motion_set_x(phys.reset.x);
  motion_set_y(phys.reset.y);
  motion_set_a(phys.reset.a);

  // Enable avoidance
  avd_enable();

}

// Called at the end of the match
void ai_stop(void)
{
  avd_disable();
  motion_traj_hard_stop();
  motion_power_disable();
  bb_power_down();
}

// Main manager, called from the sequencer during match execution
void ai_manage(bool notified, uint32_t sw_notification)
{

  // Manage task notifications
  if(notified)
  {
    // Match management
    // -----------------------------------------------
    if(sw_notification & OS_NOTIFY_MATCH_PAUSE)
    {
      match.paused = true;
      led_set_mode(BB_LED_STATIC);
    }

    else if(sw_notification & OS_NOTIFY_MATCH_RESUME)
    {
      match.paused = false;
      led_set_mode(BB_LED_BLINK_SLOW);
    }

    // Hardware events
    // -----------------------------------------------

    // An avoidance event has occurred
    if(sw_notification & OS_NOTIFY_AVOIDANCE_EVT)
    {
      // Check the avoidance state
      // After a detection occurs, the strategy needs to handle the following things:
      // - Switch the current task to the SUSPENDED state
      // - Increase the task's trials count.
      // - Apply the A.I. "on_suspend" policy, which can contain failure conditions
      //   or other dependencies freeing conditions
      //if(av.state == AV_STATE_DETECT) {
      //  task_mgt.active_task->state = TASK_STATE_SUSPENDED;
      //}

      DEBUG_INFO("AI mgt event handle = %X!"DEBUG_EOL, (uint32_t) task_mgt.active_task->handle);
      task_print(task_mgt.active_task);

    }

    // Notify the current AI task with the same notifications (forward)
    xTaskNotify(task_mgt.active_task->handle, sw_notification, eSetBits);

  }

  // Clear avoidance state, so another detection can be triggered
  /*if(av.state == AV_STATE_REROUTE)
  {
    xTaskNotify(handle_task_avoidance, OS_NOTIFY_AVOIDANCE_CLR, eSetBits);
  }*/

  // Check to see if active task is valid
  if(task_mgt.active_task != NULL)
  {

    // Depending on the state of the task we need to do different things
    switch(task_mgt.active_task->state)
    {

      // On-going task, do nothing except for idle task
      case TASK_STATE_RUNNING:
        break;

      // Apply the dedicated policies
      case TASK_STATE_SUSPENDED:
        ai_on_suspend_policy(task_mgt.active_task);
        break;

      case TASK_STATE_FAILED:
        ai_on_failure_policy(task_mgt.active_task);
        break;

      // Everything went fine, no specific policy to be applied,
      case TASK_STATE_SUCCESS:
        task_print(task_mgt.active_task);
        DEBUG_INFO("[TASK] Removing task %s (SUCCESS)"DEBUG_EOL, task_mgt.active_task->name);
        vTaskDelete(task_mgt.active_task->handle);
        break;

      // Task is not active, we simply need to start it
        // TODO:  temporary -- to be checked
      case TASK_STATE_INACTIVE:
        ai_task_launch(task_mgt.active_task);
        task_print(task_mgt.active_task);
        DEBUG_INFO("[TASK] Launching new task %s"DEBUG_EOL, task_mgt.active_task->name);
        break;

      default:
        break;

    } // switch active task

    // States after which we need to find a new task and start it
    if((task_mgt.active_task->state == TASK_STATE_SUSPENDED) ||
       (task_mgt.active_task->state == TASK_STATE_SUCCESS)   ||
       (task_mgt.active_task->state == TASK_STATE_FAILED))
    {

      // Retrieve a new task. This function can have mainly 3 different issues:
      // - Next task is INACTIVE (fresh), simply start execution
      // - Next task was SUSPENDED, we need to clean up before starting it again
      // - No correct task was found and the IDLE task is returned (do nothing special)
      task_mgt.active_task = task_get_next();

      // Check to see if found task is valid
      if(task_mgt.active_task != NULL)
      {

        // Cleanup flags & previous execution traces
        if(task_mgt.active_task->state == TASK_STATE_SUSPENDED) {
          // TBD
        }

        // Start FreeRTOS task, state will also change to RUNNING
        ai_task_launch(task_mgt.active_task);
        task_print(task_mgt.active_task);
        DEBUG_INFO("[TASK] Launching new task %s"DEBUG_EOL, task_mgt.active_task->name);

      // Idle
      } else {

        // Simply ensure that we stop
        motion_traj_stop();

      }

    }

  // Active task is not valid
  } else {

    // Launch a new task if we can find one that is OK
    // (will be managed at next ai_manage() call)
    task_mgt.active_task = task_get_next();

  }

} // ai_manage()


// Handles the start or restart of a task
BaseType_t ai_task_launch(task_t* task)
{
  BaseType_t ret;

  switch(task->state)
  {
    // Cases considered for starting (again) the task
    case TASK_STATE_INACTIVE:
    case TASK_STATE_SUSPENDED:
    case TASK_STATE_FAILED:

      // Create the task, pass its own containing pointer
      ret = sys_create_task(task->function,
                            task->name,
                            OS_TASK_STACK_AI_TASKS,
                            (void*) task,
                            OS_TASK_PRIORITY_AI_TASKS,
                            &task->handle);

      if(ret == pdPASS)
      {
        task->trials++;
        task->state = TASK_STATE_RUNNING;
      }

      break;

    // If the task is already running or finished,
    // prevent a new start
    case TASK_STATE_RUNNING:
    case TASK_STATE_SUCCESS:
      DEBUG_ERROR("AI task %s already running or finished!"DEBUG_EOL, task->name);
      ret = pdFAIL;
      break;
  }

  return ret;
}

/**
********************************************************************************
**
**  AI Tasks definitions
**
********************************************************************************
*/

// Define all the tasks
void ai_tasks_def(void)
{
  uint8_t id;

  // START Task
  id = TASK_ID_START;
  snprintf(tasks[id].name, configMAX_TASK_NAME_LEN, "AI_START");
  tasks[id].function = ai_task_start;
  tasks[id].value = TASK_INIT_VALUE_START;

}

/**
********************************************************************************
**
**  AI Tasks Priorities
**
********************************************************************************
*/

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
      task->priority = TASK_PRIORITY_MIN;
    }

  }

}

/**
********************************************************************************
**
**  AI Tasks Policies
**
********************************************************************************
*/

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





