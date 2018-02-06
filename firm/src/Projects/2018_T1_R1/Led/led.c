/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       led.c
 * @author     Paul
 * @date       Jan 5, 2016
 * -----------------------------------------------------------------------------
 * @brief
 *   This module handles the RGB Led
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

/* Inclusion */
#include "../../2018_T1_R1/include/main.h"

/* Local Variable Mutex */
static xSemaphoreHandle xLedColorMutex;
static xSemaphoreHandle xLedModeMutex;
static BB_LED_ColorTypeDef LedColor=BB_LED_OFF;
static BB_LED_ModeTypeDef LedMode=BB_LED_STATIC;

/* Local, Private functions */
static void led_task(void *pvParameters);


BaseType_t led_start(void)
{
	xLedColorMutex = xSemaphoreCreateMutex();
	xLedModeMutex = xSemaphoreCreateMutex();

  return sys_create_task(led_task, "LED", OS_TASK_STACK_LED, NULL, OS_TASK_PRIORITY_LED, NULL );
}

static void led_task( void *pvParameters )
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
            case BB_LED_BLINK_SLOW:
                blinkPeriod = LED_BLINK_SLOW;
                break;

            case BB_LED_BLINK_FAST:
                blinkPeriod = LED_BLINK_FAST;
                break;

            default:
            case BB_LED_STATIC:
                        break;
        }

        /* Handles blinking counter */
        if(blinkCounter++ > blinkPeriod)
            blinkCounter = 0;

        if((blinkCounter > blinkPeriod / 2) || LedMode == BB_LED_STATIC) {

            /* Duration ON */
            bb_led_set_color(LedColor);
            vTaskDelayUntil( &xNextWakeTime, LED_PWM_DUTY_TICK);

            /* Duration OFF */
            bb_led_set_color(BB_LED_OFF);
            vTaskDelayUntil( &xNextWakeTime, LED_PWM_PERIOD_TICK-LED_PWM_DUTY_TICK);

        } else {
            vTaskDelayUntil( &xNextWakeTime, LED_PWM_PERIOD_TICK);
        }
    }
}

void led_set_color(BB_LED_ColorTypeDef color)
{
	xSemaphoreTake(xLedColorMutex, 10);
	LedColor = color;
	xSemaphoreGive(xLedColorMutex);
}

void led_set_mode(BB_LED_ModeTypeDef mode)
{
	xSemaphoreTake(xLedModeMutex, 10);
	LedMode = mode;
	xSemaphoreGive(xLedModeMutex);
}
