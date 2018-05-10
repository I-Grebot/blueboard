/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       sys_modules.h
 * @author     Paul
 * @date       2017/05/19
 * -----------------------------------------------------------------------------
 * @brief
 *   See module file
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#ifndef _SYS_MODULES_H_
#define _SYS_MODULES_H_

#include <stdint.h>

#include "../../2018_T1_R1/include/analog_servo.h"
#include "../../2018_T1_R1/include/digital_servo.h"

typedef enum
{
  SYS_MOD_RESET,      // Just woke up, system is in unknown state
  SYS_MOD_INIT,       // Initializing system
  SYS_MOD_SELF_TEST,  // Performing self-test
  SYS_MOD_READY,      // Ready to run
  SYS_MOD_ERROR       // When something terrible happened
} sys_mode_state_e;

typedef struct
{
  // Actuators
  dxl_servo_t left_arm;       // RX28
  dxl_servo_t right_arm;      // RX28
  dxl_servo_t pusher;         // RX28
  dxl_servo_t opener;         // RX28
  dxl_servo_t index;          // XL320

  // Parameters
  uint16_t left_arm_pos;
  uint16_t right_arm_pos;
  uint16_t pusher_pos;
  uint16_t opener_pos;
  uint16_t index_pos;
  uint8_t shooter_height;
  uint8_t shooter_number;

  // System management
  sys_mode_state_e current_state;
  TaskHandle_t* calling_task;

} sys_mod_t;

#endif /* _SYS_MODULES_H_ */
