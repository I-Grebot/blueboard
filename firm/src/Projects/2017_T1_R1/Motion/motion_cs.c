/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       motion_cs.c
 * @author     Bebop35
 * @date       Apr 20, 2017
 * -----------------------------------------------------------------------------
 * @brief
 *   This task handles the motion's control-system
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "main.h"

/* Global functions */
extern robot_t robot;

/* Local Mutex for Aversive */
static xSemaphoreHandle xEncoderAngleMutex;
static xSemaphoreHandle xEncoderDistanceMutex;
static xSemaphoreHandle xAngleConsignMutex;
static xSemaphoreHandle xDistanceConsignMutex;
static xSemaphoreHandle xRobotPositionMutex;

/* Local, Private functions */
static void motion_cs_init(void);
static void motion_cs_task(void *pvParameters);

/* -----------------------------------------------------------------------------
 * Initializations
 * -----------------------------------------------------------------------------
 */

BaseType_t motion_cs_start(void)
{
  BaseType_t ret;

  /* Initialize global variables */
  motion_cs_init();

  // Start the motion control task
  ret = xTaskCreate(motion_cs_task, "MOTION_CS", OS_TASK_STACK_MOTION_CS, NULL, OS_TASK_PRIORITY_MOTION_CS, NULL );
  if(ret != pdPASS) {
    DEBUG_CRITICAL("Could not start motion control-system task!"DEBUG_EOL);

  } else {
    DEBUG_INFO("Starting MOTION_CS task"DEBUG_EOL);
  }

  return ret;

}

