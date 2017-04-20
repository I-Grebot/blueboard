/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       motion_traj.c
 * @author     Bebop35
 * @date       Apr 20, 2017
 * -----------------------------------------------------------------------------
 * @brief
 *   This task handles the motion's trajectory
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "main.h"

/* Global functions */
extern RobotTypeDef robot;

static double traj_near_window_d = TRAJECTORY_NEAR_WINDOW_D;
static double traj_near_window_a = TRAJECTORY_NEAR_WINDOW_A;

// TODO: check
extern av_t av;

/* Local private variables */
static xQueueHandle   xWaypointQueue;

/* Local Private functions */
static void motion_traj_task(void *pvParameters);

/* -----------------------------------------------------------------------------
 * Initializations
 * -----------------------------------------------------------------------------
 */

BaseType_t motion_traj_start(void)
{
  BaseType_t ret;

  // Allocate memory for the waypoint queue
  xWaypointQueue = xQueueCreate(MOTION_MAX_WP_IN_QUEUE, sizeof(wp_t));
  if(xWaypointQueue == 0)
  {
    DEBUG_CRITICAL("Insufficient heap RAM available for Waypoint Queue"DEBUG_EOL);
    ret = pdFAIL;

  // Create the motion trajectory task
  } else {

    ret = xTaskCreate(motion_traj_task, "MOTION_TRAJ", OS_TASK_STACK_MOTION_TRAJ, NULL, OS_TASK_PRIORITY_MOTION_TRAJ, NULL);
    if(ret != pdPASS) {
      DEBUG_CRITICAL("Could not start motion trajectory task!"DEBUG_EOL);

    } else {
      DEBUG_INFO("Starting MOTION_TRAJ task"DEBUG_EOL);
    }

  }

  return ret;

}

/* -----------------------------------------------------------------------------
 * Main motion trajectory management task
 * -----------------------------------------------------------------------------
 */

static void motion_traj_task( void *pvParameters )
{
  static wp_t current_waypoint;

  /* Remove compiler warning about unused parameter. */
  ( void ) pvParameters;

  for( ;; )
  {
    xQueueReceive(xWaypointQueue, &current_waypoint, portMAX_DELAY);

    // TODO: check
    // Handle avoidance blockage
    av.mask_back_left = true;
    av.mask_back_right = true;
    av.mask_front_center = true;
    av.mask_front_left = true;
    av.mask_front_right = true;

    motion_execute_wp(&current_waypoint);

    while(!motion_is_traj_done(&current_waypoint))
    {
      vTaskDelay(pdMS_TO_TICKS(MOTION_CONTROL_PERIOD_MS));
    }

    av.mask_back_left = false;
    av.mask_back_right = false;
    av.mask_front_center = false;
    av.mask_front_left = false;
    av.mask_front_right = false;
  } // traj done

}

/* -----------------------------------------------------------------------------
 * Shorthands
 * -----------------------------------------------------------------------------
 */

inline bool motion_is_traj_near(void)
{
  return (bool) trajectory_in_window(&robot.cs.traj, traj_near_window_d, traj_near_window_a);
}

inline bool motion_is_traj_finished(void)
{
  return (bool) (trajectory_finished(&robot.cs.traj)&&(robot.cs.traj.scheduler_task==NULL));
}

/* -----------------------------------------------------------------------------
 * Trajectory characteristics Setters
 * -----------------------------------------------------------------------------
 */

inline void motion_set_window(double window_d, double window_a, double a_start)
{
  trajectory_set_windows(&robot.cs.traj, window_d, window_a, a_start);
}

void motion_set_near_window(double window_d, double window_a)
{
  traj_near_window_d = (double) window_d;
  traj_near_window_a  = (double) window_a;
}

inline void motion_set_speed(int16_t speed_d, int16_t speed_a)
{
  trajectory_set_speed(&robot.cs.traj, speed_d, speed_a);
}

inline void motion_traj_hard_stop(void)
{
  trajectory_hardstop(&robot.cs.traj);
}

inline void motion_traj_stop(void)
{
  trajectory_stop(&robot.cs.traj);
}

