/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       digital_servo.c
 * @author     Pierrick
 * @date       Apr 27, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   This module handles the digital servos
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

/* Inclusion */
#include "main.h"


static USART_InitTypeDef Dsv_Config;

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

void dsv_init(void)
{
    /*
     * Configure the Digital Servo UART init structure:
     *   8 bits length + 1 stop bit, no parity
     *   Baudrate 57600 kbps
     */
     Dsv_Config.USART_Mode                = USART_Mode_Tx;
     Dsv_Config.USART_BaudRate            = 57600;
     Dsv_Config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
     Dsv_Config.USART_Parity              = USART_Parity_No;
     Dsv_Config.USART_StopBits            = USART_StopBits_1;
     Dsv_Config.USART_WordLength          = USART_WordLength_8b;

     /* Initialize DSV Hardware */
     bb_dsv_init(&Dsv_Config);

     /* Initialize XL-320 Library */
     xl_320_init(XL_320_TX_ONLY);
     xl_320_set_hw_send(bb_dsv_put);
}

BaseType_t dsv_start(void)
{

	xDSVMsgQueue = xQueueCreate( MAX_DSV_IN_QUEUE, sizeof(DSV_ControlTypeDef));
    if(xDSVMsgQueue==NULL)
    {
    	serial_puts("Error: Insufficient heap RAM available for DSVMsgQueue"SHELL_EOL);
    	return pdFAIL;
    }

    DSV_Create(&servo1, 23, 0, 1023);
    DSV_Create(&servo2, 42, 0, 1023);

    return xTaskCreate(OS_DSVTask, "DIGITAL SERVO", 350, NULL, OS_TASK_PRIORITY_DSV, NULL );
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

/* EXAMPLE
void DSV_SetServo1(uint16_t position)
{
	servo1.current_Position = position;
	xQueueSend( xDSVMsgQueue, &servo1, (TickType_t)0 );
}*/
