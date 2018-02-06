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
  asv_init_servo(&sys_mod.rotator, ASV_CHANNEL_ROTATOR, ASV_ROTATOR_MIN, ASV_ROTATOR_MAX);
  asv_init_servo(&sys_mod.trollet, ASV_CHANNEL_TROLLET, ASV_TROLLET_MIN, ASV_TROLLET_MAX);
  dxl_init_servo(&sys_mod.grabber_left, &dsv_chan1.dxl, "XL320");
  dxl_init_servo(&sys_mod.grabber_right, &dsv_chan1.dxl, "XL320");
  dxl_init_servo(&sys_mod.grabber_back_left_left, &dsv_chan1.dxl, "XL320");
  dxl_init_servo(&sys_mod.grabber_back_left_right, &dsv_chan1.dxl, "XL320");
  dxl_init_servo(&sys_mod.grabber_back_right_left, &dsv_chan1.dxl, "XL320");
  dxl_init_servo(&sys_mod.grabber_back_right_right, &dsv_chan1.dxl, "XL320");
  dxl_init_servo(&sys_mod.lander, &dsv_chan2.dxl, "RX-28");

  sys_mod.grabber_left.id = DSV_GRABBER_LEFT_ID;
  sys_mod.grabber_right.id = DSV_GRABBER_RIGHT_ID;
  sys_mod.grabber_back_left_left.id = DSV_GRABBER_BACK_LEFT_LEFT_ID;
  sys_mod.grabber_back_left_right.id = DSV_GRABBER_BACK_LEFT_RIGHT_ID;
  sys_mod.grabber_back_right_left.id = DSV_GRABBER_BACK_RIGHT_LEFT_ID;
  sys_mod.grabber_back_right_right.id = DSV_GRABBER_BACK_RIGHT_RIGHT_ID;
  sys_mod.lander.id = DSV_LANDER_ID;

  sys_mod.grab_pos = 0;
  sys_mod.land_pos = 0;
  sys_mod.land_angle = 0;
  sys_mod.current_state = SYS_MOD_INIT;

}

