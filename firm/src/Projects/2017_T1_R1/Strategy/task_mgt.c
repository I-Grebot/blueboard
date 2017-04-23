/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       task_mgt.c
 * @author     Paul
 * @date       2015/04/05
 * -----------------------------------------------------------------------------
 * @brief
 *   Tasks management module
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "main.h"

// -----------------------------------------------------------------------------
// GLOBALS
// -----------------------------------------------------------------------------

// Container for all the tasks
task_t tasks[TASKS_NB];

// External definitions
extern robot_t robot;
extern phys_t phys;
extern path_t pf;

// -----------------------------------------------------------------------------
// TASKS INITIALIZER
// -----------------------------------------------------------------------------

// Initialize all the available tasks
void tasks_init(void)
{
  uint8_t id;

  // Initialize default values for each task
  for(id = 0; id < TASKS_NB; id++) {
    tasks[id].id = id;
    tasks[id].handle = NULL;
    tasks[id].function = NULL;
    tasks[id].priority = TASK_PRIORITY_DEFAULT;
    tasks[id].value = 0;
    tasks[id].state = TASK_STATE_INACTIVE;
    tasks[id].nb_dependencies = 0;
    tasks[id].trials = 0;
  }

  // Initialize the Idle task, special task: do not remove
  id = TASK_ID_IDLE;
  tasks[id].name = "AI_IDLE";
  tasks[id].function = ai_task_idle;
  tasks[id].value = TASK_INIT_VALUE_IDLE;

  // Define all other tasks
  ai_tasks_def();

}

// -----------------------------------------------------------------------------
// TASKS HANDLERS
// -----------------------------------------------------------------------------

// Returns false if the task cannot be executed as a next task i,e:
// - It was already executed (failed or success)
// - At least one of the dependency is not done
// Even if a dependency failed, it will be considered as "done" so the given task
// can be performed. The main strategy handler is responsible for affecting
// correct priorities if the task then become unrelevant.
bool task_is_valid(task_t* task)
{

  uint8_t idx;

  // Task that were already executed are not available
  if((task->state == TASK_STATE_FAILED) ||
     (task->state == TASK_STATE_SUCCESS)) {
    return false;
  }

  // Go through all dependencies
  for(idx = 0; idx < task->nb_dependencies; idx++) {

    // We found at least one dependency which is inactive
    if(task->dependencies[idx]->state == TASK_STATE_INACTIVE) {
      return false;
    }
  }

  // All dependencies were done, the task is valid
  return true;
}

// Find the next task to do. This function should be called once the current
// task is finished (or ended up badly), so we know what to do next.
// This function will process the remaining task list in the following way:
// - Filter out already done tasks
// - Filter out tasks which do not have all dependencies satisfyied
// - Compute the priority value for the remaining tasks
// - Select the task with the highest priority
task_t* task_get_next(void) {

  task_t* task;
  uint16_t max_priority;
  task_t* next_task;

  max_priority = 0;

  // Next task is initialized to the "idle" task, just in case nothing is found.
  next_task = &tasks[TASK_ID_IDLE];

  // Compute the task priorities
  ai_compute_task_priorities();

  //TODO: filter instead
  for(task = tasks; task < tasks + TASKS_NB; task++) {

    if(task_is_valid(task)) {
      //task_compute_priority(task);

      // Task has a higher priority than anything found before,
      // remmember this task
      if(task->priority > max_priority) {
        max_priority = task->priority;
        next_task = task;
      }

    }

  } // for

  return next_task;

}


// Add a dependency to the task's array
void task_add_dep(task_t* task, task_t* dep) {

  // Cannot add more!
  if(task->nb_dependencies >= TASK_MAX_DEPENDENCIES) {
    return;
  }

  // Add the new dependency to the end of the array
  // Also increment the number of dependencies
  task->dependencies[task->nb_dependencies++] = dep;

}

