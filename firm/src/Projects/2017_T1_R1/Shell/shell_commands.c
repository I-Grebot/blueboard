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
static BaseType_t OS_SHL_SubCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ); // Sub systems

/* High-level modules management */
static BaseType_t OS_SHL_LedCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ); // Led
static BaseType_t OS_SHL_AvsCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ); // Aversive
static BaseType_t OS_SHL_AvdCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ); // Avoidance
static BaseType_t OS_SHL_SeqCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString ); // Sequencer


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
    "  - [ping] [if] [id]"SHELL_EOL
    "  - [reset] [if] [id]"SHELL_EOL
    "  - [write] [if] [id] [add] [size] [data]"SHELL_EOL
    "  - [read] [if] [id] [add] [size]"SHELL_EOL
    "  - [dump] [if] [id]"SHELL_EOL
    "  - [scan]"SHELL_EOL
    /*" List of available commands:"SHELL_EOL
    "  - [set_id] [new id]"SHELL_EOL
    "  - [set_br] [id] [servo baudrate]"SHELL_EOL
    "  - [ping] [id]"SHELL_EOL
    "  - [set_pos] [id] [position]"SHELL_EOL
    "  - [set_torque] [id] [torque]"SHELL_EOL
    "  - [set_led] [id] [led value]"SHELL_EOL*/
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

// Sequencer commands
static const CLI_Command_Definition_t xSeq =
{
    "seq",
    SHELL_EOL
    "seq [command] [value1]... [valueN]: Run a Sequencer command."SHELL_EOL
    " List of available commands:"SHELL_EOL
    "  - [cmd1] [value1] [value2]"SHELL_EOL
    ,OS_SHL_SeqCmd,
    -1 // Variable
};

// Sub-system commands
static const CLI_Command_Definition_t xSub =
{
    "sub",
    SHELL_EOL
    "sub [system] [command] [value1]... [valueN]: Run a sub-system command."SHELL_EOL
    ,OS_SHL_SubCmd,
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
    FreeRTOS_CLIRegisterCommand( &xSeq );
    FreeRTOS_CLIRegisterCommand( &xSub );
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
	snprintf( pcWriteBuffer, xWriteBufferLen, "Test: %f\n\r", 1.42);

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

// Motion
// [goto] [x] [y]

static BaseType_t OS_SHL_MotCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{

    char* pcParameter;
    BaseType_t lParameterStringLength;
    BaseType_t xReturn;
    static BaseType_t lParameterNumber = 0;


    static char* command;
    static BaseType_t command_str_length;

    static wp_t wp;
    //static int32_t pos_x;
    //static int32_t pos_y;

    // Nothing to display by default
    memset( pcWriteBuffer, 0x00, xWriteBufferLen );

    // Command start
    if(lParameterNumber == 0)
    {
        lParameterNumber = 1;
        xReturn = pdPASS;

    } else {

        // Get the parameter as a string
        pcParameter = (char*) FreeRTOS_CLIGetParameter(pcCommandString, lParameterNumber, &lParameterStringLength );

        // Parameter is fetched
        if(pcParameter != NULL) {

            switch(lParameterNumber) {
                case 1:
                    command = pcParameter;
                    command_str_length = lParameterStringLength;
                    break;

                case 2: wp.coord.abs.x = strtol(pcParameter, NULL, 10); break;
                case 3: wp.coord.abs.y = strtol(pcParameter, NULL, 10); break;
                default: break;
            }

            // Static items of the waypoints
            wp.coord.abs.a = 0;
            wp.offset.x = 0;
            wp.offset.y = 0;
            wp.offset.a = 0;
            //wp.speed = WP_SPEED_NORMAL;
            wp.speed = WP_SPEED_FAST;
            wp.trajectory_must_finish = true;

            // Ensure we keep going for the next parameter
            xReturn = pdTRUE;
            lParameterNumber++;

        // End of decoding, launch the command
        } else {

            // Terminate the command string. Can be done only after all parameters are fetched
            command[command_str_length] = 0;

            // Decode the command
            // ------------------

            // Goto Cases
            if((!strcasecmp(command, "goto_auto")) && (lParameterNumber == 4)) {
                snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_MOT_PFX"Goto Auto (%d;%d)"SHELL_EOL, wp.coord.abs.x, wp.coord.abs.y);
                pcWriteBuffer += strlen(pcWriteBuffer);
                wp.type = WP_GOTO_AUTO;
                motion_add_new_wp(&wp);

            }

            else if((!strcasecmp(command, "goto_fwd")) && (lParameterNumber == 4)) {
              snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_MOT_PFX"Goto Forward (%d;%d)"SHELL_EOL, wp.coord.abs.x, wp.coord.abs.y);
              pcWriteBuffer += strlen(pcWriteBuffer);
              wp.type = WP_GOTO_FWD;
              motion_add_new_wp(&wp);
            }

            else if((!strcasecmp(command, "goto_bwd")) && (lParameterNumber == 4)) {
              snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_MOT_PFX"Goto Backward (%d;%d)"SHELL_EOL, wp.coord.abs.x, wp.coord.abs.y);
              pcWriteBuffer += strlen(pcWriteBuffer);
              wp.type = WP_GOTO_BWD;
              motion_add_new_wp(&wp);
            }

            // Turnto Cases
            else if((!strcasecmp(command, "turnto_front")) && (lParameterNumber == 4)) {
              snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_MOT_PFX"Turnto Front (%d;%d)"SHELL_EOL, wp.coord.abs.x, wp.coord.abs.y);
              pcWriteBuffer += strlen(pcWriteBuffer);
              wp.type = WP_ORIENT_FRONT;
              motion_add_new_wp(&wp);
            }

            else if((!strcasecmp(command, "turnto_behind")) && (lParameterNumber == 4)) {
              snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_MOT_PFX"Turnto Behind (%d;%d)"SHELL_EOL, wp.coord.abs.x, wp.coord.abs.y);
              pcWriteBuffer += strlen(pcWriteBuffer);
              wp.type = WP_ORIENT_BEHIND;
              motion_add_new_wp(&wp);
            }

            // Stop/Break
            else if((!strcasecmp(command, "stop")) && (lParameterNumber == 2)) {
              snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_MOT_PFX"Stop"SHELL_EOL);
              pcWriteBuffer += strlen(pcWriteBuffer);
              motion_clear_all_wp();
              motion_traj_hard_stop();
            }


            else {
                snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"Unrecognized command '%s' or parameters error"SHELL_EOL, command);
            }

            // Ensure the function can start again
            lParameterNumber = 0;
            xReturn = pdFALSE;
        }

    }

    return xReturn;
}

