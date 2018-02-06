/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       analog_servo.c
 * @author     Pierrick
 * @date       Apr 26, 2016
 * -----------------------------------------------------------------------------
 * @brief
 *   This module handles the analog servos
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

/* Inclusion */
#include "../../2018_T1_R1/include/main.h"

/* Definition */
#define MAX_ASV_IN_QUEUE	5
#define MIN_INDEX 2000
#define MAX_INDEX 3000
#define DOWN 2000
#define UP 2950

/* Local structures */

/* Local Variable Mutex */
static xQueueHandle xASVMsgQueue;
//static ASV_ControlTypeDef leftArm, rightArm, centralIndex, parasol;

/* Local, Private functions */
//static void OS_ASVTask(void *pvParameters);


/*BaseType_t asv_start(void)
{
	xASVMsgQueue = xQueueCreate( MAX_ASV_IN_QUEUE, sizeof(ASV_ControlTypeDef));
    if(xASVMsgQueue==NULL)
    {
    	serial_puts("Error: Insufficient heap RAM available for ASVMsgQueue"SHELL_EOL);
    	return pdFAIL;
    }
    ASV_Create(&parasol, BB_ASV_CHANNEL1, 1400, 2300);
    ASV_Create(&centralIndex, BB_ASV_CHANNEL2, MIN_INDEX, MAX_INDEX);
    ASV_Create(&leftArm, BB_ASV_CHANNEL4, 2000, 4000);
    ASV_Create(&rightArm, BB_ASV_CHANNEL2, 2000, 4000);

	return xTaskCreate(OS_ASVTask, "ANALOG SERVO", OS_TASK_STACK_ASV, NULL, OS_TASK_PRIORITY_ASV, NULL );
}*/

/*static void OS_ASVTask( void *pvParameters )
{
	ASV_ControlTypeDef ASV_To_Manage;

   ( void ) pvParameters;

    for( ;; )
    {

    	xQueueReceive(xASVMsgQueue, &ASV_To_Manage, portMAX_DELAY);
    	if (ASV_To_Manage.current_Position > ASV_To_Manage.max_Position)
    		ASV_To_Manage.current_Position = ASV_To_Manage.max_Position;
    	if (ASV_To_Manage.current_Position < ASV_To_Manage.min_Position)
    		ASV_To_Manage.current_Position = ASV_To_Manage.min_Position;

    	bb_asv_set_pwm_pulse_length(ASV_To_Manage.channel,ASV_To_Manage.current_Position);
    }
}
*/

/*void ASV_Create(ASV_ControlTypeDef* ASV, BB_ASV_ChannelTypeDef channel, uint16_t min_Position, uint16_t max_Position)
{
	ASV->channel = channel;
	ASV->max_Position = max_Position;
	ASV->min_Position = min_Position;
}*/

void asv_init_servo(asv_servo_t* servo, BB_ASV_ChannelTypeDef channel, uint16_t min_pos, uint16_t max_pos)
{
  servo->channel = channel;
  servo->min_pos = min_pos;
  servo->max_pos = max_pos;
  servo->current_pos = min_pos;
}

/* EXAMPLE
void ASV_DeployLeftArm(void)
{
	GPIO_WriteBit((GPIO_TypeDef*) END6_GPIO_PORT, END6_PIN, Bit_RESET);
}*/

