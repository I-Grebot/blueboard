/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       sys_modules.c
 * @author     Paul
 * @date       2017/05/19
 * -----------------------------------------------------------------------------
 * @brief
 *   This system module handles the lunar modules based on:
 *   - 2x XL-320 digital servos for the gripper
 *   - 1x ServoIFace (asv if) for the trollet
 *   - 1x RX-28 digital servo for the lifter
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "main.h"

// External variables
extern dsv_channel_t dsv_chan1;
extern dsv_channel_t dsv_chan2;

// Local private variables
static sys_mod_t sys_mod;
static TaskHandle_t handle_task_sys_modules;

// Local private functions
static void sys_modules_task(void *pvParameters);
static void sys_modules_init(void);


void sys_modules_init(void)
{
  // Initialize the hardware handlers
  asv_init_servo(&sys_mod.rotator, ASV_CHANNEL_ROTATOR, ASV_ROTATOR_MIN, ASV_ROTATOR_MAX);
  asv_init_servo(&sys_mod.trollet, ASV_CHANNEL_TROLLET, ASV_TROLLET_MIN, ASV_TROLLET_MAX);
  dxl_init_servo(&sys_mod.grabber_left, &dsv_chan1.dxl, "XL320");
  dxl_init_servo(&sys_mod.grabber_right, &dsv_chan1.dxl, "XL320");
  dxl_init_servo(&sys_mod.lander, &dsv_chan2.dxl, "RX-28");


  sys_mod.current_state = SYS_MOD_INIT;

}

BaseType_t sys_modules_start(void)
{
	return sys_create_task(sys_modules_task, "SYS_MOD",
			                   OS_TASK_STACK_SYS_MODULES,
			                   NULL, OS_TASK_PRIORITY_AVOIDANCE, &handle_task_sys_modules );
}

void sys_modules_task(void *pvParameters)
{
  TickType_t next_wake_time = xTaskGetTickCount();

  BaseType_t notified;
  uint32_t sw_notification;

  sys_modules_init();

  ( void ) pvParameters;

  for( ;; )
  {
    // Block for a new event or during the given time
    notified = xTaskNotifyWait(0, UINT32_MAX, &sw_notification, pdMS_TO_TICKS(OS_SYS_MODULES_PERIOD_MS));

    // Manage current state transition to a new state
    switch(sys_mod.current_state)
    {
      case SYS_MOD_INIT:

        // Only manage transition to self-test (required)
        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_SELF_TEST))
        {
          sys_mod.current_state = SYS_MOD_SELF_TEST;
          DEBUG_INFO("[SYS_MOD] Start Self-Test"DEBUG_EOL);
        }

        break;


      case SYS_MOD_SELF_TEST:

        // TODO
        sys_mod.current_state = SYS_MOD_READY;
        DEBUG_INFO("[SYS_MOD] Ready"DEBUG_EOL);
        break;


      case SYS_MOD_READY:

        // Transition to GRAB_READY
        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_GRAB))
        {
          //...
          // TODO
          //...
          sys_mod.current_state = SYS_MOD_GRAB_READY;
          DEBUG_INFO("[SYS_MOD] Grab Ready"DEBUG_EOL);
        }

        // Transition to FOLDING
        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_FOLD))
        {
          //...
          // TODO
          //...
          sys_mod.current_state = SYS_MOD_FOLDING;
          DEBUG_INFO("[SYS_MOD] Folding"DEBUG_EOL);
        }

        // Transition to LANDING
        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_LAND))
        {
          //...
          // TODO
          //...
          sys_mod.current_state = SYS_MOD_LANDING;
          DEBUG_INFO("[SYS_MOD] Landing"DEBUG_EOL);
        }
        break;


      case SYS_MOD_GRAB_READY:
        if(SW_SYS_MOD_DETECT_VALUE == SW_SYS_MOD_DETECT_ON)
        {
          sys_mod.current_state = SYS_MOD_GRABBING;
          DEBUG_INFO("[SYS_MOD] Detection! Grabbing"DEBUG_EOL);
        }
        break;


      case SYS_MOD_GRABBING:

        //...
        // TODO
        //...
        sys_mod.current_state = SYS_MOD_LOADED;
        DEBUG_INFO("[SYS_MOD] Loaded"DEBUG_EOL);

        break;



      case SYS_MOD_LOADED:

        // Transition to LANDING
        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_LAND))
        {
          //...
          // TODO
          //...
          sys_mod.current_state = SYS_MOD_LANDING;
          DEBUG_INFO("[SYS_MOD] Landing"DEBUG_EOL);
        }

        break;


      case SYS_MOD_LANDING:

        //...
        // TODO
        //...
        sys_mod.current_state = SYS_MOD_READY;
        DEBUG_INFO("[SYS_MOD] Ready"DEBUG_EOL);

        break;


      case SYS_MOD_FOLDING:

        //...
        // TODO
        //...
        sys_mod.current_state = SYS_MOD_READY;
        DEBUG_INFO("[SYS_MOD] Ready"DEBUG_EOL);

        break;


      case SYS_MOD_ERROR:
      default:
        break;


    } // switch(current_state)

    // Perform actions for the current state


  }

}

// ----------------------------------------------------------------------------
// Public accessors
// ----------------------------------------------------------------------------

void sys_mod_do_init(TaskHandle_t* caller)
{
  sys_mod.calling_task = caller;
  xTaskNotify(handle_task_sys_modules, OS_NOTIFY_SYS_MOD_INIT, eSetBits);
}

void sys_mod_do_self_test(TaskHandle_t* caller)
{
  sys_mod.calling_task = caller;
  xTaskNotify(handle_task_sys_modules, OS_NOTIFY_SYS_MOD_SELF_TEST, eSetBits);
}

void sys_mod_do_grab(TaskHandle_t* caller, uint16_t grab_pos)
{
  sys_mod.calling_task = caller;
  sys_mod.grab_pos = grab_pos;
  xTaskNotify(handle_task_sys_modules, OS_NOTIFY_SYS_MOD_GRAB, eSetBits);
}

void sys_mod_do_land(TaskHandle_t* caller, uint16_t land_pos, uint16_t land_angle)
{
  sys_mod.calling_task = caller;
  sys_mod.land_pos = land_pos;
  sys_mod.land_angle = land_angle;
  xTaskNotify(handle_task_sys_modules, OS_NOTIFY_SYS_MOD_LAND, eSetBits);
}

void sys_mod_do_fold(TaskHandle_t* caller)
{
  sys_mod.calling_task = caller;
  xTaskNotify(handle_task_sys_modules, OS_NOTIFY_SYS_MOD_FOLD, eSetBits);
}
