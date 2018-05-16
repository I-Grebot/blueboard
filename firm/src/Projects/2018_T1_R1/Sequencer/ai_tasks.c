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

  phys_update_with_color_xya(&wp->coord.abs.x, &wp->coord.abs.y, &wp->coord.abs.a);
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

      // Stop and wait 1 seconds
      motion_traj_hard_stop();
      vTaskDelay(pdMS_TO_TICKS(1000));

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


void ai_task_test_odometry(void)
{
	  // Clear avoidance state in case it was triggered during init
	  // (avoid dead lock)
	  xTaskNotify(handle_task_avoidance, OS_NOTIFY_AVOIDANCE_CLR, eSetBits);
	  avd_mask_all(false);

	  /* ROTATION */
      if(match.color == MATCH_COLOR_GREEN){
	  	  // Clear avoidance state in case it was triggered during init
	  	  // (avoid dead lock)
	  	  avd_mask_all(false);

	  	  /* ROTATION */
	      motion_set_speed(SPEED_NORMAL_D,200);
	      motion_move_relative(0,-3600);		//10turns
	      while(!motion_is_traj_finished())
		     vTaskDelay(pdMS_TO_TICKS(100));
		  vTaskDelay(pdMS_TO_TICKS(5000));
      }
      else{
	  	  // Clear avoidance state in case it was triggered during init
	  	  // (avoid dead lock)
	  	  avd_mask_all(false);

	  	  /* ROTATION */
	      motion_set_speed(SPEED_NORMAL_D,200);
	      motion_move_relative(0,3600);		//10turns
	      while(!motion_is_traj_finished())
			 vTaskDelay(pdMS_TO_TICKS(100));
		  vTaskDelay(pdMS_TO_TICKS(5000));
      }


	  /* TRANSLATION */
	  motion_set_speed(SPEED_SLOW_D,800);
	  motion_move_relative(1000,0);
}

// -----------------------------------------------------------------------------
// Stall at start task
//   Description: call during startup to stall for starting position
// -----------------------------------------------------------------------------


