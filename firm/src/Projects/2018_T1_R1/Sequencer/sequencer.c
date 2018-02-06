/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       sequencer.c
 * @author     Pierrick
 * @date       21/01/2018
 * -----------------------------------------------------------------------------
 * @brief
 *   This module implements the main sequencer of the robot
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

// Strategy task handle
TaskHandle_t handle_task_sequencer;

// Main structures
robot_t robot;
match_t match;

extern task_mgt_t task_mgt;

// Local, Private functions
static void sequencer_init(void);
static void sequencer_task(void *pvParameters);
static void sequencer_color_sample(void);


/**
********************************************************************************
**
**  Initialization
**
********************************************************************************
*/

BaseType_t sequencer_start(void)
{
  // Start main sequencer task
  return sys_create_task(sequencer_task, "SEQUENCER", OS_TASK_STACK_SEQUENCER, NULL, OS_TASK_PRIORITY_SEQUENCER, &handle_task_sequencer);
}

void sequencer_init(void)
{
  // Initialize the robot structure
  memset(&robot, 0, sizeof(robot_t));

  // Initialize the match content fields
  match.color = MATCH_COLOR_UNKNOWN;
  match.state = MATCH_STATE_RESET;
  match.paused = false;
  match.scored_points = 0;
  match.timer_msec = 0;
  match.sw_init = false;

}

/**
********************************************************************************
**
**  Main Strategy
**
********************************************************************************
*/

void sequencer_task( void *pvParameters )
{
  TickType_t next_wake_time = xTaskGetTickCount();

  BaseType_t notified;
  uint32_t sw_notification;

  uint32_t tick_cnt = 0;

  bool start_sw_debounced;

  // Remove compiler warning about unused parameter.
  ( void ) pvParameters;

  // Ensure shell is up, so that we can display init messages
  vTaskDelayUntil(&next_wake_time, pdMS_TO_TICKS(OS_SHELL_BOOT_WAIT_MS));

  // Start modules initialization

  sequencer_init();

  //dsv_init();
  //asv_start();
  //beacons_start();
  motion_cs_start();
  motion_traj_start();
  monitoring_start();
  led_start();

  // Sub-systems
  //sys_modules_start();
  //sys_modules_init();

  // High-level
  //avoidance_start();
  path_init();
  phys_init();
  //tasks_init();

  // Sequencer main loop
  for( ;; )
  {
    // Wait for potential new notification, block for the strategy evaluation period
    notified = xTaskNotifyWait(0, UINT32_MAX, &sw_notification, pdMS_TO_TICKS(OS_SEQUENCER_PERIOD_MS));

    // Main match branching
    switch(match.state) {

    // We are barely alive, but at least we got this far
    case MATCH_STATE_RESET:
    //-------------------------------------------------------------------------

      match.state = MATCH_STATE_READY;
      sequencer_print_match();
      break;

    // Start to do some actual business
    case MATCH_STATE_READY:
    //-------------------------------------------------------------------------

      if(notified && (sw_notification & OS_NOTIFY_INIT_START))
      {
        match.sw_init = true;
      }

      sequencer_color_sample();

      if((SW_START == MATCH_START_INSERTED) ||  // Hardware init
         match.sw_init)                         // Software init
      {
        match.state = MATCH_STATE_INIT;

        sequencer_print_match();
        //led_set_mode(BB_LED_STATIC);
        //led_set_color(BB_LED_WHITE);
      }
      break;

    // Wakeup everyone!
    case MATCH_STATE_INIT:
    //-------------------------------------------------------------------------

      // Sample color at init only
      sequencer_color_sample();
      motion_power_enable();
      //ai_init();

      match.state = MATCH_STATE_SELF_CHECK;

      sequencer_print_match();
      //led_set_mode(BB_LED_BLINK_FAST);
      //led_set_color(BB_LED_GREEN);

      break;

    // I'am a big boy now
    case MATCH_STATE_SELF_CHECK:
    //-------------------------------------------------------------------------

      //ai_self_test();

      match.state = MATCH_STATE_WAIT_START;

      sequencer_print_match();
      break;

    // Awaiting for doomsday
    case MATCH_STATE_WAIT_START:
    //-------------------------------------------------------------------------

      // Manage a software debounce for the start switch
      // We need to have at least a basic software debouncing
      // to prevent false starts. START_JACK must be 1 after the
      // given time in order to detect the match
      //
      // Note: when init is performed by software, the match must be also
      //       started by software (to avoid headaches...)
      if(!match.sw_init && (SW_START == MATCH_START_REMOVED))
      {
        if(match.timer_msec >= pdMS_TO_TICKS(START_DEBOUNCING_DELAY_MSEC))
        {
          start_sw_debounced = true;
        } else {
          match.timer_msec += OS_SEQUENCER_PERIOD_MS;
        }

      } else {
        //sequencer_color_sample();
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
        //sequencer_color_sample();

        //ai_start();

        sequencer_print_match();
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

        // Call AI ending stuff
        //ai_stop();

        sequencer_print_match();
        led_set_color(BB_LED_RED);
        led_set_mode(BB_LED_STATIC);
      }
      else
      {
        // Main AI management call
       // ai_manage(notified, sw_notification);

        if(!match.paused) {
          match.timer_msec += OS_SEQUENCER_PERIOD_MS;
        }

        // Print match status every sec
        if(!(match.timer_msec % 1000)) {
          sequencer_print_match();
        }

      }
      break;

    // The party is over
    case MATCH_STATE_STOPPED:
    //-------------------------------------------------------------------------

      // Nothing to do, maybe software restart? TBD

      break;

    // Error, actually
    default:
      break;

    } // switch(match)

  } // for(;;)
}

/**
********************************************************************************
**
**  Helpers
**
********************************************************************************
*/

void sequencer_color_sample(void)
{
  // When the match is initialized by software, avoid latching the color
  // (value is directly set)
  if(!match.sw_init)
  {
    if(SW_COLOR == MATCH_COLOR_GREEN)
    {
      match.color = MATCH_COLOR_GREEN;
    }
    else
    {
      match.color = MATCH_COLOR_ORANGE;
    }
  }

  // Display the selected color
  if(match.color == MATCH_COLOR_GREEN)
  {
    led_set_color(BB_LED_GREEN);
  } else {
    led_set_color(BB_LED_YELLOW);
  }

}

// Print current match state
void sequencer_print_match(void)
{
  DEBUG_INFO_NOPFX("[MATCH] %-16s %-8s %-16s %-03u %-05u"DEBUG_EOL,
      match_state_to_str(match.state),
      match_color_to_str(match.color),
      task_mgt.active_task == NULL ? "IDLE" : task_mgt.active_task->name,
      match.timer_msec/1000,
      match.scored_points
      );

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
    case MATCH_COLOR_GREEN:      return "GREEN"; break;
    case MATCH_COLOR_ORANGE:     return "ORANGE"; break;
    case MATCH_COLOR_UNKNOWN:    return "UNKNOWN"; break;
    default:                     return "NA"; break;
  }
}
