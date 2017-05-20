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
#include "digital_servo.h"
#include "analog_servo.h"

typedef enum
{
  SYS_MOD_INIT,       // Just woke up, system is initialized
  SYS_MOD_SELF_TEST,  // Performing self-test
  SYS_MOD_READY,      // Ready to run
  SYS_MOD_GRAB_READY, // Modules System is ready to grab and will do so upon detection
  SYS_MOD_GRABBING,   // Grabbing an object
  SYS_MOD_LOADED,     // A module is loaded
  SYS_MOD_LANDING,    // Landing the module
  SYS_MOD_FOLDING,    // Fold the system
  SYS_MOD_ERROR       // When something terrible happened
} sys_mode_state_e;

typedef struct
{
  // Actuators
  dxl_servo_t grabber_left;   // XL320
  dxl_servo_t grabber_right;  // XL320
  dxl_servo_t lander;         // RX28
  asv_servo_t trollet;        // ServoIFace
  asv_servo_t rotator;        // MG995

  // Parameters
  uint16_t grab_pos;
  uint16_t land_pos;
  uint16_t land_angle;

  // System management
  sys_mode_state_e current_state;
  TaskHandle_t* calling_task;

} sys_mod_t;

#endif /* _SYS_MODULES_H_ */
