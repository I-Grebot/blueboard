/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2017
 * -----------------------------------------------------------------------------
 * @file       shell_commands.c
 * @author     Bebop35 [Paul M]
 * @date       Dec 04, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   Implements CLI commands.
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/firm_blueboard.git
 * -----------------------------------------------------------------------------
 */

/* Inclusion */
#include "blueboard.h"

/* -----------------------------------------------------------------------------
 * Prototypes of shell commands (local and private functions)
 * -----------------------------------------------------------------------------
 */

/* System commands */
static BaseType_t OS_SHL_SysCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

/* Parameters and variables management */
static BaseType_t OS_SHL_SetCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t OS_SHL_GetCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t OS_SHL_StoCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

/* Hardware low-level modules management */
static BaseType_t OS_SHL_PowCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ); // Power
static BaseType_t OS_SHL_MotCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ); // Motors
static BaseType_t OS_SHL_DsvCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ); // Digital Servos
static BaseType_t OS_SHL_AsvCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ); // Analog Servos
static BaseType_t OS_SHL_MonCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ); // Monitoring
static BaseType_t OS_SHL_DioCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ); // Digital I/Os
static BaseType_t OS_SHL_HmiCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ); // HMI Board

/* High-level modules management */
static BaseType_t OS_SHL_LedCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ); // Led
static BaseType_t OS_SHL_AvsCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ); // Aversive
static BaseType_t OS_SHL_AvdCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ); // Avoidance
static BaseType_t OS_SHL_StrCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ); // Strategy


/* -----------------------------------------------------------------------------
 * Commands definitions
 * -----------------------------------------------------------------------------
 */

// System commands
static const CLI_Command_Definition_t xSys =
{
    "sys",
    SHELL_EOL
    "sys [command]: Run a system command."SHELL_EOL
    " List of available commands:"SHELL_EOL
    "  - 'reset'      : Reset the microcontroller"SHELL_EOL
    "  - 'tasks-list' : Display tasks info"SHELL_EOL
    "  - 'tasks-stats' : Display tasks statistics"SHELL_EOL
    ,OS_SHL_SysCmd,
    1
};

// Variable set command
static const CLI_Command_Definition_t xSet =
{
    "set",
    SHELL_EOL
    "set [name] [value]: Set a variable value (in RAM)."SHELL_EOL
    " - [name]  : Name of the variable to set"SHELL_EOL
    " - [value] : New value"SHELL_EOL
    ,OS_SHL_SetCmd,
    2
};

// Variable get command
static const CLI_Command_Definition_t xGet =
{
    "get",
    SHELL_EOL
    "get [name]: Retrieve a variable value (from RAM)."SHELL_EOL
    " - [name]  : Name of the variable to get"SHELL_EOL
    ,OS_SHL_GetCmd,
    1
};

// Variable store command
static const CLI_Command_Definition_t xSto =
{
    "sto",
    SHELL_EOL
    "-- UNIMPLEMENTED --"SHELL_EOL
    "sto [name]: Store a variable value (from RAM into FLASH)."SHELL_EOL
    " - [name]  : Name of the variable to store"SHELL_EOL
    ,OS_SHL_StoCmd,
    1
};

// Power commands
static const CLI_Command_Definition_t xPow =
{
    "pow",
    SHELL_EOL
    "pow [1|2|3|ALL] [ON|OFF]: Enable or Disable a power-supply"SHELL_EOL
    ,OS_SHL_PowCmd,
    2
};

// Motor commands
static const CLI_Command_Definition_t xMot =
{
    "mot",
    SHELL_EOL
    "mot [command] [value1]... [valueN]: Run a motor command."SHELL_EOL
    " List of available commands:"SHELL_EOL
    "  - [cmd1] [value1] [value2]"SHELL_EOL
    "  - [cmd2] [value1]"SHELL_EOL
    ,OS_SHL_MotCmd,
    -1 // Variable
};

// Digital Servos commands
static const CLI_Command_Definition_t xDsv =
{
    "dsv",
    SHELL_EOL
    "dsv [command] [value1]... [valueN]: Run a digital-servo command."SHELL_EOL
    " List of available commands:"SHELL_EOL
    "  - [cmd1] [value1] [value2]"SHELL_EOL
    ,OS_SHL_DsvCmd,
    -1 // Variable
};