static BaseType_t OS_SHL_DsvCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    char* pcParameter;
    BaseType_t lParameterStringLength;
    BaseType_t xReturn;
    static BaseType_t lParameterNumber = 0;
    dxl_status_t status;
    char error_str[128];

    static char* command;
    static BaseType_t command_str_length;
    static uint8_t itf;
    static uint8_t id;
    static uint16_t add;
    static uint8_t size;
    static uint32_t data;
    static bool dump_started;
    static dxl_servo_t servo;

    static uint16_t servo_model_id;

    uint8_t data_arr[4];

    // Interface Channels, defined outside of this module
    extern dsv_channel_t dsv_chan1;
    extern dsv_channel_t dsv_chan2;

    // Nothing to display by default
    memset( pcWriteBuffer, 0x00, xWriteBufferLen );

    // Command start
    if(lParameterNumber == 0)
    {
        lParameterNumber = 1;
        xReturn = pdPASS;

    } else {

        // Get the parameter as a string
        pcParameter = (char*) FreeRTOS_CLIGetParameter(pcCommandString, lParameterNumber, &lParameterStringLength );

        // Parameter is fetched
        if(pcParameter != NULL) {

            switch(lParameterNumber) {
                case 1:
                    command = pcParameter;
                    command_str_length = lParameterStringLength;
                    break;

                case 2: itf = strtol(pcParameter, NULL, 10); break;
                case 3: id = strtol(pcParameter, NULL, 10); break;
                case 4: add = strtol(pcParameter, NULL, 10); break;
                case 5: size = strtol(pcParameter, NULL, 10); break;
                case 6: data = strtol(pcParameter, NULL, 10); break;
                default: break;
            }

            // Ensure we keep going for the next parameter
            xReturn = pdTRUE;
            lParameterNumber++;

            // Default states before command execution
            dump_started = false;

        // End of decoding, launch the command
        } else {

            // Terminate the command string. Can be done only after all parameters are fetched
            command[command_str_length] = 0;

            // Set the servo interface with the correct channel
            switch(itf) {
                case 1: servo.itf = &(dsv_chan1.dxl); break;
                case 2: servo.itf = &(dsv_chan2.dxl); break;
            }

            // Set the servo ID
            servo.id = id;

            // Decode the command
            if((!strcasecmp(command, "ping")) && (lParameterNumber == 4)) {
                snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_DSV_PFX"Pinging on interface #%u ID %u..."SHELL_EOL, itf, id);
                pcWriteBuffer += strlen(pcWriteBuffer);

                status = dxl_ping(&servo);

                if(status != DXL_PASS) {
                    dxl_get_error_str(error_str, sizeof(error_str), status, servo.itf->protocol);
                    snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_DSV_PFX"Error(s): %s"SHELL_EOL, error_str);
                } else {
                    snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_DSV_PFX"Found!"SHELL_EOL);
                }
            } // ping


            else if((!strcasecmp(command, "reset")) && (lParameterNumber == 4)) {
                snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_DSV_PFX"Factory Reset on interface #%u ID %u..."SHELL_EOL, itf, id);
                pcWriteBuffer += strlen(pcWriteBuffer);

                status = dxl_reset(&servo);

                if(status != DXL_PASS) {
                    dxl_get_error_str(error_str, sizeof(error_str), status, servo.itf->protocol);
                    snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_DSV_PFX"Error(s): %s"SHELL_EOL, error_str);
                } else {
                    snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_DSV_PFX"Done"SHELL_EOL);
                }
            } // reset

            else if((!strcasecmp(command, "read")) && (lParameterNumber == 6)) {
                status = dxl_read(&servo, add, data_arr, size);

                if(status != DXL_PASS) {
                    dxl_get_error_str(error_str, sizeof(error_str), status, servo.itf->protocol);
                    snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_DSV_PFX"Error(s): %s"SHELL_EOL, error_str);
                } else {
                    dxl_bytes_array_to_data(&data, size, data_arr);
                    snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_DSV_PFX"Reading on interface #%u ID %u: @0x%02X = 0x%X (%u Bytes)"SHELL_EOL, itf, id, add, data, size);
                }
            } // read

            else if((!strcasecmp(command, "write")) && (lParameterNumber == 7)) {
                snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_DSV_PFX"Writing on interface #%u ID %u: @0x%02X = 0x%X (%u Bytes)"SHELL_EOL, itf, id, add, data, size);
                pcWriteBuffer += strlen(pcWriteBuffer);
                dxl_data_to_bytes_array(data, size, data_arr);

                status = dxl_write(&servo, add, data_arr, size, false);

                if(status != DXL_PASS) {
                    dxl_get_error_str(error_str, sizeof(error_str), status, servo.itf->protocol);
                    snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_DSV_PFX"Error(s): %s"SHELL_EOL, error_str);
                }
            } // write

            else if((!strcasecmp(command, "scan")) && (lParameterNumber == 3)) {
                snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_DSV_PFX"Scanning interface #%u ..."SHELL_EOL, itf);
                pcWriteBuffer += strlen(pcWriteBuffer);

				// Scan all IDs. Broadcast ID automatically skipped
				for(servo.id = 0; servo.id < 254; servo.id++) {

					// Check to see if a servo at ID %x is found
					if(dxl_ping(&servo) == DXL_PASS) {

						// This is also used to retrieve potential errors like overheating, input voltage etc.
						status = dxl_get_model(&servo, &servo_model_id);
						servo.model = (dxl_servo_model_t*) dxl_find_servo_model_by_id(servo_model_id);

						if(servo.model != NULL) {
							snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_DSV_PFX"[SCAN] %u %-3u %-20s %-3u"SHELL_EOL, itf, servo.id, servo.model->name, status);
							pcWriteBuffer += strlen(pcWriteBuffer);
						}

					}

				}

      } // scan

      // Dump
      else if((!strcasecmp(command, "dump")) && (lParameterNumber == 4))
      {

        // Fetch model number before starting the dump
        if(dump_started == false)
        {
          // Get model ID
          status = dxl_get_model(&servo, &servo_model_id);

          if(status != DXL_PASS) {
              dxl_get_error_str(error_str, sizeof(error_str), status, servo.itf->protocol);
              snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_DSV_PFX"Error(s): %s"SHELL_EOL, error_str);
              lParameterNumber = 0;
              return pdFALSE;
          }

          //snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_DSV_PFX"Read model %u"SHELL_EOL, servo_model_id);
          //return pdFALSE;

          // Fetch model
          //servo_model_id = 28;
          servo.model = (dxl_servo_model_t*) dxl_find_servo_model_by_id(servo_model_id);
          //servo.model = &dxl_servo_models[6]; // RX28, just for tests
          //servo.model = &dxl_servo_models[15]; // XL320, just for tests

          if(servo.model == NULL) {
            snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_DSV_PFX"Could not fetch servo model %u"SHELL_EOL, servo_model_id);
            lParameterNumber = 0;
            return pdFALSE;
          }

          dump_started = true;
          return pdTRUE;

        // Actual dump
        } else {
          xReturn = dsv_dump_servo(&servo, pcWriteBuffer, xWriteBufferLen);

          // Cleanup when finished
          if(xReturn == pdFALSE) {
            lParameterNumber = 0;
          }

          return xReturn;
        }

        // Dump servo registers and print them out
        /*} else {
          xReturn = dsv_dump_servo(&servo, pcWriteBuffer, xWriteBufferLen);
          if(xReturn == pdFALSE) {
            lParameterNumber = 0;
          }
        //}

        return xReturn;*/
      }

			// Testings
			else if(!strcasecmp(command, "test"))
			{
				snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_DSV_PFX"Test on interface #%u ..."SHELL_EOL, itf);

				//xl_320_set_led(254, XL_320_LED_CYAN);
				servo.itf = &(dsv_chan1.dxl);
				servo.id = 45;
				dxl_write_int(&servo, 25, XL_320_LED_CYAN, 1, false);

            }
            else
            {
                snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"Unrecognized command '%s' or parameters error"SHELL_EOL, command);
            }

            // Ensure the function can start again
            lParameterNumber = 0;
            xReturn = pdFALSE;
        }

    }

    return xReturn;

}