// Remove a specific dependency from a task
void task_remove_dep(task_t* task, task_t* dep) {

  uint8_t idx;
  uint8_t found;

  // List is empty!
  if(task->nb_dependencies == 0) {
    return;
  }

  // Look for the dependency
  found = 0;
  for(idx = 0; idx < task->nb_dependencies; idx++) {

    // We've found it!
    if(task->dependencies[idx] == dep) {
      found = 1;
      task->nb_dependencies--;
    }

    // Resize the array from the previous location of the old dependency
    // If this was the last element, there is no need for resizing
    if(found && (idx < task->nb_dependencies)) {
      task->dependencies[idx] = task->dependencies[idx+1];
    }

  }

}

// Remove a dependency from all the tasks
void task_remove_dep_from_all(task_t* dep) {

  uint8_t idx;

  // Go through all existing tasks
  for(idx = 0; idx < TASKS_NB; idx++) {
    task_remove_dep(&tasks[idx], dep);
  }

}

// -----------------------------------------------------------------------------
// TASKS ELEMENTS MANIPULATION
// -----------------------------------------------------------------------------

// Create a new default element and attach it to the task
/*task_elt_t* task_new_elt(task_t* task) {
  task_elt_t* elt = malloc(sizeof(task_elt_t));
  //elt->launched = false;
  elt->timeout_ms = TASK_NO_TIMEOUT;
  elt->timer_ms = TASK_DEFAULT_TIMER_MS;
  elt->next = NULL;
  task_add_elt(task, elt);
  return elt;
}*/

// Add a task element to the given task's list
/*void task_add_elt(task_t* task, task_elt_t* elt) {

  // This is the first element, so it's now both start and end!
  if(task->nb_elt == 0) {
    task->first_elt = elt;
    task->last_elt = elt;

  // Otherwise we need to find the last element and define its next element
  // Then replace the task's last element pointer
  } else {
    task->last_elt->next = elt;
    task->last_elt = elt;
  }

  // Remember that we add a new element
  task->nb_elt++;

  // Anyway the element is added and we save its parent
  elt->parent = task;

}
*/

// -----------------------------------------------------------------------------
// TASKS MAIN MANAGER
// -----------------------------------------------------------------------------

