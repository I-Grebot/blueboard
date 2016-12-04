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
#include "string.h"

/* Prototypes of shell commands (local and private functions) */
static BaseType_t OS_SHL_ResetCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t OS_SHL_TaskListCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );
static BaseType_t OS_SHL_LedCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

/* -----------------------------------------------------------------------------
 * Commands definitions
 * -----------------------------------------------------------------------------
 */

/* reset command:
 * parameters: none
 * return:     none
 */
static const CLI_Command_Definition_t xReset =
{
	"reset",
	SHELL_EOL
	"reset: Reset the microcontroller"SHELL_EOL,
	OS_SHL_ResetCmd,
	0
};

/* task-stats command:
 * parameters: none
 * return:     table of current task and status
 */
static const CLI_Command_Definition_t xTaskList =
{
	"task-list",
	SHELL_EOL
	"task-list: Displays a table showing the state of each task"SHELL_EOL,
	OS_SHL_TaskListCmd,
	0
};

/* led command:
 * parameters:
 *  - mode:  select the LED mode
 *  - color: select the LED color
 * return: none
 */
static const CLI_Command_Definition_t xLeds =
{
	"led",
	""SHELL_EOL
	"led <mode> <color>: Set the LED state"SHELL_EOL
	"  <mode>  [{STATIC} | {BLINK_SLOW} | {BLINK_FAST}]"SHELL_EOL
	"  <color> [{OFF} {WHITE}"SHELL_EOL
	"           {RED} {GREEN} {BLUE}"SHELL_EOL
	"		    {CYAN} {MAGENTA} {YELLOW} ]"SHELL_EOL,
	OS_SHL_LedCmd,
	2
};

/*
 * TODO:
 * - power on / off
 * - bootload
 * - chgt config
 * - live modes (probe values)
 * - .... tartiflette
 */

/* -----------------------------------------------------------------------------
 * Commands Registration
 * -----------------------------------------------------------------------------
 */


void OS_SHL_RegisterCommands( void )
{
	FreeRTOS_CLIRegisterCommand( &xReset );
	FreeRTOS_CLIRegisterCommand( &xTaskList );
	FreeRTOS_CLIRegisterCommand( &xLeds );
}

/* -----------------------------------------------------------------------------
 * Commands functions
 * -----------------------------------------------------------------------------
 */

static BaseType_t OS_SHL_ResetCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	snprintf( pcWriteBuffer, xWriteBufferLen, "Resetting..."SHELL_EOL);
	NVIC_SystemReset(); /* Auto-kill */
	return pdFALSE;
}

static BaseType_t OS_SHL_TaskListCmd( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	const char *const pcHeader =
			"     State   Priority  Stack    #"SHELL_EOL
			"************************************************"SHELL_EOL;
	BaseType_t xSpacePadding;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

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
	strcpy( pcWriteBuffer, pcHeader );
	vTaskList( pcWriteBuffer + strlen( pcHeader ) );

	/* There is no more data to return after this single string, so return
	pdFALSE. */
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






