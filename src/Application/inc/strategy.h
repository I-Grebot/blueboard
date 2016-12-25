/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       strategy.h
 * @author     Pierrick
 * @date       Apr 29, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 */

#ifndef STRATEGY_H_
#define	STRATEGY_H_

#include "blueboard.h"


// -----------------------------------------------------------------------------
// SYSTEM AND PHYSICAL CONSTANTS
// -----------------------------------------------------------------------------

// Delay after RESET before the system goes into READY state
#define READY_DELAY_MSEC 1000

// Delay after INIT before becomming sensitive to match START
#define WAIT_START_DELAY_MSEC 1000

// Delay that is required for the IO_START to be high before the
// match start is considered valid
#define START_DEBOUNCING_DELAY_MSEC 200

// Time from the start of the match to the 1st motion.
// Egrobit GR needs to be gone from the starting area after that time.
#define WAIT_BEFORE_START_MSEC  3000

// Match duration (RUN state)
#define MATCH_DURATION_MSEC  90000

// Strategy Rate
#define DO_STRATEGY_MSEC	100

// Some constants
#ifndef PI
    #define PI 3.141592653589
#endif

// Macros
#define DEG_TO_RAD(a) (((double) (a)) * PI / 180.0)
#define RAD_TO_DEG(a) (((double) (a)) * 180.0 / PI)

// Playground size, in mm
#define TABLE_X_MIN     0
#define TABLE_X_MAX  3000
#define TABLE_LENGTH (TABLE_X_MAX - TABLE_X_MIN)

#define TABLE_Y_MIN     0
#define TABLE_Y_MAX  2000
#define TABLE_HEIGHT (TABLE_Y_MAX - TABLE_Y_MIN)

// Playground elements
#define ROCK_GREEN_X_MAX        2750
#define ROCK_PURPLE_X_MAX       250
#define ROCK_GREEN_Y_MIN        1750
#define ROCK_PURPLE_Y_MIN       1750
#define CONSTRUCTION_AREA_X_MIN 900
#define CONSTRUCTION_AREA_X_MAX 2100
#define CONSTRUCTION_AREA_MIDDLE_X		1500
#define CONSTRUCTION_AREA_Y_MIN 750
#define CONSTRUCTION_AREA_Y_MAX 1350
#define SAND_DUNE_X_MIN         800
#define SAND_DUNE_X_MAX         2200
#define SAND_DUNE_Y_MAX         200

// Robot dimensions in mm
#define ROBOT_FRONT_TO_CENTER  	85 // Distance from the ball bearing to the center
#define ROBOT_LENGTH           	130 // Distance from the front to the back
#define ROBOT_BACK_TO_CENTER	(ROBOT_LENGTH - ROBOT_FRONT_TO_CENTER)
#define ROBOT_BASE_WIDTH       	250 // Distance between the two encoder wheels
#define ROBOT_LOW_WIDTH        	230 // Distance from the two arm in idle position
#define ROBOT_HIGH_WIDTH       	290 // Distance from the two arm when moving
#define ROBOT_RADIUS           	130 // In Idle position the robot can fit in a cylinder of this radius.
#define ROBOT_RADIUS_W_CUBE    	260 // With Cube the robot can fit in a cylinder of this radius.

#define TRAJECTORY_NEAR_WINDOW_D	200
#define TRAJECTORY_NEAR_WINDOW_A	10

#define OPPONENT_POS_INIT_X     6000 // Out of table
#define OPPONENT_POS_INIT_Y     4000

// Avoidance constants

// Margin from one of the table edge to the robot's external radius where we
// decide that the avoidance should be OFF.
// We consider that no robot can be present in-between, and it is very useful
// to prevent false detection (e.g. if there is a human around the table).
#define AV_TABLE_MARGIN 400 // From the robot's edge, not the center

#define AV_CONSTRUCTION_AREA_MARGIN 300 // Same but for contruction area
#define AV_SAND_DUNE_MARGIN 300 		// Same but for sand dune

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

// Sizes for opponent robots
// Lengths of one small segment of the octogon.
// The larger length is equal to ~1.41 x small length
#define OPPONENT1_SIZE  300
#define OPPONENT2_SIZE  100

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

#define START_JACK	SW2_VALUE
#define COLOR		SW1_VALUE

// Define a match state enumeration
typedef enum {
    MATCH_STATE_RESET,        // State just after system reset
    MATCH_STATE_READY,        // System is ready (boot is finished),
                              // awaits for user input for starting the INIT procedure
    MATCH_STATE_INIT,         // Initialization are running
    MATCH_STATE_WAIT_START,   // Initialization finished,
                              // awaits for user input for starting the match!
    MATCH_STATE_RUN,          // Match is running, do some stuff...
    MATCH_STATE_STOPPED       // End of the match
} match_state_e;

