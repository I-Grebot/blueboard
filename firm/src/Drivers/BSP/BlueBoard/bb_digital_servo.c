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
void bb_dsv_init(uint8_t chan_idx, USART_InitTypeDef * USART_InitStruct)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;

    /* Configure Channel 1 */
    if(chan_idx == 1) {

        /* Enable TXRX Clock */
        DSV_TXRX_GPIO_CLK_ENABLE();

        /* Enable USART clock */
        DSV_CLK_ENABLE();

        /* Configure USART TXRX as alternate function */
        GPIO_InitStructure.GPIO_Pin = DSV_TXRX_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Medium_Speed; /* < 25 MHz */
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

    /* Configure channel 2 -- Only available when RS485 interface is remapped */
    }

#ifdef BB_USE_RS485_DSV_CHAN2
    else if(chan_idx == 2) {

        /* Enable TXRX Clock */
        RS485_TX_GPIO_CLK_ENABLE();

        /* Enable USART clock */
        RS485_CLK_ENABLE();

        /* Configure USART TXRX as alternate function */
        GPIO_InitStructure.GPIO_Pin = RS485_TX_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Medium_Speed; /* < 25 MHz */
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(RS485_TX_GPIO_PORT, &GPIO_InitStructure);

        /* Connect Alternate functions */
        GPIO_PinAFConfig(RS485_TX_GPIO_PORT,  RS485_TX_PIN_SOURCE,  RS485_TX_AF);

        /* USART configuration */
        USART_Init(RS485_COM, USART_InitStruct);

        /* USART Clock default configuration */
        USART_ClockStructInit(&USART_ClockInitStruct);
        USART_ClockInit(RS485_COM, &USART_ClockInitStruct);

        /* Enable USART */
        USART_Cmd(RS485_COM, ENABLE);
    }
#endif /* BB_USE_RS485_DSV_CHAN2 */

}

/* Switch a given interface from TX to RX */
void bb_dsv_switch(uint8_t chan_idx, dxl_switch_mode_e mode)
{
    if(chan_idx == 1) {
        // TODO
    }

#ifdef BB_USE_RS485_DSV_CHAN2
    else if(chan_idx == 2) {
        // TODO
    }
#endif /* BB_USE_RS485_DSV_CHAN2 */

}

/**
  * @brief  Send a byte through DSV UART and wait for end of transmission
  * @param  ch: character to send
  * @retval None
  */
void bb_dsv_put(uint8_t chan_idx, uint8_t ch)
{
    if(chan_idx == 1) {
        // TEMP
        USART_SendData(DSV_COM, (uint8_t) ch);
        while(USART_GetFlagStatus(DSV_COM, USART_FLAG_TC) == RESET);
    }

#ifdef BB_USE_RS485_DSV_CHAN2
    else if(chan_idx == 2) {
        // TEMP
        USART_SendData(RS485_COM, (uint8_t) ch);
        while(USART_GetFlagStatus(RS485_COM, USART_FLAG_TC) == RESET);
    }
#endif /* BB_USE_RS485_DSV_CHAN2 */

}
