/* -----------------------------------------------------------------------------
 * Igrebrain
 * I-Grebot strategy board
 * -----------------------------------------------------------------------------
 * File        : ai.c
 * Language    : C
 * Author      : Pierrick B.
 * Date        : 2016-05-05
 * -----------------------------------------------------------------------------
 * Description
 *   See main module file
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev$
 * $LastChangedBy$
 * $LastChangedDate$
 * -----------------------------------------------------------------------------
 * Version     Comment                                   Author       Date
 * 1.0         Creation                                  Pierrick B. 2016-05-05
 * -----------------------------------------------------------------------------
 */

#include "main.h"

// -----------------------------------------------------------------------------
// GLOBALS
// -----------------------------------------------------------------------------

// External definitions
extern wp_t checkpoints[20];
extern phys_t phys;

// -----------------------------------------------------------------------------
// TASKS DEFINITIONS
// -----------------------------------------------------------------------------

// Main holder for inits
void ai_checkpoints_def(void) {
    checkpoints[STRAT_EXIT].coord.abs.x = phys.exit_start.x;
    checkpoints[STRAT_EXIT].coord.abs.y = phys.exit_start.y;
    checkpoints[STRAT_EXIT].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_EXIT].trajectory_must_finish=true;
    checkpoints[STRAT_EXIT].type=WP_GOTO_AUTO;

    checkpoints[STRAT_TURN_TO_CUBE1].coord.abs.x = phys.cube[PHYS_ID_CUBE_1].x;
    checkpoints[STRAT_TURN_TO_CUBE1].coord.abs.y = phys.cube[PHYS_ID_CUBE_1].y;
    checkpoints[STRAT_TURN_TO_CUBE1].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_TURN_TO_CUBE1].trajectory_must_finish = true;
    checkpoints[STRAT_TURN_TO_CUBE1].type = WP_ORIENT_FRONT;

    checkpoints[STRAT_PUSH_CUBE1].coord.abs.x = phys.drop.x;
    checkpoints[STRAT_PUSH_CUBE1].coord.abs.y = phys.drop.y;
    checkpoints[STRAT_PUSH_CUBE1].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_PUSH_CUBE1].trajectory_must_finish=true;
    checkpoints[STRAT_PUSH_CUBE1].type = WP_GOTO_AUTO;

    checkpoints[STRAT_RETURN_HUT_2].coord.abs.x = phys.huts[PHYS_ID_HUT_2].x;
    checkpoints[STRAT_RETURN_HUT_2].coord.abs.y = phys.cube[PHYS_ID_CUBE_1].y;
    checkpoints[STRAT_RETURN_HUT_2].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_RETURN_HUT_2].trajectory_must_finish=true;
    checkpoints[STRAT_RETURN_HUT_2].type=WP_GOTO_BWD;

    checkpoints[STRAT_TURN_TO_HUT_2].coord.abs.x = phys.huts[PHYS_ID_HUT_2].x;
    checkpoints[STRAT_TURN_TO_HUT_2].coord.abs.y = phys.huts[PHYS_ID_HUT_2].y;
    checkpoints[STRAT_TURN_TO_HUT_2].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_TURN_TO_HUT_2].trajectory_must_finish=true;
    checkpoints[STRAT_TURN_TO_HUT_2].type = WP_ORIENT_FRONT;

    checkpoints[STRAT_CLOSE_HUT_2].coord.abs.x = phys.huts[PHYS_ID_HUT_2].x;
    checkpoints[STRAT_CLOSE_HUT_2].coord.abs.y = phys.huts[PHYS_ID_HUT_2].y;
    checkpoints[STRAT_CLOSE_HUT_2].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_CLOSE_HUT_2].trajectory_must_finish=true;
    checkpoints[STRAT_CLOSE_HUT_2].type = WP_GOTO_FWD;

    checkpoints[STRAT_RETURN_HUT_1].coord.abs.x = phys.huts[PHYS_ID_HUT_1].x;
    checkpoints[STRAT_RETURN_HUT_1].coord.abs.y = 500;
    checkpoints[STRAT_RETURN_HUT_1].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_RETURN_HUT_1].trajectory_must_finish=true;
    checkpoints[STRAT_RETURN_HUT_1].type=WP_GOTO_BWD;

    checkpoints[STRAT_TURN_TO_HUT_1].coord.abs.x = phys.huts[PHYS_ID_HUT_1].x;
    checkpoints[STRAT_TURN_TO_HUT_1].coord.abs.y = phys.huts[PHYS_ID_HUT_1].y;
    checkpoints[STRAT_TURN_TO_HUT_1].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_TURN_TO_HUT_1].trajectory_must_finish=true;
    checkpoints[STRAT_TURN_TO_HUT_1].type = WP_ORIENT_FRONT;

    checkpoints[STRAT_CLOSE_HUT_1].coord.abs.x = phys.huts[PHYS_ID_HUT_1].x;
    checkpoints[STRAT_CLOSE_HUT_1].coord.abs.y = phys.huts[PHYS_ID_HUT_1].y;
    checkpoints[STRAT_CLOSE_HUT_1].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_CLOSE_HUT_1].trajectory_must_finish=true;
    checkpoints[STRAT_CLOSE_HUT_1].type=WP_GOTO_AUTO;

    checkpoints[STRAT_CLOSE_HUT_1].coord.abs.x = phys.huts[PHYS_ID_HUT_1].x;
    checkpoints[STRAT_CLOSE_HUT_1].coord.abs.y = phys.huts[PHYS_ID_HUT_1].y;
    checkpoints[STRAT_CLOSE_HUT_1].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_CLOSE_HUT_1].trajectory_must_finish=true;
    checkpoints[STRAT_CLOSE_HUT_1].type=WP_GOTO_AUTO;

    checkpoints[STRAT_PREPARE_TO_CUBE2].coord.abs.x = phys.huts[PHYS_ID_HUT_1].x;
    checkpoints[STRAT_PREPARE_TO_CUBE2].coord.abs.y = 500;
    checkpoints[STRAT_PREPARE_TO_CUBE2].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_PREPARE_TO_CUBE2].trajectory_must_finish=true;
    checkpoints[STRAT_PREPARE_TO_CUBE2].type=WP_GOTO_BWD;

    checkpoints[STRAT_TURN_TO_FACE_SHELLS].coord.abs.x = phys.shells[PHYS_ID_SHELL_SW].x;
    checkpoints[STRAT_TURN_TO_FACE_SHELLS].coord.abs.y = phys.shells[PHYS_ID_SHELL_SW].y;
    checkpoints[STRAT_TURN_TO_FACE_SHELLS].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_TURN_TO_FACE_SHELLS].trajectory_must_finish=true;
    checkpoints[STRAT_TURN_TO_FACE_SHELLS].type=WP_ORIENT_FRONT;

    checkpoints[STRAT_GOTO_SHELL_SW].coord.abs.x = phys.shells[PHYS_ID_SHELL_SW].x;
    checkpoints[STRAT_GOTO_SHELL_SW].coord.abs.y = phys.shells[PHYS_ID_SHELL_SW].y;
    checkpoints[STRAT_GOTO_SHELL_SW].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_GOTO_SHELL_SW].trajectory_must_finish=true;
    checkpoints[STRAT_GOTO_SHELL_SW].type=WP_GOTO_FWD;

    checkpoints[STRAT_GOTO_SHELL_SE].coord.abs.x = phys.shells[PHYS_ID_SHELL_SE].x;
    checkpoints[STRAT_GOTO_SHELL_SE].coord.abs.y = phys.shells[PHYS_ID_SHELL_SE].y;
    checkpoints[STRAT_GOTO_SHELL_SE].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_GOTO_SHELL_SE].trajectory_must_finish=true;
    checkpoints[STRAT_GOTO_SHELL_SE].type=WP_GOTO_FWD;

    checkpoints[STRAT_GOTO_SHELL_NE].coord.abs.x = phys.shells[PHYS_ID_SHELL_NE].x;
    checkpoints[STRAT_GOTO_SHELL_NE].coord.abs.y = phys.shells[PHYS_ID_SHELL_NE].y;
    checkpoints[STRAT_GOTO_SHELL_NE].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_GOTO_SHELL_NE].trajectory_must_finish=true;
    checkpoints[STRAT_GOTO_SHELL_NE].type=WP_GOTO_FWD;

    checkpoints[STRAT_GOTO_START].coord.abs.x = 300;
    checkpoints[STRAT_GOTO_START].coord.abs.y = TABLE_Y_MAX/2;
    checkpoints[STRAT_GOTO_START].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_GOTO_START].trajectory_must_finish=true;
    checkpoints[STRAT_GOTO_START].type=WP_GOTO_FWD;

  /*  checkpoints[STRAT_GO_TO_CUBE2].coord.abs.x = phys.cube[PHYS_ID_CUBE_2].x;
    checkpoints[STRAT_GO_TO_CUBE2].coord.abs.y = 500;
    checkpoints[STRAT_GO_TO_CUBE2].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_GO_TO_CUBE2].trajectory_must_finish=true;
    checkpoints[STRAT_GO_TO_CUBE2].type=WP_GOTO_AUTO;

    checkpoints[STRAT_TURN_TO_CUBE2].coord.abs.x = phys.cube[PHYS_ID_CUBE_2].x;
    checkpoints[STRAT_TURN_TO_CUBE2].coord.abs.y = phys.cube[PHYS_ID_CUBE_2].y;
    checkpoints[STRAT_TURN_TO_CUBE2].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_TURN_TO_CUBE2].trajectory_must_finish=true;
    checkpoints[STRAT_TURN_TO_CUBE2].type=WP_ORIENT_FRONT;

    checkpoints[STRAT_TOUCH_CUBE2].coord.abs.x = phys.cube[PHYS_ID_CUBE_2].x;
    checkpoints[STRAT_TOUCH_CUBE2].coord.abs.y = phys.cube[PHYS_ID_CUBE_2].y;
    checkpoints[STRAT_TOUCH_CUBE2].speed = WP_SPEED_VERY_SLOW;
    checkpoints[STRAT_TOUCH_CUBE2].trajectory_must_finish=true;
    checkpoints[STRAT_TOUCH_CUBE2].type=WP_GOTO_AUTO;

    checkpoints[STRAT_PULL_CUBE2].coord.abs.x = phys.cube[PHYS_ID_CUBE_2].x;
    checkpoints[STRAT_PULL_CUBE2].coord.abs.y = 500;
    checkpoints[STRAT_PULL_CUBE2].speed = WP_SPEED_VERY_SLOW;
    checkpoints[STRAT_PULL_CUBE2].trajectory_must_finish=true;
    checkpoints[STRAT_PULL_CUBE2].type=WP_GOTO_AUTO;

    checkpoints[STRAT_RETURN_TO_START].coord.abs.x = phys.exit_start.x;
    checkpoints[STRAT_RETURN_TO_START].coord.abs.y = phys.exit_start.y;
    checkpoints[STRAT_RETURN_TO_START].speed = WP_SPEED_VERY_SLOW;
    checkpoints[STRAT_RETURN_TO_START].trajectory_must_finish=true;
    checkpoints[STRAT_RETURN_TO_START].type=WP_GOTO_FWD;

    checkpoints[STRAT_TURN_TO_PUSH_CUBE2].coord.abs.x = phys.drop.x;
    checkpoints[STRAT_TURN_TO_PUSH_CUBE2].coord.abs.y = phys.drop.y;
    checkpoints[STRAT_TURN_TO_PUSH_CUBE2].speed = WP_SPEED_VERY_SLOW;
    checkpoints[STRAT_TURN_TO_PUSH_CUBE2].trajectory_must_finish=true;
    checkpoints[STRAT_TURN_TO_PUSH_CUBE2].type=WP_ORIENT_FRONT;

    checkpoints[STRAT_PUSH_CUBE2].coord.abs.x = phys.drop.x;
    checkpoints[STRAT_PUSH_CUBE2].coord.abs.y = phys.drop.y;
    checkpoints[STRAT_PUSH_CUBE2].speed = WP_SPEED_SLOW;
    checkpoints[STRAT_PUSH_CUBE2].trajectory_must_finish=true;
    checkpoints[STRAT_PUSH_CUBE2].type=WP_GOTO_FWD;*/
}
