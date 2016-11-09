/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       task_ASV.c
 * @author     Pierrick
 * @date       Apr 26, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   This module handles the annalog servos
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

/* Definition */
#define MAX_ASV_IN_QUEUE	5
#define MIN_INDEX 2000
#define MAX_INDEX 3000
#define DOWN 2000
#define UP 2950

/* Local structures */
typedef struct {
	HW_ASV_ChannelTypeDef channel;
	uint16_t current_Position;
	uint16_t min_Position;
	uint16_t max_Position;
}ASV_ControlTypeDef;

/* Local Variable Mutex */
static xQueueHandle xASVMsgQueue;
static ASV_ControlTypeDef leftArm, rightArm, centralIndex, parasol;

/* Local, Private functions */
static void OS_ASVTask(void *pvParameters);
void ASV_Create(ASV_ControlTypeDef* ASV, HW_ASV_ChannelTypeDef channel, uint16_t min_Position, uint16_t max_Position);


void OS_CreateASVTask(void)
{
	xASVMsgQueue = xQueueCreate( MAX_ASV_IN_QUEUE, sizeof(ASV_ControlTypeDef));
    if(xASVMsgQueue==NULL)
    {
    	printf("insufficient heap RAM available for ASVMsgQueue\r\n");
    	while(1);
    }
    ASV_Create(&parasol, HW_ASV_CHANNEL1, 1400, 2300);
    ASV_Create(&centralIndex, HW_ASV_CHANNEL2, MIN_INDEX, MAX_INDEX);
    ASV_Create(&leftArm, HW_ASV_CHANNEL4, 2000, 4000);
    ASV_Create(&rightArm, HW_ASV_CHANNEL2, 2000, 4000);



	xTaskCreate(OS_ASVTask, "Analog servo", 350, NULL, OS_TASK_PRIORITY_ASV, NULL );
}

static void OS_ASVTask( void *pvParameters )
{
	ASV_ControlTypeDef ASV_To_Manage;

    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    for( ;; )
    {
    	/* Block on the queue to wait for data to arrive */
    	xQueueReceive(xASVMsgQueue, &ASV_To_Manage, portMAX_DELAY);
    	if (ASV_To_Manage.current_Position > ASV_To_Manage.max_Position)
    		ASV_To_Manage.current_Position = ASV_To_Manage.max_Position;
    	if (ASV_To_Manage.current_Position < ASV_To_Manage.min_Position)
    		ASV_To_Manage.current_Position = ASV_To_Manage.min_Position;

    	HW_ASV_SetPwmPulseLength(ASV_To_Manage.channel,ASV_To_Manage.current_Position);
    }
}

void ASV_Create(ASV_ControlTypeDef* ASV, HW_ASV_ChannelTypeDef channel, uint16_t min_Position, uint16_t max_Position)
{
	ASV->channel = channel;
	ASV->max_Position = max_Position;
	ASV->min_Position = min_Position;
}

void ASV_DeployLeftArm(void)
{
	GPIO_WriteBit((GPIO_TypeDef*) END6_GPIO_PORT, END6_PIN, Bit_RESET);
}

void ASV_DeployRightArm(void)
{
	GPIO_WriteBit((GPIO_TypeDef*) END7_GPIO_PORT, END7_PIN, Bit_RESET);
}

void ASV_IdleLeftArm(void)
{
	GPIO_WriteBit((GPIO_TypeDef*) END6_GPIO_PORT, END6_PIN, Bit_SET);
}

void ASV_IdleRightArm(void)
{
	GPIO_WriteBit((GPIO_TypeDef*) END7_GPIO_PORT, END7_PIN, Bit_SET);
}

void ASV_MoveIndex(uint16_t position)
{
	centralIndex.current_Position = position;
	xQueueSend( xASVMsgQueue, &centralIndex, (TickType_t)0 );
}

void ASV_DeployParasol(uint16_t position)
{
	parasol.current_Position = position;
	xQueueSend( xASVMsgQueue, &parasol, (TickType_t)0 );
}
