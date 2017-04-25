/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       strategy.h
 * @author     Paul
 * @date       2017/04/23
 * -----------------------------------------------------------------------------
 * @brief
 *   Definitions for the strategy module
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#ifndef _STRATEGY_H_
#define	_STRATEGY_H_

#include <stdint.h>

/**
********************************************************************************
**
**  Definitions
**
********************************************************************************
*/

// -----------------------------------------------------------------------------
// SYSTEM AND PHYSICAL CONSTANTS
// -----------------------------------------------------------------------------

// Delay after RESET before the system goes into READY state
#define READY_DELAY_MSEC                1000U

// Delay after INIT before becoming sensitive to match START
#define WAIT_START_DELAY_MSEC           1000U

// Delay that is required for the IO_START to be high before the
// match start is considered valid
#define START_DEBOUNCING_DELAY_MSEC      200U

// Time from the start of the match to the 1st motion.
// Egrobit GR needs to be gone from the starting area after that time.
#define WAIT_BEFORE_START_MSEC          3000U

// Match duration (RUN state)
#define MATCH_DURATION_MSEC            90000U




// Place-holder for physicals definitions (robot or game elements)
// Warning: all coordinates are expressed with the PURPLE point of view.
// Coordinate system is the same than the rules metric.
// (0;0) point is always the same, whatever the color is.
// This origin is placed on the top-left side of the table,
//   i.e. on the yellow side and on the stairs side.
typedef struct {

  // Game elements
  poi_t huts[2];          // Location of the huts center
  poi_t cube[2];			// Location of the cube
  poi_t shells[4];

  // Robot
  poi_t reset;            // Location of the robot after reset (Purple)
  poi_t exit_start;
  poi_t drop;
  poi_t offset_center;
  poi_t offset_cube_E;
  poi_t offset_cube_W;

  // Path-finder static polygons
  //   path_poly_t* pf_construction_area;  // Obstacle delimited by the construction area
  //  path_poly_t* pf_sand_dune;          // Obstacle of the sand dune

  // Path-finder dynamic polygons (initialized with dummy values but updated
  // by the avoidance module)
  //    path_poly_t* pf_opponent1;          // First opponent's robot
  //   path_poly_t* pf_opponent2;          // Second opponent's robot

} phys_t;

// IDs for each object
#define PHYS_ID_HUT_1 0
#define PHYS_ID_HUT_2 1
#define PHYS_ID_CUBE_1 0
#define PHYS_ID_CUBE_2 1
#define PHYS_ID_SHELL_NW 0
#define PHYS_ID_SHELL_SW 1
#define PHYS_ID_SHELL_NE 2
#define PHYS_ID_SHELL_SE 3

// -----------------------------------------------------------------------------
// MAIN ROBOT & MATCH DEFINITIONS
// -----------------------------------------------------------------------------


// Define a match state enumeration
typedef enum {
  MATCH_STATE_RESET,        // State just after system reset
  MATCH_STATE_READY,        // System is ready (boot is finished),
  // awaits for user input for starting the INIT procedure
  MATCH_STATE_INIT,         // Initialization are running
  MATCH_STATE_SELF_CHECK,   // Self-check procedure
  MATCH_STATE_WAIT_START,   // Initialization + self-check finished,
  // awaits for user input for starting the match!
  MATCH_STATE_RUN,          // Match is running, do some stuff...
  MATCH_STATE_STOPPED       // End of the match
} match_state_e;

// Define the possible match colors
typedef enum {
  MATCH_COLOR_BLUE   = SW_COLOR_TEAM1,
  MATCH_COLOR_YELLOW = SW_COLOR_TEAM2,
  MATCH_COLOR_UNKNOWN // To be used when we don't know (yet)
} match_color_e;

// Define the possible start jack position
typedef enum {
  MATCH_START_INSERTED = SW_START_INSERTED,
  MATCH_START_REMOVED  = SW_START_REMOVED
} match_start_e;

// Define a match structure containing the various match parameters
typedef struct {
  match_state_e state;        // Current match state
  bool paused;                // Paused flag (only used with remote debug)
  match_color_e color;        // Selected color (sampled at the end of INIT)
  uint32_t timer_msec;        // Match time in milliseconds used for match time
  uint16_t scored_points;     // Calculated number of scored points
  // ...
} match_t;

// Define a robot structure
// TODO: update me
typedef struct
{
  // Aversive control system
  avs_cs_t cs;

  // Opponent position
  poi_t opp_pos;

} robot_t;

#endif	/* _STRATEGY_H_ */