// Analog Servos commands
static const CLI_Command_Definition_t xAsv =
{
    "asv",
    SHELL_EOL
    "asv [command] [value1]... [valueN]: Run an analog-servo command."SHELL_EOL
    " List of available commands:"SHELL_EOL
    "  - [cmd1] [value1] [value2]"SHELL_EOL
    ,OS_SHL_AsvCmd,
    -1 // Variable
};

// Monitoring commands
static const CLI_Command_Definition_t xMon =
{
    "mon",
    SHELL_EOL
    "mon [command] [value1]... [valueN]: Run a monitoring command."SHELL_EOL
    " List of available commands:"SHELL_EOL
    "  - [cmd1] [value1] [value2]"SHELL_EOL
    ,OS_SHL_MonCmd,
    -1 // Variable
};

// Digital I/Os commands
static const CLI_Command_Definition_t xDio =
{
    "dio",
    SHELL_EOL
    "dio [command] [value1]... [valueN]: Run an digital I/O command."SHELL_EOL
    " List of available commands:"SHELL_EOL
    "  - [cmd1] [value1] [value2]"SHELL_EOL
    ,OS_SHL_DioCmd,
    -1 // Variable
};

// Human-Machine Interface commands
static const CLI_Command_Definition_t xHmi =
{
    "hmi",
    SHELL_EOL
    "hmi [command] [value1]... [valueN]: Run a HMI command."SHELL_EOL
    " List of available commands:"SHELL_EOL
    "  - [cmd1] [value1] [value2]"SHELL_EOL
    ,OS_SHL_HmiCmd,
    -1 // Variable
};

// Led commands
static const CLI_Command_Definition_t xLed =
{
    "led",
    SHELL_EOL
    "led [mode] [color]: Set the LED state"SHELL_EOL
    "  [mode]  : STATIC|BLINK_SLOW|BLINK_FAST"SHELL_EOL
    "  [color] : OFF  | WHITE   |"SHELL_EOL
    "            RED  | GREEN   | BLUE |"SHELL_EOL
    "            CYAN | MAGENTA | YELLOW ]"SHELL_EOL
    ,OS_SHL_LedCmd,
    2
};

// Aversive commands
static const CLI_Command_Definition_t xAvs =
{
    "avs",
    SHELL_EOL
    "avs [command] [value1]... [valueN]: Run an Aversive command."SHELL_EOL
    " List of available commands:"SHELL_EOL
    "  - [cmd1] [value1] [value2]"SHELL_EOL
    ,OS_SHL_AvsCmd,
    -1 // Variable
};

// Avoidance commands
static const CLI_Command_Definition_t xAvd =
{
    "avd",
    SHELL_EOL
    "avd [command] [value1]... [valueN]: Run an Avoidance command."SHELL_EOL
    " List of available commands:"SHELL_EOL
    "  - [cmd1] [value1] [value2]"SHELL_EOL
    ,OS_SHL_AvdCmd,
    -1 // Variable
};

// Strategy commands
static const CLI_Command_Definition_t xStr =
{
    "str",
    SHELL_EOL
    "str [command] [value1]... [valueN]: Run a Strategy command."SHELL_EOL
    " List of available commands:"SHELL_EOL
    "  - [cmd1] [value1] [value2]"SHELL_EOL
    ,OS_SHL_StrCmd,
    -1 // Variable
};



/*
 * TODO:
 * - chgt config (load aversive cfg)
 * - live modes (probe values)
 * - 'helpall [command]' command (move parameters description into that)
 * - .... tartiflette
 */

/* -----------------------------------------------------------------------------
 * Commands Registration
 * -----------------------------------------------------------------------------
 */