static BaseType_t OS_SHL_SubCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{

    char* pcParameter;
    BaseType_t lParameterStringLength;
    BaseType_t xReturn;
    static BaseType_t lParameterNumber = 0;


    static char* system;
    static BaseType_t system_str_length;
    static char* command;
    static BaseType_t command_str_length;

    static int value1;
    static int value2;

    // Nothing to display by default
    memset( pcWriteBuffer, 0x00, xWriteBufferLen );

    // Command start
    if(lParameterNumber == 0)
    {
        lParameterNumber = 1;
        xReturn = pdPASS;

    } else {

        // Get the parameter as a string
        pcParameter = (char*) FreeRTOS_CLIGetParameter(pcCommandString, lParameterNumber, &lParameterStringLength );

        // Parameter is fetched
        if(pcParameter != NULL) {

            switch(lParameterNumber) {
              case 1:
                system = pcParameter;
                system_str_length = lParameterStringLength;
                break;

              case 2:
                command = pcParameter;
                command_str_length = lParameterStringLength;
                break;

              case 3: value1 = strtol(pcParameter, NULL, 10); break;
              case 4: value2 = strtol(pcParameter, NULL, 10); break;
                default: break;
            }


            // Ensure we keep going for the next parameter
            xReturn = pdTRUE;
            lParameterNumber++;

        // End of decoding, launch the command
        } else {

            // Terminate the command string. Can be done only after all parameters are fetched
            system[system_str_length] = 0;
            command[command_str_length] = 0;

            // Decode the system
            // ------------------
            if(!strcasecmp(system, "mod")) {

              if((!strcasecmp(command, "init")) && (lParameterNumber == 3)) {
                  //snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_SUB_PFX"Initializing"SHELL_EOL);
                  //pcWriteBuffer += strlen(pcWriteBuffer);
                  sys_mod_do_init(NULL);
              }

              else if((!strcasecmp(command, "self_test")) && (lParameterNumber == 3)) {
                  //snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_SUB_PFX"Starting Self-test"SHELL_EOL);
                  //pcWriteBuffer += strlen(pcWriteBuffer);
                  sys_mod_do_self_test(NULL);
              }

              else if((!strcasecmp(command, "grab")) && (lParameterNumber == 4)) {
                  //snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_SUB_PFX"Grabbing at %d"SHELL_EOL, value1);
                  //pcWriteBuffer += strlen(pcWriteBuffer);
                  sys_mod_do_grab(NULL, (uint16_t) value1);
              }

              else if((!strcasecmp(command, "land")) && (lParameterNumber == 5)) {
                  //snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_SUB_PFX"Landing at %d, %d"SHELL_EOL, value1, value2);
                  //pcWriteBuffer += strlen(pcWriteBuffer);
                  sys_mod_do_land(NULL, (uint16_t) value1, (uint16_t) value2);
              }

              else if((!strcasecmp(command, "fold")) && (lParameterNumber == 3)) {
                  //snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_SUB_PFX"Folding"SHELL_EOL);
                  //pcWriteBuffer += strlen(pcWriteBuffer);
                  sys_mod_do_fold(NULL);
              }

              else {
                  snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"Unrecognized command '%s' or parameters error"SHELL_EOL, command);
              }
            }

            else {
              snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"Unrecognized sub system '%s' or parameters error"SHELL_EOL, system);
            }



            // Ensure the function can start again
            lParameterNumber = 0;
            xReturn = pdFALSE;
        }

    }

    return xReturn;
}

