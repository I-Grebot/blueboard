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

#include "../../2018_T1_R1/include/main.h"

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
extern TaskHandle_t handle_task_avoidance;

/**
********************************************************************************
**
**  Common and useful functions for the tasks
**    Warning: This functions must absolutely be reentrant!!
**
********************************************************************************
*/

// Simple motion:
// - Launch the waypoint
// - Hardstop if an avoidance event is received
// - Wait 3 seconds
// - Clear avoidance event
// - Restart
// - Loop until waypoint is reached
void motion_move_block_on_avd(wp_t* wp)
{
  TickType_t new_wake_time = xTaskGetTickCount();

  // OS Software notifier
  BaseType_t notified;
  uint32_t sw_notification;

  phys_update_with_color_xy(&wp->coord.abs.x, &wp->coord.abs.y);
  motion_add_new_wp(wp);

  while(!motion_is_traj_done(wp))
  {
    // Wait for potential new notification, this will unblock upon notification RX
    notified = xTaskNotifyWait(0, UINT32_MAX, &sw_notification, pdMS_TO_TICKS(OS_AI_TASKS_PERIOD_MS));

    // Check to see for avoidance event
    // This is normally handled by the ai_manage() function
    if(notified && (sw_notification & OS_NOTIFY_AVOIDANCE_EVT))
    {
      DEBUG_INFO("Avoidance event!"DEBUG_EOL);

      // Stop and wait 3 seconds
      motion_traj_hard_stop();
      vTaskDelayUntil( &new_wake_time, pdMS_TO_TICKS(3000));

      // Clear avoidance state
      xTaskNotify(handle_task_avoidance, OS_NOTIFY_AVOIDANCE_CLR, eSetBits);

      // Re-go
      motion_add_new_wp(wp);
    }
  }

}

BaseType_t ai_move_with_pf(wp_t* wp)
{

  uint8_t nb_checkpoints;
  uint8_t idx_checkpoint;

  wp_t dest_wp;
  wp_t checkpoint_wp;

  // Copy the destination waypoint so it can be re-used and we don't apply
  // a second time the coordinate transform and we can modify it locally
  // without consequences.
  dest_wp = *wp;

  DEBUG_INFO("Move AI with PF"DEBUG_EOL);

  // Start
  phys_update_with_color_xy(&dest_wp.coord.abs.x, &dest_wp.coord.abs.y);


  path_set_objective(robot.cs.pos.pos_s16.x, robot.cs.pos.pos_s16.y, // Origin
                     dest_wp.coord.abs.x, dest_wp.coord.abs.y);      // Destination

  nb_checkpoints = path_process();

  if(nb_checkpoints == PATH_RESULT_ERROR)
  {
    DEBUG_ERROR("[AI] Error while computing path!"DEBUG_EOL);
    return pdFAIL;
  }

  // Beginning of path display, also add current robot location
  DEBUG_INFO_NOPFX("[PHYS] [PATH] %d;%d ",
      robot.cs.pos.pos_s16.x,
      robot.cs.pos.pos_s16.y);

  // Do actual motion sequence
  for(idx_checkpoint = 0; idx_checkpoint < nb_checkpoints; idx_checkpoint++)
  {
    // For the last point use the desired offset & stop
    // Also, we use the original destination point as we want mm precision
    if(idx_checkpoint == nb_checkpoints - 1)
    {
      checkpoint_wp.coord.abs = dest_wp.coord.abs;
      checkpoint_wp.offset    = dest_wp.offset;
      checkpoint_wp.trajectory_must_finish = dest_wp.trajectory_must_finish;

    // For intermediate points:
    // - Offset is not taken into account
    // - No need to stop between points
    } else {
      checkpoint_wp.coord.abs = pf.u.res[idx_checkpoint];
      checkpoint_wp.offset = phys.offset_center;
      checkpoint_wp.trajectory_must_finish = false;
    }

    // Copy checkpoint speed and motion type for each checkpoint
    checkpoint_wp.speed = dest_wp.speed;
    checkpoint_wp.type = dest_wp.type;

    // Finally add the checkpoint to the list.
    // The waypoint is copied
    motion_add_new_wp(&checkpoint_wp);

    // Print
    DEBUG_INFO_NOPFX("%d;%d ",
                     checkpoint_wp.coord.abs.x,
                     checkpoint_wp.coord.abs.y)
  }

  DEBUG_INFO_NOPFX(DEBUG_EOL);

  return pdPASS;
}

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