typedef enum {
	STRAT_EXIT = 0,
	STRAT_TURN_TO_CUBE1 = 1,
	STRAT_PUSH_CUBE1 = 2,
	STRAT_RETURN_HUT_2 = 3,
	STRAT_TURN_TO_HUT_2 = 4,
	STRAT_CLOSE_HUT_2 = 5,
	STRAT_RETURN_HUT_1 = 6,
	STRAT_TURN_TO_HUT_1 = 7,
	STRAT_CLOSE_HUT_1 = 8,
	STRAT_PREPARE_TO_CUBE2 = 9,
	STRAT_TURN_TO_FACE_SHELLS = 10,
	STRAT_GOTO_SHELL_SW = 11,
	STRAT_GOTO_SHELL_SE = 12,
	STRAT_GOTO_SHELL_NE = 13,
	STRAT_GOTO_START = 14,
	STRAT_FINISHED = 15
/*	STRAT_GO_TO_CUBE2 = 10,
	STRAT_TURN_TO_CUBE2 = 11,
	STRAT_TOUCH_CUBE2 = 12,
	STRAT_PULL_CUBE2 = 13,
	STRAT_RETURN_TO_START = 14,
	STRAT_TURN_TO_PUSH_CUBE2 = 15,
	STRAT_PUSH_CUBE2 = 16,*/

} strategy_state_e;

// Define the possible match colors
typedef enum {
    MATCH_COLOR_UNKNOWN, // To be used when we don't know (yet)
    MATCH_COLOR_GREEN = 0,
    MATCH_COLOR_PURPLE = 1,
} match_color_e;

// Define the possible start jack position
typedef enum {
    PRESENT = 1, // To be used when we don't know (yet)
    ABSENT = 0,
} jack_status_e;

// Define a match structure containing the various match parameters
typedef struct {
    match_state_e state;        // Current match state
    match_color_e color;        // Selected color (sampled at the end of INIT)
    uint32_t timer_msec;        // Match time in milliseconds used for match time
    uint16_t scored_points;     // Calculated number of scored points
    // ...
} match_t;

// Define a robot structure, holder of CAN feedbacks
typedef struct {

    // Current robot absolute position
    poi_t pos;

    // Opponent position
    poi_t opp_pos;

    // Avoidance feedbacks (fed by CAN status)
    bool av_front_left;
    bool av_front_center;
    bool av_front_right;
    bool av_back_left;
    bool av_back_right;

    // Some execution flags & traces
    uint8_t init_done;

} robot_t;

// -----------------------------------------------------------------------------
// AVOIDANCE TYPES DEFINITION
// -----------------------------------------------------------------------------

typedef enum {
    AV_STATE_CLEAR,     // No detection
    AV_STATE_FILTER,    // Filtering potential glitches
    AV_STATE_DETECT,    // An opponent was detected, stop and wait
    AV_STATE_REROUTE    // Robot is rerouting, await for clear irq
} av_state_e;

