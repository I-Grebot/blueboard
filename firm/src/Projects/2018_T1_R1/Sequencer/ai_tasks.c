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

  // Tick timer
  TickType_t new_wake_time = xTaskGetTickCount();

  uint8_t led;

/*
  for(;;)
  {
    vTaskDelayUntil( &new_wake_time, pdMS_TO_TICKS(200));

    if(SW_COLOR)
    {
      led_set_color(BB_LED_BLUE);
      sys_mod_proc_release_back(true);
      sys_mod_proc_release_back(false);
    } else {
      led_set_color(BB_LED_YELLOW);
      sys_mod_proc_grab_back(true);
      sys_mod_proc_grab_back(false);
    }

  }

*/
  // Static items of the waypoints
  wp.coord.abs.a = 0;
  wp.offset.x = 0;
  wp.offset.y = 0;
  wp.offset.a = 0;
  wp.speed = WP_SPEED_SLOW;
  wp.trajectory_must_finish = false;

  // Clear avoidance state in case it was triggered during init
  // (avoid dead lock)
  xTaskNotify(handle_task_avoidance, OS_NOTIFY_AVOIDANCE_CLR, eSetBits);

  // Exit
  // ------------------

  avd_mask_all(false);

  wp.type = WP_GOTO_BWD;
  wp.speed = WP_SPEED_FAST;
  wp.trajectory_must_finish = false;
  wp.coord.abs.x = 1300;
  wp.coord.abs.y = 600;
  motion_move_block_on_avd(&wp);

  wp.coord.abs.x = 1000;
  wp.coord.abs.y = 1000;
  motion_move_block_on_avd(&wp);

  wp.speed = WP_SPEED_NORMAL;
  wp.coord.abs.x = 500;
  wp.coord.abs.y = 1400;
  motion_move_block_on_avd(&wp);


  // 1ST
  // ------------------------

  sys_mod_proc_prepare_grab();

  avd_mask_front(false);
  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_GOTO_FWD;
  wp.coord.abs.x = 700;
  wp.coord.abs.y = 1700;
  wp.trajectory_must_finish = true;
  motion_move_block_on_avd(&wp);

  sys_mod_proc_grab();

  wp.type = WP_GOTO_BWD;
  wp.coord.abs.x = 400;
  wp.coord.abs.y = 1270;
  wp.trajectory_must_finish = false;
  motion_move_block_on_avd(&wp);

  avd_mask_front(true);
  wp.speed = WP_SPEED_NORMAL;
  wp.coord.abs.x = 600;
  wp.coord.abs.y = 920;
  wp.trajectory_must_finish = true;
  motion_move_block_on_avd(&wp);

  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_GOTO_FWD;
  wp.coord.abs.x = 330;
  wp.coord.abs.y = 920;
  motion_move_block_on_avd(&wp);

  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_ORIENT_FRONT;
  wp.coord.abs.x = 0;
  wp.coord.abs.y = motion_get_y();
  motion_move_block_on_avd(&wp);

  sys_mod_proc_land();

  wp.type = WP_GOTO_BWD;
  wp.coord.abs.x = 400;
  wp.coord.abs.y = 900;
  motion_move_block_on_avd(&wp);

  sys_mod_proc_fold();

  avd_mask_front(true);
  wp.type = WP_GOTO_BWD;
  wp.coord.abs.x = 600;
  wp.coord.abs.y = 900;
  motion_move_block_on_avd(&wp);

  // 2ND
  // ------------------------

  sys_mod_proc_prepare_grab();

  wp.speed = WP_SPEED_NORMAL;
  wp.type = WP_GOTO_FWD;
  wp.coord.abs.x = 400;
  wp.coord.abs.y = 730;
  wp.trajectory_must_finish = false;
  motion_move_block_on_avd(&wp);


  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_GOTO_FWD;
  wp.coord.abs.x = 300;
  wp.coord.abs.y = 650;
  wp.trajectory_must_finish = true;
  motion_move_block_on_avd(&wp);

  sys_mod_proc_grab();

  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_GOTO_BWD;
  wp.coord.abs.x = 400;
  wp.coord.abs.y = 770;
  wp.trajectory_must_finish = false;
  motion_move_block_on_avd(&wp);

  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_GOTO_FWD;
  wp.coord.abs.x = 325;
  wp.coord.abs.y = 770;
  wp.trajectory_must_finish = true;
  motion_move_block_on_avd(&wp);

  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_ORIENT_FRONT;
  wp.coord.abs.x = 0;
  wp.coord.abs.y = motion_get_y();
  motion_move_block_on_avd(&wp);

  sys_mod_proc_land();

  avd_mask_front(true);
  wp.type = WP_GOTO_BWD;
  wp.speed = WP_SPEED_FAST;
  wp.coord.abs.x = 600;
  wp.coord.abs.y = 1000;
  wp.trajectory_must_finish = false;
  motion_move_block_on_avd(&wp);

  // ------------------

  sys_mod_proc_fold();

  /*
  wp.type = WP_GOTO_FWD;
  wp.speed = WP_SPEED_NORMAL;
  wp.coord.abs.x = 650;
  wp.coord.abs.y = 1150;
  motion_move_block_on_avd(&wp);
*/

  // 3RD
  // ------------------------

