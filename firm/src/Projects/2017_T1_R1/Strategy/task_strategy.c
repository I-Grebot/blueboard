/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       task_strategy.c
 * @author     Pierrick
 * @date       Apr 29, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   This task handle the whole strategy
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev$
 * $LastChangedBy$
 * $LastChangedDate$
 * -----------------------------------------------------------------------------
 */

/* Inclusion */
#include "main.h"

#define UP 2950
#define DOWN 2000

/* Local Variable */
match_t match;
static robot_t robot;
static strategy_state_e strat;

extern uint8_t sw_irq;
// Array containing the different tasks
extern task_t tasks[TASKS_NB];
// Physical definitions
extern phys_t phys;
// Avoidance handler
extern av_t av;
char cBuffer[50];
wp_t checkpoints[20];
/* Local, Private functions */
static void OS_StrategyTask(void *pvParameters);


void OS_CreateStrategyTask(void)
{
    xTaskCreate(OS_StrategyTask, "Strategy", 250, NULL, OS_TASK_PRIORITY_STRATEGY, NULL );
}

static void OS_StrategyTask( void *pvParameters )
{
	TickType_t xNextWakeTime;
    poi_t reset_pos;
    uint16_t parasol_pos = 1500;
   // path_init();
    ASV_DeployParasol(parasol_pos);
    strategy_init();

    uint32_t tick_cnt = 0;
    char str[64];

    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    /* Initialise xNextWakeTime - this only needs to be done once. */
    xNextWakeTime = xTaskGetTickCount();

    LedSetColor(HW_LED_BLUE);
    //OS_DebugTaskPrint("Starting!\n");

    // TEMP
    while(1) {
    	sprintf(str, "Tick %lu\n", tick_cnt++);
    	OS_DebugTaskPrint(str);
    	vTaskDelayUntil( &xNextWakeTime, 1000);
    }

    for( ;; )
    {
    	// Main match branching
    	switch(match.state) {

        // --------------------
    	case MATCH_STATE_RESET:
        // --------------------
    		parasol_pos += 10;
    		ASV_DeployParasol(parasol_pos);
    		if(parasol_pos>=2200)
    		{
        		vTaskDelay(READY_DELAY_MSEC/portTICK_RATE_MS);
    			match.state = MATCH_STATE_READY;


    		}
        break;
        // --------------------
        case MATCH_STATE_READY:
        // --------------------
    	    LedSetColor(HW_LED_WHITE);
        	// Awaits for user input to start the INIT procedure
            if(START_JACK==PRESENT)
            {
            	match.state = MATCH_STATE_INIT;
            	ASV_MoveIndex(UP);
            }
        break;
        // --------------------
        case MATCH_STATE_INIT:
        // --------------------
        	if(!robot.init_done)
        	{
        		// Default speed
        		OS_MotionSetSpeed(SPEED_VERY_SLOW_D, SPEED_VERY_SLOW_A);
        		//can_send_motion_set_near_window(200, 10);		TODO

        		// Set the flag so INIT commands are sent only once
        		robot.init_done = 1;

        		// Init commands are done but we want to wait some time
        		// Before being sensitive to the match start
                OS_MotionPowerEnable();
        	}
        	else
        	{
        		vTaskDelay(WAIT_START_DELAY_MSEC/portTICK_RATE_MS);
        		match.state = MATCH_STATE_WAIT_START;
        	}
        break;
        // -------------------------
        case MATCH_STATE_WAIT_START:
        // -------------------------
        	// Awaits for Start trigger
            if(START_JACK==ABSENT)
            {
            	// We need to have at least a basic software debouncing
                // to prevent false starts. START_JACK must be 1 after the
                // given time in order to detect the match
            	if(match.timer_msec>=START_DEBOUNCING_DELAY_MSEC/portTICK_RATE_MS)
                {
            		match.state = MATCH_STATE_RUN;
            		LedSetMode(HW_LED_BLINK_SLOW);
                    match.timer_msec = 0;
                    match.scored_points = 0;

                    // Sample the color
                    if(COLOR==MATCH_COLOR_PURPLE)
                    {
                    	LedSetColor(HW_LED_MAGENTA);
                        match.color = MATCH_COLOR_PURPLE;
                    }
                    else
                    {
                    	LedSetColor(HW_LED_GREEN);
                        match.color = MATCH_COLOR_GREEN;
                    }
                    // Update all variables that depends on the color choice

                    // Robot position
                          reset_pos = phys.reset;
                          phys_update_with_color(&reset_pos);
                    // Nasty patch: offset reset for green
//                    if(match.color == MATCH_COLOR_GREEN) {
                  //             reset_pos.x -= 10;
  //                         }

                //           robot.pos = reset_pos;

                           // Define static obstacles positions
                      //     phys_set_obstacle_positions();

                           // Opponent position: probably in starting zone but we don't know
                    robot.opp_pos.x = OPPONENT_POS_INIT_X;
                    robot.opp_pos.y = OPPONENT_POS_INIT_Y;
                    robot.opp_pos.a = 0;
                    // phys_update_with_color(&robot.opp_pos);
                    //    phys_set_opponent_position(1, robot.opp_pos.x, robot.opp_pos.y);

                    // Reset motion position and enable motor power
                    OS_MotionSetX(reset_pos.x);
                    OS_MotionSetY(reset_pos.y);
                    OS_MotionSetA(reset_pos.a);


                    phys_update_with_color(&phys.exit_start);
                    phys_update_with_color(&phys.drop);
                    phys_update_with_color(&phys.cube[PHYS_ID_CUBE_1]);
                    phys_update_with_color(&phys.huts[PHYS_ID_HUT_1]);
                    phys_update_with_color(&phys.huts[PHYS_ID_HUT_2]);
                    strat = STRAT_EXIT;
                }
            	else
                    match.timer_msec += DO_STRATEGY_MSEC;
            }
            else
            {
                // Sample the color
                if(COLOR==MATCH_COLOR_PURPLE)
                {
                	LedSetColor(HW_LED_MAGENTA);
                    match.color = MATCH_COLOR_PURPLE;
                }
                else
                {
                	LedSetColor(HW_LED_GREEN);
                    match.color = MATCH_COLOR_GREEN;
                }
            	match.timer_msec = 0;
            }
        break;
        // ------------------
        case MATCH_STATE_RUN:
        // ------------------
        	// Count the time and execute the match strategy
        	if(match.timer_msec >= MATCH_DURATION_MSEC)
        	{
        		OS_MotionTrajectoryHardStop();
        		OS_MotionPowerDisable();
     //       motion_clear();
        		match.state = MATCH_STATE_STOPPED;
        		LedSetColor(HW_LED_RED);
        		LedSetMode(HW_LED_STATIC);
        	}
        	else
        	{
        		do_match();
        		match.timer_msec += DO_STRATEGY_MSEC;
        	}
        break;
        // ------------------
        case MATCH_STATE_STOPPED:
        // ------------------
        	ASV_DeployParasol(1500);
        break;
        default:
    	break;
        }
        vTaskDelayUntil( &xNextWakeTime, DO_STRATEGY_MSEC);
    }
}



