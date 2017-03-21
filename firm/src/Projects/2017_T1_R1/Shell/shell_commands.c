/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       shell_commands.c
 * @author     Bebop35
 * @date       Dec 04, 2016
 * -----------------------------------------------------------------------------
 * @brief
 *   Implements CLI commands.
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

/* Inclusion */
#include "main.h"

/* -----------------------------------------------------------------------------
 * Prototypes of shell commands (local and private functions)
 * -----------------------------------------------------------------------------
 */

/* System commands */
static BaseType_t OS_SHL_SysCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

/* Parameters and variables management */
static BaseType_t OS_SHL_VarCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t OS_SHL_SetCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t OS_SHL_GetCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t OS_SHL_PrbCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
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

// Variable list command
static const CLI_Command_Definition_t xVar =
{
    "var",
    SHELL_EOL
    "var: Display the list of available variables)."SHELL_EOL
    ,OS_SHL_VarCmd,
    0
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
    "get [name] : Retrieve variable value (from RAM)."SHELL_EOL
    " - [name]  : Name of the variable to get"SHELL_EOL
    ,OS_SHL_GetCmd,
    1 // Variable
};

// Variable probe command
static const CLI_Command_Definition_t xPrb =
{
    "prb",
    SHELL_EOL
    "prb [id1]... [idN] : Retrieve N values (from RAM)."SHELL_EOL
    " - [id]  : Id of the variable to get"SHELL_EOL
    ,OS_SHL_PrbCmd,
    -1 // Variable
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
    "  - [set_id] [new id]"SHELL_EOL
    "  - [set_br] [id] [servo baudrate]"SHELL_EOL
    "  - [ping] [id]"SHELL_EOL
    "  - [set_pos] [id] [position]"SHELL_EOL
    "  - [set_torque] [id] [torque]"SHELL_EOL
    "  - [set_led] [id] [led value]"SHELL_EOL
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


void shell_register_commands(void)
{
    FreeRTOS_CLIRegisterCommand( &xSys );
    FreeRTOS_CLIRegisterCommand( &xVar );
    FreeRTOS_CLIRegisterCommand( &xSet );
    FreeRTOS_CLIRegisterCommand( &xGet );
    FreeRTOS_CLIRegisterCommand( &xPrb );
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
                        SHELL_SYS_PFX"   State   Prio.   Stack   #"SHELL_EOL
                        SHELL_SYS_PFX"------------------------------------------"SHELL_EOL;
    BaseType_t xSpacePadding;


    char* pcParameter1;
    BaseType_t xParameter1StringLength;

    /* Get parameters */
    pcParameter1 = (char*) FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameter1StringLength);

    /* Terminate both strings */
    pcParameter1[ xParameter1StringLength ] = 0x00;

    /* 'RESET' Sub-command: reset the microcontroller */
    if(!strcasecmp(pcParameter1, "reset")) {
        snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_SYS_PFX"Resetting..."SHELL_EOL);
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
        // TODO add SHELL_SYS_PFX for each entry
        strcpy( pcWriteBuffer, pcHeader_list );
        vTaskList( pcWriteBuffer + strlen( pcHeader_list ) );
        return pdFALSE;

    /* 'TASKS-STATS' Sub-command: display tasks statistics */
    } else if(!strcasecmp(pcParameter1, "tasks-stats")) {
        sys_get_run_time_stats( pcWriteBuffer );
        return pdFALSE;

    /* Error case */
    } else {
        snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"Unknown system command %s"SHELL_EOL, pcParameter1);
        return pdFALSE;
    }

}

static BaseType_t OS_SHL_VarCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    return shell_get_variables_list(pcWriteBuffer, xWriteBufferLen);

}