typedef struct {
    
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

// -----------------------------------------------------------------------------
// TASK MANAGER
// -----------------------------------------------------------------------------

// Total Number of tasks
#define TASKS_NB  3

// IDs of the different tasks
// --------------------------

// Special tasks
#define TASK_ID_IDLE             0   // Special task used when there is nothing to do
#define TASK_ID_START            1   // Special task used after init

// Sea tasks
#define TASK_ID_SEA              2   // Go to the sea and catch fish


// Initial values of the different tasks
// -------------------------------------
#define TASK_INIT_VALUE_IDLE        1 // Lowest but non-zero value
#define TASK_INIT_VALUE_START   50000
#define TASK_INIT_VALUE_SEA     20000

// Priorities
#define PRIORITY_MIN    0
#define PRIORITY_MAX    0xFFFF

// Maximum amount of dependencies that a given task can have.
// Dependences are inherited from dependant tasks.
// For example:
// Task A (TA) can be executed freely
// Task B (TB) requires that TA is finished
// Task C (TC) requires that TB is finished
// Then TC also depends on TA, so the dependencies array size for this example
// is 0 for TA, 1 for TB and 1 for TC.
#define TASK_MAX_DEPENDENCIES   8

// Value used when we don't want to set a timeout for a task element
#define TASK_NO_TIMEOUT 0xFFFF

// Default timer value
#define TASK_DEFAULT_TIMER_MS   1000

// Number of characters for task String-ID
#define TASK_STRING_ID_NB_CHAR  64

// Define a task structure:
// A task is a sequential chain of motion and/or action. These elements
// are stored in a task element structure and are organized in a chain pointer
// fashion.

typedef struct task     task_t;
typedef struct task_elt task_elt_t; // Forward declaration

typedef enum {
    TASK_STATE_INACTIVE  = 0xFF, // Task is not active
    TASK_STATE_START     = 0x10, // Task has just started (nothing was executed yet)
    TASK_STATE_RUNNING   = 0x11, // Task is running, elements are being executed
    TASK_STATE_SUSPENDED = 0x20, // The last task element could not finished
    TASK_STATE_FAILED    = 0x21, // The task stopped due to an error
    TASK_STATE_SUCCESS   = 0x30  // Task finished with no error
} task_state_e;

struct task {
    uint8_t id;            // ID of the task (value affected from TASK_ID_xxx)
    task_state_e state;    // Current state of the task
    uint8_t nb_elt;        // Number of elements in the chain
    uint8_t nb_elt_done;   // Number of elements already performed
    task_elt_t* first_elt; // Pointer to the 1st element in the chain
    task_elt_t* last_elt;  // Pointer to the last element in the chain
    task_elt_t* current_elt;    // Current task element being executed
    uint8_t nb_dependencies;                        // Number of dependencies
    task_t* dependencies[TASK_MAX_DEPENDENCIES];    // Array of pointers on dependencies
    uint16_t value;     // Value given to a task so it'll affect its priority    
    uint16_t priority;  // Task priority calculated after the current task is finished
    uint8_t trials;     // Counter to measure the number of attemps for a task
};

// Defines the various actions that a task element can launch
typedef enum {
    TASK_MOTION,
    TASK_ACTUATOR,
    TASK_WAIT       // A dummy kind of task which can be used to wait
} task_action_e;

typedef struct {

    // Flags containing the actuators to be executed
    uint8_t flags;

    // Commands for spots
    struct {
        uint8_t north_builder;
        uint8_t south_builder;
        uint8_t bull_bar;
    } spots;

    // Commands for pop-corn
    struct {
        uint8_t north_pop;
        uint8_t south_pop;
    } pops;

    // Commands for claps
    struct {
        uint8_t north_clap;
        uint8_t south_clap;
    } claps;

    // Commands for cups
    struct {
        uint8_t north_cup;
        uint8_t south_cup;
    } cups;

} act_t;

// Define a task element structure.
struct task_elt {

    // Parent of the task element
    task_t* parent;

    // Next task element in the chain
    task_elt_t* next;

    // Type of action to do for this element
    task_action_e action;

    // Parameters for the action to be executed
    union {
        struct {
            wp_t dest;
            bool use_pf;
        } mot;
        act_t act;
    } params ;

    // Timer: used for handling task elements transition.
    // After an action is launched, the timer will be started
    // When it reaches 0, then the task element is considered as done.
    uint32_t timer_ms;
    
    // Timeout will lead to task's state = FAIL if reached
    // Set to TASK_NO_TIMEOUT for no timeout
    uint32_t timeout_ms;
};

// -----------------------------------------------------------------------------
// PROTOTYPES
// -----------------------------------------------------------------------------

// Main strategy manager
void strategy_init(void);
void do_strategy(void);
void do_match(void);

// Task manager
void task_init(void);
bool task_is_valid(task_t* task);
task_t* task_get_next(void);
void task_add_dep(task_t* task, task_t* dep);
void task_remove_dep(task_t* task, task_t* dep);
void task_remove_dep_from_all(task_t* dep);
task_elt_t* task_new_elt(task_t* task);
void task_add_elt(task_t* task, task_elt_t* elt);
void do_task(task_t* task);

// Physical engine
void phys_init(void);
uint8_t phys_is_north_left(void);
void phys_update_with_color(poi_t* poi);
void phys_update_with_color_xya(int16_t* x, int16_t* y, int16_t* a);
void phys_update_with_color_xy(int16_t* x, int16_t* y);
void phys_apply_offset(poi_t* src, poi_t* dest, const poi_t* offset);
void phys_apply_polar_offset(int16_t* x, int16_t* y, int16_t d, int16_t a_deg);
void phys_set_obstacle_positions(void);
void phys_set_opponent_position(uint8_t robot_idx, int16_t x, int16_t y);

// A.I.
void ai_checkpoints_def(void);


#endif	/* STRATEGY_H_ */