void sys_mod_set_trollet_cmd(uint8_t cmd)
{
  SW_TROLLET_CMD1(cmd & 0x02 ? Bit_SET: Bit_RESET);
  SW_TROLLET_CMD0(cmd & 0x01 ? Bit_SET: Bit_RESET);
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
          sys_mod_proc_prepare_grab();
        }

        // Transition to FOLDING
        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_FOLD))
        {
          sys_mod.current_state = SYS_MOD_FOLDING;
          sys_mod_proc_fold();
        }

        // Transition to LANDING
        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_LAND))
        {
          sys_mod.current_state = SYS_MOD_LANDING;
          sys_mod_proc_land();
        }
        break;


      case SYS_MOD_GRAB_READY:

        // Transition to grabbing
        if(SW_SYS_MOD_DETECT_VALUE == SW_SYS_MOD_DETECT_ON)
        {
          sys_mod.current_state = SYS_MOD_GRABBING;
          sys_mod_proc_grab();
        }

        // Transition to folding
        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_FOLD))
        {
          sys_mod.current_state = SYS_MOD_FOLDING;
          sys_mod_proc_fold();
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
          sys_mod_proc_land();
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
  bb_asv_set_pwm_pulse_length(sys_mod.rotator.channel, ASV_ROTATOR_90);
  //bb_asv_set_pwm_pulse_length(sys_mod.trollet.channel, ASV_TROLLET_LEFT);

  // Initializing digital servos
  dxl_set_speed(&sys_mod.grabber_left, DSV_GRABBERS_SPEED);
  dxl_set_speed(&sys_mod.grabber_right, DSV_GRABBERS_SPEED);
  dxl_set_speed(&sys_mod.grabber_back_left_left, DSV_GRABBERS_SPEED);
  dxl_set_speed(&sys_mod.grabber_back_left_right, DSV_GRABBERS_SPEED);
  dxl_set_speed(&sys_mod.grabber_back_right_left, DSV_GRABBERS_SPEED);
  dxl_set_speed(&sys_mod.grabber_back_right_right, DSV_GRABBERS_SPEED);
  dxl_set_speed(&sys_mod.lander, DSV_LANDER_SPEED_DOWN);

  dxl_set_torque_enable(&sys_mod.grabber_left, 1);
  dxl_set_torque_enable(&sys_mod.grabber_right, 1);
  dxl_set_torque_enable(&sys_mod.grabber_back_left_left, 1);
  dxl_set_torque_enable(&sys_mod.grabber_back_left_right, 1);
  dxl_set_torque_enable(&sys_mod.grabber_back_right_left, 1);
  dxl_set_torque_enable(&sys_mod.grabber_back_right_right, 1);
  dxl_set_torque_enable(&sys_mod.lander, 1);

  dxl_set_torque(&sys_mod.grabber_left, DSV_GRABBERS_TORQUE);
  dxl_set_torque(&sys_mod.grabber_right, DSV_GRABBERS_TORQUE);
  dxl_set_torque(&sys_mod.grabber_back_left_left, DSV_GRABBERS_TORQUE);
  dxl_set_torque(&sys_mod.grabber_back_left_right, DSV_GRABBERS_TORQUE);
  dxl_set_torque(&sys_mod.grabber_back_right_left, DSV_GRABBERS_TORQUE);
  dxl_set_torque(&sys_mod.grabber_back_right_right, DSV_GRABBERS_TORQUE);
  dxl_set_position(&sys_mod.lander, DSV_LANDER_TORQUE);

  /*
  dxl_set_position(&sys_mod.grabber_left, DSV_GRABBER_LEFT_POS_OPENED);
  dxl_set_position(&sys_mod.grabber_right, DSV_GRABBER_RIGHT_POS_OPENED);
  dxl_set_position(&sys_mod.grabber_back_left_left, DSV_GRABBER_BACK_LEFT_LEFT_POS_OPENED);
  dxl_set_position(&sys_mod.grabber_back_left_right, DSV_GRABBER_BACK_LEFT_RIGHT_POS_OPENED);
  dxl_set_position(&sys_mod.grabber_back_right_left, DSV_GRABBER_BACK_RIGHT_LEFT_POS_OPENED);
  dxl_set_position(&sys_mod.grabber_back_right_right, DSV_GRABBER_BACK_RIGHT_RIGHT_POS_OPENED);
  */

  dxl_set_position(&sys_mod.lander, DSV_LANDER_POS_UP);

  dxl_set_led(&sys_mod.grabber_left, DXL_LED_CYAN);
  dxl_set_led(&sys_mod.grabber_right, DXL_LED_CYAN);
  dxl_set_led(&sys_mod.grabber_back_left_left, DXL_LED_CYAN);
  dxl_set_led(&sys_mod.grabber_back_left_right, DXL_LED_CYAN);
  dxl_set_led(&sys_mod.grabber_back_right_left, DXL_LED_CYAN);
  dxl_set_led(&sys_mod.grabber_back_right_right, DXL_LED_CYAN);
  dxl_set_led(&sys_mod.lander, DXL_LED_RED);

  sys_mod_set_trollet_cmd(SW_TROLLET_GOTO_RIGHT);

  return pdPASS;
}

void sys_mod_set_led(uint8_t led)
{
  dxl_set_led(&sys_mod.grabber_left, led);
  dxl_set_led(&sys_mod.grabber_right, led);
}

// Launch a series of self-test actions
BaseType_t sys_mod_proc_self_test(void)
{

  DEBUG_INFO("[SYS_MOD] Start Self-Test"DEBUG_EOL);

  // Test
  vTaskDelay(pdMS_TO_TICKS(1000));

  return pdPASS;
}

// Prepare for the grab, position the system in the correct position
BaseType_t sys_mod_proc_prepare_grab(void)
{
  //DEBUG_INFO("[SYS_MOD] Prepare Grab at %u"DEBUG_EOL, sys_mod.grab_pos);

  // Up position
  dxl_set_speed(&sys_mod.lander, DSV_LANDER_SPEED_UP);
  dxl_set_position(&sys_mod.lander, DSV_LANDER_POS_UP);
  bb_asv_set_pwm_pulse_length(sys_mod.rotator.channel, ASV_ROTATOR_0);
  sys_mod_set_trollet_cmd(SW_TROLLET_GOTO_MIDDLE);

  // Open grabbers
  dxl_set_position(&sys_mod.grabber_left, DSV_GRABBER_LEFT_POS_OPENED);
  dxl_set_position(&sys_mod.grabber_right, DSV_GRABBER_RIGHT_POS_OPENED);

  vTaskDelay(pdMS_TO_TICKS(300));

  // Down position
  dxl_set_speed(&sys_mod.lander, DSV_LANDER_SPEED_DOWN);
  dxl_set_position(&sys_mod.lander, DSV_LANDER_POS_DOWN);

  // Trollet at requested pos
  sys_mod_set_trollet_cmd(SW_TROLLET_GOTO_MIDDLE);

  dxl_set_led(&sys_mod.grabber_left, DXL_LED_BLUE);
  dxl_set_led(&sys_mod.grabber_right, DXL_LED_BLUE);

  return pdPASS;
}

