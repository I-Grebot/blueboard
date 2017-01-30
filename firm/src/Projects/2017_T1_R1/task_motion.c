/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       task_motion.c
 * @author     Paul
 * @date       Jan 5, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   This task handles the motion control
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev: 1470 $
 * $LastChangedBy: Pierrick_Boissard $
 * $LastChangedDate: 2016-05-06 00:47:14 +0200 (ven., 06 mai 2016) $
 * -----------------------------------------------------------------------------
 */

#include "main.h"




/* Main robot structure containing all operational variables */
RobotTypeDef robot;
static double traj_near_window_d = TRAJECTORY_NEAR_WINDOW_D;
static double traj_near_window_a = TRAJECTORY_NEAR_WINDOW_A;

/* Local definitions */
#define AVERSIVE_PERIOD_TICKS         (AVERSIVE_PERIOD_MS / portTICK_PERIOD_MS)
#define MOTION_CONTROL_PERIOD_TICKS   (MOTION_CONTROL_PERIOD_MS / portTICK_PERIOD_MS)
#define MAX_WP_IN_QUEUE	5

extern av_t av;

/* Local Mutex */
static xSemaphoreHandle xEncoderAngleMutex;
static xSemaphoreHandle xEncoderDistanceMutex;
static xSemaphoreHandle xAngleConsignMutex;
static xSemaphoreHandle xDistanceConsignMutex;
static xSemaphoreHandle xRobotPositionMutex;
static xQueueHandle 	xWaypointQueue;

/* Local, Private functions */
static void OS_MotionTask(void *pvParameters);
static void vCreateAllMutex(void);
static void AVS_Init(void);
static void AVS_CsTask(void *pvParameters);
static bool motion_is_traj_done(wp_t *waypoint);

void OS_CreateMotionTask(void)
{
    /* Initialize global variables */
    memset(&robot, 0, sizeof(RobotTypeDef));
	vCreateAllMutex();
    AVS_Init();
	xTaskCreate(AVS_CsTask, "AVERSIVE", 500, NULL, OS_TASK_PRIORITY_AVERSIVE, NULL );

	xWaypointQueue = xQueueCreate( MAX_WP_IN_QUEUE, sizeof(wp_t));
    if(xWaypointQueue==NULL)
    {
    	printf("insufficient heap RAM available for xELTQueue\r\n");
    	while(1);
    }
	xTaskCreate(OS_MotionTask, "MOTION", 500, NULL, OS_TASK_PRIORITY_MOTION, NULL );

}

static void OS_MotionTask( void *pvParameters )
{
    static wp_t current_waypoint;
    //    char cBuffer[50];

    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    for( ;; )
    {
    	xQueueReceive(xWaypointQueue, &current_waypoint, portMAX_DELAY);

    	// Handle avoidance blockage
        av.mask_back_left = true;
        av.mask_back_right = true;
        av.mask_front_center = true;
        av.mask_front_left = true;
        av.mask_front_right = true;

        motion_send_wp(&current_waypoint);

        while(!motion_is_traj_done(&current_waypoint))
        {
        	vTaskDelay(MOTION_CONTROL_PERIOD_TICKS);
        }

        av.mask_back_left = false;
        av.mask_back_right = false;
        av.mask_front_center = false;
        av.mask_front_left = false;
        av.mask_front_right = false;
    } // traj done


        //sprintf(cBuffer,"%ld\t%ld\t%lf\t%lf\t%lf\r\n", HW_ENC_GetChannel(HW_ENC_CHANNEL1),HW_ENC_GetChannel(HW_ENC_CHANNEL2),position_get_a_rad_double(&robot.cs.pos),position_get_x_double(&robot.cs.pos),position_get_y_double(&robot.cs.pos));
        //OS_DebugTaskPrint(cBuffer);
}

