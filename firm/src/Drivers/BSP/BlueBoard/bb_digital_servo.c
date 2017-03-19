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
void bb_dsv_init(uint8_t dsv_chan, USART_InitTypeDef * USART_InitStruct)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;

    /* Configure Channel 1 */
    if(dsv_chan == BB_DSV_CHANNEL1) {

        /* Disable USART during config*/
        USART_Cmd(DSV_COM, DISABLE);

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

        /* Default state: RX */
        bb_dsv_switch(BB_DSV_CHANNEL1, DXL_MODE_RX);

        /* Enable USART */
        USART_Cmd(DSV_COM, ENABLE);

    /* Configure channel 2 -- Only available when RS485 interface is remapped */
    }

#ifdef BB_USE_RS485_DSV_CHAN2
    else if(dsv_chan == BB_DSV_CHANNEL2) {

        /* Disable USART during config*/
        USART_Cmd(RS485_COM, DISABLE);

        /* Enable I/O Clocks */
        RS485_TX_GPIO_CLK_ENABLE();
        RS485_RX_GPIO_CLK_ENABLE();
        RS485_DE_GPIO_CLK_ENABLE();
        RS485_NRE_GPIO_CLK_ENABLE();

        /* Enable USART clock */
        RS485_CLK_ENABLE();

        /* Configure DE/nRE I/Os */
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;

        GPIO_InitStructure.GPIO_Pin = RS485_DE_PIN;
        GPIO_Init(RS485_DE_GPIO_PORT, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = RS485_NRE_PIN;
        GPIO_Init(RS485_NRE_GPIO_PORT, &GPIO_InitStructure);

        /* Configure USART TX as alternate function */
        GPIO_InitStructure.GPIO_Pin = RS485_TX_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Medium_Speed; /* < 25 MHz */
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(RS485_TX_GPIO_PORT, &GPIO_InitStructure);

        /* Configure USART RX as alternate function */
        GPIO_InitStructure.GPIO_Pin = RS485_RX_PIN;
        GPIO_Init(RS485_RX_GPIO_PORT, &GPIO_InitStructure);

        /* Connect Alternate functions */
        GPIO_PinAFConfig(RS485_TX_GPIO_PORT,  RS485_TX_PIN_SOURCE,  RS485_TX_AF);
        GPIO_PinAFConfig(RS485_RX_GPIO_PORT,  RS485_RX_PIN_SOURCE,  RS485_RX_AF);

        /* USART configuration */
        USART_Init(RS485_COM, USART_InitStruct);

        /* USART Clock default configuration */
        USART_ClockStructInit(&USART_ClockInitStruct);
        USART_ClockInit(RS485_COM, &USART_ClockInitStruct);

        /* Default state: RX */
        bb_dsv_switch(BB_DSV_CHANNEL2, DXL_MODE_RX);

        /* Enable USART */
        USART_Cmd(RS485_COM, ENABLE);
    }
#endif /* BB_USE_RS485_DSV_CHAN2 */

}

void bb_dsv_enable(uint8_t dsv_chan, uint32_t nvic_priority)
{
    if(dsv_chan == BB_DSV_CHANNEL1)
    {
        /* Enable USART Interrupts */
        USART_ITConfig(DSV_COM, USART_IT_RXNE, ENABLE);
        NVIC_SetPriority(DSV_IRQn, nvic_priority);
        NVIC_EnableIRQ(DSV_IRQn);

        /* Enable USART */
        USART_Cmd(DSV_COM, ENABLE);

    } else if(dsv_chan == BB_DSV_CHANNEL2)
    {
        /* Enable USART Interrupts */
        USART_ITConfig(RS485_COM, USART_IT_RXNE, ENABLE);
        NVIC_SetPriority(RS485_IRQn, nvic_priority);
        NVIC_EnableIRQ(RS485_IRQn);

        /* Enable USART */
        USART_Cmd(RS485_COM, ENABLE);
    }
}

void bb_dsv_disable(uint8_t dsv_chan)
{
    if(dsv_chan == BB_DSV_CHANNEL1)
    {
        /* Disable IRQs */
        USART_ITConfig(DSV_COM, USART_IT_RXNE, DISABLE);
        NVIC_DisableIRQ(DSV_IRQn);

        /* Stop UART */
        USART_Cmd(DSV_COM, DISABLE);

    } else if(dsv_chan == BB_DSV_CHANNEL2)
    {
        /* Disable IRQs */
        USART_ITConfig(RS485_COM, USART_IT_RXNE, DISABLE);
        NVIC_DisableIRQ(RS485_IRQn);

        /* Stop UART */
        USART_Cmd(RS485_COM, DISABLE);
    }
}

/* Switch a given interface from TX to RX */
void bb_dsv_switch(uint8_t dsv_chan, dxl_switch_mode_e mode)
{

    if(dsv_chan == BB_DSV_CHANNEL1) {
        if(mode == DXL_MODE_TX) {
                USART_DirectionModeCmd(DSV_COM, USART_Mode_Tx, ENABLE);
                USART_DirectionModeCmd(DSV_COM, USART_Mode_Rx, DISABLE);
            } else {
                USART_DirectionModeCmd(DSV_COM, USART_Mode_Tx, DISABLE);
                USART_DirectionModeCmd(DSV_COM, USART_Mode_Rx, ENABLE);
            }
    }

#ifdef BB_USE_RS485_DSV_CHAN2
    else if(dsv_chan == BB_DSV_CHANNEL2) {

        // In TX: DE and nRE are high
        // In RX: DE and nRE are low
        // nRE could be always low if loopback is desired
        if(mode == DXL_MODE_TX) {
            RS485_DE_WRITE(Bit_SET);
            RS485_NRE_WRITE(Bit_SET);
            LEDB_WRITE(LEDx_ON);
        } else {
            RS485_DE_WRITE(Bit_RESET);
            RS485_NRE_WRITE(Bit_RESET);
            LEDB_WRITE(LEDx_OFF);
        }
    }
#endif /* BB_USE_RS485_DSV_CHAN2 */

}

/**
  * @brief  Send a byte through DSV UART and wait for end of transmission
  * @param  ch: character to send
  * @retval None
  */
/*
void bb_dsv_put(uint8_t dsv_chan, uint8_t ch)
{
    if(dsv_chan == BB_DSV_CHANNEL1) {
        // TEMP
        USART_SendData(DSV_COM, (uint8_t) ch);
        while(USART_GetFlagStatus(DSV_COM, USART_FLAG_TC) == RESET);
    }

#ifdef BB_USE_RS485_DSV_CHAN2
    else if(dsv_chan == BB_DSV_CHANNEL2) {
        // TEMP
        USART_SendData(RS485_COM, (uint8_t) ch);
        while(USART_GetFlagStatus(RS485_COM, USART_FLAG_TC) == RESET);
        USART_ClearFlag(RS485_COM, USART_FLAG_TC);
    }
#endif // BB_USE_RS485_DSV_CHAN2


}

uint8_t bb_dsv_receive(uint8_t dsv_chan, uint8_t* rx_data)
{
    // TEMP
    return 0;
}

void bb_dsv_flush(uint8_t dsv_chan)
{

}
*/

