/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       task_led.c
 * @author     Paul
 * @date       Jan 5, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   This module handles the RGB Led
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev: 1458 $
 * $LastChangedBy: Pierrick_Boissard $
 * $LastChangedDate: 2016-04-29 11:29:31 +0200 (ven., 29 avr. 2016) $
 * -----------------------------------------------------------------------------
 */

/* Inclusion */
#include "blueboard.h"

/* Local Variable Mutex */
static xSemaphoreHandle xLedColorMutex;
static xSemaphoreHandle xLedModeMutex;
static HW_LED_ColorTypeDef LedColor=HW_LED_OFF;
static HW_LED_ModeTypeDef LedMode=HW_LED_STATIC;

/* Local, Private functions */
static void OS_LedTask(void *pvParameters);


void OS_CreateLedTask(void)
{
	xLedColorMutex = xSemaphoreCreateMutex();
	xLedModeMutex = xSemaphoreCreateMutex();
    xTaskCreate(OS_LedTask, "LED", configMINIMAL_STACK_SIZE, NULL, OS_TASK_PRIORITY_LED, NULL );
}

static void OS_LedTask( void *pvParameters )
{
    TickType_t xNextWakeTime;

    uint32_t blinkCounter = 0;
    uint32_t blinkPeriod = 0;

    /* Initialise xNextWakeTime - this only needs to be done once. */
    xNextWakeTime = xTaskGetTickCount();

    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    for( ;; )
    {
        switch(LedMode)
        {
            case HW_LED_BLINK_SLOW:
                blinkPeriod = LED_BLINK_SLOW;
                break;

            case HW_LED_BLINK_FAST:
                blinkPeriod = LED_BLINK_FAST;
                break;

            default:
            case HW_LED_STATIC:
                        break;
        }

        /* Handles blinking counter */
        if(blinkCounter++ > blinkPeriod)
            blinkCounter = 0;

        if((blinkCounter > blinkPeriod / 2) || LedMode == HW_LED_STATIC) {

            /* Duration ON */
            HW_LED_SetColor(LedColor);
            vTaskDelayUntil( &xNextWakeTime, LED_PWM_DUTY_TICK);

            /* Duration OFF */
            HW_LED_SetColor(HW_LED_OFF);
            vTaskDelayUntil( &xNextWakeTime, LED_PWM_PERIOD_TICK-LED_PWM_DUTY_TICK);

        } else {
            vTaskDelayUntil( &xNextWakeTime, LED_PWM_PERIOD_TICK);
        }
    }
}

void LedSetColor(HW_LED_ColorTypeDef color)
{
	xSemaphoreTake(xLedColorMutex, 10);
	LedColor = color;
	xSemaphoreGive(xLedColorMutex);
}

void LedSetMode(HW_LED_ModeTypeDef mode)
{
	xSemaphoreTake(xLedModeMutex, 10);
	LedMode = mode;
	xSemaphoreGive(xLedModeMutex);
}
