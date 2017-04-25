#ifndef _SHELL_H_
#define _SHELL_H_

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

/* Prefixes for returned strings */
#define SHELL_ERR_PFX           "[ERR] "    // For all errors

#define SHELL_SYS_PFX           "[SYS] "    // For returns of Sys command
#define SHELL_GET_PFX           "[GET] "    // For returns of Get command
#define SHELL_PRB_PFX           "[PRB] "    // For returns of Prb command
#define SHELL_SET_PFX           "[SET] "    // For returns of Set command
#define SHELL_VAR_PFX           "[VAR] "    // For returns of Var command
#define SHELL_POW_PFX           "[POW] "    // For returns of Pow command
#define SHELL_LED_PFX           "[LED] "    // For returns of Led command
#define SHELL_DSV_PFX           "[DSV] "    // For returns of Dsv command
#define SHELL_MOT_PFX           "[MOT] "    // For returns of Mot command
#define SHELL_STR_PFX           "[STR] "    // For returns of Str command

/* String displayed after each output */
#define SHELL_END_OF_OUTPUT_STR         "\n\r> "

/* Delimiter for hierarchical paths */
#define SHELL_PATH_DELIM                 "."

/* Delimiter for multiple variables output */
#define SHELL_VAR_DELIM                  ":"

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

typedef enum {
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_UINT8,
    TYPE_INT8,
    TYPE_UINT16,
    TYPE_INT16,
    TYPE_UINT32,
    TYPE_INT32,
    TYPE_UINT64,
    TYPE_INT64,
    TYPE_FLOAT,
    TYPE_DOUBLE
} OS_SHL_VarTypeEnum;

typedef enum {
    ACC_WR, // Write / Read
    ACC_RD, // Read-Only
    ACC_RQ // Request (Write then auto-reset)
} OS_SHL_VarAccessEnum;

typedef struct {
    char* name;                     // Name
    OS_SHL_VarTypeEnum type;        // Type enum (for fast lookup)
    OS_SHL_VarAccessEnum access;    // Access type of the variable
    const volatile void* var;       // Pointer on the variable
    char* unit;                     // Unit of the variable (if relevant)
} OS_SHL_VarItemTypeDef;

#endif /* _SHELL_H_ */
