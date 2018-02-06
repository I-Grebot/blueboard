/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       analog_servo.h
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
#ifndef _ANALOG_SERVO_H_
#define _ANALOG_SERVO_H_

#include <stdint.h>
#include "blueboard.h"

typedef struct {
  BB_ASV_ChannelTypeDef channel;
  uint16_t current_pos;
  uint16_t min_pos;
  uint16_t max_pos;
} asv_servo_t;



#endif /* _ANALOG_SERVO_H_ */