// Manage the given task.
// Handles transitions of the task elements:
// - Check for timeout
// - Handle timer
// - Update task status
// - Send appropriate commands
/*
void do_task(task_t* task) {

  int8_t pf_nb_checkpoints;
  int8_t idx;
  wp_t checkpoint_wp;
  static uint32_t timeout_ms = TASK_NO_TIMEOUT;
  static uint32_t timer_ms = 0;

  switch(task->state) {

    // Very first execution of the task
    case TASK_STATE_START:
      if(task->nb_elt > 0) {
        task->current_elt = task->first_elt;
        timeout_ms = task->current_elt->timeout_ms;
        timer_ms = task->current_elt->timer_ms;
        task->current_elt->launched = false;
        task->state = TASK_STATE_RUNNING;

      // There is nothing to do!
      } else {
        task->state = TASK_STATE_SUCCESS;
      }

      break;

    // Chaining execution of the elements
    case TASK_STATE_RUNNING:

      // Handle timeout, moved to failed if it is reached
      if(timeout_ms != TASK_NO_TIMEOUT) {
        if(timeout_ms > 0) {
          timeout_ms -= STRATEGY_PERIOD_MS;
        } else {
          task->state = TASK_STATE_FAILED;
        }
      } // timeout

      // Handle timer
      if(timer_ms > 0) {
        timer_ms -= STRATEGY_PERIOD_MS;
      } // timer_ms

      // Current task element was not launched, so we proceed
      if(!task->current_elt->launched) {

        switch(task->current_elt->action) {

          // We need to send an actuator order
          case TASK_ACTUATOR:
            //actuators_send_cmd(&(task->current_elt->params.act));
            break;

          // We need to add a new WP. Depending on the path-finding flag, this
          // could lead to more than 1 point.
          case TASK_MOTION:

            if((task->current_elt->params.mot.use_pf) &&
               ((task->current_elt->params.mot.dest.type == WP_GOTO_AUTO) ||
                (task->current_elt->params.mot.dest.type == WP_GOTO_FWD)  ||
                (task->current_elt->params.mot.dest.type == WP_GOTO_BWD))) {

              // Setup the path-finder objective. Starting point is the current
              // robot location.
              path_set_objective(robot.pos.x, robot.pos.y,
                                 task->current_elt->params.mot.dest.coord.abs.x,
                                 task->current_elt->params.mot.dest.coord.abs.y);


              pf_nb_checkpoints = path_process();

              // A solution was found
              if(pf_nb_checkpoints != PATH_RESULT_ERROR) {

                // Add the list of checkpoints to the motion controller's FIFO.
                // In this list, the starting point is not present but the
                // destination point is.
                for(idx = 0; idx < pf_nb_checkpoints; idx++) {

                  // For the last point use the desired offset & stop
                  // Also, we use the original destination point as we want mm precision
                  if(idx == pf_nb_checkpoints - 1) {
                    checkpoint_wp.coord.abs = task->current_elt->params.mot.dest.coord.abs;
                    checkpoint_wp.offset = task->current_elt->params.mot.dest.offset;
                    checkpoint_wp.trajectory_must_finish = task->current_elt->params.mot.dest.trajectory_must_finish;

                  // For intermediate points:
                  // - Offset is not taken into account
                  // - No need to stop between points
                  } else {
                    checkpoint_wp.coord.abs = pf.u.res[idx];
                    checkpoint_wp.offset = phys.offset_center;
                    checkpoint_wp.trajectory_must_finish = false;
                  }

                  // Copy checkpoint speed and motion type for each checkpoint
                  checkpoint_wp.speed = task->current_elt->params.mot.dest.speed;
                  checkpoint_wp.type = task->current_elt->params.mot.dest.type;

                  // Finally add the checkpoint to the list.
                  // The wp element is copied
                  motion_add_wp(&checkpoint_wp);

                } // for all checkpoints

              // No possible path, mark the task as suspended and increment the
              // trials counter
              } else {
                task->state = TASK_STATE_SUSPENDED;
                task->trials++;
              }

            // No path-finding, or the motion type is not appropriate
            } else {
              motion_add_wp(&(task->current_elt->params.mot.dest));
            }

          case TASK_WAIT:
            // Do nothing but waiting.
            break;
        }

        // Flag it
        task->current_elt->launched = true;

      // Check for the end of the launch
      // Timer value needs to be cleared (might be unused)
      } else if(timer_ms == 0) {

        // For the actuators, we do not wait for feedbacks
        // (only the timer can be used to space/delay actions)
        // For the motions we need to ensure that the trajectory is
        // finished before moving to the next task element
        if(  (task->current_elt->action == TASK_ACTUATOR)
          || (task->current_elt->action == TASK_WAIT)
          || ((task->current_elt->action == TASK_MOTION) && motion_is_finished())) {

          task->nb_elt_done++;

          // Move to next element
          // Also copy the variables so we don't affect the element (useful for re-run)
          if(task->current_elt != task->last_elt) {
            task->current_elt = task->current_elt->next;
            timeout_ms = task->current_elt->timeout_ms;
            timer_ms = task->current_elt->timer_ms;
            task->current_elt->launched = false;

          // Finished to process the task!
          } else {
            task->state = TASK_STATE_SUCCESS;
          }
        } // task-specific condition

      } // timer is finished

      break;

    // Cases where there is nothing to do or that cannot occur
    default:
    case TASK_STATE_SUSPENDED:
    case TASK_STATE_INACTIVE:
    case TASK_STATE_FAILED:
    case TASK_STATE_SUCCESS:
      break;

  } // switch task state

}

*/


