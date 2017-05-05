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
#include "main.h"

/* TODO: Local Variable Mutex */
av_t av;	// Avoidance handler

/* Local, Private functions */
static void avoidance_task(void *pvParameters);
static void avd_init(void);

static void avd_mask_sensor_from_wall(int16_t a, int16_t wall_a);
//bool av_compute_opponent_position(void);
//void do_avoidance(void);

TaskHandle_t handle_task_avoidance;

// Handle on the strategy task
extern TaskHandle_t handle_task_sequencer;

void avoidance_start(void)
{

  // Start avoidance task
  return sys_create_task(avoidance_task, "AVOIDANCE", OS_TASK_STACK_AVOIDANCE, NULL, OS_TASK_PRIORITY_AVOIDANCE, &handle_task_avoidance );
}

static void avoidance_task( void *pvParameters )
{
  TickType_t xNextWakeTime;
  BaseType_t xHigherPriorityTaskWoken = pdTRUE;

  avd_init();

  /* Initialise xNextWakeTime - this only needs to be done once. */
  xNextWakeTime = xTaskGetTickCount();

  static uint32_t i = 0;

  /* Remove compiler warning about unused parameter. */
  ( void ) pvParameters;

  for( ;; )
  {
    av.det_front_left = IND4_VALUE;
    av.det_front_center = IND7_VALUE;
    av.det_front_right = IND6_VALUE;
    av.det_back_left = IND1_VALUE;
    av.det_back_center = IND2_VALUE;
    av.det_back_right = IND3_VALUE;

    //    	do_avoidance();

    // Test notify every sec strategy
    /*if(!(++i % 100)) {

      if(i < 1000) {
        xTaskNotify(handle_task_sequencer, OS_NOTIFY_AVOIDANCE_EVT, eSetBits);
      }

    }*/

    vTaskDelayUntil( &xNextWakeTime, OS_AVOIDANCE_PERIOD_MS);
  }
}

static void avd_init(void)
{
  av.state = AV_STATE_CLEAR;
  av.action_done = 0;
  av.timer_ms = 0;
  av.timer_opp_validity_ms = 0;

  av.mask_front_left = true;
  av.mask_front_center = true;
  av.mask_front_right = true;
  av.mask_back_left = true;
  av.mask_back_center = true;
  av.mask_back_right = true;
}

// From the sensor values, the robot current position/orientation,
// and the table dimension and shape decide if an opponent detection
// should occur. Each sensor value is masked depending on these condition.
// Finally if a detection is valid (and we should stop!) returns true.
bool avoidance_detection_is_valid(void) {

  int16_t x;
  int16_t y;
  int16_t a;

  // Sample values at once and use local variables only from here.
  // This ensure atomicity (almost ~).
  //x = OS_MotionGetX();		// TODO : handle mutex here
  //y = OS_MotionGetY();
  //a = OS_MotionGetA();

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

  // Mask if required
  av.det_front_left &= av.mask_front_left;
  av.det_front_center &= av.mask_front_center;
  av.det_front_right &= av.mask_front_right;
  av.det_back_left &= av.mask_back_left;
  av.det_back_center &= av.mask_back_center;
  av.det_back_right &= av.mask_back_right;

  return av.det_front_left || av.det_front_center || av.det_front_right ||
         av.det_back_left  || av.det_back_center || av.det_back_right;
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
// For
static void avd_mask_sensor_from_wall(int16_t a, int16_t wall_a) {

  // Offset the robot position with the wall orientation
  a += wall_a;

  // Ensure that a is in a correct [-180;+180[ range
  if(a < -180)
    a += 360;
  else if(a >= 180)
    a -= 360;

  // South: use L + C +BL
  if((a >= 90 - AV_ANGULAR_CONE) && (a < 90 + AV_ANGULAR_CONE)) {
    av.det_front_right = false;
    av.det_back_right = false;

  // South-West: use BL + BR
  } else if((a >= 90 + AV_ANGULAR_CONE) && (a < 180 - AV_ANGULAR_CONE)) {
    av.det_front_left = false;
    av.det_front_center = false;
    av.det_front_right = false;

  // West: use BL + BR + BC
  } else if((a >= 180 - AV_ANGULAR_CONE) || (a < -180 + AV_ANGULAR_CONE)) {
    av.det_front_left = false;
    av.det_front_center = false;
    av.det_front_right = false;

   // North-West : use BL + BR
  } else if((a >= -180 + AV_ANGULAR_CONE) && (a < -90 - AV_ANGULAR_CONE)) {
    av.det_front_left = false;
    av.det_front_center = false;
    av.det_front_right = false;

  // North : use R + C + BR
  } else if((a >= -90 - AV_ANGULAR_CONE) && (a < -90 + AV_ANGULAR_CONE)) {
    av.det_front_left = false;
    av.det_back_left = false;

  // North-East : use R + C
  } else if((a >= -90 + AV_ANGULAR_CONE) && (a < - AV_ANGULAR_CONE)) {
      av.det_front_left = false;
      av.det_back_left = false;
      av.det_back_center = false;
      av.det_back_right = false;

  // East : use L + C + R
  } else if((a >= - AV_ANGULAR_CONE) && (a < AV_ANGULAR_CONE)) {
    av.det_back_left = false;
    av.det_back_center = false;
    av.det_back_right = false;

  // South-Est: use C + L
  } else {
    av.det_front_right = false;
    av.det_back_left = false;
    av.det_back_center = false;
    av.det_back_right = false;
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
