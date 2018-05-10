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
  dxl_init_servo(&sys_mod.pusher, &dsv_chan2.dxl, "RX-28");
  dxl_init_servo(&sys_mod.opener, &dsv_chan2.dxl, "RX-28");
  dxl_init_servo(&sys_mod.index, &dsv_chan1.dxl, "XL320");
  dxl_init_servo(&sys_mod.thumb, &dsv_chan1.dxl, "XL320");

  sys_mod.left_arm.id = DSV_LEFT_ARM_ID;
  sys_mod.right_arm.id = DSV_RIGHT_ARM_ID;
  sys_mod.opener.id = DSV_OPENER_ID;
  sys_mod.pusher.id = DSV_PUSHER_ID;
  sys_mod.index.id = DSV_INDEX_ID;
  sys_mod.thumb.id = DSV_THUMB_ID;

  sys_mod.current_state = SYS_MOD_RESET;
}

void sys_mod_set_shoot_cmd(uint8_t cmd)
{
  SW_SHOOTER_CMD0(cmd & 0x02 ? Bit_SET: Bit_RESET);
  SW_SHOOTER_CMD1(cmd & 0x01 ? Bit_SET: Bit_RESET);
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
          xTaskNotify(*sys_mod.calling_task, OS_FEEDBACK_SYS_MOD_INIT, eSetBits);
        }
        break;

      case SYS_MOD_INIT:
        // Only manage transition to self-test (required)
        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_SELF_TEST))
        {
          sys_mod.current_state = SYS_MOD_SELF_TEST;
          sys_mod_proc_self_test();
          xTaskNotify(*sys_mod.calling_task, OS_FEEDBACK_SYS_MOD_SELF_TEST, eSetBits);
        }
        break;


      case SYS_MOD_SELF_TEST:
        sys_mod.current_state = SYS_MOD_READY;
        DEBUG_INFO("[SYS_MOD] Ready"DEBUG_EOL);
        break;


      case SYS_MOD_READY:
        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_LEFT_ARM))
        {
        	sys_mod_set_servo(&sys_mod.left_arm,sys_mod.left_arm_pos);
            xTaskNotify(*sys_mod.calling_task, OS_FEEDBACK_SYS_MOD_LEFT_ARM, eSetBits);
        }

        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_RIGHT_ARM))
        {
        	sys_mod_set_servo(&sys_mod.right_arm,sys_mod.right_arm_pos);
            xTaskNotify(*sys_mod.calling_task, OS_FEEDBACK_SYS_MOD_RIGHT_ARM, eSetBits);
        }

        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_PUSH))
        {
        	if(sys_mod.pusher.current_position==DSV_PUSHER_IN)
        		sys_mod_set_servo(&sys_mod.pusher,DSV_PUSHER_OUT);
        	else
        		sys_mod_set_servo(&sys_mod.pusher,DSV_PUSHER_IN);
            xTaskNotify(*sys_mod.calling_task, OS_FEEDBACK_SYS_MOD_PUSH, eSetBits);
        }

        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_OPEN))
        {
        	if(sys_mod.opener.current_position==DSV_OPENER_POS_RIGHT)
        		sys_mod_set_servo(&sys_mod.opener,DSV_OPENER_POS_LEFT);
        	else
        		sys_mod_set_servo(&sys_mod.opener,DSV_OPENER_POS_RIGHT);
            xTaskNotify(*sys_mod.calling_task, OS_FEEDBACK_SYS_MOD_OPEN, eSetBits);
        }

        if(notified && (sw_notification & OS_NOTIFY_SYS_MOD_SHOOT))
        {
        	sys_mod_proc_do_shoot();
            xTaskNotify(*sys_mod.calling_task, OS_FEEDBACK_SYS_MOD_SHOOT, eSetBits);
        }
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

  // Initializing digital servos
  while(dxl_set_speed(&sys_mod.left_arm, DSV_ARMS_SPEED_SLOW)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_speed(&sys_mod.right_arm, DSV_ARMS_SPEED_SLOW)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_speed(&sys_mod.opener, DSV_OPENER_SPEED_SLOW)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_speed(&sys_mod.pusher, DSV_PUSHER_SPEED)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_speed(&sys_mod.index, DSV_INDEX_SPEED)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_speed(&sys_mod.thumb, DSV_THUMB_SPEED_SLOW)!=DXL_STATUS_NO_ERROR);

  while(dxl_set_torque_enable(&sys_mod.left_arm, 1)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_torque_enable(&sys_mod.right_arm, 1)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_torque_enable(&sys_mod.opener, 1)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_torque_enable(&sys_mod.pusher, 1)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_torque_enable(&sys_mod.index, 1)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_torque_enable(&sys_mod.thumb, 1)!=DXL_STATUS_NO_ERROR);

  while(dxl_set_torque(&sys_mod.left_arm, DSV_ARMS_TORQUE)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_torque(&sys_mod.right_arm, DSV_ARMS_TORQUE)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_torque(&sys_mod.opener, DSV_OPENER_TORQUE)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_torque(&sys_mod.pusher, DSV_PUSHER_TORQUE)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_torque(&sys_mod.index, DSV_INDEX_TORQUE)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_torque(&sys_mod.thumb, DSV_THUMB_TORQUE_LOW)!=DXL_STATUS_NO_ERROR);

  sys_mod_set_servo(&sys_mod.left_arm,DSV_LEFT_ARM_POS_UP);
  sys_mod_set_servo(&sys_mod.right_arm,DSV_RIGHT_ARM_POS_UP);
  sys_mod_set_servo(&sys_mod.opener,DSV_OPENER_POS_LEFT);
  sys_mod_set_servo(&sys_mod.pusher,DSV_PUSHER_IN);
  sys_mod_set_servo(&sys_mod.index,DSV_INDEX_POS_GET);
  sys_mod_set_servo(&sys_mod.thumb,DSV_THUMB_POS_DOWN);

  // Initialize ServoIFace
  sys_mod_set_shoot_cmd(SW_SHOOTER_INIT);
  sys_mod.shooter_height = SW_SHOOTER_SHOOT_HIGH;
  sys_mod.shooter_number = 2;

  return pdPASS;
}