static BaseType_t OS_SHL_SetCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    char* pcParameter1;
    char* pcParameter2;
    BaseType_t xParameter1StringLength;
    BaseType_t xParameter2StringLength;
    OS_SHL_VarItemTypeDef* var;

    /* Get parameters */
    pcParameter1 = (char*) FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameter1StringLength);
    pcParameter2 = (char*) FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParameter2StringLength);

    /* Terminate both strings */
    pcParameter1[ xParameter1StringLength ] = 0x00;
    pcParameter2[ xParameter2StringLength ] = 0x00;

    /* Find the variable by its name */
        if(shell_find_variable_by_name(pcParameter1, &var) == pdTRUE)
        {
            if(shell_set_variable(var, pcParameter2) == pdTRUE)
            {
                snprintf(pcWriteBuffer, xWriteBufferLen, "");

            /* Error during set */
            } else {
                snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"Error while setting variable '%s' to %s"SHELL_EOL, pcParameter1, pcParameter2);
            }

        /* Not found */
        } else {
            snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"Could not find variable '%s'"SHELL_EOL, pcParameter1);
        }

        /* This is always a one-shot print */
        return pdFALSE;
}

static BaseType_t OS_SHL_GetCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    char* pcParameter1;
    BaseType_t xParameter1StringLength;

    OS_SHL_VarItemTypeDef* var;
    char valueStr[32];
    const size_t valueStrLen = sizeof(valueStr) / sizeof(char);

    /* Get parameters */
    pcParameter1 = (char*) FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameter1StringLength);

    /* Terminate string */
    pcParameter1[ xParameter1StringLength ] = 0x00;

    /* Find the variable by its name */
    if(shell_find_variable_by_name(pcParameter1, &var) == pdTRUE)
    {
        if(shell_get_variable(var, valueStr, valueStrLen) == pdTRUE)
        {
            snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_GET_PFX"%s=%s"SHELL_EOL, var->name, valueStr);
        } else {
            snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"Error while reading variable '%s'"SHELL_EOL, pcParameter1);
        }

    /* Not found */
    } else {
        snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"Could not find variable '%s'"SHELL_EOL, pcParameter1);
    }

    /* This is always a one-shot print */
    return pdFALSE;
}

