

#ifndef __DIGITAL_SERVO_H_
#define __DIGITAL_SERVO_H_

#include "FreeRTOS.h"
#include "blueboard.h"

/* Queues size for each channel, in Bytes */
#define DSV_RX_QUEUE_SIZE   64
#define DSV_TX_QUEUE_SIZE   64

typedef struct {

    /* Uart configuration handler for the DSV channel */
    USART_InitTypeDef uart;

    /* Uart RX and TX queues */
    QueueHandle_t rx_queue;
    QueueHandle_t tx_queue;

    /* Dynamixel Interface */
    dxl_interface_t dxl;

} dsv_channel_t;

#endif /* __DIGITAL_SERVO_H_ */

