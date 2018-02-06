/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       avoidance.c
 * @author     Pierrick
 * @date       Apr 29, 2016
 * -----------------------------------------------------------------------------
 * @brief
 *   This task read industrial sensor and manage robot avoidance
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

/* Inclusion */
#include "../../2018_T1_R1/include/main.h"

/* TODO: Local Variable Mutex */
av_t av;	// Avoidance handler

/* Local, Private functions */
static void avoidance_task(void *pvParameters);
static void avd_init(void);

static void avd_mask_sensor_from_wall(int16_t a, int16_t wall_a);
static inline uint16_t avd_mask_static_get_word(void);
static inline uint16_t avd_mask_dynamic_get_word(void);
static inline uint16_t avd_det_get_word(void);
//bool av_compute_opponent_position(void);
//void do_avoidance(void);

TaskHandle_t handle_task_avoidance;

// Handle on the strategy task
// TODO: fixme; use functions instead and leave handles private
extern TaskHandle_t handle_task_sequencer;


void avoidance_start(void)
{

  // Start avoidance task
  return sys_create_task(avoidance_task, "AVOIDANCE", OS_TASK_STACK_AVOIDANCE, NULL, OS_TASK_PRIORITY_AVOIDANCE, &handle_task_avoidance );
}

static void avoidance_task( void *pvParameters )
{
  TickType_t xNextWakeTime;

  BaseType_t notified;
  uint32_t sw_notification;
  bool valid_detection;

  avd_init();

  /* Initialise xNextWakeTime - this only needs to be done once. */
  xNextWakeTime = xTaskGetTickCount();

  static uint32_t i = 0;

  /* Remove compiler warning about unused parameter. */
  ( void ) pvParameters;

  for( ;; )
  {
    // Sample sensors, apply polarity
    av.det_front_left   = SW_AVD_FRONT_LEFT_VALUE   == SW_AVD_ON;
    av.det_front_center = SW_AVD_FRONT_CENTER_VALUE == SW_AVD_ON;
    av.det_front_right  = SW_AVD_FRONT_RIGHT_VALUE  == SW_AVD_ON;
    av.det_back_left    = SW_AVD_BACK_LEFT_VALUE    == SW_AVD_ON;
    av.det_back_center  = SW_AVD_BACK_CENTER_VALUE  == SW_AVD_ON;
    av.det_back_right   = SW_AVD_BACK_RIGHT_VALUE   == SW_AVD_ON;
    av.det_word = avd_det_get_word();

    //do_avoidance();

    // Check to see if there is a valid detection
    // Also update dynamic masks and effective detection values
    //valid_detection = avd_detection_is_valid();
    valid_detection = false;

    // A new detection condition occurs
    // This is a transitional state
    if(av.state == AV_STATE_CLEAR && valid_detection)
    {
      DEBUG_INFO("[AVD] Detection!"DEBUG_EOL);

      xTaskNotify(handle_task_sequencer, OS_NOTIFY_AVOIDANCE_EVT, eSetBits);

      av.state = AV_STATE_DETECT;

      vTaskDelayUntil( &xNextWakeTime, pdMS_TO_TICKS(OS_AVOIDANCE_PERIOD_MS));
    }

    // A detection was triggered, wait for the clear notification
    else if(av.state == AV_STATE_DETECT)
    {

      notified = xTaskNotifyWait(0, UINT32_MAX, &sw_notification, pdMS_TO_TICKS(OS_AVOIDANCE_PERIOD_MS));

      // Clear instruction received
      if(notified && (sw_notification & OS_NOTIFY_AVOIDANCE_CLR))
      {
        DEBUG_INFO("[AVD] Clear!"DEBUG_EOL);
        av.state = AV_STATE_CLEAR;
      }
    }

    // Nothing happened.
    else
    {
      vTaskDelayUntil( &xNextWakeTime, pdMS_TO_TICKS(OS_AVOIDANCE_PERIOD_MS));
    }


  }
}

static void avd_init(void)
{
  av.state = AV_STATE_DISABLE;
  av.action_done = 0;
  av.timer_ms = 0;
  av.timer_opp_validity_ms = 0;
  avd_mask_all(true);

}

void avd_enable(void)
{
  // Re-init
  avd_init();

  // Move to clear
  av.state = AV_STATE_CLEAR;

}

void avd_disable(void)
{
  av.state = AV_STATE_DISABLE;

}

void avd_mask_all(bool value)
{
  av.mask_static_front_left = value;
  av.mask_static_front_center = value;
  av.mask_static_front_right = value;
  av.mask_static_back_left = value;
  av.mask_static_back_center = value;
  av.mask_static_back_right = value;
  av.mask_static_word = avd_mask_static_get_word();
}

void avd_mask_front(bool value)
{
  av.mask_static_front_left = value;
  av.mask_static_front_center = value;
  av.mask_static_front_right = value;
  av.mask_static_word = avd_mask_static_get_word();
}


void avd_mask_back(bool value)
{
  av.mask_static_back_left = value;
  av.mask_static_back_center = value;
  av.mask_static_back_right = value;
  av.mask_static_word = avd_mask_static_get_word();
}