/* -----------------------------------------------------------------------------
 * Direct motion commands
 * -----------------------------------------------------------------------------
 */

void motion_move_relative(double d_mm, double a_deg_rel)
{
  trajectory_d_a_rel(&robot.cs.traj, d_mm, a_deg_rel);
}

void motion_goto_auto(double pos_x, double pos_y)
{
  trajectory_goto_xy_abs(&robot.cs.traj, pos_x, pos_y);
}

void motion_goto_forward(double pos_x, double pos_y)
{
  trajectory_goto_forward_xy_abs(&robot.cs.traj, pos_x, pos_y);
}

void motion_goto_backward(double pos_x, double pos_y)
{
  trajectory_goto_backward_xy_abs(&robot.cs.traj, pos_x, pos_y);
}

void motion_turnto_front(double pos_x, double pos_y)
{
  trajectory_turnto_xy(&robot.cs.traj, pos_x, pos_y);
}

void motion_turnto_behind(double pos_x, double pos_y)
{
  trajectory_turnto_xy_behind(&robot.cs.traj, pos_x, pos_y);
}

/* -----------------------------------------------------------------------------
 * Waypoint management
 * -----------------------------------------------------------------------------
 */

void motion_clear_all_wp(void)
{
  wp_t lost;
  while(xQueueReceive(xWaypointQueue, &lost, 0));
}

BaseType_t motion_add_new_wp(wp_t *waypoint)
{
  return xQueueSend(xWaypointQueue, waypoint, 0);
}

bool motion_is_traj_done(wp_t *waypoint)
{
  if(waypoint->trajectory_must_finish)
    return motion_is_traj_finished();
  else
    return motion_is_traj_near();
}

void motion_execute_wp(wp_t *waypoint)
{

  // TODO: add relative offset mgmt

  // Maybe we need to move the reference to take into account an offset
  // from the robot's center and an actuator
  //   if(waypoint->type != WP_MOVE_REL) {
  //    if((waypoint->offset.x != 0) || (waypoint->offset.y != 0)) {
  //    phys_apply_offset(&(robot.cs.pos), (waypoint->coord.abs), (waypoint->offset));
  //  }
  // }

  // Pop a waypoint and send the speed and position it
  switch(waypoint->speed)
  {
    case WP_SPEED_FAST:
      motion_set_speed(SPEED_FAST_D, SPEED_FAST_A);
      break;
    case WP_SPEED_NORMAL:
      motion_set_speed(SPEED_NORMAL_D, SPEED_NORMAL_A);
      break;
    case WP_SPEED_SLOW:
      motion_set_speed(SPEED_SLOW_D, SPEED_SLOW_A);
      break;
    case WP_SPEED_VERY_SLOW:
    default: // this is quite an error
      DEBUG_ERROR("Unrecognized waypoint speed"DEBUG_EOL);
      motion_set_speed(SPEED_VERY_SLOW_D, SPEED_VERY_SLOW_A);
      break;
  }

  switch(waypoint->type)
  {
    // "GOTO" motion (full-motions)
    case WP_GOTO_AUTO:
      motion_goto_auto(waypoint->coord.abs.x, waypoint->coord.abs.y);
      break;
    case WP_GOTO_FWD:
      motion_goto_forward(waypoint->coord.abs.x, waypoint->coord.abs.y);
      break;
    case WP_GOTO_BWD:
      motion_goto_backward(waypoint->coord.abs.x, waypoint->coord.abs.y);
      break;

      // Relative motion
    case WP_MOVE_REL:
      // Depending on the sign of the "d", we will go forward or backward
      motion_move_relative(waypoint->coord.rel.d, waypoint->coord.rel.a);
      break;

      // "ORIENT" motions (pure rotation)
    case WP_ORIENT_BEHIND:
      motion_turnto_behind(waypoint->coord.abs.x, waypoint->coord.abs.y);
      break;
    case WP_ORIENT_FRONT:
      motion_turnto_front(waypoint->coord.abs.x, waypoint->coord.abs.y);
      break;

    default:
      DEBUG_ERROR("Unrecognized waypoint type"DEBUG_EOL);
      break;
  } // switch

}