void AVS_Init(void)
{
    /* Robot System */
    rs_init(&robot.cs.rs);
    rs_set_left_pwm(&robot.cs.rs,  (void*) HW_MOT_SetMotorSpeedFastDecay, (void*) MOT_CHANNEL_LEFT);
    rs_set_right_pwm(&robot.cs.rs, (void*) HW_MOT_SetMotorSpeedFastDecay, (void*) MOT_CHANNEL_RIGHT);

    /* External Encoders */
    rs_set_left_ext_encoder(&robot.cs.rs,  (void*) HW_ENC_GetChannel, (void*) ENC_CHANNEL_LEFT,  PHYS_ROBOT_ENCODER_LEFT_GAIN);
    rs_set_right_ext_encoder(&robot.cs.rs, (void*) HW_ENC_GetChannel, (void*) ENC_CHANNEL_RIGHT, PHYS_ROBOT_ENCODER_RIGHT_GAIN);
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


/* Main CS Managment Task
 * TBC: might require a mutex for some variables
 * TODO: handle re-init of the task
 */
void AVS_CsTask(void *pvParameters)
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
			HW_MOT_SetMotorSpeedFastDecay(MOT_CHANNEL_LEFT, 0);
			HW_MOT_SetMotorSpeedFastDecay(MOT_CHANNEL_RIGHT,0);
		}

		if(robot.cs.cs_events & DO_POS)
		{
			position_manage(&robot.cs.pos);
			/*printf("robot.cs.pos=%d:%d:%d\n",  robot.cs.pos.pos_s16.x,
                                               robot.cs.pos.pos_s16.y,
                                               robot.cs.pos.pos_s16.a);*/
		}

		/* Blocking-detection manager: TO BE ADDED */
		/* trajectory_hardstop(pRobot.traj);*/

        /* Wakes-up when required */
        vTaskDelayUntil( &xNextWakeTime, AVERSIVE_PERIOD_TICKS);
    }
}

bool Os_MotionTrajectoryNear(void)
{
	return (bool)trajectory_in_window(&robot.cs.traj, traj_near_window_d, traj_near_window_a);
}

bool Os_MotionTrajectoryFinished(void)
{
	return (bool)(trajectory_finished(&robot.cs.traj)&&(robot.cs.traj.scheduler_task==NULL));
}

void OS_MotionSetWindow(double window_d, double window_a, double a_start)
{
	trajectory_set_windows(&robot.cs.traj, window_d, window_a, a_start);
}

void OS_MotionSetNearWindow(double window_d, double window_a)
{
	traj_near_window_d = (double) window_d;
    traj_near_window_a  = (double) window_a;
}

void OS_MotionSetSpeed(int16_t speed_d, int16_t speed_a)
{
	trajectory_set_speed(&robot.cs.traj, speed_d, speed_a);
}
void OS_MotionTrajectoryHardStop(void)
{
	trajectory_hardstop(&robot.cs.traj);
}
void OS_MotionTrajectoryStop(void)
{
	trajectory_stop(&robot.cs.traj);
}
void OS_MotionSetX(int16_t pos_x)
{
	int16_t pos_y = position_get_y_s16(&robot.cs.pos);
	int16_t pos_a = position_get_a_deg_s16(&robot.cs.pos);
	position_set(&robot.cs.pos, pos_x, pos_y, pos_a);
}
void OS_MotionSetY(int16_t pos_y)
{
	int16_t pos_x = position_get_x_s16(&robot.cs.pos);
	int16_t pos_a = position_get_a_deg_s16(&robot.cs.pos);
	position_set(&robot.cs.pos, pos_x, pos_y, pos_a);
}
void OS_MotionSetA(int16_t pos_a)
{
	int16_t pos_x = position_get_x_s16(&robot.cs.pos);
	int16_t pos_y = position_get_y_s16(&robot.cs.pos);
	position_set(&robot.cs.pos, pos_x, pos_y, pos_a);
}

int16_t OS_MotionGetX(void)
{
	return position_get_x_s16(&robot.cs.pos);
}
int16_t OS_MotionGetY(void)
{
	return position_get_y_s16(&robot.cs.pos);
}
int16_t OS_MotionGetA(void)
{
	return position_get_a_deg_s16(&robot.cs.pos);
}

void OS_MotionPowerEnable(void)
{
	robot.cs.cs_events |=   DO_POWER;
	robot.cs.cs_events |=   DO_CS;
}
void OS_MotionPowerDisable(void)
{
	robot.cs.cs_events &= ~ DO_POWER;
	robot.cs.cs_events &= ~ DO_CS;
}

void OS_MotionMoveRelative(double d_mm, double a_deg_rel)
{
	trajectory_d_a_rel(&robot.cs.traj, d_mm, a_deg_rel);
}

void OS_MotionGoToAuto(double pos_x, double pos_y)
{
	trajectory_goto_xy_abs(&robot.cs.traj, pos_x, pos_y);
}

void OS_MotionGoToFwd(double pos_x, double pos_y)
{
	trajectory_goto_forward_xy_abs(&robot.cs.traj, pos_x, pos_y);
}

void OS_MotionGoToBwd(double pos_x, double pos_y)
{
	trajectory_goto_backward_xy_abs(&robot.cs.traj, pos_x, pos_y);
}

void OS_MotionTurnToFront(double pos_x, double pos_y)
{
	trajectory_turnto_xy(&robot.cs.traj, pos_x, pos_y);
}

void OS_MotionTurnToBehind(double pos_x, double pos_y)
{
	trajectory_turnto_xy_behind(&robot.cs.traj, pos_x, pos_y);
}