uint16_t avd_mask_static_get_word(void)
{
  return
      (av.mask_static_front_left   << 0U)  |
      (av.mask_static_front_center << 1U)  |
      (av.mask_static_front_right  << 2U)  |
      (av.mask_static_back_left    << 3U)  |
      (av.mask_static_back_center  << 4U)  |
      (av.mask_static_back_right   << 5U);
}

uint16_t avd_mask_dynamic_get_word(void)
{
  return
      (av.mask_dyn_front_left   << 0U)  |
      (av.mask_dyn_front_center << 1U)  |
      (av.mask_dyn_front_right  << 2U)  |
      (av.mask_dyn_back_left    << 3U)  |
      (av.mask_dyn_back_center  << 4U)  |
      (av.mask_dyn_back_right   << 5U);
}

uint16_t avd_det_get_word(void)
{
  return
      (av.det_front_left   << 0U)  |
      (av.det_front_center << 1U)  |
      (av.det_front_right  << 2U)  |
      (av.det_back_left    << 3U)  |
      (av.det_back_center  << 4U)  |
      (av.det_back_right   << 5U);
}

// From the sensor values, the robot current position/orientation,
// and the table dimension and shape decide if an opponent detection
// should occur. Each sensor value is masked depending on these condition.
// Finally if a detection is valid (and we should stop!) returns true.
bool avd_detection_is_valid(void) {

  int16_t x;
  int16_t y;
  int16_t a;

  // Sample values at once and use local variables only from here.
  // This ensure atomicity (almost ~).
  x = motion_get_x();
  y = motion_get_y();
  a = motion_get_a();

  // Initialize dynamic masks (enabled)
  av.mask_dyn_front_left    = true;
  av.mask_dyn_front_center  = true;
  av.mask_dyn_front_right   = true;
  av.mask_dyn_back_left     = true;
  av.mask_dyn_back_center   = true;
  av.mask_dyn_back_right    = true;

  // Look at the robot orientation and mask sensors pointing outside
  // of the playground.
  // Use the table coordinates minus an inner ring (= robot radius + some margin)
  // The robot orientation is split into 8 angular values

  // X axis on the left side: a detection can occur if we try to go on the up, right or down sides.

  // Table walls

  if(x < TABLE_X_MIN + ROBOT_RADIUS + AV_TABLE_MARGIN)
    avd_mask_sensor_from_wall(a, AV_ANGLE_WEST_WALL);
  if(x > TABLE_X_MAX - ROBOT_RADIUS - AV_TABLE_MARGIN)
    avd_mask_sensor_from_wall(a, AV_ANGLE_EAST_WALL);
  if(y < TABLE_Y_MIN + ROBOT_RADIUS + AV_TABLE_MARGIN)
    avd_mask_sensor_from_wall(a, AV_ANGLE_NORTH_WALL);
  if(y > TABLE_Y_MAX - ROBOT_RADIUS - AV_TABLE_MARGIN)
    avd_mask_sensor_from_wall(a, AV_ANGLE_SOUTH_WALL);


  // Construction area walls
/*
  // West side of the construction area
  if(    (x <=  CONSTRUCTION_AREA_MIDDLE_X)
      && (x >= CONSTRUCTION_AREA_MIDDLE_X - ROBOT_RADIUS - AV_CONSTRUCTION_AREA_MARGIN)
      && (y >= CONSTRUCTION_AREA_Y_MIN - ROBOT_RADIUS - AV_CONSTRUCTION_AREA_MARGIN)
      && (y <=  CONSTRUCTION_AREA_Y_MAX + ROBOT_RADIUS + AV_CONSTRUCTION_AREA_MARGIN))
      avd_mask_sensor_from_wall(a, AV_ANGLE_EAST_WALL);

  // East side of the construction area
  if(    (x >=  CONSTRUCTION_AREA_MIDDLE_X)
      && (x <= CONSTRUCTION_AREA_MIDDLE_X + ROBOT_RADIUS + AV_CONSTRUCTION_AREA_MARGIN)
      && (y >= CONSTRUCTION_AREA_Y_MIN - ROBOT_RADIUS - AV_CONSTRUCTION_AREA_MARGIN)
      && (y <=  CONSTRUCTION_AREA_Y_MAX + ROBOT_RADIUS + AV_CONSTRUCTION_AREA_MARGIN))
      avd_mask_sensor_from_wall(a, AV_ANGLE_WEST_WALL);
*/
// We also take into account static elements as:
  // - Starting zone (the opponent cannot be present in here)
  // - Opponent zone (we will never go there)

  // Mask if required with static and dynamic masks
  av.det_front_left   &= av.mask_static_front_left    & av.mask_dyn_front_left;
  av.det_front_center &= av.mask_static_front_center  & av.mask_dyn_front_center;
  av.det_front_right  &= av.mask_static_front_right   & av.mask_dyn_front_right;
  av.det_back_left    &= av.mask_static_back_left     & av.mask_dyn_back_left;
  av.det_back_center  &= av.mask_static_back_center   & av.mask_dyn_back_center;
  av.det_back_right   &= av.mask_static_back_right    & av.mask_dyn_back_right;

  // Update feedbacks
  av.mask_dynamic_word = avd_mask_dynamic_get_word();
  av.det_effective_word = avd_det_get_word();

  return av.det_effective_word != 0;
}

