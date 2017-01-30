/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       task_DSV.c
 * @author     Pierrick
 * @date       Apr 27, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   This module handles the digital servos
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev:$
 * $LastChangedBy:$
 * $LastChangedDate:$
 * -----------------------------------------------------------------------------
 */

/* Inclusion */
#include "main.h"

/* Definition */
#define MAX_DSV_IN_QUEUE	5

/* Local structures */
typedef struct {
	uint8_t id;
	uint16_t current_Position;
	uint16_t min_Position;
	uint16_t max_Position;
}DSV_ControlTypeDef;

/* Local Variable Mutex */
static xQueueHandle xDSVMsgQueue;
static DSV_ControlTypeDef servo1, servo2; // TODO : define a comprehensive name

/* Local, Private functions */
static void OS_DSVTask(void *pvParameters);
void DSV_Create(DSV_ControlTypeDef* DSV, uint8_t id, uint16_t min_Position, uint16_t max_Position);


void OS_CreateDSVTask(void)
{
	xDSVMsgQueue = xQueueCreate( MAX_DSV_IN_QUEUE, sizeof(DSV_ControlTypeDef));
    if(xDSVMsgQueue==NULL)
    {
    	printf("insufficient heap RAM available for DSVMsgQueue\r\n");
    	while(1);
    }
	xTaskCreate(OS_DSVTask, "DIGITAL SERVO", 350, NULL, OS_TASK_PRIORITY_DSV, NULL );

    DSV_Create(&servo1, 23, 0, 1023);
    DSV_Create(&servo2, 42, 0, 1023);
}

static void OS_DSVTask( void *pvParameters )
{
	DSV_ControlTypeDef DSV_To_Manage;

    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    for( ;; )
    {
    	/* Block on the queue to wait for data to arrive */
    	xQueueReceive(xDSVMsgQueue, &DSV_To_Manage, portMAX_DELAY);
    	if (DSV_To_Manage.current_Position > DSV_To_Manage.max_Position)
    		DSV_To_Manage.current_Position = DSV_To_Manage.max_Position;
    	if (DSV_To_Manage.current_Position < DSV_To_Manage.min_Position)
    		DSV_To_Manage.current_Position = DSV_To_Manage.min_Position;

    	xl_320_set_position(DSV_To_Manage.id, DSV_To_Manage.current_Position);
    	vTaskDelay(5/portTICK_RATE_MS);
    	xl_320_set_led(DSV_To_Manage.id, XL_320_LED_BLUE);
    }
}

void DSV_Create(DSV_ControlTypeDef* DSV, uint8_t id, uint16_t min_Position, uint16_t max_Position)
{
	DSV->id = id;
	DSV->max_Position = max_Position;
	DSV->min_Position = min_Position;
	xl_320_set_control_mode(DSV->id, XL_320_JOIN_MODE);
}

void DSV_SetServo1(uint16_t position)
{
	servo1.current_Position = position;
	xQueueSend( xDSVMsgQueue, &servo1, (TickType_t)0 );
}

void DSV_SetServo2(uint16_t position)
{
	servo2.current_Position = position;
	xQueueSend( xDSVMsgQueue, &servo2, (TickType_t)0 );
}
