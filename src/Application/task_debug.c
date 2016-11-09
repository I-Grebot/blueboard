/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       task_debug.c
 * @author     Pierrick
 * @date       Mar 12, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   This module handles the uart debug communication
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev$
 * $LastChangedBy$
 * $LastChangedDate$
 * -----------------------------------------------------------------------------
 */

/* Inclusion */
#include "blueboard.h"
#include "string.h"

/* Definition */
#define MAX_MSG_LENGTH		50
#define MAX_MSG_IN_QUEUE	10

/* Local, Private functions */
static void OS_DebugTask(void *pvParameters);

/* Local, Private variables */
static xQueueHandle xDebugMsgQueue;

void OS_CreateDebugTask(void)
{
    xTaskCreate(OS_DebugTask, "DEBUG", 250, NULL, OS_TASK_PRIORITY_DEBUG, NULL );
    xDebugMsgQueue = xQueueCreate( MAX_MSG_IN_QUEUE, (unsigned portBASE_TYPE) (MAX_MSG_LENGTH *sizeof(char)));
    if(xDebugMsgQueue==NULL)
    {
    	printf("insufficient heap RAM available for DebugMsgQueue\r\n");
    	while(1);
    }
}

static void OS_DebugTask(void *pvParameters)
{
	char pcString[MAX_MSG_LENGTH];

    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    for( ;; )
    {
       	/* Block on the queue to wait for data to arrive */
    	xQueueReceive(xDebugMsgQueue, pcString, portMAX_DELAY);

    	/* Print out the string received */
    	HW_DBG_Puts(pcString);
    }
}

void OS_DebugTaskPrint( char ppcMessageToSend[] )
{
	if(strlen(ppcMessageToSend)<=MAX_MSG_LENGTH)
		xQueueSend( xDebugMsgQueue, ppcMessageToSend, (TickType_t)0 );
	else
		xQueueSend( xDebugMsgQueue, "Message too long!\r\n", (TickType_t)0 );
}
/*-----------------------------------------------------------*/
