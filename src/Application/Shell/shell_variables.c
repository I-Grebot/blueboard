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
extern OS_SHL_ConfigTypeDef OS_SHL_Config;

/*
 * Decode a variable path/name and update it with the provided value
 */
BaseType_t OS_SHL_SetVariable(char* path, char* value, char* ret, size_t retLength)
{

    char* item;

    /* 1st level decoding */
    item = strsep(&path, SHELL_PATH_DELIM);

    if(!strcasecmp(item, "sys")) {
       if((item = strsep(&path, SHELL_PATH_DELIM))) {

           if(!strcasecmp(item, "shell")) {
               if((item = strsep(&path, SHELL_PATH_DELIM))) {

                   if(!strcasecmp(item, "echo")) {
                       // TODO: bool fetcher
                       if(!strcasecmp(value, "1")) {
                           OS_SHL_Config.echo = true;
                       }
                       else if(!strcasecmp(value, "0")) {
                           OS_SHL_Config.echo = false;
                       }


                   }
               }

           } // shell
       }

   } // sys

    snprintf(ret, retLength, "");
    return pdFALSE;
}

/*
 * Decode a variable path/name and write its current value
 * into the result string.
 */
BaseType_t OS_SHL_GetVariable(char* path, char* ret, size_t retLength)
{
    char* item;

    /*snprintf(ret, retLength, "%d"SHELL_VAR_DELIM"%d"SHELL_VAR_DELIM"%d"SHELL_EOL,
                                        robot.cs.pos.pos_s16.x,
                                        robot.cs.pos.pos_s16.y,
                                        robot.cs.pos.pos_s16.a);*/

    /* 1st level decoding */
    item = strsep(&path, SHELL_PATH_DELIM);

    if(!strcasecmp(item, "robot")) {
        if((item = strsep(&path, SHELL_PATH_DELIM))) {

            if(!strcasecmp(item, "cs")) {
                if((item = strsep(&path, SHELL_PATH_DELIM))) {

                    if(!strcasecmp(item, "pos")) {
                        if((item = strsep(&path, SHELL_PATH_DELIM))) {

                            if(!strcasecmp(item, "x")) {
                                snprintf(ret, retLength, "%d"SHELL_EOL, robot.cs.pos.pos_s16.x);
                            }
                            else if(!strcasecmp(item, "y")) {
                                snprintf(ret, retLength, "%d"SHELL_EOL, robot.cs.pos.pos_s16.y);
                            }
                            else if(!strcasecmp(item, "a")) {
                                snprintf(ret, retLength, "%d"SHELL_EOL, robot.cs.pos.pos_s16.a);
                            }
                        } else {
                            snprintf(ret, retLength, "robot.cs.pos=%d"SHELL_VAR_DELIM"%d"SHELL_VAR_DELIM"%d"SHELL_EOL,
                                    robot.cs.pos.pos_s16.x,
                                    robot.cs.pos.pos_s16.y,
                                    robot.cs.pos.pos_s16.a);
                        }
                    }

                    else if(!strcasecmp(item, "cs_a")) {
                        if((item = strsep(&path, SHELL_PATH_DELIM))) {

                            if(!strcasecmp(item, "consign"))
                            {
                                snprintf(ret, retLength, "%ld"SHELL_EOL, robot.cs.cs_a.consign_value);
                            }
                            else if(!strcasecmp(item, "out"))
                            {
                                snprintf(ret, retLength, "%ld"SHELL_EOL, robot.cs.cs_a.out_value);
                            }

                        }

                    }

                }

            }

        }

    } // robot
    else if(!strcasecmp(item, "sys")) {
        if((item = strsep(&path, SHELL_PATH_DELIM))) {

            if(!strcasecmp(item, "shell")) {
                if((item = strsep(&path, SHELL_PATH_DELIM))) {

                    if(!strcasecmp(item, "echo")) {
                        snprintf(ret, retLength, "%u"SHELL_EOL, OS_SHL_Config.echo);
                    }
                }

            } // shell
        }

    } // sys

    // If this point is reached; there was an error
    return pdFALSE;

error:
    snprintf(ret, retLength, "Error: unrecognized value"SHELL_EOL);
    return pdFALSE;

   //printf("* %s"SHELL_EOL, item);

}
