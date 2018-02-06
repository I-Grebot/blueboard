/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       strategy.h
 * @author     Pierrick
 * @date       21/01/2018
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
#define START_DEBOUNCING_DELAY_MSEC     200U

// Time from the start of the match to the 1st motion.
#define WAIT_BEFORE_START_MSEC          300U

// Match duration (RUN state)
#define MATCH_DURATION_MSEC             10000U


// Place-holder for physicals definitions (robot or game elements)
// Warning: all coordinates are expressed with the GREEN point of view.
// Coordinate system is the same than the rules metric.
// (0;0) point is always the same, whatever the color is.
// This origin is placed on the top-left side of the table,
// (same than the rules drawings)
typedef struct {

  // Game elements
  poi_t construction_cubes[6];			// Construction modules arranged on the table
  poi_t wastewater_recuperator;      	// Recuperator containing only our color
  poi_t mixed_wastewater_recuperator[2];// Recuperator containing half green half orange

  // Robot offsets
  poi_t offset_center;      			// Center of the robot

  // Important waypoints
  poi_t reset;              			// Location of the robot after reset (blue)
  poi_t exit_start;         			// Exit coordinate
  poi_t beehive;    					// starting area of the bee
  poi_t home_automation_switch;  		// Coordinate for shooting ores in the basket

  // Path-finder static polygons
  path_poly_t* pf_opp_start_zone;
  path_poly_t* pf_treatment_plant;


  // Path-finder dynamic polygons (initialized with dummy values but updated
  // by the avoidance module)
  path_poly_t* pf_teammate;           // Our teammate
  path_poly_t* pf_opponent1;          // First opponent's robot
  path_poly_t* pf_opponent2;          // Second opponent's robot

} phys_t;

// IDs for each object

// Mono modules on the table
#define PHYS_ID_CUBES_WG   0		// WEST GREEN SIDE
#define PHYS_ID_CUBES_SG   1		// SOUTH (GREEN SIDE)
#define PHYS_ID_CUBES_EG   2		// EAST GREEN SIDE
#define PHYS_ID_CUBES_WO   3		// WEST ORANGE SIDE
#define PHYS_ID_CUBES_NO   4		// NORTH (ORANGE SIDE)
#define PHYS_ID_CUBES_EO   5		// EAST ORANGE SIDE

// Wastewater recuperator
#define PHYS_ID_MIXED_G   0			// Mixed wastewater on green side
#define PHYS_ID_MIXED_O   1			// Mixed wastewater on orange side

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
  MATCH_COLOR_GREEN   = SW_COLOR_TEAM1,
  MATCH_COLOR_ORANGE = SW_COLOR_TEAM2,
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
  bool sw_init;               // Flag'd to true if the init is performed by software
} match_t;

// Define a robot structure
// TODO: update me
typedef struct
{
  // Aversive control system
  avs_cs_t cs;

  // Other robots positions
  poi_t teammate_pos;
  poi_t opp1_pos;
  poi_t opp2_pos;

} robot_t;

#endif	/* _STRATEGY_H_ */