static BaseType_t OS_SHL_PrbCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{

    const char *pcParameter;
    BaseType_t lParameterStringLength;
    UBaseType_t lParameterNumber;

    uint32_t id;
    OS_SHL_VarItemTypeDef* var;
    char valueStr[32];
    const size_t valueStrLen = sizeof(valueStr) / sizeof(char);

    snprintf(pcWriteBuffer, xWriteBufferLen, SHELL_PRB_PFX);
    pcWriteBuffer += strlen(pcWriteBuffer);

    // Get parameters one by one
    lParameterNumber = 1;
    while((pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, lParameterNumber, &lParameterStringLength )) != NULL)
    {
       // Convert ID string to int
       id = strtoul(pcParameter, NULL, 10);

       if(shell_find_variable_by_id(id, &var) == pdTRUE)
       {
           if(shell_get_variable(var, valueStr, valueStrLen) == pdTRUE)
           {
               snprintf(pcWriteBuffer, xWriteBufferLen, "%s ", valueStr);
           } else {
               snprintf(pcWriteBuffer, xWriteBufferLen, "--");
           }

       // Not found
       } else {
           snprintf(pcWriteBuffer, xWriteBufferLen, "--");
       }

       pcWriteBuffer += strlen(pcWriteBuffer);
       lParameterNumber++;
    }

    // Always one-shot
    return pdFALSE;

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
    BB_PWR_TypeDef powerSupply;

    /* Get parameters */
    pcParameter1 = (char*) FreeRTOS_CLIGetParameter(pcCommandString, 1, &xParameter1StringLength);
    pcParameter2 = (char*) FreeRTOS_CLIGetParameter(pcCommandString, 2, &xParameter2StringLength);

    /* Terminate both strings */
    pcParameter1[ xParameter1StringLength ] = 0x00;
    pcParameter2[ xParameter2StringLength ] = 0x00;

    /* Decode <no> argument */
    if(!strcasecmp(pcParameter1, "1")) {
        powerSupply = BB_PWR_VP1;
    } else if(!strcasecmp(pcParameter1, "2")) {
        powerSupply = BB_PWR_VP2;
    } else if(!strcasecmp(pcParameter1, "3")) {
        powerSupply = BB_PWR_VP3;
    } else if(!strcasecmp(pcParameter1, "ALL")) {
        allSupplies = 1;
    } else {
        snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"Could not define power-supply no to %s"SHELL_EOL, pcParameter1);
        return pdFALSE;
    }

    /* Decode <state> argument */
    if(!strcasecmp(pcParameter2, "OFF")) {
        if(!allSupplies) {
            bb_pwr_disable(powerSupply);
        } else {
            bb_pwr_disable(BB_PWR_VP1);
            bb_pwr_disable(BB_PWR_VP2);
            bb_pwr_disable(BB_PWR_VP3);
        }
    } else if(!strcasecmp(pcParameter2, "ON")) {
        if(!allSupplies) {
            bb_pwr_enable(powerSupply);
        } else {
            bb_pwr_enable(BB_PWR_VP1);
            bb_pwr_enable(BB_PWR_VP2);
            bb_pwr_enable(BB_PWR_VP3);
        }
    } else {
        snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"Could not define power-supply state to %s"SHELL_EOL, pcParameter2);
        return pdFALSE;
    }

    snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_POW_PFX"Defined power-supply %s to %s"SHELL_EOL, pcParameter1, pcParameter2);
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
    char *pcCommand;
    char *pcParameter;
    BaseType_t lCommandStringLength;
    BaseType_t lParameterStringLength;
    UBaseType_t lParameterNumber;

    //snprintf(pcWriteBuffer, xWriteBufferLen, SHELL_PRB_PFX);
    //pcWriteBuffer += strlen(pcWriteBuffer);

    uint8_t servoId;
    uint8_t servoBaudrate;
    uint16_t servoPosition;
    uint16_t servoTorque;
    uint8_t servoLed;


    uint8_t address;
    uint8_t data;

    lParameterNumber = 1;

    // Get [command] parameter
    if((pcCommand = (char*) FreeRTOS_CLIGetParameter(pcCommandString, lParameterNumber, &lCommandStringLength )) != NULL)
    {
        lParameterNumber++;

        //second parameter is always the [id]
        if((pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, lParameterNumber, &lParameterStringLength )) != NULL) {
            servoId = strtol(pcParameter, NULL, 10);
        } else {
            snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"No id defined"SHELL_EOL);
            return pdFALSE;
        }

        lParameterNumber++;

        if(!strncasecmp(pcCommand, "update", strlen( "update"))) {
            // [init]
            dsv_update_config();
            snprintf( pcWriteBuffer, xWriteBufferLen, "end init"SHELL_EOL);
            pcWriteBuffer += strlen(pcWriteBuffer);

        } else if(!strncasecmp(pcCommand, "set_id", strlen( "set_id"))) {
            // [set_id] [new id]

            snprintf( pcWriteBuffer, xWriteBufferLen, "start set_id"SHELL_EOL);
            pcWriteBuffer += strlen(pcWriteBuffer);

            vTaskDelay(pdMS_TO_TICKS(2000));
            xl_320_set_led(254, 1);

            xl_320_set_id(servoId);

            vTaskDelay(pdMS_TO_TICKS(2000));
            xl_320_set_led(servoId, 2);

        } else if(!strncasecmp(pcCommand, "set_br", strlen( "set_br"))) {
            //[set_br] [id] [servo baudrate]

            if((pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, lParameterNumber, &lParameterStringLength )) != NULL) {
                servoBaudrate = strtol(pcParameter, NULL, 10);
                xl_320_set_baudrate(servoId, servoBaudrate);

                lParameterNumber++;
            } else {
                snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"No baudrate defined"SHELL_EOL);
                pcWriteBuffer += strlen(pcWriteBuffer);
                return pdFALSE;
            }
        } else if(!strncasecmp(pcCommand, "ping", strlen( "ping"))) {
            // [ping] [id]

            /*snprintf( pcWriteBuffer, xWriteBufferLen, "Pinging Servo %u"SHELL_EOL, servoId);
            pcWriteBuffer += strlen(pcWriteBuffer);*/
            dsv_ping(servoId);
            return pdFALSE;

            /*while(1) {
                dsv_ping(servoId);
                vTaskDelay(pdMS_TO_TICKS(2));
            }*/

            // not implemented yet

            // Temp; TODO: add interface no
        } else if(!strncasecmp(pcCommand, "read", strlen( "read"))) {
        // [read] [id] [address]

            if((pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, lParameterNumber, &lParameterStringLength )) != NULL) {
                address = strtol(pcParameter, NULL, 10);

                data = dsv_read(2, servoId, address);

                snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_DSV_PFX"Read data: %u"SHELL_EOL, data);
                return pdFALSE;

                lParameterNumber++;
            } else {
                snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"No address defined"SHELL_EOL);
                return pdFALSE;
            }


        } else if(!strncasecmp(pcCommand, "set_pos", strlen( "set_pos"))) {
            // [set_pos] [id] [position]

            if((pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, lParameterNumber, &lParameterStringLength )) != NULL) {
                servoPosition = strtol(pcParameter, NULL, 10);
                //xl_320_set_position(servoId, servoPosition);

                //bb_dsv_switch(2, DXL_MODE_TX);
                //dsv_test_led(servoId, servoPosition);

                dsv_test_pos(servoId, servoPosition);
                //dsv_reset();

                //dsv_scan_servos();

                lParameterNumber++;
            } else {
                snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"No position defined"SHELL_EOL);
                return pdFALSE;
            }

        } else if(!strncasecmp(pcCommand, "set_torque", strlen( "set_torque"))) {
            // [set_torque] [id] [torque]

            if((pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, lParameterNumber, &lParameterStringLength )) != NULL) {
                servoTorque = strtol(pcParameter, NULL, 10);
                xl_320_set_goal_torque(servoId, servoTorque);

                lParameterNumber++;
            } else {
                snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"No torque defined"SHELL_EOL);
                return pdFALSE;
            }
        } else if(!strncasecmp(pcCommand, "set_led", strlen( "set_led"))) {
            // [set_led] [id] [led]

            if((pcParameter = FreeRTOS_CLIGetParameter(pcCommandString, lParameterNumber, &lParameterStringLength )) != NULL) {

                pcParameter[ lParameterStringLength ] = 0x00;

                servoLed = strtol(pcParameter, NULL, 10);
                xl_320_set_led(servoId, servoLed);

                lParameterNumber++;
            } else {
                snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"No led value defined"SHELL_EOL);
                return pdFALSE;
            }
        } else {
            snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"Unknown system command %s"SHELL_EOL, pcCommand);
            return pdFALSE;
        }
    }
    else
    {
        //error no param
        snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"This command requires parameters"SHELL_EOL);
        return pdFALSE;
    }

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
        led_set_mode(BB_LED_STATIC);
    } else if(!strcasecmp(pcParameter1, "BLINK_SLOW")) {
        led_set_mode(BB_LED_BLINK_SLOW);
    } else if(!strcasecmp(pcParameter1, "BLINK_FAST")) {
        led_set_mode(BB_LED_BLINK_FAST);
    } else {
        snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"Could not set LED mode to %s"SHELL_EOL, pcParameter1);
        return pdFALSE;
    }

    /* Decode <color> argument */
    if(!strcasecmp(pcParameter2, "OFF")) {
        led_set_color(BB_LED_OFF);
    } else if(!strcasecmp(pcParameter2, "WHITE")) {
        led_set_color(BB_LED_WHITE);
    } else if(!strcasecmp(pcParameter2, "RED")) {
        led_set_color(BB_LED_RED);
    } else if(!strcasecmp(pcParameter2, "GREEN")) {
        led_set_color(BB_LED_GREEN);
    } else if(!strcasecmp(pcParameter2, "BLUE")) {
        led_set_color(BB_LED_BLUE);
    } else if(!strcasecmp(pcParameter2, "CYAN")) {
        led_set_color(BB_LED_CYAN);
    } else if(!strcasecmp(pcParameter2, "MAGENTA")) {
        led_set_color(BB_LED_MAGENTA);
    } else if(!strcasecmp(pcParameter2, "YELLOW")) {
        led_set_color(BB_LED_YELLOW);
    } else {
        snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"Could not set LED color to %s"SHELL_EOL, pcParameter2);
        return pdFALSE;
    }

    snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_LED_PFX"Defined LED to %s %s"SHELL_EOL, pcParameter1, pcParameter2);
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






