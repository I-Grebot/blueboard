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

#include "../../2018_T1_R1/include/main.h"

// External variables
extern dsv_channel_t dsv_chan1;
extern dsv_channel_t dsv_chan2;

// Local private variables
sys_mod_t sys_mod;
static TaskHandle_t handle_task_sys_modules;

// Local private functions
static void sys_modules_task(void *pvParameters);



void sys_modules_init(void)
{
  // Initialize the hardware handlers
  dxl_init_servo(&sys_mod.left_arm, &dsv_chan2.dxl, "RX-28");
  dxl_init_servo(&sys_mod.right_arm, &dsv_chan2.dxl, "RX-28");
  dxl_init_servo(&sys_mod.index, &dsv_chan2.dxl, "RX-28");
  dxl_init_servo(&sys_mod.pusher, &dsv_chan2.dxl, "RX-28");

  sys_mod.left_arm.id = DSV_LEFT_ARM_ID;
  sys_mod.right_arm.id = DSV_RIGHT_ARM_ID;
  sys_mod.index.id = DSV_INDEX_ID;
  sys_mod.pusher.id = DSV_PUSHER_ID;

  sys_mod.current_state = SYS_MOD_INIT;
}


BaseType_t sys_modules_start(void)
{
	return sys_create_task(sys_modules_task, "SYS_MOD",
			                   OS_TASK_STACK_SYS_MODULES,
			                   NULL, OS_TASK_PRIORITY_SYS_MODULES, &handle_task_sys_modules );
}

void sys_modules_task(void *pvParameters)
{
  BaseType_t notified;
  uint32_t sw_notification;
  TickType_t next_wake_time = xTaskGetTickCount();

  sys_modules_init();

  ( void ) pvParameters;

  for( ;; )
  {
    // Block for a new event or during the given time
    notified = xTaskNotifyWait(0, UINT32_MAX, &sw_notification, pdMS_TO_TICKS(OS_SYS_MODULES_PERIOD_MS));

    // Manage current state transition to a new state
    switch(sys_mod.current_state)
    {
      // Must re-init after error
      case SYS_MOD_RESET:
      case SYS_MOD_ERROR:

        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_INIT))
        {
          sys_mod.current_state = SYS_MOD_INIT;
          sys_mod_proc_init();
        }

        break;

      case SYS_MOD_INIT:

        // Only manage transition to self-test (required)
        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_SELF_TEST))
        {
          sys_mod.current_state = SYS_MOD_SELF_TEST;
          sys_mod_proc_self_test();
        }

        break;


      case SYS_MOD_SELF_TEST:
        sys_mod.current_state = SYS_MOD_READY;
        DEBUG_INFO("[SYS_MOD] Ready"DEBUG_EOL);
        break;


      case SYS_MOD_READY:

        // Transition to GRAB_READY
        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_GRAB))
        {
          sys_mod.current_state = SYS_MOD_GRAB_READY;
        }

        // Transition to FOLDING
        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_FOLD))
        {
          sys_mod.current_state = SYS_MOD_FOLDING;
        }

        // Transition to LANDING
        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_LAND))
        {
          sys_mod.current_state = SYS_MOD_LANDING;
        }
        break;


      case SYS_MOD_GRAB_READY:

        // Transition to grabbing
        if(SW_SYS_MOD_DETECT_VALUE == SW_SYS_MOD_DETECT_ON)
        {
          sys_mod.current_state = SYS_MOD_GRABBING;
        }

        // Transition to folding
        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_FOLD))
        {
          sys_mod.current_state = SYS_MOD_FOLDING;
        }
        break;


      case SYS_MOD_GRABBING:
        sys_mod.current_state = SYS_MOD_LOADED;
        DEBUG_INFO("[SYS_MOD] Loaded"DEBUG_EOL);

        break;


      case SYS_MOD_LOADED:

        // Transition to LANDING
        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_LAND))
        {
          sys_mod.current_state = SYS_MOD_LANDING;
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


      default:
        sys_mod.current_state = SYS_MOD_ERROR;
        DEBUG_CRITICAL("[SYS_MOD] Error: running into an unknown state!"DEBUG_EOL);
        break;


    } // switch(current_state)

  }

}

// ----------------------------------------------------------------------------
// Actual sub-system actions
// ----------------------------------------------------------------------------

