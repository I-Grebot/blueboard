/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       strategy.c
 * @author     Paul
 * @date       Apr 23, 2017
 * -----------------------------------------------------------------------------
 * @brief
 *   This task handle the whole strategy
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

/* Inclusion */
#include "main.h"

#define UP 2950
#define DOWN 2000

// Match
match_t match;

// Main robot structure
extern phys_t phys;
extern robot_t robot;
extern av_t av;

// Tasks management
extern task_t tasks[TASKS_NB];
extern task_mgt_t task_mgt;


/* Local, Private functions */
static void strategy_task(void *pvParameters);
static void strategy_color_sample(void);

BaseType_t strategy_start(void)
{
  BaseType_t ret;

  strategy_init();
  // path_init();


  // Start main strategy task
  ret = xTaskCreate(strategy_task, "STRATEGY", OS_TASK_STACK_STRATEGY, NULL, OS_TASK_PRIORITY_STRATEGY, NULL );

  if(ret != pdPASS)
  {
    DEBUG_CRITICAL("Could not start STRATEGY task!"DEBUG_EOL);
  } else {
    DEBUG_INFO("Starting STRATEGY task"DEBUG_EOL);
  }

  return ret;

}

void strategy_init(void)
{
  poi_t reset_pos;

  // Initialize the match content fields
  match.color = MATCH_COLOR_UNKNOWN;
  match.state = MATCH_STATE_RESET;
  match.scored_points = 0;
  match.timer_msec = 0;

  // Default reset position
  reset_pos = phys.reset;
  phys_update_with_color(&reset_pos);
  motion_set_x(reset_pos.x);
  motion_set_y(reset_pos.y);
  motion_set_a(reset_pos.a);

  // Initialize physical engine
  phys_init();

  // Initialize all tasks + task manager
  tasks_init();

  // Initialize the path-finder module
  path_init();

}