// Mask the robot sensors based on its the orientation, in order to ignore
// the sensors that are facing to a wall.
// 'a' parameter is the robot angle in the [-180;+180[ range
// 'wall_a' is used to parameter the different walls position:
//    0 for a West wall (vertical wall, the robot on its right)
// -180 for a East wall (vertical wall, the robot on its left)
//  +90 for a South wall (horizontal wall, the robot is above)
//  -90 for a North wall (horizontal wall, the robot is bellow)
// and a robot with an orientation = 0 (looking to the est).
// This updates the dynamic masks
static void avd_mask_sensor_from_wall(int16_t a, int16_t wall_a)
{

  // Offset the robot position with the wall orientation
  a += wall_a;

  // Ensure that a is in a correct [-180;+180[ range
  if(a < -180)
    a += 360;
  else if(a >= 180)
    a -= 360;

  // South: Mask Right sensors
  if((a >= 90 - AV_ANGULAR_CONE) && (a < 90 + AV_ANGULAR_CONE)) {
    av.mask_dyn_front_right = false;
    av.mask_dyn_back_right = false;

  // South-East: Mask FR + FC
  } else if((a >= 90 + AV_ANGULAR_CONE) && (a < 180 - AV_ANGULAR_CONE)) {
    av.mask_dyn_front_center = false;
    av.mask_dyn_front_right = false;

  // East: Mask Front sensors
  } else if((a >= 180 - AV_ANGULAR_CONE) || (a < -180 + AV_ANGULAR_CONE)) {
    av.mask_dyn_front_left = false;
    av.mask_dyn_front_center = false;
    av.mask_dyn_front_right = false;

   // North-East : Mask FL + FC
  } else if((a >= -180 + AV_ANGULAR_CONE) && (a < -90 - AV_ANGULAR_CONE)) {
    av.mask_dyn_front_left = false;
    av.mask_dyn_front_center = false;

  // North : Mask Left sensors
  } else if((a >= -90 - AV_ANGULAR_CONE) && (a < -90 + AV_ANGULAR_CONE)) {
    av.mask_dyn_front_left = false;
    av.mask_dyn_back_left = false;

  // North-West : Mask BL + BC
  } else if((a >= -90 + AV_ANGULAR_CONE) && (a < - AV_ANGULAR_CONE)) {
    av.mask_dyn_back_left = false;
    av.mask_dyn_back_center = false;

  // West : Mask Back sensors
  } else if((a >= - AV_ANGULAR_CONE) && (a < AV_ANGULAR_CONE)) {
    av.mask_dyn_back_left = false;
    av.mask_dyn_back_center = false;
    av.mask_dyn_back_right = false;

  // South-West: Mask BC + BR
  } else {
    av.mask_dyn_back_center = false;
    av.mask_dyn_back_right = false;

  }

}

/*
void do_avoidance(void) {


    switch(av.state) {

        case AV_STATE_CLEAR:

            // Clean up IRQs
            sw_irq &= ~(IRQ_AV_CLEAR);

            // Check to see if there is a valid detection,
            // then we need to filter it
            if(av_detection_is_valid()) {
                av.state = AV_STATE_FILTER;
            }
            break;

        case AV_STATE_FILTER:

            // TODO: maybe add a filter
            // This means to be triggered once, after the end of the filter
            if(1) {

              // Go to Detect after filtering, setup a timer
              av.timer_ms = AV_TIMER_STOP_MS;
              av.state = AV_STATE_DETECT;
              av.action_done = false;

              // STOP!! and clear motion
              can_send_motion_cmd(CAN_MOTION_FLAG_HARDSTOP, 0, 0, 0);
              motion_clear();

              // Calculate the new opponent position
              // and update PF if it's correct
              if(av_compute_opponent_position()) {
                phys_set_opponent_position(1, robot.opp_pos.x, robot.opp_pos.y);
              }

            }

            break;

        case AV_STATE_DETECT:

            if(av.timer_ms <= 0) {

                av.state = AV_STATE_REROUTE;
                //av.state = AV_STATE_CLEAR;
                av.timer_ms = 0;
                av.action_done = false;

            } else {
                av.timer_ms -= SCHEDULER_RATE_DO_AVOIDANCE_MS;
            }

            break;

        case AV_STATE_REROUTE:

            // Awaits for rerouting flag of strategy before starting
            // the ignore timer
            if(!av.action_done) {
                if(sw_irq & IRQ_AV_CLEAR) {
                    av.timer_ms = AV_TIMER_IGNORE_MS;
                    av.action_done = true;
                }

            } else {

                // Ignore timer is over: go back to detection state
                if(av.timer_ms <= 0) {
                    av.state = AV_STATE_CLEAR;
                    av.timer_ms = 0;
                    av.action_done = false;

                } else {
                    av.timer_ms -= SCHEDULER_RATE_DO_AVOIDANCE_MS;
                }

            }

            break;

    }


}
*/