void ai_task_stall_at_start(void)
{
	  // A waypoint for our motions
	  wp_t wp;

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

	  // Stall at start
	  // ------------------
	  wp.speed = WP_SPEED_VERY_SLOW;
	  wp.type = WP_STALL_BACK_X;
	  wp.coord.abs.a = phys.reset.a;
	  wp.coord.abs.x = ROBOT_BACK_TO_CENTER;
	  wp.trajectory_must_finish = true;
	  motion_move_block_on_avd(&wp);				// Stall x axis

	  wp.speed = WP_SPEED_SLOW;
	  wp.type = WP_GOTO_FWD;
	  wp.coord.abs = phys.reset;
	  wp.trajectory_must_finish = true;
	  motion_move_block_on_avd(&wp);				// go to reset pos

	  wp.speed = WP_SPEED_SLOW;
	  wp.type = WP_ORIENT_BEHIND;
	  wp.coord.abs.x = phys.reset.x;
	  wp.coord.abs.y = 0;
	  wp.trajectory_must_finish = true;
	  motion_move_block_on_avd(&wp);				// turn to face the bee

	  wp.speed = WP_SPEED_VERY_SLOW;
	  wp.type = WP_STALL_BACK_Y;
	  wp.coord.abs.a = 90;
	  wp.coord.abs.y = ROBOT_BACK_TO_CENTER;
	  wp.trajectory_must_finish = true;
	  motion_move_block_on_avd(&wp);				// Stall y axis

	  wp.speed = WP_SPEED_SLOW;
	  wp.type = WP_GOTO_FWD;
	  wp.coord.abs = phys.reset;
	  wp.trajectory_must_finish = true;
	  motion_move_block_on_avd(&wp);				// go to reset pos

	  wp.speed = WP_SPEED_SLOW;
	  wp.type = WP_ORIENT_BEHIND;
	  wp.coord.abs.x = 0;
	  wp.coord.abs.y = phys.reset.y;
	  wp.trajectory_must_finish = true;
	  motion_move_block_on_avd(&wp);				// turn to face opponent
}
void ai_task_switch(void *params)
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

  // Static items of the waypoints
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
  wp.speed = WP_SPEED_NORMAL;
  wp.type = WP_GOTO_FWD;
  wp.coord.abs = phys.exit_start;
  wp.trajectory_must_finish = true;
  avd_mask_back(false);
  avd_mask_front(true);
  motion_move_block_on_avd(&wp);

  // push home automation switch
  // ------------------------

  wp.type = WP_ORIENT_FRONT;
  wp.speed = WP_SPEED_NORMAL;
  wp.coord.abs = phys.home_automation_switch;
  wp.trajectory_must_finish = true;
  avd_mask_all(false);
  motion_move_block_on_avd(&wp);

  wp.type = WP_STALL_FRONT_Y;
  wp.speed = WP_SPEED_VERY_SLOW;
  wp.coord.abs.y = ROBOT_FRONT_TO_CENTER;
  wp.coord.abs.a = -90;
  wp.trajectory_must_finish = true;
  avd_mask_all(false);
  motion_move_block_on_avd(&wp);

  sys_mod_do_push(&(self->handle));
  do
  {
	  notified = xTaskNotifyWait(0, UINT32_MAX, &sw_notification, pdMS_TO_TICKS(OS_AI_TASKS_PERIOD_MS));
  }while(!(sw_notification & OS_FEEDBACK_SYS_MOD_PUSH));
  match.scored_points += 25; 					// pushed 100% of the time
  sys_mod_do_push(&(self->handle));

  wp.speed = WP_SPEED_FAST;
  wp.type = WP_GOTO_BWD;
  wp.coord.abs.x = SWITCH_CENTER_X;
  wp.coord.abs.y = TABLE_Y_MAX/2;
  wp.trajectory_must_finish = false;
  avd_mask_back(true);
  avd_mask_front(false);
  motion_move_block_on_avd(&wp);
  while(!motion_is_traj_near())
	  vTaskDelay(pdMS_TO_TICKS(100));

  self->state = TASK_STATE_SUCCESS;

  for(;;)
  {
    vTaskDelayUntil( &new_wake_time, pdMS_TO_TICKS(OS_AI_TASKS_PERIOD_MS));
  }
}
void ai_task_bee(void *params)
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

  // Static items of the waypoints
  wp.offset.x = 0;
  wp.offset.y = 0;
  wp.offset.a = 0;
  wp.trajectory_must_finish = true;

  // Clear avoidance state in case it was triggered during init
  // (avoid dead lock)
  xTaskNotify(handle_task_avoidance, OS_NOTIFY_AVOIDANCE_CLR, eSetBits);
  avd_mask_all(false);

  wp.type = WP_GOTO_BWD;
  wp.speed = WP_SPEED_FAST;
  wp.coord.abs = phys.beehive;
  wp.trajectory_must_finish = true;
  avd_mask_back(true);
  avd_mask_front(false);
  motion_move_block_on_avd(&wp);

  wp.type = WP_ORIENT_BEHIND;					// we are at the hive, need to stall
  wp.speed = WP_SPEED_NORMAL;
  wp.coord.abs.x = BEE_START_X;
  wp.coord.abs.y = TABLE_Y_MAX;
  wp.trajectory_must_finish = true;
  avd_mask_all(false);
  motion_move_block_on_avd(&wp);

  wp.type = WP_STALL_BACK_Y;
  wp.speed = WP_SPEED_VERY_SLOW;
  wp.coord.abs.a = -90;
  wp.coord.abs.y = TABLE_Y_MAX-ROBOT_BACK_TO_CENTER;
  wp.trajectory_must_finish = true;
  motion_move_block_on_avd(&wp);				// stall y axis

  wp.type = WP_GOTO_FWD;
  wp.speed = WP_SPEED_NORMAL;
  wp.coord.abs.x = phys.beehive.x;
  wp.coord.abs.y = BEE_START_Y;
  wp.trajectory_must_finish = true;
  avd_mask_front(true);
  avd_mask_back(false);
  motion_move_block_on_avd(&wp);

  wp.type = WP_ORIENT_BEHIND;
  wp.speed = WP_SPEED_NORMAL;
  wp.coord.abs.x = 0;
  wp.coord.abs.y = BEE_START_Y;
  wp.trajectory_must_finish = true;
  avd_mask_all(false);
  motion_move_block_on_avd(&wp);

  wp.type = WP_STALL_BACK_X;
  wp.speed = WP_SPEED_VERY_SLOW;
  wp.coord.abs.a = 0;
  wp.coord.abs.x = ROBOT_BACK_TO_CENTER;
  wp.trajectory_must_finish = true;
  motion_move_block_on_avd(&wp);				// stall x axis

  if(match.color == MATCH_COLOR_GREEN)
	  sys_mod_do_right_arm(&(self->handle),DSV_RIGHT_ARM_POS_DOWN);
  else
	  sys_mod_do_left_arm(&(self->handle),DSV_LEFT_ARM_POS_DOWN);

  do
  {
	  notified = xTaskNotifyWait(0, UINT32_MAX, &sw_notification, pdMS_TO_TICKS(OS_AI_TASKS_PERIOD_MS) );
  }while(!((sw_notification & OS_FEEDBACK_SYS_MOD_LEFT_ARM)||(sw_notification & OS_FEEDBACK_SYS_MOD_RIGHT_ARM)));

  wp.type = WP_GOTO_FWD;
  wp.speed = WP_SPEED_SLOW;
  wp.coord.abs.y = BEE_START_Y;
  wp.coord.abs.x = ROBOT_BACK_TO_CENTER+300;
  wp.trajectory_must_finish = true;
  avd_mask_all(false);
  motion_move_block_on_avd(&wp);
  match.scored_points += 50; 					// bee smash the balloon 100% of the time

  if(match.color == MATCH_COLOR_GREEN)
	  sys_mod_do_right_arm(&(self->handle),DSV_RIGHT_ARM_POS_UP);
  else
	  sys_mod_do_left_arm(&(self->handle),DSV_LEFT_ARM_POS_UP);

  do
  {
	  notified = xTaskNotifyWait(0, UINT32_MAX, &sw_notification, pdMS_TO_TICKS(OS_AI_TASKS_PERIOD_MS) );
  }while(!((sw_notification & OS_FEEDBACK_SYS_MOD_LEFT_ARM)||(sw_notification & OS_FEEDBACK_SYS_MOD_RIGHT_ARM)));


  self->state = TASK_STATE_SUCCESS;

  for(;;)
  {
    vTaskDelayUntil( &new_wake_time, pdMS_TO_TICKS(OS_AI_TASKS_PERIOD_MS));
  }

 }