//"  - [ping] [if] [id]"SHELL_EOL
//"  - [reset] [if] [id]"SHELL_EOL
//"  - [write] [if] [id] [add] [size] [data]"SHELL_EOL
//"  - [read] [if] [id] [add] [size]"SHELL_EOL
//"  - [dump] [if] [id]
//"  - [scan]"SHELL_EOL

/*
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

            //snprintf( pcWriteBuffer, xWriteBufferLen, "Pinging Servo %u"SHELL_EOL, servoId);
           // pcWriteBuffer += strlen(pcWriteBuffer);
            dsv_ping(servoId);
            return pdFALSE;


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

}*/

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

// str [cmd] [val1] [val2]
static BaseType_t OS_SHL_SeqCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
  char* pcParameter;
  BaseType_t lParameterStringLength;
  BaseType_t xReturn;
  static BaseType_t lParameterNumber = 0;

  static char* command;
  static BaseType_t command_str_length;

  // Currently limited to 2 parameters per command
  static uint32_t value1;
  static uint32_t value2;


  // Handle on the strategy task
  extern TaskHandle_t handle_task_sequencer;
  extern match_t match;

  // Nothing to display by default
  memset( pcWriteBuffer, 0x00, xWriteBufferLen );

  // Command start
  if(lParameterNumber == 0)
  {
    lParameterNumber = 1;
    xReturn = pdPASS;

  } else {

    // Get the parameter as a string
    pcParameter = (char*) FreeRTOS_CLIGetParameter(pcCommandString, lParameterNumber, &lParameterStringLength );

    // Parameter is fetched
    if(pcParameter != NULL) {

      switch(lParameterNumber) {
      case 1:
        command = pcParameter;
        command_str_length = lParameterStringLength;
        break;

      case 2: value1 = strtol(pcParameter, NULL, 10); break;
      case 3: value2 = strtol(pcParameter, NULL, 10); break;
      default: break;
      }

      // Ensure we keep going for the next parameter
      xReturn = pdTRUE;
      lParameterNumber++;

      // End of decoding, launch the command
    } else {

      // Terminate the command string. Can be done only after all parameters are fetched
      command[command_str_length] = 0;

      // Decode the command
      // ------------------

      // Sequencer controls
      if((!strcasecmp(command, "init")) && (lParameterNumber == 2)) {
        xTaskNotify(handle_task_sequencer, OS_NOTIFY_INIT_START, eSetBits);
      }

      else if((!strcasecmp(command, "start")) && (lParameterNumber == 2)) {
        xTaskNotify(handle_task_sequencer, OS_NOTIFY_MATCH_START, eSetBits);
      }

      else if((!strcasecmp(command, "pause")) && (lParameterNumber == 2)) {
        xTaskNotify(handle_task_sequencer, OS_NOTIFY_MATCH_PAUSE, eSetBits);
      }

      else if((!strcasecmp(command, "resume")) && (lParameterNumber == 2)) {
        xTaskNotify(handle_task_sequencer, OS_NOTIFY_MATCH_RESUME, eSetBits);
      }

      else if((!strcasecmp(command, "abort")) && (lParameterNumber == 2)) {
        xTaskNotify(handle_task_sequencer, OS_NOTIFY_MATCH_ABORT, eSetBits);
      }

      // Match color
      else if((!strcasecmp(command, "color")) && (lParameterNumber == 3)) {
        match.color = (match_color_e) value1;
      }

      // Some reporting
      else if((!strcasecmp(command, "tasks")) && (lParameterNumber == 2)) {

        xReturn = task_print_list(pcWriteBuffer, xWriteBufferLen);

        // Cleanup when finished
        if(xReturn == pdFALSE) {
          lParameterNumber = 0;
        }
        return xReturn;

      }

      else {
        snprintf( pcWriteBuffer, xWriteBufferLen, SHELL_ERR_PFX"Unrecognized command '%s' or parameters error"SHELL_EOL, command);
      }

      // Ensure the function can start again
      lParameterNumber = 0;
      xReturn = pdFALSE;
    }

  }

  return xReturn;
}






