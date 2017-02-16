/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       bb_digital_servo.c
 * @author     Paul
 * @date       Feb 16, 2017
 * -----------------------------------------------------------------------------
 * @brief
 *   This module implements the RS485 hardware functions
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "blueboard.h"

/* When the RS485 is used as a second DSV channel,
 * nothing from this module is available */
#ifndef BB_USE_RS485_DSV_CHAN2

/**
  * @brief  Initialize the Sensor Interface UART
  * @param  None
  * @retval None
  */
void bb_rs485_init(USART_InitTypeDef * USART_InitStruct)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;

    /* Enable Pins Clock */
    RS485_TX_GPIO_CLK_ENABLE();
    RS485_RX_GPIO_CLK_ENABLE();
    RS485_DE_GPIO_CLK_ENABLE();
    RS485_RE_GPIO_CLK_ENABLE();

    /* Enable RS485 USART clock */
    RS485_CLK_ENABLE();

    /* Configure USART as asynchroneous USART with RS485 hardware flow control */
    /* TODO */

}

#endif BB_USE_RS485_DSV_CHAN2
