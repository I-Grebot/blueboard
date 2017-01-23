/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       hw_debug.c
 * @author     Paul
 * @date       Jan 3, 2016
 * -----------------------------------------------------------------------------
 * @brief
 *   This module implements the serial interface functions
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/firm_blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "blueboard.h"

/* Queues used to hold received characters, and characters waiting to be transmitted. */
static QueueHandle_t DBG_RxQueue;
static QueueHandle_t DBG_TxQueue;

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

    /* Create USART Queues */
    DBG_RxQueue = xQueueCreate(DBG_RX_QUEUE_LEN, sizeof(char));
    DBG_TxQueue = xQueueCreate(DBG_TX_QUEUE_LEN, sizeof(char));

    /* Enable USART Interrupts */
    USART_ITConfig(DBG_COM, USART_IT_RXNE, ENABLE);
    NVIC_SetPriority(DBG_IRQn, OS_ISR_PRIORITY_SER);
    NVIC_EnableIRQ(DBG_IRQn);

    /* Enable USART */
    USART_Cmd(DBG_COM, ENABLE);

}

/**
  * @brief  Add a byte to send through Serial Interface.
  *         Might be started right away or stored in the TX Queue.
  * @param  ch: character to send
  * @retval pdPASS if the character was sent or added to the TX Queue
  *         pdFAIL if the TX Queue is full
  */

BaseType_t HW_DBG_Put(char ch)
{

    /* If the queue is empty and no message is being transmitted,
     * the line is idle so we can initiate directly the transmission.
     */
    if((uxQueueMessagesWaiting(DBG_TxQueue) == 0) &&
       (USART_GetFlagStatus(DBG_COM, USART_FLAG_TXE) == SET)) {

        USART_ITConfig(DBG_COM, USART_IT_TC, ENABLE);
        USART_SendData(DBG_COM, (uint16_t) ch);

        return pdPASS;

    /* Otherwise, at least 1 transmission is pending,
     * so we add it the the transmit queue.
     */
    } else {

        if(xQueueSend(DBG_TxQueue, &ch, DBG_TX_TIMEOUT) == pdPASS)
        {
            USART_ITConfig(DBG_COM, USART_IT_TC, ENABLE);
            return pdPASS;
        } else {
            return pdFAIL;
        }
    }

}


/**
  * @brief  Send a string through Serial Interface
  * @param  str: string to send
  * @retval Pass/Fail status
  */
BaseType_t HW_DBG_Puts(const char *str)
{
    while (*str)
    {
    	if((HW_DBG_Put(*str)) == pdPASS) {
    	    str++;
    	} else {
    	    return pdFAIL;
    	}
    }

    return pdPASS;
}

/**
  * @brief  Receive a byte from debug UART
  * @param  Const pointer to read value
  * @retval Pass/Fail status
  */
BaseType_t HW_DBG_Get(const char* str)
{
    return(xQueueReceive(DBG_RxQueue, str, DBG_RX_TIMEOUT));
}


/*
 * Serial Interface ISR
 */
void DBG_ISR (void)
{
    char rxChar;
    char txChar;

    // We have not woken a task at the start of the ISR.
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Hand RX Interrupt
    if(USART_GetITStatus(DBG_COM, USART_IT_RXNE) != RESET)
    {
        // RXNE Flag cleared with the USAR_ReceiveData() call
        rxChar = USART_ReceiveData(DBG_COM);

        // Put the received char into the Rx Queue
        xQueueSendFromISR(DBG_RxQueue, &rxChar, &xHigherPriorityTaskWoken);

    }

    // Handle TX Interrupt
    else if(USART_GetITStatus(DBG_COM, USART_IT_TC) != RESET)
    {
        // Clear IT
        USART_ClearITPendingBit(DBG_COM, USART_IT_TC);

        /* Check to see if there is more data to send.
         * The TC interrupt will be re-generated at the end of the transmission.
         * Therefore, this will auto-reload until the queue is empty.
         */
        if((xQueueReceiveFromISR(DBG_TxQueue, &txChar, &xHigherPriorityTaskWoken)) == pdPASS)
        {
            USART_SendData(DBG_COM, (uint16_t) txChar);
        }
        else
        {
            // Nothing to send! Disable back the ISR
            USART_ITConfig(DBG_COM, USART_IT_TC, DISABLE);
        }

    }
}