void vCreateAllMutex(void)
{
	xEncoderAngleMutex = xSemaphoreCreateMutex();
	xEncoderDistanceMutex = xSemaphoreCreateMutex();
	xAngleConsignMutex = xSemaphoreCreateMutex();
	xDistanceConsignMutex = xSemaphoreCreateMutex();
	xRobotPositionMutex = xSemaphoreCreateMutex();
}
void vLockEncoderAngle(void)
{
	xSemaphoreTake(xEncoderAngleMutex, portMAX_DELAY);
}
void vLockEncoderDistance(void)
{
	xSemaphoreTake(xEncoderDistanceMutex, portMAX_DELAY);
}
void vLockAngleConsign(void)
{
	xSemaphoreTake(xAngleConsignMutex, portMAX_DELAY);
}
void vLockDistanceConsign(void)
{
	xSemaphoreTake(xDistanceConsignMutex, portMAX_DELAY);
}
void vLockRobotPosition(void)
{
	xSemaphoreTake(xRobotPositionMutex, portMAX_DELAY);
}
void vUnlockEncoderAngle(void)
{
	xSemaphoreGive(xEncoderAngleMutex);
}
void vUnlockEncoderDistance(void)
{
	xSemaphoreGive(xEncoderDistanceMutex);
}
void vUnlockAngleConsign(void)
{
	xSemaphoreGive(xAngleConsignMutex);
}
void vUnlockDistanceConsign(void)
{
	xSemaphoreGive(xDistanceConsignMutex);
}
void vUnlockRobotPosition(void)
{
	xSemaphoreGive(xRobotPositionMutex);
}

// -----------------------------------------------------------------------------
// HANDLING FUNCTIONS
// -----------------------------------------------------------------------------

void motion_set_wp(wp_t *waypoint) {
	xQueueSend(xWaypointQueue, waypoint, 0);
}

bool motion_is_traj_done(wp_t *waypoint) {
	if(waypoint->trajectory_must_finish)
		return Os_MotionTrajectoryFinished();
	else
		return Os_MotionTrajectoryNear();
}
void motion_clear(void){
	wp_t lost;
	while(xQueueReceive(xWaypointQueue, &lost, 0));
}

void motion_send_wp(wp_t *waypoint) {
    // Maybe we need to move the reference to take into account an offset
    // from the robot's center and an actuator
 //   if(waypoint->type != WP_MOVE_REL) {
  //  	if((waypoint->offset.x != 0) || (waypoint->offset.y != 0)) {
    //		phys_apply_offset(&(robot.cs.pos), (waypoint->coord.abs), (waypoint->offset));
    //  }
 // }

    // Pop a waypoint and send the speed and position it
    switch(waypoint->speed) {
    	case WP_SPEED_FAST:
    		OS_MotionSetSpeed(SPEED_FAST_D, SPEED_FAST_A);
    		break;
    	case WP_SPEED_NORMAL:
    		OS_MotionSetSpeed(SPEED_NORMAL_D, SPEED_NORMAL_A);
    		break;
    	case WP_SPEED_SLOW:
    		OS_MotionSetSpeed(SPEED_SLOW_D, SPEED_SLOW_A);
    		break;
    	case WP_SPEED_VERY_SLOW:
    	default: // this is quite an error
    		OS_MotionSetSpeed(SPEED_VERY_SLOW_D, SPEED_VERY_SLOW_A);
    		break;
    }

    switch(waypoint->type) {
    	// "GOTO" motion (full-motions)
    	case WP_GOTO_AUTO:
    		OS_MotionGoToAuto(waypoint->coord.abs.x, waypoint->coord.abs.y);
    		break;
    	case WP_GOTO_FWD:
    		OS_MotionGoToFwd(waypoint->coord.abs.x, waypoint->coord.abs.y);
    		break;
    	case WP_GOTO_BWD:
    		OS_MotionGoToBwd(waypoint->coord.abs.x, waypoint->coord.abs.y);
    		break;

    	// Relative motion
    	case WP_MOVE_REL:
        // Depending on the sign of the "d", we will go forward or backward
    		OS_MotionMoveRelative(waypoint->coord.rel.d, waypoint->coord.rel.a);
    		break;

  		// "ORIENT" motions (pure rotation)
    	case WP_ORIENT_BEHIND:
    		OS_MotionTurnToBehind(waypoint->coord.abs.x, waypoint->coord.abs.y);
    		break;
    	case WP_ORIENT_FRONT:
    		OS_MotionTurnToFront(waypoint->coord.abs.x, waypoint->coord.abs.y);
    		break;

    	default:
    		// error
    		break;
    } // switch
}