// Actual action for grabbing a module
BaseType_t sys_mod_proc_grab(void)
{
  //DEBUG_INFO("[SYS_MOD] Detection! Grabbing at %u"DEBUG_EOL, sys_mod.grab_pos);

  dxl_set_led(&sys_mod.grabber_left, DXL_LED_GREEN);
  dxl_set_led(&sys_mod.grabber_right, DXL_LED_GREEN);

  // Close grabbers
  dxl_set_position(&sys_mod.grabber_left, DSV_GRABBER_LEFT_POS_CLOSED);
  dxl_set_position(&sys_mod.grabber_right, DSV_GRABBER_RIGHT_POS_CLOSED);

  // TODO: replace with actual check (read) on correct closure
  vTaskDelay(pdMS_TO_TICKS(300));

  // Up position
  dxl_set_speed(&sys_mod.lander, DSV_LANDER_SPEED_UP);
  dxl_set_position(&sys_mod.lander, DSV_LANDER_POS_UP);

  bb_asv_set_pwm_pulse_length(sys_mod.rotator.channel, ASV_ROTATOR_90);

  return pdPASS;
}

BaseType_t sys_mod_proc_grab_back(bool left_not_right)
{
  dxl_servo_t* grabber_left;
  dxl_servo_t* grabber_right;
  uint16_t grab_pos_left;
  uint16_t grab_pos_right;

  if(left_not_right)
  {
    grabber_left = &sys_mod.grabber_back_left_left;
    grabber_right = &sys_mod.grabber_back_left_right;
    grab_pos_left = DSV_GRABBER_BACK_LEFT_LEFT_POS_CLOSED;
    grab_pos_right = DSV_GRABBER_BACK_LEFT_RIGHT_POS_CLOSED;

  } else {
    grabber_left = &sys_mod.grabber_back_right_left;
    grabber_right = &sys_mod.grabber_back_right_right;
    grab_pos_left = DSV_GRABBER_BACK_RIGHT_LEFT_POS_CLOSED;
    grab_pos_right = DSV_GRABBER_BACK_RIGHT_RIGHT_POS_CLOSED;
  }

  dxl_set_led(grabber_left, DXL_LED_GREEN);
  dxl_set_led(grabber_right, DXL_LED_GREEN);

  // Close grabbers
  dxl_set_position(grabber_left, grab_pos_left);
  dxl_set_position(grabber_right, grab_pos_right);

  vTaskDelay(pdMS_TO_TICKS(300));
}


BaseType_t sys_mod_proc_close_back(bool left_not_right)
{
  dxl_servo_t* grabber_left;
  dxl_servo_t* grabber_right;
  uint16_t grab_pos_left;
  uint16_t grab_pos_right;

  if(left_not_right)
  {
    grabber_left = &sys_mod.grabber_back_left_left;
    grabber_right = &sys_mod.grabber_back_left_right;
    grab_pos_left = DSV_GRABBER_BACK_LEFT_LEFT_POS_CLOSED_FULL;
    grab_pos_right = DSV_GRABBER_BACK_LEFT_RIGHT_POS_CLOSED_FULL;

  } else {
    grabber_left = &sys_mod.grabber_back_right_left;
    grabber_right = &sys_mod.grabber_back_right_right;
    grab_pos_left = DSV_GRABBER_BACK_RIGHT_LEFT_POS_CLOSED_FULL;
    grab_pos_right = DSV_GRABBER_BACK_RIGHT_RIGHT_POS_CLOSED_FULL;
  }

  dxl_set_led(grabber_left, DXL_LED_BLUE);
  dxl_set_led(grabber_right, DXL_LED_BLUE);

  // Close grabbers
  dxl_set_position(grabber_left, grab_pos_left);
  vTaskDelay(pdMS_TO_TICKS(300));
  dxl_set_position(grabber_right, grab_pos_right);
  vTaskDelay(pdMS_TO_TICKS(300));
}

