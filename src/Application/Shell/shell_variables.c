/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2017
 * -----------------------------------------------------------------------------
 * @file       shell_variables.c
 * @author     Bebop35 [Paul M]
 * @date       Dec 25, 2016
 * -----------------------------------------------------------------------------
 * @brief
 *   Short-hands for shell variable handling
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/firm_blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "blueboard.h"

/* External variables; for setting and probing */
extern RobotTypeDef robot;

/*
 * Decode a variable path/name and update it with the provided value
 */
BaseType_t OS_SHL_SetVariable(char* path, char* value)
{
    return pdFALSE;
}

/*
 * Decode a variable path/name and write its current value
 * into the result string.
 */
BaseType_t OS_SHL_GetVariable(char* path, char* value, size_t valueLength)
{
    char* item;

    /* 1st level decoding */
    item = strsep(&path, SHELL_PATH_DELIM);

    if(!strcasecmp(item, "robot")) {
        if((item = strsep(&path, SHELL_PATH_DELIM))) {

            if(!strcasecmp(item, "cs")) {
                if((item = strsep(&path, SHELL_PATH_DELIM))) {

                    if(!strcasecmp(item, "pos")) {
                        if((item = strsep(&path, SHELL_PATH_DELIM))) {

                            if(!strcasecmp(item, "x")) {
                                snprintf(value, valueLength, "%d"SHELL_EOL, robot.cs.pos.pos_s16.x);
                            }
                            else if(!strcasecmp(item, "y")) {
                                snprintf(value, valueLength, "%d"SHELL_EOL, robot.cs.pos.pos_s16.y);
                            }
                            else if(!strcasecmp(item, "a")) {
                                snprintf(value, valueLength, "%d"SHELL_EOL, robot.cs.pos.pos_s16.a);
                            }
                        } else {
                            snprintf(value, valueLength, "%d"SHELL_VAR_DELIM"%d"SHELL_VAR_DELIM"%d"SHELL_EOL,
                                    robot.cs.pos.pos_s16.x,
                                    robot.cs.pos.pos_s16.y,
                                    robot.cs.pos.pos_s16.a);
                        }
                    }

                    else if(!strcasecmp(item, "cs_a")) {
                        if((item = strsep(&path, SHELL_PATH_DELIM))) {

                            if(!strcasecmp(item, "consign"))
                            {
                                snprintf(value, valueLength, "%ld"SHELL_EOL, robot.cs.cs_a.consign_value);
                            }
                            else if(!strcasecmp(item, "out"))
                            {
                                snprintf(value, valueLength, "%ld"SHELL_EOL, robot.cs.cs_a.out_value);
                            }

                        }

                    }

                } else {
                    printf("robot.cs base structure"SHELL_EOL);
                }

            }

        } else {

            printf("robot base structure"SHELL_EOL);
        }

    } // robot
    else if(!strcasecmp(item, "system")) {
        printf("system structure"SHELL_EOL);
    }

    // If this point is reached; there was an error
    return pdFALSE;

error:
    snprintf(value, valueLength, "Error: unrecognized value"SHELL_EOL);
    return pdFALSE;

   //printf("* %s"SHELL_EOL, item);

}
