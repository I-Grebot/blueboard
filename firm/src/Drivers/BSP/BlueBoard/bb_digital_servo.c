/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       bb_digital_servo.c
 * @author     Pierrick
 * @date       Mar 19, 2016
 * -----------------------------------------------------------------------------
 * @brief
 *   This module implements the digital servo hardware functions
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "blueboard.h"

/**
  * @brief  Initialize the Digital Servo UART
  * @param  None
  * @retval None
  */
void bb_dsv_init(USART_InitTypeDef * USART_InitStruct)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;

    /* Enable TXRX Clock */
    DSV_TXRX_GPIO_CLK_ENABLE();

    /* Enable DSV USART clock */
    DSV_CLK_ENABLE();

    /* Configure USART TXRX as alternate function */
    GPIO_InitStructure.GPIO_Pin = DSV_TXRX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed; /* 2 MHz */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(DSV_TXRX_GPIO_PORT, &GPIO_InitStructure);

    /* Connect Alternate functions */
    GPIO_PinAFConfig(DSV_TXRX_GPIO_PORT,  DSV_TXRX_PIN_SOURCE,  DSV_TXRX_AF);

    /* USART configuration */
    USART_Init(DSV_COM, USART_InitStruct);

    /* USART Clock default configuration */
    USART_ClockStructInit(&USART_ClockInitStruct);
    USART_ClockInit(DSV_COM, &USART_ClockInitStruct);

    /* Enable USART */
    USART_Cmd(DSV_COM, ENABLE);
}

/**
  * @brief  Send a byte through DSV UART and wait for end of transmission
  * @param  ch: character to send
  * @retval None
  */
void bb_dsv_put(uint8_t ch)
{
      USART_SendData(DSV_COM, (uint8_t) ch);
      while(USART_GetFlagStatus(DSV_COM, USART_FLAG_TC) == RESET);
}
