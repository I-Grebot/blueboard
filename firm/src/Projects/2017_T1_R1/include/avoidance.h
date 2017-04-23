/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       avoidance.h
 * @author     Paul
 * @date       2017/04/23
 * -----------------------------------------------------------------------------
 * @brief
 *   Definitions for the avoidance module
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#ifndef _AVOIDANCE_H_
#define _AVOIDANCE_H_

#include <stdint.h>

/**
********************************************************************************
**
**  Constants
**
********************************************************************************
*/

// Margin from one of the table edge to the robot's external radius where we
// decide that the avoidance should be OFF.
// We consider that no robot can be present in-between, and it is very useful
// to prevent false detection (e.g. if there is a human around the table).
#define AV_TABLE_MARGIN 400 // From the robot's edge, not the center

#define AV_CONSTRUCTION_AREA_MARGIN 300 // Same but for contruction area
#define AV_SAND_DUNE_MARGIN 300     // Same but for sand dune

// Semi angle of the front cone used for the avoidance split.
// It is common for front & back.
// Primarly it is useful to have a < 45 value to keep usage of the front central sensor
// when we travel almost parallel to the table edges.
#define AV_ANGULAR_CONE 25


// Wall rotation angles for sensor-selector
#define AV_ANGLE_WEST_WALL    0
#define AV_ANGLE_EAST_WALL -180
#define AV_ANGLE_SOUTH_WALL  90
#define AV_ANGLE_NORTH_WALL -90

// Angle used for the opponent's position estimation
#define AV_FRONT_APERTURE 25
#define AV_BACK_APERTURE  25

// Distance value to be used for the opponent's position estimation
// This can be set independently for front and back
#define AV_FRONT_DISTANCE  500
#define AV_BACK_DISTANCE   450  // from the robot center

// Avoidance timers
#define AV_TIMER_STOP_MS    1000  // The robot is stopped during this time after a detection
#define AV_TIMER_IGNORE_MS  1000  // Time when the sensor are ignored during the beginning of a rerouting

#define AV_OPP_VALIDITY_MS 3000  // Opponent position is valid 3sec after detection

/**
********************************************************************************
**
**  Types & Enumerations
**
********************************************************************************
*/

typedef enum
{
  AV_STATE_CLEAR,     // No detection
  AV_STATE_FILTER,    // Filtering potential glitches
  AV_STATE_DETECT,    // An opponent was detected, stop and wait
  AV_STATE_REROUTE    // Robot is rerouting, await for clear irq
} av_state_e;

typedef struct
{
  // Current FSM state
  av_state_e state;

  // Sensor masks, can be set by external systems
  bool mask_front_left;
  bool mask_front_center;
  bool mask_front_right;
  bool mask_back_left;
  bool mask_back_center;
  bool mask_back_right;

  // Sensors detection flags, after filtering
  // (basically different than robot.X flags)
  bool det_front_left;
  bool det_front_center;
  bool det_front_right;
  bool det_back_left;
  bool det_back_center;
  bool det_back_right;

  // Generic purpose timer
  int16_t timer_ms;

  // Timer of the opponent position validity
  int16_t timer_opp_validity_ms;

  // Execution flags
  bool action_done;

} av_t;

#endif /* _AVOIDANCE_H_ */