void OS_SHL_RegisterCommands( void )
{
    FreeRTOS_CLIRegisterCommand( &xSys );
    FreeRTOS_CLIRegisterCommand( &xSet );
    FreeRTOS_CLIRegisterCommand( &xGet );
    FreeRTOS_CLIRegisterCommand( &xSto );
    FreeRTOS_CLIRegisterCommand( &xPow );
    FreeRTOS_CLIRegisterCommand( &xMot );
    FreeRTOS_CLIRegisterCommand( &xDsv );
    FreeRTOS_CLIRegisterCommand( &xAsv );
    FreeRTOS_CLIRegisterCommand( &xMon );
    FreeRTOS_CLIRegisterCommand( &xDio );
    FreeRTOS_CLIRegisterCommand( &xHmi );
    FreeRTOS_CLIRegisterCommand( &xLed );
    FreeRTOS_CLIRegisterCommand( &xAvs );
    FreeRTOS_CLIRegisterCommand( &xAvd );
    FreeRTOS_CLIRegisterCommand( &xStr );
}

/* -----------------------------------------------------------------------------
 * Commands functions
 * -----------------------------------------------------------------------------
 */

static BaseType_t OS_SHL_SysCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char *const pcHeader_list =
                "   State   Prio.   Stack   #"SHELL_EOL
                "------------------------------------------"SHELL_EOL;
    BaseType_t xSpacePadding;


    char* pcParameter1;
    BaseType_t xParameter1StringLength;

    /* Get parameters */
    pcParameter1 = (char*) FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameter1StringLength);

    /* Terminate both strings */
    pcParameter1[ xParameter1StringLength ] = 0x00;

    /* 'RESET' Sub-command: reset the microcontroller */
    if(!strcasecmp(pcParameter1, "reset")) {
        snprintf( pcWriteBuffer, xWriteBufferLen, "Resetting..."SHELL_EOL);
        NVIC_SystemReset(); /* Auto-kill */
        return pdFALSE;

    /* 'TASKS-LIST' Sub-command: display tasks informations */
    } else if(!strcasecmp(pcParameter1, "tasks-list")) {

        /* Generate a table of task stats. */
        strcpy( pcWriteBuffer, "Task" );
        pcWriteBuffer += strlen( pcWriteBuffer );

        /* Minus three for the null terminator and half the number of characters in
        "Task" so the column lines up with the centre of the heading. */
        configASSERT( configMAX_TASK_NAME_LEN > 3 );
        for( xSpacePadding = strlen( "Task" ); xSpacePadding < ( configMAX_TASK_NAME_LEN - 3 ); xSpacePadding++ )
        {
            /* Add a space to align columns after the task's name. */
            *pcWriteBuffer = ' ';
            pcWriteBuffer++;

            /* Ensure always terminated. */
            *pcWriteBuffer = 0x00;
        }
        strcpy( pcWriteBuffer, pcHeader_list );
        vTaskList( pcWriteBuffer + strlen( pcHeader_list ) );
        return pdFALSE;

    /* 'TASKS-STATS' Sub-command: display tasks statistics */
    } else if(!strcasecmp(pcParameter1, "tasks-stats")) {
        HW_SYS_GetRunTimeStats( pcWriteBuffer );
        return pdFALSE;

    /* Error case */
    } else {
        snprintf( pcWriteBuffer, xWriteBufferLen, "Error: unknown system command %s"SHELL_EOL, pcParameter1);
        return pdFALSE;
    }

}

static BaseType_t OS_SHL_SetCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    char* pcParameter1;
    char* pcParameter2;
    BaseType_t xParameter1StringLength;
    BaseType_t xParameter2StringLength;

    /* Get parameters */
    pcParameter1 = (char*) FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameter1StringLength);
    pcParameter2 = (char*) FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParameter2StringLength);

    /* Terminate both strings */
    pcParameter1[ xParameter1StringLength ] = 0x00;
    pcParameter2[ xParameter2StringLength ] = 0x00;

    /* Decode variable path */
    return OS_SHL_SetVariable(pcParameter1, pcParameter2, pcWriteBuffer, xWriteBufferLen);
}

