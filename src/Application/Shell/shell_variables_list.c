/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2017
 * -----------------------------------------------------------------------------
 * @file       shell_variables_list.c
 * @author     Bebop35 [Paul M]
 * @date       Jan 6, 2017
 * -----------------------------------------------------------------------------
 * @brief
 *   Details the variables that are accessible through the shell
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/firm_blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "blueboard.h"

/* External variables; for setting and probing */
extern OS_SHL_ConfigTypeDef OS_SHL_Config;
extern RobotTypeDef robot;

/* Variables definition list holder */
const OS_SHL_VarItemTypeDef varList[] = {
        {"sys.shell.echo"       , TYPE_BOOL,  ACC_WR, &OS_SHL_Config.echo},
        {"robot.cs.pos.x"       , TYPE_INT16, ACC_RD, &robot.cs.pos.pos_s16.x},
        {"robot.cs.pos.y"       , TYPE_INT16, ACC_RD, &robot.cs.pos.pos_s16.y},
        {"robot.cs.pos.a"       , TYPE_INT16, ACC_RD, &robot.cs.pos.pos_s16.a}
};
const size_t varListLength = sizeof(varList) / sizeof(OS_SHL_VarItemTypeDef);