void motion_cs_init(void)
{
  /* Initialize mutexes */
  xEncoderAngleMutex = xSemaphoreCreateMutex();
  xEncoderDistanceMutex = xSemaphoreCreateMutex();
  xAngleConsignMutex = xSemaphoreCreateMutex();
  xDistanceConsignMutex = xSemaphoreCreateMutex();
  xRobotPositionMutex = xSemaphoreCreateMutex();

  /* Robot System */
  rs_init(&robot.cs.rs);
  rs_set_left_pwm(&robot.cs.rs,  (void*) bb_mot_set_motor_speed_fast_decay, (void*) MOT_CHANNEL_LEFT);
  rs_set_right_pwm(&robot.cs.rs, (void*) bb_mot_set_motor_speed_fast_decay, (void*) MOT_CHANNEL_RIGHT);

  /* External Encoders */
  rs_set_left_ext_encoder(&robot.cs.rs,  (void*) bb_enc_get_channel, (void*) ENC_CHANNEL_LEFT,  PHYS_ROBOT_ENCODER_LEFT_GAIN);
  rs_set_right_ext_encoder(&robot.cs.rs, (void*) bb_enc_get_channel, (void*) ENC_CHANNEL_RIGHT, PHYS_ROBOT_ENCODER_RIGHT_GAIN);
  rs_set_flags(&robot.cs.rs, RS_USE_EXT);

  /* Position Manager */
  position_init(&robot.cs.pos);
  position_set_physical_params(&robot.cs.pos, PHYS_ROBOT_ENCODERS_TRACK_MM, PHYS_ROBOT_NB_IMP_PER_MM);
  position_set_related_robot_system(&robot.cs.pos, &robot.cs.rs);
  position_use_ext(&robot.cs.pos);
  //position_set_centrifugal_coef(&robot.cs.pos, PHYS_ROBOT_CENTRIFUGAL_COEF);

  /* Control System filter in Distance */
  pid_init(&robot.cs.pid_d);
  pid_set_gains(&robot.cs.pid_d, PHYS_CS_D_PID_KP, PHYS_CS_D_PID_KI, PHYS_CS_D_PID_KD);
  pid_set_maximums(&robot.cs.pid_d, PHYS_CS_D_PID_MAX_IN, PHYS_CS_D_PID_MAX_I, PHYS_CS_D_PID_MAX_OUT);
  pid_set_out_shift(&robot.cs.pid_d, PHYS_CS_D_PID_OUT_SHIFT);
  pid_set_derivate_filter(&robot.cs.pid_d, PHYS_CS_D_PID_DRV_FILTER);
  quadramp_init(&robot.cs.qr_d);
  quadramp_set_1st_order_vars(&robot.cs.qr_d, PHYS_CS_D_QUAD_POS_SPEED, PHYS_CS_D_QUAD_NEG_SPEED);
  quadramp_set_2nd_order_vars(&robot.cs.qr_d, PHYS_CS_D_QUAD_POS_ACCEL, PHYS_CS_D_QUAD_NEG_ACCEL);
  cs_init(&robot.cs.cs_d);
  cs_set_consign_filter(&robot.cs.cs_d, quadramp_do_filter, &robot.cs.qr_d);
  cs_set_correct_filter(&robot.cs.cs_d, pid_do_filter, &robot.cs.pid_d);
  cs_set_process_in(&robot.cs.cs_d, rs_set_distance, &robot.cs.rs);
  cs_set_process_out(&robot.cs.cs_d, rs_get_distance, &robot.cs.rs);
  cs_set_consign(&robot.cs.cs_d, 0);

  /* Control System filter in Angle */
  pid_init(&robot.cs.pid_a);
  pid_set_gains(&robot.cs.pid_a, PHYS_CS_A_PID_KP, PHYS_CS_A_PID_KI, PHYS_CS_A_PID_KD);
  pid_set_maximums(&robot.cs.pid_a, PHYS_CS_A_PID_MAX_IN, PHYS_CS_A_PID_MAX_I, PHYS_CS_A_PID_MAX_OUT);
  pid_set_out_shift(&robot.cs.pid_a, PHYS_CS_A_PID_OUT_SHIFT);
  pid_set_derivate_filter(&robot.cs.pid_a, PHYS_CS_A_PID_DRV_FILTER);
  quadramp_init(&robot.cs.qr_a);
  quadramp_set_1st_order_vars(&robot.cs.qr_a, PHYS_CS_A_QUAD_POS_SPEED, PHYS_CS_A_QUAD_NEG_SPEED);
  quadramp_set_2nd_order_vars(&robot.cs.qr_a, PHYS_CS_A_QUAD_POS_ACCEL, PHYS_CS_A_QUAD_NEG_ACCEL);
  cs_init(&robot.cs.cs_a);
  cs_set_consign_filter(&robot.cs.cs_a, quadramp_do_filter, &robot.cs.qr_a);
  cs_set_correct_filter(&robot.cs.cs_a, pid_do_filter, &robot.cs.pid_a);
  cs_set_process_in(&robot.cs.cs_a, rs_set_angle, &robot.cs.rs);
  cs_set_process_out(&robot.cs.cs_a, rs_get_angle, &robot.cs.rs);
  cs_set_consign(&robot.cs.cs_a, 0);

  /* Trajectory Manager */
  trajectory_init(&robot.cs.traj);
  trajectory_set_cs(&robot.cs.traj, &robot.cs.cs_d, &robot.cs.cs_a);
  trajectory_set_robot_params(&robot.cs.traj, &robot.cs.rs, &robot.cs.pos);
  trajectory_set_speed(&robot.cs.traj, PHYS_TRAJ_D_DEFAULT_SPEED, PHYS_TRAJ_A_DEFAULT_SPEED);
  trajectory_set_windows(&robot.cs.traj, PHYS_TRAJ_DEFAULT_WIN_D, PHYS_TRAJ_DEFAULT_WIN_A_DEG, PHYS_TRAJ_DEFAULT_WIN_A_START_DEG);

  /* Blocking detection */
  bd_init(&robot.cs.bd_l);
  bd_init(&robot.cs.bd_r);
  bd_set_current_thresholds(&robot.cs.bd_r, PHYS_BD_K1, PHYS_BD_K2, PHYS_BD_THR, PHYS_BD_CPT);
  bd_set_current_thresholds(&robot.cs.bd_l, PHYS_BD_K1, PHYS_BD_K2, PHYS_BD_THR, PHYS_BD_CPT);
  bd_set_speed_threshold(&robot.cs.bd_l, PHYS_BD_SPD);
  bd_set_speed_threshold(&robot.cs.bd_r, PHYS_BD_SPD);

  /* CS EVENT */
  //scheduler_add_periodical_event_priority(do_cs, NULL, 5000 / SCHEDULER_UNIT, 150);  /* 5 ms */
  /* BD_EVENT*/
  //scheduler_add_periodical_event_priority(sample_current, NULL, 25000 / SCHEDULER_UNIT, 200);  /* 25 ms */
  /* STATUS_EVENT */

  /* init struct robot */
  //robot.cs.cs_events = DO_CS | DO_RS | DO_POS | DO_BD | DO_POWER ;
  robot.cs.cs_events =  DO_RS | DO_POS | DO_BD | DO_STATUS;
  //right_current = 0;
  //left_current = 0;
}

/* -----------------------------------------------------------------------------
 * Main Motion Control System Managment Task
 * TODO: handle re-init of the task
 * -----------------------------------------------------------------------------
 */

