#ifndef _MOTION_H_
#define _MOTION_H_

// -----------------------------------------------------------------------------
// MOTION & ROBOT STRUCTURES
// -----------------------------------------------------------------------------


/* Aversive's Control System manager structure
 * Contains all required control variables for the robot motion
 */

typedef struct {

    /* General */
    int8_t cs_events;

    /* Positioning */
    struct robot_system rs;
    struct robot_position pos;
    struct trajectory traj;

    /* Control system in Angle */
    struct cs cs_a;
    struct pid_filter pid_a;
    struct quadramp_filter qr_a;

    /* Control system in Distance */
    struct cs cs_d;
    struct pid_filter pid_d;
    struct quadramp_filter qr_d;

    /* Blocking detection */
    struct blocking_detection bd_l;
    struct blocking_detection bd_r;

    /* Control variables */
    int32_t pwm_l;
    int32_t pwm_r;

    /* Speed */
    volatile int16_t speed_a;
    volatile int16_t speed_d;

    /* Acceleration */
    volatile int16_t acceleration_a;
    volatile int16_t acceleration_d;

} AVS_ControlSystemTypeDef;

/* Main Robot structure */
typedef struct {

    /* Control system */
    AVS_ControlSystemTypeDef cs;

    /* Other robot variables come here */

} RobotTypeDef;

// -----------------------------------------------------------------------------
// MOTION-CONTROLLER
// -----------------------------------------------------------------------------

// Flags
#define MOTION_FLAG_WP_SENT     0x01 // Set when a new WP is sent. Awaits for ACK
#define MOTION_FLAG_WP_ACKED    0x02 // Set when the WP was ACK'd by Igresive
#define MOTION_FLAG_PAUSED      0x40 // Pause the motion control execution

// Pre-defined speeds
#define SPEED_FAST_D         1200 // For long motions only
#define SPEED_FAST_A         500
#define SPEED_NORMAL_D       800 // Normal and moderate motions
#define SPEED_NORMAL_A       300
#define SPEED_SLOW_D         400 // Actuator handling, narrow path or avoidance
#define SPEED_SLOW_A         200
#define SPEED_VERY_SLOW_D    200 // Position reset
#define SPEED_VERY_SLOW_A    100

typedef enum {
    WP_NO_MOTION, // isn't it explicit?
    WP_GOTO_AUTO,
    WP_GOTO_FWD,
    WP_GOTO_BWD,
    WP_MOVE_REL,
    WP_ORIENT_FRONT,
    WP_ORIENT_BEHIND
} wp_type_e;

typedef enum {
    WP_SPEED_FAST ,
    WP_SPEED_NORMAL,
    WP_SPEED_SLOW,
    WP_SPEED_VERY_SLOW
} wp_speed_e;

// Coordinates of a POI + its orientation
typedef struct {
    int16_t x;
    int16_t y;
    int16_t a;
} poi_t;

typedef struct {

    // Type of waypoint
    wp_type_e type;

    // Context-dependant offset to apply to the coordinates
    poi_t offset;

    // Related coordinates: Absolute or Relative
    struct {
        poi_t abs;
        struct {
            int16_t d;
            int16_t a;
        } rel;
    } coord;

    // Associated Vectorial and Angular speeds
    wp_speed_e speed;

    // Flags
    bool trajectory_must_finish;

} wp_t;

// Number of waypoints that can be stored in the motion controller's FIFO
// We don't need to have a large amount since we feed it one after the other
#define MOTION_WP_FIFO_SIZE 8



#endif /* _MOTION_H_ */