void strategy_task( void *pvParameters )
{
  TickType_t next_wake_time = xTaskGetTickCount();;
  poi_t reset_pos;

  uint32_t tick_cnt = 0;

  /* Remove compiler warning about unused parameter. */
  ( void ) pvParameters;

  led_set_mode(BB_LED_OFF);

  // Launch IDLE task
  ai_task_launch(&tasks[TASK_ID_IDLE]);

  // TEMP
  while(1) {
    DEBUG_INFO("Tick %lu"DEBUG_EOL, tick_cnt++);
    vTaskDelayUntil( &next_wake_time, 1000);
  }

  // TODO: add software triggers
  //        - color selection
  //        - jack on/off
  //        - abort/stop/reset (ok with sys reset)

  for( ;; )
  {
    // Main match branching
    switch(match.state) {

    // We are barely alive, but at least we got this far
    case MATCH_STATE_RESET:
    //-------------------------------------------------------------------------

      match.state = MATCH_STATE_READY;
      break;

    // Start to do some actual business
    case MATCH_STATE_READY:
    //-------------------------------------------------------------------------

      if(SW_START == MATCH_START_INSERTED) // We don't care if this bounces
      {
        match.state = MATCH_STATE_INIT;

        led_set_mode(BB_LED_STATIC);
        led_set_color(BB_LED_WHITE);
      }
      break;

    // Wakeup everyone!
    case MATCH_STATE_INIT:
    //-------------------------------------------------------------------------

      // Set some default motion parameters
      motion_set_speed(SPEED_SLOW_D, SPEED_SLOW_A);

      // Power-up
      bb_power_up();
      motion_power_enable();

      match.state = MATCH_STATE_WAIT_START;

      led_set_mode(BB_LED_BLINK_FAST);
      led_set_color(BB_LED_GREEN);

      break;

    // I'am a big boy now
    case MATCH_STATE_SELF_CHECK:
    //-------------------------------------------------------------------------

      // TODO: define a dedicated task for that

      match.state = MATCH_STATE_WAIT_START;
      break;

    // Awaiting for doomsday
    case MATCH_STATE_WAIT_START:
    //-------------------------------------------------------------------------

      // Awaits for Start trigger
      if(SW_START == MATCH_START_REMOVED)
      {
        // We need to have at least a basic software debouncing
        // to prevent false starts. START_JACK must be 1 after the
        // given time in order to detect the match
        if(match.timer_msec >= pdMS_TO_TICKS(START_DEBOUNCING_DELAY_MSEC))
        {
          match.state = MATCH_STATE_RUN;
          led_set_mode(BB_LED_BLINK_SLOW);
          match.timer_msec = 0;
          match.scored_points = 0;

          // Sample the color, just in case the jack was removed during init
          strategy_color_sample();

          // Update all variables that depends on the color choice

          // Robot position
          reset_pos = phys.reset;
          phys_update_with_color(&reset_pos);

          // Define static obstacles positions
          //     phys_set_obstacle_positions();

          // Opponent position: probably in starting zone but we don't know
          robot.opp_pos.x = OPPONENT_POS_INIT_X;
          robot.opp_pos.y = OPPONENT_POS_INIT_Y;
          robot.opp_pos.a = 0;
          // phys_update_with_color(&robot.opp_pos);
          //    phys_set_opponent_position(1, robot.opp_pos.x, robot.opp_pos.y);

          // Reset motion position and enable motor power
          motion_set_x(reset_pos.x);
          motion_set_y(reset_pos.y);
          motion_set_a(reset_pos.a);


          phys_update_with_color(&phys.exit_start);
          phys_update_with_color(&phys.drop);
          phys_update_with_color(&phys.cube[PHYS_ID_CUBE_1]);
          phys_update_with_color(&phys.huts[PHYS_ID_HUT_1]);
          phys_update_with_color(&phys.huts[PHYS_ID_HUT_2]);

        }
        else
        {
          match.timer_msec += STRATEGY_PERIOD_MS;
        }

      }
      else
      {
        // Sample the color
        strategy_color_sample();

        // Actively reset the timer (for debouncing)
        match.timer_msec = 0;
      }
      break;

    case MATCH_STATE_RUN:
    //-------------------------------------------------------------------------

      // Count the time and execute the match strategy
      if(match.timer_msec >= MATCH_DURATION_MSEC)
      {
        match.state = MATCH_STATE_STOPPED;
        led_set_color(BB_LED_RED);
        led_set_mode(BB_LED_STATIC);
      }
      else
      {
        do_match();
        match.timer_msec += STRATEGY_PERIOD_MS;
      }
      break;

    // The party is over
    case MATCH_STATE_STOPPED:
    //-------------------------------------------------------------------------

      // We loop it because we like it
      motion_traj_hard_stop();
      motion_power_disable();
      bb_power_down();
      break;

    default:
      break;
    }

    // Block until the next strategy time period
    vTaskDelayUntil( &next_wake_time, STRATEGY_PERIOD_MS);
  }
}

void strategy_color_sample(void)
{
  if(SW_COLOR == MATCH_COLOR_BLUE)
  {
    led_set_color(BB_LED_BLUE);
    match.color = MATCH_COLOR_BLUE;
  }
  else
  {
    led_set_color(BB_LED_YELLOW);
    match.color = MATCH_COLOR_YELLOW;
  }
}

void do_match(void) {
  //static strategy_state_e strat_state = STRAT_EXIT;

  /*if(Os_MotionTrajectoryFinished()&&(strat<STRAT_FINISHED))
	{
		phys_update_with_color(&checkpoints[strat].coord.abs);
		motion_send_wp(&checkpoints[strat++]);
		if(strat>STRAT_TURN_TO_FACE_SHELLS)
		{
				ASV_DeployLeftArm();
				ASV_DeployRightArm();
		}
		else
		{
			ASV_IdleLeftArm();
			ASV_IdleRightArm();
			ASV_MoveIndex(UP);
		}
	}
	if(av_detection_is_valid())
	{
		motion_clear();
		OS_MotionTrajectoryHardStop();
		if(strat>STRAT_EXIT)
			strat--;
	}*/
} // do_match()