void ai_task_our_water(void *params){
	  // A waypoint for our motions
	  wp_t wp;

	  // A pointer to corresponding task-structure (self) is passed
	  task_t* self = (task_t*) params;

	  // OS Software notifier
	  BaseType_t notified;
	  uint32_t sw_notification;

	  // Tick timer
	  TickType_t new_wake_time = xTaskGetTickCount();

	  // Static items of the waypoints
	  wp.offset.x = 0;
	  wp.offset.y = 0;
	  wp.offset.a = 0;
	  wp.trajectory_must_finish = true;

	  // Clear avoidance state in case it was triggered during init
	  // (avoid dead lock)
	  xTaskNotify(handle_task_avoidance, OS_NOTIFY_AVOIDANCE_CLR, eSetBits);
	  avd_mask_all(false);

	  wp.type = WP_GOTO_FWD;
	  wp.speed = WP_SPEED_NORMAL;
	  wp.coord.abs.x = phys.mixed_wastewater_recuperator[PHYS_ID_MIXED_G].x;
	  wp.coord.abs.y = phys.construction_cubes[PHYS_ID_CUBES_EG].y;
	  wp.trajectory_must_finish = false;
	  motion_move_block_on_avd(&wp);				// start trajectory to avoid cube SG
	  while(!motion_is_traj_near())
		  vTaskDelay(pdMS_TO_TICKS(100));

	  wp.type = WP_GOTO_FWD;
	  wp.speed = WP_SPEED_NORMAL;
	  wp.coord.abs.x = phys.mixed_wastewater_recuperator[PHYS_ID_MIXED_G].x;
	  wp.coord.abs.y = WASTEWATER_RECUPERATOR_Y;
	  wp.trajectory_must_finish = true;
	  avd_mask_front(true);
	  avd_mask_back(true);
	  motion_move_block_on_avd(&wp);			//  Avoid cube SG

	  wp.type = WP_GOTO_FWD;
	  wp.speed = WP_SPEED_SLOW;
	  wp.coord.abs = phys.wastewater_recuperator;
	  wp.trajectory_must_finish = false;
	  avd_mask_all(false);
	  motion_move_block_on_avd(&wp);
	  vTaskDelay(pdMS_TO_TICKS(1500));

	  sys_mod_do_open(&(self->handle));
	  do
	  {
		  notified = xTaskNotifyWait(0, UINT32_MAX, &sw_notification, pdMS_TO_TICKS(OS_AI_TASKS_PERIOD_MS) );
	  }while(!(sw_notification & OS_FEEDBACK_SYS_MOD_OPEN));
	  vTaskDelay(pdMS_TO_TICKS(1000));

	  match.scored_points += 10; 				// recuperator open 100% of the time

	  wp.type = WP_GOTO_BWD;
	  wp.speed = WP_SPEED_NORMAL;
	  wp.coord.abs.x = phys.mixed_wastewater_recuperator[PHYS_ID_MIXED_G].x;
	  wp.coord.abs.y = WASTEWATER_RECUPERATOR_Y;
	  wp.trajectory_must_finish = false;
	  avd_mask_front(false);
	  avd_mask_back(true);
	  motion_move_block_on_avd(&wp);
	  while(!motion_is_traj_near())
		  vTaskDelay(pdMS_TO_TICKS(100));

	  wp.type = WP_GOTO_FWD;
	  wp.speed = WP_SPEED_NORMAL;
	  wp.coord.abs.x = phys.reset.x;
	  wp.coord.abs.y = phys.reset.y + 150;
	  wp.trajectory_must_finish = true;
	  avd_mask_all(false);
	  motion_move_block_on_avd(&wp);

	  wp.type = WP_ORIENT_FRONT;
	  wp.speed = WP_SPEED_NORMAL;
	  if(match.color == MATCH_COLOR_GREEN){
		  wp.coord.abs.x = 200;
	  }
	  else{
		  wp.coord.abs.x = 150;
	  }
	  wp.coord.abs.y = 0;
	  wp.trajectory_must_finish = true;
	  avd_mask_all(false);
	  motion_move_block_on_avd(&wp);

	  sys_mod_do_shoot(&(self->handle),SW_SHOOTER_SHOOT_HIGH,7);
	  do
	  {
		  notified = xTaskNotifyWait(0, UINT32_MAX, &sw_notification, pdMS_TO_TICKS(OS_AI_TASKS_PERIOD_MS) );
	  }while(!(sw_notification & OS_FEEDBACK_SYS_MOD_SHOOT));

	  match.scored_points += 10; 				// 4	 balls in the water tower

	  wp.type = WP_GOTO_BWD;
	  wp.speed = WP_SPEED_FAST;
	  wp.coord.abs.x = phys.mixed_wastewater_recuperator[PHYS_ID_MIXED_G].x;
	  wp.coord.abs.y = WASTEWATER_RECUPERATOR_Y;
	  wp.trajectory_must_finish = false;
	  avd_mask_front(false);
	  avd_mask_back(true);
	  motion_move_block_on_avd(&wp);
	  while(!motion_is_traj_near())
		  vTaskDelay(pdMS_TO_TICKS(100));

      self->state = TASK_STATE_SUCCESS;

	  for(;;)
	  {
	    vTaskDelayUntil( &new_wake_time, pdMS_TO_TICKS(OS_AI_TASKS_PERIOD_MS));
	  }
	}
