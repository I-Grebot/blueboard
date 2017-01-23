/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       hw_led.c
 * @author     Paul
 * @date       Jan 3, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   This module handles the RGB LED
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev:: 1431                                                                 $
 * $LastChangedBy:: paul.m                                                     $
 * $LastChangedDate:: 2016-01-19 22:06:16 +0100 (mar., 19 janv. 2016)          $
 * -----------------------------------------------------------------------------
 */

#include "blueboard.h"

/**
  * @brief  Configure LEDs GPIOs (push-pull, no pull-up)
  * @param  None
  * @retval None
  */
void HW_LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    LEDR_GPIO_CLK_ENABLE();
    LEDG_GPIO_CLK_ENABLE();
    LEDB_GPIO_CLK_ENABLE();

    /* Common configuration for all LEDs */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;

    GPIO_InitStructure.GPIO_Pin = LEDR_PIN;
    GPIO_Init(LEDR_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LEDG_PIN;
    GPIO_Init(LEDG_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LEDB_PIN;
    GPIO_Init(LEDB_GPIO_PORT, &GPIO_InitStructure);

    /* Default state: LEDs OFF */
    LEDR_WRITE(LEDx_OFF);
    LEDG_WRITE(LEDx_OFF);
    LEDB_WRITE(LEDx_OFF);
}

/**
  * @brief  Setup a new RGB Led color
  * @param  color: Color to setup
  * @retval None
  */
void HW_LED_SetColor(HW_LED_ColorTypeDef color)
{
    switch(color)
    {
    case HW_LED_RED:
        LEDR_WRITE(LEDx_ON);
        LEDG_WRITE(LEDx_OFF);
        LEDB_WRITE(LEDx_OFF);
        break;
    case HW_LED_GREEN:
        LEDR_WRITE(LEDx_OFF);
        LEDG_WRITE(LEDx_ON);
        LEDB_WRITE(LEDx_OFF);
        break;
    case HW_LED_BLUE:
        LEDR_WRITE(LEDx_OFF);
        LEDG_WRITE(LEDx_OFF);
        LEDB_WRITE(LEDx_ON);
        break;
    case HW_LED_CYAN:
        LEDR_WRITE(LEDx_OFF);
        LEDG_WRITE(LEDx_ON);
        LEDB_WRITE(LEDx_ON);
        break;
    case HW_LED_YELLOW:
        LEDR_WRITE(LEDx_ON);
        LEDG_WRITE(LEDx_ON);
        LEDB_WRITE(LEDx_OFF);
        break;
    case HW_LED_MAGENTA:
        LEDR_WRITE(LEDx_ON);
        LEDG_WRITE(LEDx_OFF);
        LEDB_WRITE(LEDx_ON);
        break;
    case HW_LED_WHITE:
        LEDR_WRITE(LEDx_ON);
        LEDG_WRITE(LEDx_ON);
        LEDB_WRITE(LEDx_ON);
        break;

    default:
    case HW_LED_OFF:
        LEDR_WRITE(LEDx_OFF);
        LEDG_WRITE(LEDx_OFF);
        LEDB_WRITE(LEDx_OFF);
        break;

    }

}