BaseType_t sys_mod_set_servo(dxl_servo_t* servo, uint16_t position)
{
  uint16_t return_position=0;
  static uint8_t timout=0;
  while(dxl_set_position(servo, position)!=DXL_STATUS_NO_ERROR);
  for(timout=0;timout<20;timout++)
  {
	  servo->current_position=position;
	  dxl_get_position(servo, &return_position);	// Check position
	  if((return_position<=position+5) && (return_position>=position-5))
	  {
		return pdPASS;
	  }
	  else
	    vTaskDelay(pdMS_TO_TICKS(100));
  }
  return pdFAIL;
}

// Launch a series of self-test actions
BaseType_t sys_mod_proc_self_test(void)
{

  DEBUG_INFO("[SYS_MOD] Start Self-Test"DEBUG_EOL);

  sys_mod_proc_do_shoot();

  // Test
  while(dxl_set_speed(&sys_mod.opener, DSV_OPENER_SPEED_FAST)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_speed(&sys_mod.left_arm, DSV_ARMS_SPEED_FAST)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_speed(&sys_mod.right_arm, DSV_ARMS_SPEED_FAST)!=DXL_STATUS_NO_ERROR);
  sys_mod_set_servo(&sys_mod.opener,DSV_OPENER_POS_LEFT);
  sys_mod_set_servo(&sys_mod.opener,DSV_OPENER_POS_RIGHT);
  sys_mod_set_servo(&sys_mod.left_arm,DSV_LEFT_ARM_POS_DOWN);
  sys_mod_set_servo(&sys_mod.left_arm,DSV_LEFT_ARM_POS_UP);
  sys_mod_set_servo(&sys_mod.right_arm,DSV_RIGHT_ARM_POS_DOWN);
  sys_mod_set_servo(&sys_mod.right_arm,DSV_RIGHT_ARM_POS_UP);
  sys_mod_set_servo(&sys_mod.pusher,DSV_PUSHER_OUT);
  sys_mod_set_servo(&sys_mod.pusher,DSV_PUSHER_IN);
  sys_mod_set_servo(&sys_mod.thumb,DSV_THUMB_POS_UP);
  while(dxl_set_speed(&sys_mod.thumb, DSV_THUMB_SPEED_FAST)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_torque(&sys_mod.thumb, DSV_THUMB_TORQUE_HIGH)!=DXL_STATUS_NO_ERROR);
  sys_mod_set_servo(&sys_mod.thumb,DSV_THUMB_POS_DOWN);
  while(dxl_set_speed(&sys_mod.thumb, DSV_THUMB_SPEED_SLOW)!=DXL_STATUS_NO_ERROR);
  while(dxl_set_torque(&sys_mod.thumb, DSV_THUMB_TORQUE_LOW)!=DXL_STATUS_NO_ERROR);

  return pdPASS;
}

