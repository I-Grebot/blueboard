#ifndef _SHELL_H_
#define _SHELL_H_

/**
 ********************************************************************************
 **
 **  Types definition
 **
 ********************************************************************************
 */

typedef struct {
    bool echo;
} OS_SHL_ConfigTypeDef;

/**
 ********************************************************************************
 **
 **  Shell Constants
 **
 ********************************************************************************
 */

/* Dimensions the buffer into which input characters are placed. */
#define SHELL_MAX_INPUT_SIZE        100

/* DEL acts as a backspace. */
#define SHELL_ASCII_DEL             ( 0x7F )

/* The maximum time to wait for the mutex that guards the UART to become
available. */
#define SHELL_MAX_MUTEX_WAIT        pdMS_TO_TICKS( 300 )

/* End-Of-Line characters that should be used */
#define SHELL_EOL                       "\n\r"

/* String displayed after each output */
#define SHELL_END_OF_OUTPUT_STR         "\n\r> "

/* Delimiter for hierarchical paths */
#define SHELL_PATH_DELIM                 "."

/* Delimiter for multiple variables output */
#define SHELL_VAR_DELIM                  ":"

/* The welcome message display at shell's startup */
#define SHELL_WELCOME_MESSAGE "\f"\
    "-----------------------------------------------------------\n\r"\
    "  IgreBot "YEAR_STR" ~ Command Shell\n\r"\
    "-----------------------------------------------------------\n\r"\
    "  Robot : "ROBOT_NAME_STR"\n\r"\
    "  Build : "BUILD_VERSION_STR"\n\r"\
    "-----------------------------------------------------------\n\r"\
    " Type 'help' for the list of available commands\n\r"\
    "-----------------------------------------------------------\n\r\n\r"\
    "> "

#endif /* _SHELL_H_ */