/*void ai_task_idle(void *params)
{
  // A pointer to corresponding task-structure (self) is passed
  task_t* self = (task_t*) params;

  // Tick timer
  TickType_t new_wake_time = xTaskGetTickCount();

  for(;;)
  {

    //DEBUG_INFO("Alive!"DEBUG_EOL);
    // Block until next iteration
    vTaskDelayUntil( &new_wake_time, pdMS_TO_TICKS(1000)); // OS_AI_TASKS_PERIOD_MS
  }

}*/

// -----------------------------------------------------------------------------
// START task
//   Description: called once at the beginning of the match, exit the starting
//                area and do nothing more.
// -----------------------------------------------------------------------------


void ai_task_start(void *params)
{
  // A waypoint for our motions
  wp_t wp;

  // A pointer to corresponding task-structure (self) is passed
  task_t* self = (task_t*) params;


  // OS Software notifier
  BaseType_t notified;
  uint32_t sw_notification;
  uint8_t led;

  // Tick timer
  TickType_t new_wake_time = xTaskGetTickCount();

  // Static items of the waypoints
  wp.coord.abs.a = 0;
  wp.offset.x = 0;
  wp.offset.y = 0;
  wp.offset.a = 0;
  wp.trajectory_must_finish = true;

  // Clear avoidance state in case it was triggered during init
  // (avoid dead lock)
  xTaskNotify(handle_task_avoidance, OS_NOTIFY_AVOIDANCE_CLR, eSetBits);
  avd_mask_all(false);

  // Exit
  // ------------------
  wp.speed = WP_SPEED_FAST;
  wp.type = WP_GOTO_FWD;
  wp.coord.abs = phys.exit_start;
  motion_move_block_on_avd(&wp);

  // push home automation switch
  // ------------------------
  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_ORIENT_FRONT;
  wp.coord.abs = phys.home_automation_switch;
  motion_move_block_on_avd(&wp);

  sys_mod_set_pusher(DSV_PUSHER_OUT);

  wp.type = WP_GOTO_FWD;
  motion_move_block_on_avd(&wp);

  wp.type = WP_GOTO_BWD;
  wp.coord.abs = phys.exit_start;
  wp.trajectory_must_finish = true;
  motion_move_block_on_avd(&wp);

  sys_mod_set_pusher(DSV_PUSHER_IN);

  motion_traj_stop();

  for(;;)
  {
    vTaskDelayUntil( &new_wake_time, pdMS_TO_TICKS(OS_AI_TASKS_PERIOD_MS));

    // Autokill (should be handled by main strat but we never know)
    if(match.timer_msec >= MATCH_DURATION_MSEC)
    {
      self->state = TASK_STATE_SUCCESS;
    }

    if(!(match.timer_msec % 500)) {
      led = (led+1) % 8;
 //     sys_mod_set_led(led);
    }

  }


}

// GET_ORES_S_1A
// GET_ORES_S_1B
// GET_ORES_B_1

// THROW_ORES

// GET_MODS_M_S
// GET_MODS_M_TA
// GET_MODS_M_TB
// GET_MODS_M_TC


// BUILD_MODS_S
// BUILD_MODS_C

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

// GET_MODS_P_S
// GET_MODS_P_TA/B/C

// GET_ORES_S_2A
// GET_ORES_S_2B
// GET_ORES_B_2