void strategy_init(void) {
	// Initialize the match content fields
	match.color = MATCH_COLOR_UNKNOWN;
    match.state = MATCH_STATE_RESET;
    match.scored_points = 0;
    match.timer_msec = 0;

    // Initialize physical engine
    phys_init();

    // Initialize all checkpoint
    ai_checkpoints_def();

    // Initialize the robot content fields
    robot.av_back_left = false;
    robot.av_back_right = false;
    robot.av_front_center = false;
    robot.av_front_left = false;
    robot.av_front_right = false;
    robot.init_done = 0;
}

void do_match(void) {
	//static strategy_state_e strat_state = STRAT_EXIT;

	if(Os_MotionTrajectoryFinished()&&(strat<STRAT_FINISHED))
	{
		phys_update_with_color(&checkpoints[strat].coord.abs);
		motion_send_wp(&checkpoints[strat++]);
		if(strat>STRAT_TURN_TO_FACE_SHELLS)
		{
				ASV_DeployLeftArm();
				ASV_DeployRightArm();
		}
		else
		{
			ASV_IdleLeftArm();
			ASV_IdleRightArm();
			ASV_MoveIndex(UP);
		}
	}
	if(av_detection_is_valid())
	{
		motion_clear();
		OS_MotionTrajectoryHardStop();
		if(strat>STRAT_EXIT)
			strat--;
	}
} // do_match()