void motion_cs_task(void *pvParameters)
{
  /* Static local variables */
  static int32_t old_a        = 0;
  static int32_t old_d        = 0;
  static int32_t old_speed_a  = 0;
  static int32_t old_speed_d  = 0;
  TickType_t xNextWakeTime;

  /* Initialise xNextWakeTime - this only needs to be done once. */
  xNextWakeTime = xTaskGetTickCount();

  /* Remove compiler warning about unused parameter. */
  ( void ) pvParameters;

  for( ;; )
  {

    if(robot.cs.cs_events & DO_RS) {

      // Manage Robot System
      rs_update(&robot.cs.rs);

      robot.cs.speed_a = rs_get_angle(&robot.cs.rs) - old_a;
      robot.cs.speed_d = rs_get_distance(&robot.cs.rs) - old_d;
      old_a = rs_get_angle(&robot.cs.rs);
      old_d = rs_get_distance(&robot.cs.rs);

      robot.cs.acceleration_a = robot.cs.speed_a - old_speed_a;
      robot.cs.acceleration_d = robot.cs.speed_d - old_speed_d;
      old_speed_a = robot.cs.speed_a;
      old_speed_d = robot.cs.speed_d;
    }

    if (robot.cs.cs_events & DO_POWER)
    {
      // Main CS Management
      vLockDistanceConsign();
      cs_manage(&robot.cs.cs_d);
      vUnlockDistanceConsign();
      vLockAngleConsign();
      cs_manage(&robot.cs.cs_a);
      vUnlockAngleConsign();
    }
    else
    {
      bb_mot_set_motor_speed_fast_decay(MOT_CHANNEL_LEFT, 0);
      bb_mot_set_motor_speed_fast_decay(MOT_CHANNEL_RIGHT,0);
    }

    if(robot.cs.cs_events & DO_POS)
    {
      position_manage(&robot.cs.pos);
    }

    /* Blocking-detection manager:
     * TODO: Add me */
    /* trajectory_hardstop(pRobot.traj);*/

    /* Wakes-up when required */
    vTaskDelayUntil( &xNextWakeTime, pdMS_TO_TICKS(OS_AVERSIVE_PERIOD_MS));
  }
}

/* -----------------------------------------------------------------------------
 * Control system software flags
 * -----------------------------------------------------------------------------
 */

void motion_power_enable(void)
{
  robot.cs.cs_events |=   DO_POWER;
  robot.cs.cs_events |=   DO_CS;
}
void motion_power_disable(void)
{
  robot.cs.cs_events &= ~ DO_POWER;
  robot.cs.cs_events &= ~ DO_CS;
}

/* -----------------------------------------------------------------------------
 * Position setters
 * -----------------------------------------------------------------------------
 */

void motion_set_x(int16_t pos_x)
{
  int16_t pos_y = position_get_y_s16(&robot.cs.pos);
  int16_t pos_a = position_get_a_deg_s16(&robot.cs.pos);
  position_set(&robot.cs.pos, pos_x, pos_y, pos_a);
}
void motion_set_y(int16_t pos_y)
{
  int16_t pos_x = position_get_x_s16(&robot.cs.pos);
  int16_t pos_a = position_get_a_deg_s16(&robot.cs.pos);
  position_set(&robot.cs.pos, pos_x, pos_y, pos_a);
}
void motion_set_a(int16_t pos_a)
{
  int16_t pos_x = position_get_x_s16(&robot.cs.pos);
  int16_t pos_y = position_get_y_s16(&robot.cs.pos);
  position_set(&robot.cs.pos, pos_x, pos_y, pos_a);
}

/* -----------------------------------------------------------------------------
 * Position getters
 * -----------------------------------------------------------------------------
 */

int16_t motion_get_x(void)
{
  return position_get_x_s16(&robot.cs.pos);
}
int16_t motion_get_y(void)
{
  return position_get_y_s16(&robot.cs.pos);
}
int16_t motion_get_a(void)
{
  return position_get_a_deg_s16(&robot.cs.pos);
}

/* -----------------------------------------------------------------------------
 * Aversive mutexes management
 * -----------------------------------------------------------------------------
 */

inline void vLockEncoderAngle(void)
{
  xSemaphoreTake(xEncoderAngleMutex, portMAX_DELAY);
}

inline void vLockEncoderDistance(void)
{
  xSemaphoreTake(xEncoderDistanceMutex, portMAX_DELAY);
}

inline void vLockAngleConsign(void)
{
  xSemaphoreTake(xAngleConsignMutex, portMAX_DELAY);
}

inline void vLockDistanceConsign(void)
{
  xSemaphoreTake(xDistanceConsignMutex, portMAX_DELAY);
}

inline void vLockRobotPosition(void)
{
  xSemaphoreTake(xRobotPositionMutex, portMAX_DELAY);
}

inline void vUnlockEncoderAngle(void)
{
  xSemaphoreGive(xEncoderAngleMutex);
}

inline void vUnlockEncoderDistance(void)
{
  xSemaphoreGive(xEncoderDistanceMutex);
}

inline void vUnlockAngleConsign(void)
{
  xSemaphoreGive(xAngleConsignMutex);
}

inline void vUnlockDistanceConsign(void)
{
  xSemaphoreGive(xDistanceConsignMutex);
}

inline void vUnlockRobotPosition(void)
{
  xSemaphoreGive(xRobotPositionMutex);
}