void ai_task_mixed_water(void *params){
	  // A waypoint for our motions
	  wp_t wp;

	  // A pointer to corresponding task-structure (self) is passed
	  task_t* self = (task_t*) params;

	  // OS Software notifier
	  BaseType_t notified;
	  uint32_t sw_notification;

	  // Tick timer
	  TickType_t new_wake_time = xTaskGetTickCount();

	  // Static items of the waypoints
	  wp.offset.x = 0;
	  wp.offset.y = 0;
	  wp.offset.a = 0;
	  wp.trajectory_must_finish = true;

	  // Clear avoidance state in case it was triggered during init
	  // (avoid dead lock)
	  xTaskNotify(handle_task_avoidance, OS_NOTIFY_AVOIDANCE_CLR, eSetBits);

	  wp.type = WP_GOTO_BWD;
	  wp.speed = WP_SPEED_FAST;
	  wp.coord.abs.x = 1800;
	  wp.coord.abs.y = TABLE_Y_MAX/2;
	  wp.trajectory_must_finish = false;
	  avd_mask_front(false);
	  avd_mask_back(true);
	  motion_move_block_on_avd(&wp);
	  while(!motion_is_traj_near())
		  vTaskDelay(pdMS_TO_TICKS(100));

	  wp.type = WP_GOTO_BWD;
	  wp.speed = WP_SPEED_NORMAL;
	  wp.coord.abs.x = phys.mixed_wastewater_recuperator[PHYS_ID_MIXED_O].x;
	  wp.coord.abs.y = 1600;
	  wp.trajectory_must_finish = false;
	  avd_mask_front(false);
	  avd_mask_back(true);
	  motion_move_block_on_avd(&wp);
	  while(!motion_is_traj_near())
		  vTaskDelay(pdMS_TO_TICKS(100));

	  wp.type = WP_GOTO_BWD;
	  wp.speed = WP_SPEED_NORMAL;
	  wp.coord.abs.x = 2800;
	  wp.coord.abs.y = 1600;
	  wp.trajectory_must_finish = false;
	  avd_mask_front(false);
	  avd_mask_back(true);
	  motion_move_block_on_avd(&wp);
	  while(!motion_is_traj_near())
		  vTaskDelay(pdMS_TO_TICKS(100));

	  wp.type = WP_STALL_BACK_X;
	  wp.speed = WP_SPEED_VERY_SLOW;
	  wp.coord.abs.a = 180;
	  wp.coord.abs.x = TABLE_X_MAX-ROBOT_BACK_TO_CENTER;
	  wp.trajectory_must_finish = true;
	  avd_mask_all(false);
	  motion_move_block_on_avd(&wp);				// stall x axis

	  wp.type = WP_GOTO_FWD;
	  wp.speed = WP_SPEED_NORMAL;
	  wp.coord.abs.x = phys.mixed_wastewater_recuperator[PHYS_ID_MIXED_O].x;
	  wp.coord.abs.y = 1600;
	  wp.trajectory_must_finish = true;
	  avd_mask_front(true);
	  avd_mask_back(false);
	  motion_move_block_on_avd(&wp);

	  wp.type = WP_GOTO_FWD;
	  wp.speed = WP_SPEED_NORMAL;
	  wp.coord.abs.x = phys.mixed_wastewater_recuperator[PHYS_ID_MIXED_O].x;
	  wp.coord.abs.y = 1800;
	  wp.trajectory_must_finish = true;
	  avd_mask_all(false);
	  motion_move_block_on_avd(&wp);

	  wp.type = WP_STALL_FRONT_Y;
	  wp.speed = WP_SPEED_SLOW;
	  wp.coord.abs.a = 90;
	  wp.coord.abs.x = TABLE_Y_MAX-ROBOT_FRONT_TO_CENTER;
	  wp.trajectory_must_finish = true;
	  avd_mask_all(false);
	  motion_move_block_on_avd(&wp);				// stall Y axis

	  sys_mod_do_open(&(self->handle));
	  do
	  {
		  notified = xTaskNotifyWait(0, UINT32_MAX, &sw_notification, pdMS_TO_TICKS(OS_AI_TASKS_PERIOD_MS) );
	  }while(!(sw_notification & OS_FEEDBACK_SYS_MOD_OPEN));
	  match.scored_points += 10; 				// recuperator open 100% of the time

	  wp.type = WP_GOTO_BWD;
	  wp.speed = WP_SPEED_NORMAL;
	  wp.coord.abs.x = phys.mixed_wastewater_recuperator[PHYS_ID_MIXED_O].x;
	  wp.coord.abs.y = phys.mixed_wastewater_recuperator[PHYS_ID_MIXED_O].y-400;
	  wp.trajectory_must_finish = true;
	  avd_mask_front(false);
	  avd_mask_back(true);
	  motion_move_block_on_avd(&wp);

	  wp.type = WP_GOTO_FWD;
	  wp.speed = WP_SPEED_NORMAL;
	  wp.coord.abs.x = phys.mixed_wastewater_recuperator[PHYS_ID_MIXED_O].x-100;
	  if(match.color == MATCH_COLOR_GREEN){
		  wp.coord.abs.y = phys.mixed_wastewater_recuperator[PHYS_ID_MIXED_O].y-300;
	  }
	  else{
		  wp.coord.abs.y = phys.mixed_wastewater_recuperator[PHYS_ID_MIXED_O].y-250;
	  }
	  wp.trajectory_must_finish = true;
	  avd_mask_all(false);
	  motion_move_block_on_avd(&wp);

	  wp.type = WP_ORIENT_FRONT;
	  wp.speed = WP_SPEED_NORMAL;
	  wp.coord.abs.x = TABLE_X_MAX/2;
	  wp.coord.abs.y = TABLE_Y_MAX-150;
	  wp.trajectory_must_finish = true;
	  avd_mask_all(false);
	  motion_move_block_on_avd(&wp);

	  sys_mod_do_shoot(&(self->handle),SW_SHOOTER_SHOOT_LOW,10);
	  motion_traj_hard_stop();

	  for(;;)
	  {
	    vTaskDelayUntil( &new_wake_time, pdMS_TO_TICKS(OS_AI_TASKS_PERIOD_MS));

	    // Autokill (should be handled by main strat but we never know)
	    if(match.timer_msec >= MATCH_DURATION_MSEC)
	    {
	      self->state = TASK_STATE_SUCCESS;
	    }
	  }
}