#ifdef DO_3RD
  sys_mod_proc_prepare_grab();

  avd_mask_front(false);
  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_GOTO_FWD;
  wp.coord.abs.x = 750;
  wp.coord.abs.y = 1250;
  wp.trajectory_must_finish = true;
  motion_move_block_on_avd(&wp);


  sys_mod_proc_grab();


  wp.type = WP_GOTO_BWD;
  wp.coord.abs.x = 500;
  wp.coord.abs.y = 1080;
  motion_move_block_on_avd(&wp);


  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_GOTO_FWD;
  wp.coord.abs.x = 330;
  wp.coord.abs.y = 1080;
  motion_move_block_on_avd(&wp);

  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_ORIENT_FRONT;
  wp.coord.abs.x = 0;
  wp.coord.abs.y = motion_get_y();
  motion_move_block_on_avd(&wp);

  sys_mod_proc_land();


  avd_mask_front(true);
  wp.speed = WP_SPEED_NORMAL;
  wp.type = WP_GOTO_BWD;
  wp.coord.abs.x = 600;
  wp.coord.abs.y = motion_get_y();
  wp.trajectory_must_finish = false;
  motion_move_block_on_avd(&wp);

  sys_mod_proc_fold();

#endif

  // Fusée grab
  // ----------

  avd_mask_front(true);
  wp.speed = WP_SPEED_NORMAL;
  wp.type = WP_GOTO_FWD;
  wp.coord.abs.x = 1110;
  wp.coord.abs.y = 800;
  wp.trajectory_must_finish = false;
  motion_move_block_on_avd(&wp);

  sys_mod_proc_prepare_grab();

  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_ORIENT_FRONT;
  wp.coord.abs.x = 1110;
  wp.coord.abs.y = 30;
  motion_move_block_on_avd(&wp);

  avd_mask_front(false);
  wp.speed = WP_SPEED_VERY_SLOW;
  wp.type = WP_GOTO_FWD;
  wp.coord.abs.x = 1110;
  wp.coord.abs.y = 255;
  wp.trajectory_must_finish = true;
  motion_move_block_on_avd(&wp);


  // Patch
  extern sys_mod_t sys_mod;
  dxl_set_position(&sys_mod.grabber_left, DSV_GRABBER_LEFT_POS_CLOSED);
  dxl_set_position(&sys_mod.grabber_right, DSV_GRABBER_RIGHT_POS_CLOSED);


  vTaskDelay(pdMS_TO_TICKS(500));

  avd_mask_front(false);
  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_GOTO_BWD;
  wp.coord.abs.x = 1120;
  wp.coord.abs.y = 355;
  wp.trajectory_must_finish = false;
  motion_move_block_on_avd(&wp);

  sys_mod_proc_grab();

  avd_mask_front(false);
  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_GOTO_BWD;
  wp.coord.abs.x = 1120;
  wp.coord.abs.y = 600;
  wp.trajectory_must_finish = false;
  motion_move_block_on_avd(&wp);



  /// CHoppage arriere right

  //coords:right grab 1298;191
   //              back 1140:221

  sys_mod_proc_release_back(match.color == MATCH_COLOR_ORANGE);

  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_ORIENT_BEHIND;
  wp.coord.abs.x = 1298;
  wp.coord.abs.y = 191;
  motion_move_block_on_avd(&wp);


  wp.speed = WP_SPEED_VERY_SLOW;
  wp.type = WP_GOTO_BWD;
  wp.coord.abs.x = 1278;
  wp.coord.abs.y = 191;
  wp.trajectory_must_finish = true;
  motion_move_block_on_avd(&wp);


  sys_mod_proc_grab_back(match.color == MATCH_COLOR_ORANGE);

  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_GOTO_FWD;
  wp.coord.abs.x = 1278;
  wp.coord.abs.y = 600;
  wp.trajectory_must_finish = false;
  motion_move_block_on_avd(&wp);


  /// CHoppage arriere left

  //coords:right grab 1298;191
   //              back 1140:221

  sys_mod_proc_release_back(match.color == MATCH_COLOR_GREEN);

  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_ORIENT_BEHIND;
  wp.coord.abs.x = 1140;
  wp.coord.abs.y = 221;
  motion_move_block_on_avd(&wp);


  wp.speed = WP_SPEED_VERY_SLOW;
  wp.type = WP_GOTO_BWD;
  wp.coord.abs.x = 1140;
  wp.coord.abs.y = 221;
  wp.trajectory_must_finish = true;
  motion_move_block_on_avd(&wp);


  sys_mod_proc_grab_back(match.color == MATCH_COLOR_GREEN);

  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_GOTO_FWD;
  wp.coord.abs.x = 1140;
  wp.coord.abs.y = 700;
  wp.trajectory_must_finish = false;
  motion_move_block_on_avd(&wp);




  // Depose fusée
  // -------------

  avd_mask_front(true);
  wp.speed = WP_SPEED_FAST;
  wp.type = WP_GOTO_FWD;
  wp.coord.abs.x = 500;
  wp.coord.abs.y = 1400;
  motion_move_block_on_avd(&wp);

  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_GOTO_FWD;
  wp.coord.abs.x = 730;
  wp.coord.abs.y = 1735;
  wp.trajectory_must_finish = true;
  motion_move_block_on_avd(&wp);

  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_ORIENT_FRONT;
  wp.coord.abs.x = 790;
  wp.coord.abs.y = 1675;
  motion_move_block_on_avd(&wp);

  /*
  if(match.color == MATCH_COLOR_BLUE) {
    sys_mod_set_trollet_cmd(SW_TROLLET_GOTO_RIGHT);
  } else {
    sys_mod_set_trollet_cmd(SW_TROLLET_GOTO_LEFT);
  }
  */

  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_GOTO_FWD;
  wp.coord.abs.x = 790;
  wp.coord.abs.y = 1675;
  motion_move_block_on_avd(&wp);

  sys_mod_proc_land();

  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_GOTO_BWD;
  wp.coord.abs.x = 730;
  wp.coord.abs.y = 1735;
  motion_move_block_on_avd(&wp);

  sys_mod_proc_fold();


  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_ORIENT_BEHIND;
  wp.coord.abs.x = 790;
  wp.coord.abs.y = 1675;
  motion_move_block_on_avd(&wp);

  sys_mod_proc_release_back(false);
  sys_mod_proc_release_back(true);

  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_GOTO_BWD;
  wp.coord.abs.x = 790;
  wp.coord.abs.y = 1675;
  motion_move_block_on_avd(&wp);

  // Redepose 1er

  wp.type = WP_GOTO_FWD;
  wp.coord.abs.x = 730;
  wp.coord.abs.y = 1735;
  motion_move_block_on_avd(&wp);

  sys_mod_proc_close_back(false);
  sys_mod_proc_close_back(true);

  wp.type = WP_ORIENT_FRONT;
  wp.speed = WP_SPEED_NORMAL;
  wp.coord.abs.x = 790;
  wp.coord.abs.y = 1675;
  motion_move_block_on_avd(&wp);

  sys_mod_proc_prepare_grab();

  if(match.color == MATCH_COLOR_GREEN) {
    sys_mod_set_trollet_cmd(SW_TROLLET_GOTO_RIGHT);
  } else {
    sys_mod_set_trollet_cmd(SW_TROLLET_GOTO_LEFT);
  }

  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_GOTO_FWD;
  wp.coord.abs.x = 790;
  wp.coord.abs.y = 1675;
  motion_move_block_on_avd(&wp);

  sys_mod_proc_grab();

  vTaskDelay(pdMS_TO_TICKS(500));

  sys_mod_proc_fold();

  if(match.color == MATCH_COLOR_GREEN) {
    sys_mod_set_trollet_cmd(SW_TROLLET_GOTO_RIGHT);
  } else {
    sys_mod_set_trollet_cmd(SW_TROLLET_GOTO_LEFT);
  }

  sys_mod_proc_land();

  // Redepose 2nd

  wp.type = WP_GOTO_BWD;
  wp.speed = WP_SPEED_SLOW;
  wp.coord.abs.x = 730;
  wp.coord.abs.y = 1735;
  motion_move_block_on_avd(&wp);

  wp.type = WP_ORIENT_FRONT;
  wp.speed = WP_SPEED_NORMAL;
  wp.coord.abs.x = 790;
  wp.coord.abs.y = 1675;
  motion_move_block_on_avd(&wp);

  sys_mod_proc_prepare_grab();

  if(match.color == MATCH_COLOR_GREEN) {
    sys_mod_set_trollet_cmd(SW_TROLLET_GOTO_LEFT);
  } else {
    sys_mod_set_trollet_cmd(SW_TROLLET_GOTO_RIGHT);
  }

  wp.speed = WP_SPEED_SLOW;
  wp.type = WP_GOTO_FWD;
  wp.coord.abs.x = 790;
  wp.coord.abs.y = 1675;
  motion_move_block_on_avd(&wp);


  sys_mod_proc_grab();

  vTaskDelay(pdMS_TO_TICKS(500));

  sys_mod_proc_fold();

  if(match.color == MATCH_COLOR_GREEN) {
    sys_mod_set_trollet_cmd(SW_TROLLET_GOTO_LEFT);
  } else {
    sys_mod_set_trollet_cmd(SW_TROLLET_GOTO_RIGHT);
  }

  sys_mod_proc_land();


  // end
  // -------------

  wp.type = WP_GOTO_BWD;
  wp.coord.abs.x = 730;
  wp.coord.abs.y = 1735;
  motion_move_block_on_avd(&wp);


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
      sys_mod_set_led(led);
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