static BaseType_t OS_SHL_GetCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    char* pcParameter1;
    BaseType_t xParameter1StringLength;

    /* Get parameters */
    pcParameter1 = (char*) FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameter1StringLength);

    /* Terminate string */
    pcParameter1[ xParameter1StringLength ] = 0x00;

    /* Decode variable path */
    return OS_SHL_GetVariable(pcParameter1, pcWriteBuffer, xWriteBufferLen);

}

static BaseType_t OS_SHL_StoCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    return pdFALSE;
}

static BaseType_t OS_SHL_PowCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    char* pcParameter1;
    char* pcParameter2;
    BaseType_t xParameter1StringLength;
    BaseType_t xParameter2StringLength;
    uint8_t allSupplies = 0;
    HW_PWR_TypeDef powerSupply;

    /* Get parameters */
    pcParameter1 = (char*) FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameter1StringLength);
    pcParameter2 = (char*) FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParameter2StringLength);

    /* Terminate both strings */
    pcParameter1[ xParameter1StringLength ] = 0x00;
    pcParameter2[ xParameter2StringLength ] = 0x00;

    /* Decode <no> argument */
    if(!strcasecmp(pcParameter1, "1")) {
        powerSupply = HW_PWR_VP1;
    } else if(!strcasecmp(pcParameter1, "2")) {
        powerSupply = HW_PWR_VP2;
    } else if(!strcasecmp(pcParameter1, "3")) {
        powerSupply = HW_PWR_VP3;
    } else if(!strcasecmp(pcParameter1, "ALL")) {
        allSupplies = 1;
    } else {
        snprintf( pcWriteBuffer, xWriteBufferLen, "Error: could not define power-supply no to %s"SHELL_EOL, pcParameter1);
        return pdFALSE;
    }

    /* Decode <state> argument */
    if(!strcasecmp(pcParameter2, "OFF")) {
        if(!allSupplies) {
            HW_PWR_Disable(powerSupply);
        } else {
            HW_PWR_Disable(HW_PWR_VP1);
            HW_PWR_Disable(HW_PWR_VP2);
            HW_PWR_Disable(HW_PWR_VP3);
        }
    } else if(!strcasecmp(pcParameter2, "ON")) {
        if(!allSupplies) {
            HW_PWR_Enable(powerSupply);
        } else {
            HW_PWR_Enable(HW_PWR_VP1);
            HW_PWR_Enable(HW_PWR_VP2);
            HW_PWR_Enable(HW_PWR_VP3);
        }
    } else {
        snprintf( pcWriteBuffer, xWriteBufferLen, "Error: could not define power-supply state to %s"SHELL_EOL, pcParameter2);
        return pdFALSE;
    }

    snprintf( pcWriteBuffer, xWriteBufferLen, "Defined power-supply %s to %s"SHELL_EOL, pcParameter1, pcParameter2);
    return pdFALSE;
}

// Motor control. Not reentrant function
static BaseType_t OS_SHL_MotCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char *pcParameter;
    BaseType_t lParameterStringLength;
    BaseType_t xReturn;

    /* Note that the use of the static parameter means this function is not reentrant. */
    static BaseType_t lParameterNumber = 0;

        if( lParameterNumber == 0 )
        {
            /* lParameterNumber is 0, so this is the first time the function has been
            called since the command was entered.  Return the string "The parameters
            were:" before returning any parameter strings. */
            sprintf( pcWriteBuffer, "The parameters were:\r\n" );

            /* Next time the function is called the first parameter will be echoed
            back. */
            lParameterNumber = 1L;

            /* There is more data to be returned as no parameters have been echoed
            back yet, so set xReturn to pdPASS so the function will be called again. */
            xReturn = pdPASS;
        }
        else
        {
            /* lParameter is not 0, so holds the number of the parameter that should
            be returned.  Obtain the complete parameter string. */
            pcParameter = FreeRTOS_CLIGetParameter
                            (
                                /* The command string itself. */
                                pcCommandString,
                                /* Return the next parameter. */
                                lParameterNumber,
                                /* Store the parameter string length. */
                                &lParameterStringLength
                            );

            if( pcParameter != NULL )
            {
                /* There was another parameter to return.  Copy it into pcWriteBuffer.
                in the format "[number]: [Parameter String". */
                memset( pcWriteBuffer, 0x00, xWriteBufferLen );
                sprintf( pcWriteBuffer, "%ld: ", lParameterNumber );
                strncat( pcWriteBuffer, pcParameter, lParameterStringLength );
                strncat( pcWriteBuffer, "\r\n", strlen( "\r\n" ) );

                /* There might be more parameters to return after this one, so again
                set xReturn to pdTRUE. */
                xReturn = pdTRUE;
                lParameterNumber++;
            }
            else
            {
                /* No more parameters were found.  Make sure the write buffer does
                not contain a valid string to prevent junk being printed out. */
                pcWriteBuffer[ 0 ] = 0x00;

                /* There is no more data to return, so this time set xReturn to
                pdFALSE. */
                xReturn = pdFALSE;

                /* Start over the next time this command is executed. */
                lParameterNumber = 0;
            }
        }

        return xReturn;
}