BaseType_t sys_mod_proc_do_shoot(void)
{
	static uint8_t shoot_nb=0;

    for(shoot_nb=0;shoot_nb<sys_mod.shooter_number;shoot_nb++)
    {
    	sys_mod_set_servo(&sys_mod.index,DSV_INDEX_POS_SET);
    	sys_mod_set_servo(&sys_mod.thumb,DSV_THUMB_POS_UP);
    	while(dxl_set_speed(&sys_mod.thumb, DSV_THUMB_SPEED_FAST)!=DXL_STATUS_NO_ERROR);
    	while(dxl_set_torque(&sys_mod.thumb, DSV_THUMB_TORQUE_HIGH)!=DXL_STATUS_NO_ERROR);
    	sys_mod_set_servo(&sys_mod.thumb,DSV_THUMB_POS_DOWN);
  	    while(dxl_set_speed(&sys_mod.thumb, DSV_THUMB_SPEED_SLOW)!=DXL_STATUS_NO_ERROR);
  	    while(dxl_set_torque(&sys_mod.thumb, DSV_THUMB_TORQUE_LOW)!=DXL_STATUS_NO_ERROR);
  	    sys_mod_set_servo(&sys_mod.index,DSV_INDEX_POS_GET);
		vTaskDelay(pdMS_TO_TICKS(200));
    	sys_mod_set_shoot_cmd(sys_mod.shooter_height);
    	if(sys_mod.shooter_height==SW_SHOOTER_SHOOT_HIGH)
    	{
    		vTaskDelay(pdMS_TO_TICKS(200));
    	}
    	else{
    		vTaskDelay(pdMS_TO_TICKS(400));
    	}
    	sys_mod_set_shoot_cmd(SW_SHOOTER_INIT);
    	vTaskDelay(pdMS_TO_TICKS(500));
    }
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

void sys_mod_do_left_arm(TaskHandle_t* caller, uint16_t position)
{
	sys_mod.calling_task = caller;
	sys_mod.left_arm_pos = position;
	xTaskNotify(handle_task_sys_modules, OS_NOTIFY_SYS_MOD_LEFT_ARM, eSetBits);
}

void sys_mod_do_right_arm(TaskHandle_t* caller, uint16_t position)
{
	sys_mod.calling_task = caller;
	sys_mod.right_arm_pos = position;
	xTaskNotify(handle_task_sys_modules, OS_NOTIFY_SYS_MOD_RIGHT_ARM, eSetBits);
}

void sys_mod_do_open(TaskHandle_t* caller)
{
	sys_mod.calling_task = caller;
	xTaskNotify(handle_task_sys_modules, OS_NOTIFY_SYS_MOD_OPEN, eSetBits);
}

void sys_mod_do_push(TaskHandle_t* caller)
{
	sys_mod.calling_task = caller;
	xTaskNotify(handle_task_sys_modules, OS_NOTIFY_SYS_MOD_PUSH, eSetBits);
}

void sys_mod_do_index(TaskHandle_t* caller, uint16_t position)
{
	sys_mod_set_servo(&sys_mod.index,sys_mod.index.current_position=position);
}

void sys_mod_do_shoot(TaskHandle_t* caller, uint8_t height, uint8_t number)
{
	sys_mod.shooter_height = height;
	sys_mod.shooter_number = number;
	sys_mod.calling_task = caller;
	xTaskNotify(handle_task_sys_modules, OS_NOTIFY_SYS_MOD_SHOOT, eSetBits);
}

void sys_mod_set_color(uint8_t color)
{
	switch(color)
	{
	  case MATCH_COLOR_GREEN :
		while(dxl_set_led(&sys_mod.index,DXL_LED_GREEN)!=DXL_STATUS_NO_ERROR);
	    break;
	  case MATCH_COLOR_ORANGE :
		while(dxl_set_led(&sys_mod.index,DXL_LED_YELLOW)!=DXL_STATUS_NO_ERROR);
	    break;
	  default:
		while(dxl_set_led(&sys_mod.index,DXL_LED_RED)!=DXL_STATUS_NO_ERROR);
		break;
	}
}