BaseType_t sys_mod_proc_init(void)
{

  DEBUG_INFO("[SYS_MOD] Initializing"DEBUG_EOL);

  // Initialize analog servos

  // Initializing digital servos
  dxl_set_speed(&sys_mod.left_arm, DSV_ARMS_SPEED_SLOW);
  dxl_set_speed(&sys_mod.right_arm, DSV_ARMS_SPEED_SLOW);
  dxl_set_speed(&sys_mod.index, DSV_INDEX_SPEED_SLOW);
  dxl_set_speed(&sys_mod.pusher, DSV_PUSHER_SPEED);

  dxl_set_torque_enable(&sys_mod.left_arm, 1);
  dxl_set_torque_enable(&sys_mod.right_arm, 1);
  dxl_set_torque_enable(&sys_mod.index, 1);
  dxl_set_torque_enable(&sys_mod.pusher, 1);

  dxl_set_torque(&sys_mod.left_arm, DSV_ARMS_TORQUE);
  dxl_set_torque(&sys_mod.right_arm, DSV_ARMS_TORQUE);
  dxl_set_torque(&sys_mod.index, DSV_ARMS_TORQUE);
  dxl_set_torque(&sys_mod.pusher, DSV_ARMS_TORQUE);

  dxl_set_position(&sys_mod.left_arm, DSV_LEFT_ARM_POS_UP);
  dxl_set_position(&sys_mod.right_arm, DSV_RIGHT_ARM_POS_UP);
  dxl_set_position(&sys_mod.index, DSV_INDEX_POS_CENTER);
  dxl_set_position(&sys_mod.pusher, DSV_PUSHER_IN);

  return pdPASS;
}

BaseType_t sys_mod_set_servo(dxl_servo_t* servo, uint16_t position)
{
  uint8_t retry=4;
  uint8_t timout=0;
  uint16_t return_position;
  uint8_t move=0;
  for(retry=4;retry>0;retry--)
  {
	  dxl_set_position(servo, position);
      vTaskDelay(pdMS_TO_TICKS(100));
	  is_dxl_moving(servo, &move);
	  vTaskDelay(pdMS_TO_TICKS(100));
	  if(move>0)
	  {
		  for(timout=0;timout<40;timout++)
		  {
			  dxl_get_position(servo, &return_position);	// Check position
			  if(return_position<=position+5 && return_position>=position-5)
				  return pdPASS;
			  vTaskDelay(pdMS_TO_TICKS(100));
		  }
		  return pdFAIL;
	  }
  }
  return pdFAIL;
}

BaseType_t sys_mod_set_left_arm(uint16_t position)
{
  return sys_mod_set_servo(&sys_mod.left_arm,position);
}

BaseType_t sys_mod_set_right_arm(uint16_t position)
{
  return sys_mod_set_servo(&sys_mod.right_arm,position);
}

BaseType_t sys_mod_set_index(uint16_t position)
{
  return sys_mod_set_servo(&sys_mod.index,position);
}

BaseType_t sys_mod_set_pusher(uint16_t position)
{
  return sys_mod_set_servo(&sys_mod.pusher,position);
}


// Launch a series of self-test actions
BaseType_t sys_mod_proc_self_test(void)
{

  DEBUG_INFO("[SYS_MOD] Start Self-Test"DEBUG_EOL);

  // Test
  sys_mod_set_index(DSV_INDEX_POS_LEFT);
  sys_mod_set_left_arm(DSV_LEFT_ARM_POS_DOWN);
  sys_mod_set_index(DSV_INDEX_POS_RIGHT);
  sys_mod_set_right_arm(DSV_RIGHT_ARM_POS_DOWN);
  sys_mod_set_index(DSV_INDEX_POS_CENTER);
  sys_mod_set_pusher(DSV_PUSHER_OUT);
  dxl_set_speed(&sys_mod.index, DSV_INDEX_SPEED_FAST);
  vTaskDelay(pdMS_TO_TICKS(200));
  dxl_set_speed(&sys_mod.left_arm, DSV_ARMS_SPEED_FAST);
  vTaskDelay(pdMS_TO_TICKS(200));
  dxl_set_speed(&sys_mod.right_arm, DSV_ARMS_SPEED_FAST);
  vTaskDelay(pdMS_TO_TICKS(200));
  sys_mod_set_index(DSV_INDEX_POS_LEFT);
  sys_mod_set_left_arm(DSV_LEFT_ARM_POS_UP);
  sys_mod_set_index(DSV_INDEX_POS_RIGHT);
  sys_mod_set_right_arm(DSV_RIGHT_ARM_POS_UP);
  sys_mod_set_index(DSV_INDEX_POS_CENTER);
  sys_mod_set_pusher(DSV_PUSHER_IN);

  return pdPASS;
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