static BaseType_t OS_SHL_DsvCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    return pdFALSE;
}

static BaseType_t OS_SHL_AsvCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    return pdFALSE;
}

static BaseType_t OS_SHL_MonCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    return pdFALSE;
}

static BaseType_t OS_SHL_DioCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    return pdFALSE;
}

static BaseType_t OS_SHL_HmiCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    return pdFALSE;
}


static BaseType_t OS_SHL_LedCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    char* pcParameter1;
    char* pcParameter2;
    BaseType_t xParameter1StringLength;
    BaseType_t xParameter2StringLength;

    /* Get parameters */
    pcParameter1 = (char*) FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameter1StringLength);
    pcParameter2 = (char*) FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParameter2StringLength);

    /* Terminate both strings */
    pcParameter1[ xParameter1StringLength ] = 0x00;
    pcParameter2[ xParameter2StringLength ] = 0x00;

    /* Decode <mode> argument */
    if(!strcasecmp(pcParameter1, "STATIC")) {
        LedSetMode(HW_LED_STATIC);
    } else if(!strcasecmp(pcParameter1, "BLINK_SLOW")) {
        LedSetMode(HW_LED_BLINK_SLOW);
    } else if(!strcasecmp(pcParameter1, "BLINK_FAST")) {
        LedSetMode(HW_LED_BLINK_FAST);
    } else {
        snprintf( pcWriteBuffer, xWriteBufferLen, "Error: could not set LED mode to %s"SHELL_EOL, pcParameter1);
        return pdFALSE;
    }

    /* Decode <color> argument */
    if(!strcasecmp(pcParameter2, "OFF")) {
        LedSetColor(HW_LED_OFF);
    } else if(!strcasecmp(pcParameter2, "WHITE")) {
        LedSetColor(HW_LED_WHITE);
    } else if(!strcasecmp(pcParameter2, "RED")) {
        LedSetColor(HW_LED_RED);
    } else if(!strcasecmp(pcParameter2, "GREEN")) {
        LedSetColor(HW_LED_GREEN);
    } else if(!strcasecmp(pcParameter2, "BLUE")) {
        LedSetColor(HW_LED_BLUE);
    } else if(!strcasecmp(pcParameter2, "CYAN")) {
        LedSetColor(HW_LED_CYAN);
    } else if(!strcasecmp(pcParameter2, "MAGENTA")) {
        LedSetColor(HW_LED_MAGENTA);
    } else if(!strcasecmp(pcParameter2, "YELLOW")) {
        LedSetColor(HW_LED_YELLOW);
    } else {
        snprintf( pcWriteBuffer, xWriteBufferLen, "Error: could not set LED color to %s"SHELL_EOL, pcParameter2);
        return pdFALSE;
    }

    snprintf( pcWriteBuffer, xWriteBufferLen, "Defined LED to %s %s"SHELL_EOL, pcParameter1, pcParameter2);
    return pdFALSE;
}

static BaseType_t OS_SHL_AvsCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    return pdFALSE;
}

static BaseType_t OS_SHL_AvdCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    return pdFALSE;
}

static BaseType_t OS_SHL_StrCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    return pdFALSE;
}