BaseType_t sys_mod_proc_release_back(bool left_not_right)
{
  dxl_servo_t* grabber_left;
  dxl_servo_t* grabber_right;
  uint16_t grab_pos_left;
  uint16_t grab_pos_right;

  if(left_not_right)
  {
    grabber_left = &sys_mod.grabber_back_left_left;
    grabber_right = &sys_mod.grabber_back_left_right;
    grab_pos_left = DSV_GRABBER_BACK_LEFT_LEFT_POS_OPENED;
    grab_pos_right = DSV_GRABBER_BACK_LEFT_RIGHT_POS_OPENED;
  } else {
    grabber_left = &sys_mod.grabber_back_right_left;
    grabber_right = &sys_mod.grabber_back_right_right;
    grab_pos_left = DSV_GRABBER_BACK_RIGHT_LEFT_POS_OPENED;
    grab_pos_right = DSV_GRABBER_BACK_RIGHT_RIGHT_POS_OPENED;
  }


  dxl_set_led(grabber_left, DXL_LED_YELLOW);
  dxl_set_led(grabber_right, DXL_LED_YELLOW);

  // Open grabbers
  dxl_set_position(grabber_left, grab_pos_left);
  dxl_set_position(grabber_right, grab_pos_right);

  vTaskDelay(pdMS_TO_TICKS(300));
}

// Actual actions for landing a module
BaseType_t sys_mod_proc_land(void)
{
  //DEBUG_INFO("[SYS_MOD] Landing at %u, %u"DEBUG_EOL, sys_mod.land_pos, sys_mod.land_angle);

  dxl_set_led(&sys_mod.grabber_left, DXL_LED_YELLOW);
  dxl_set_led(&sys_mod.grabber_right, DXL_LED_YELLOW);

  // Turn to requested angle
  //bb_asv_set_pwm_pulse_length(sys_mod.rotator.channel, sys_mod.land_angle? ASV_ROTATOR_0:ASV_ROTATOR_90);

  // Up position
  dxl_set_speed(&sys_mod.lander, DSV_LANDER_SPEED_UP);
  dxl_set_position(&sys_mod.lander, DSV_LANDER_POS_UP);

  vTaskDelay(pdMS_TO_TICKS(300));

  // Horizontal position
  bb_asv_set_pwm_pulse_length(sys_mod.rotator.channel, ASV_ROTATOR_90);

  vTaskDelay(pdMS_TO_TICKS(300));

  // Trollet at requested pos
  //bb_asv_set_pwm_pulse_length(sys_mod.trollet.channel, sys_mod.grab_pos);

  // Open grabbers
  dxl_set_position(&sys_mod.grabber_left, DSV_GRABBER_LEFT_POS_OPENED);
  dxl_set_position(&sys_mod.grabber_right, DSV_GRABBER_RIGHT_POS_OPENED);

  vTaskDelay(pdMS_TO_TICKS(300));

  return pdPASS;
}

// Actions for folding the sub-system
BaseType_t sys_mod_proc_fold(void)
{
  DEBUG_INFO("[SYS_MOD] Folding"DEBUG_EOL);

  dxl_set_led(&sys_mod.grabber_left, DXL_LED_MAGENTA);
  dxl_set_led(&sys_mod.grabber_right, DXL_LED_MAGENTA);

  sys_mod_set_trollet_cmd(SW_TROLLET_GOTO_MIDDLE);

  // Up position
  dxl_set_speed(&sys_mod.lander, DSV_LANDER_SPEED_UP);
  dxl_set_position(&sys_mod.lander, DSV_LANDER_POS_UP);

  vTaskDelay(pdMS_TO_TICKS(500));

  // Close grabbers
  dxl_set_position(&sys_mod.grabber_left, DSV_GRABBER_LEFT_POS_CLOSED);
  dxl_set_position(&sys_mod.grabber_right, DSV_GRABBER_RIGHT_POS_CLOSED);

  vTaskDelay(pdMS_TO_TICKS(500));

  bb_asv_set_pwm_pulse_length(sys_mod.rotator.channel, ASV_ROTATOR_90);

  // TODO: replace with actual check (read) on correct closure
  vTaskDelay(pdMS_TO_TICKS(1000));

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
