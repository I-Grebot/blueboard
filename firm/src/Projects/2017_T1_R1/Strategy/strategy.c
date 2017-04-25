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

#include "main.h"

/**
********************************************************************************
**
**  Globals
**
********************************************************************************
*/

// Strategy task handle
TaskHandle_t handle_task_strategy;

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

extern TaskHandle_t handle_task_avoidance;

/**
********************************************************************************
**
**  Initialization
**
********************************************************************************
*/

BaseType_t strategy_start(void)
{
  BaseType_t ret;

  strategy_init();
  // path_init();

  // Start main strategy task
  ret = xTaskCreate(strategy_task, "STRATEGY", OS_TASK_STACK_STRATEGY, NULL, OS_TASK_PRIORITY_STRATEGY, &handle_task_strategy );

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
  match.paused = false;
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

/**
********************************************************************************
**
**  Main Strategy
**
********************************************************************************
*/

void strategy_task( void *pvParameters )
{
  TickType_t next_wake_time = xTaskGetTickCount();;
  poi_t reset_pos;

  BaseType_t notified;
  uint32_t sw_notification;

  uint32_t tick_cnt = 0;

  bool start_sw_debounced;

  /* Remove compiler warning about unused parameter. */
  ( void ) pvParameters;

  led_set_mode(BB_LED_OFF);

  // Launch IDLE task
  ai_task_launch(&tasks[TASK_ID_IDLE]);

  // TEMP
  /*while(1) {
    DEBUG_INFO("Tick %lu"DEBUG_EOL, tick_cnt+=5);
    // Check for software notifiers from other tasks
    if(notified == pdTRUE)
    {
      if(sw_notification & 0x01)
      {
        DEBUG_INFO("Notification from avoidance!"DEBUG_EOL);

      }

    }
  }*/

  // TODO: add software triggers
  //        - color selection
  //        - jack on/off
  //        - abort/stop/reset (ok with sys reset)

  for( ;; )
  {
    // Wait for potential new notification, block for the strategy evaluation period
    notified = xTaskNotifyWait(0, UINT32_MAX, &sw_notification, pdMS_TO_TICKS(OS_STRATEGY_PERIOD_MS));

    // Main match branching
    switch(match.state) {

    // We are barely alive, but at least we got this far
    case MATCH_STATE_RESET:
    //-------------------------------------------------------------------------

      match.state = MATCH_STATE_READY;
      strategy_print_match();
      break;

    // Start to do some actual business
    case MATCH_STATE_READY:
    //-------------------------------------------------------------------------

      if((SW_START == MATCH_START_INSERTED) ||                    // Hardware init
         (notified && (sw_notification & OS_NOTIFY_INIT_START)))  // Software init
      {
        match.state = MATCH_STATE_INIT;

        strategy_print_match();
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

      strategy_print_match();
      led_set_mode(BB_LED_BLINK_FAST);
      led_set_color(BB_LED_GREEN);

      break;

    // I'am a big boy now
    case MATCH_STATE_SELF_CHECK:
    //-------------------------------------------------------------------------

      // TODO: define a dedicated task for that

      match.state = MATCH_STATE_WAIT_START;

      strategy_print_match();
      break;

    // Awaiting for doomsday
    case MATCH_STATE_WAIT_START:
    //-------------------------------------------------------------------------

      // Manage a software debounce for the start switch
      // We need to have at least a basic software debouncing
      // to prevent false starts. START_JACK must be 1 after the
      // given time in order to detect the match
      if(SW_START == MATCH_START_REMOVED)
      {
        if(match.timer_msec >= pdMS_TO_TICKS(START_DEBOUNCING_DELAY_MSEC))
        {
          start_sw_debounced = true;
        } else {
          match.timer_msec += OS_STRATEGY_PERIOD_MS;
        }

      } else {
        strategy_color_sample();
        match.timer_msec = 0;
        start_sw_debounced = false;
      }

      // Conditions for starting the match
      if( start_sw_debounced ||                                    // Hardware start
         (notified && (sw_notification & OS_NOTIFY_MATCH_START)))  // Software start
      {

        match.state = MATCH_STATE_RUN;

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


        strategy_print_match();
        led_set_mode(BB_LED_BLINK_SLOW);
      }

      break;

    case MATCH_STATE_RUN:
    //-------------------------------------------------------------------------

      // Count the time and execute the match strategy
      if((match.timer_msec >= MATCH_DURATION_MSEC) ||               // Normal match timeout end
         (notified && (sw_notification & OS_NOTIFY_MATCH_ABORT)))   // Software stop (abort)
      {
        match.state = MATCH_STATE_STOPPED;
        led_set_color(BB_LED_RED);
        led_set_mode(BB_LED_STATIC);
      }
      else
      {
        do_match(notified, sw_notification);
        match.timer_msec += OS_STRATEGY_PERIOD_MS;
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

    } // switch(match)

  } // for(;;)
}

void do_match(bool notified, uint32_t sw_notification)
{

  // Manage task notifications
  if(notified)
  {
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

  }




/*  // Holder for the current task being executed
  static task_t* current_task = &tasks[TASK_ID_START];

  // Manage the task, i.e. ensure all micro-transitions, motions & actuators stuff
  //do_task(current_task);

  // Check the avoidance state
  // After a detection occurs, the strategy needs to handle the following things:
  // - Switch the current task to the SUSPENDED state
  // - Increase the task's trials count.
  // - Apply the A.I. "on_suspend" policy, which can contain failure conditions
  //   or other dependencies freeing conditions
  if(av.state == AV_STATE_DETECT) {
    current_task->state = TASK_STATE_SUSPENDED;

    // Normal cases: the robot is allowed to move again
    // we can also apply (once) the policy after suspend
  } else {

    // Clear avoidance state, so another detection can be triggered
    if(av.state == AV_STATE_REROUTE) {
      sw_irq |= IRQ_AV_CLEAR;
    }

    // Depending on the state of the task we need to do somethings different
    switch(current_task->state) {

    // On-going task, do nothing except for idle task
    case TASK_STATE_RUNNING:
      break;

      // Apply the dedicated A.I. policy
    case TASK_STATE_SUSPENDED:
      ai_on_suspend_policy(current_task);
      break;

      // Apply the dedicated A.I. policy
    case TASK_STATE_FAILED:
      ai_on_failure_policy(current_task);
      break;

      // Everything went fine, no specific policy to be applied
    case TASK_STATE_SUCCESS:
      break;

      // All the cases that should not happen!
    default:
    case TASK_STATE_INACTIVE: // Simply because the current task cannot be inactive
    case TASK_STATE_START:    // After the 1st task_manage(), task should be RUNNING
      break;
    } // switch current_task


    // TEMPORARY spam
    can_send_task_rpt(current_task);

    // States after which we need to find a new task and start it
    if((current_task->state == TASK_STATE_SUSPENDED) ||
        (current_task->state == TASK_STATE_SUCCESS)   ||
        (current_task->state == TASK_STATE_FAILED)) {

      // Retrieve a new task. This function can have mainly 3 different issues:
      // - Next task is INACTIVE (fresh), simply start execution
      // - Next task was SUSPENDED, we need to clean up before starting it again
      // - No correct task was found and the IDLE task is returned (do nothing special)
      current_task = task_get_next();

      // Cleanup flags & previous execution traces
      if(current_task->state == TASK_STATE_SUSPENDED) {
        ai_task_restart(current_task);
      }

      current_task->state = TASK_STATE_START;

    }

  } // av.state
*/

} // do_match()

/**
********************************************************************************
**
**  Helpers
**
********************************************************************************
*/

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

// Print current match state
void strategy_print_match(void)
{
  DEBUG_INFO("[MATCH] %16s %8s %-03u %-05u",
      match_state_to_str(match.state),
      match_color_to_str(match.color),
      match.timer_msec/1000,
      match.scored_points
      )

}

const char* match_state_to_str(match_state_e state)
{
  switch(state)
  {
    case MATCH_STATE_RESET:       return "RESET"; break;
    case MATCH_STATE_READY:       return "READY"; break;
    case MATCH_STATE_INIT:        return "INIT"; break;
    case MATCH_STATE_SELF_CHECK:  return "SELF-CHECK"; break;
    case MATCH_STATE_WAIT_START:  return "WAIT-START"; break;
    case MATCH_STATE_RUN:         return "RUN"; break;
    case MATCH_STATE_STOPPED:     return "STOPPED"; break;
    default:                      return "NA"; break;
  }
}

const char* match_color_to_str(match_color_e color)
{
  switch(color)
  {
    case MATCH_COLOR_BLUE:       return "BLUE"; break;
    case MATCH_COLOR_YELLOW:     return "YELLOW"; break;
    case MATCH_COLOR_UNKNOWN:    return "UNKNOWN"; break;
    default:                     return "NA"; break;
  }
}
