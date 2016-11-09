/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       hw_debug.c
 * @author     Paul
 * @date       Jan 3, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   This module implements the debug functions
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev: 1458 $
 * $LastChangedBy: Pierrick_Boissard $
 * $LastChangedDate: 2016-04-29 11:29:31 +0200 (ven., 29 avr. 2016) $
 * -----------------------------------------------------------------------------
 */

#include "blueboard.h"

/**
  * @brief  Initialize the Debug UART
  * @param  None
  * @retval None
  */
void HW_DBG_Init(USART_InitTypeDef * USART_InitStruct)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;

    /* Enable GPIOs Clock */
    DBG_TX_GPIO_CLK_ENABLE();
    DBG_RX_GPIO_CLK_ENABLE();
    DBG_CTS_GPIO_CLK_ENABLE();
    DBG_RTS_GPIO_CLK_ENABLE();

    /* Enable Debug USART clock */
    DBG_CLK_ENABLE();

    /* Configure USART TX as alternate function */
    GPIO_InitStructure.GPIO_Pin = DBG_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed; /* 2 MHz */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(DBG_TX_GPIO_PORT, &GPIO_InitStructure);

    /* Configure USART RX as alternate function */
    GPIO_InitStructure.GPIO_Pin = DBG_RX_PIN;
    GPIO_Init(DBG_RX_GPIO_PORT, &GPIO_InitStructure);

    /* Configure USART CTS as alternate function */
    GPIO_InitStructure.GPIO_Pin = DBG_CTS_PIN;
    GPIO_Init(DBG_CTS_GPIO_PORT, &GPIO_InitStructure);

    /* Configure USART RTS as alternate function */
    GPIO_InitStructure.GPIO_Pin = DBG_RTS_PIN;
    GPIO_Init(DBG_RTS_GPIO_PORT, &GPIO_InitStructure);

    /* Connect Alternate functions */
    GPIO_PinAFConfig(DBG_TX_GPIO_PORT,  DBG_TX_PIN_SOURCE,  DBG_TX_AF);
    GPIO_PinAFConfig(DBG_RX_GPIO_PORT,  DBG_RX_PIN_SOURCE,  DBG_RX_AF);
    GPIO_PinAFConfig(DBG_CTS_GPIO_PORT, DBG_CTS_PIN_SOURCE, DBG_CTS_AF);
    GPIO_PinAFConfig(DBG_RTS_GPIO_PORT, DBG_RTS_PIN_SOURCE, DBG_RTS_AF);

    /* USART configuration */
    USART_Init(DBG_COM, USART_InitStruct);

    /* USART Clock default configuration */
    USART_ClockStructInit(&USART_ClockInitStruct);
    USART_ClockInit(DBG_COM, &USART_ClockInitStruct);

    /* Enable USART */
    USART_Cmd(DBG_COM, ENABLE);

}

/**
  * @brief  Send a byte through debug UART and wait for end of transmission
  * @param  ch: character to send
  * @retval None
  */
void HW_DBG_Put(uint8_t ch)
{
      USART_SendData(DBG_COM, (uint8_t) ch);
      while(USART_GetFlagStatus(DBG_COM, USART_FLAG_TC) == RESET);
}


/**
  * @brief  Send a string through debug UART and wait for end of transmission
  * @param  str: string to send
  * @retval None
  */
void HW_DBG_Puts(char *str)
{
    while (*str)
    {
    	HW_DBG_Put(*str);
        str++;
    }
}

/**
  * @brief  Receive a byte from debug UART
  * @param  None
  * @retval Received byte
  */
uint8_t HW_DBG_Get(void)
{
     while (USART_GetFlagStatus(DBG_COM, USART_FLAG_RXNE) == RESET);
        return (uint8_t) USART_ReceiveData(DBG_COM);
}