void ai_task_cubes(void *params){
	  // A waypoint for our motions
	  wp_t wp;

	  // A pointer to corresponding task-structure (self) is passed
	  task_t* self = (task_t*) params;

	  // OS Software notifier
	  BaseType_t notified;
	  uint32_t sw_notification;

	  // Tick timer
	  TickType_t new_wake_time = xTaskGetTickCount();

	  // Static items of the waypoints
	  wp.offset.x = 0;
	  wp.offset.y = 0;
	  wp.offset.a = 0;
	  wp.trajectory_must_finish = true;

	  // Clear avoidance state in case it was triggered during init
	  // (avoid dead lock)
	  xTaskNotify(handle_task_avoidance, OS_NOTIFY_AVOIDANCE_CLR, eSetBits);

	  wp.type = WP_GOTO_FWD;						// go for the cube
	  wp.speed = WP_SPEED_NORMAL;
	  wp.coord.abs = phys.construction_cubes[PHYS_ID_CUBES_SG];
	  wp.trajectory_must_finish = true;
	  avd_mask_front(true);
	  avd_mask_back(false);
	  motion_move_block_on_avd(&wp);

	  wp.type = WP_GOTO_FWD;
	  wp.speed = WP_SPEED_NORMAL;
	  wp.coord.abs = phys.cube_deposit;
	  wp.trajectory_must_finish = true;
	  avd_mask_front(true);
	  avd_mask_back(false);
	  motion_move_block_on_avd(&wp);
	  match.scored_points += 2; 				// let says half the cubes are in the deposit

	  self->state = TASK_STATE_SUCCESS;

      for(;;)
	  {
	     vTaskDelayUntil( &new_wake_time, pdMS_TO_TICKS(OS_AI_TASKS_PERIOD_MS));
      }
}
