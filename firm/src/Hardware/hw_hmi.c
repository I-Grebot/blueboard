/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       hw_hmi.c
 * @author     Paul
 * @date       Jan 31, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   <DESCRIPTION HERE>
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev:: 1444                                                                 $
 * $LastChangedBy:: paul.m                                                     $
 * $LastChangedDate:: 2016-02-28 22:16:18 +0100 (dim., 28 févr. 2016)         $
 * -----------------------------------------------------------------------------
 */

#include "blueboard.h"

void HW_HMI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStruct;

    /* Enable GPIOs Clock */
    HMI_CSN_GPIO_CLK_ENABLE();
    HMI_SCK_GPIO_CLK_ENABLE();
    HMI_MOSI_GPIO_CLK_ENABLE();
    HMI_MISO_GPIO_CLK_ENABLE();
    HMI_IRQ_IN_GPIO_CLK_ENABLE();
    HMI_IRQ_OUT_GPIO_CLK_ENABLE();

    /* Enable Control Interface SPI clock */
    HMI_CLK_ENABLE();

    /* Configure HMI_CSN as alternate function */
    GPIO_InitStructure.GPIO_Pin = HMI_CSN_PIN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; /* 10 MHz */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(HMI_CSN_GPIO_PORT, &GPIO_InitStructure);

    /* Configure HMI_SCK as alternate function */
    GPIO_InitStructure.GPIO_Pin = HMI_SCK_PIN;
    GPIO_Init(HMI_SCK_GPIO_PORT, &GPIO_InitStructure);

    /* Configure HMI_MOSI as alternate function */
    GPIO_InitStructure.GPIO_Pin = HMI_MOSI_PIN;
    GPIO_Init(HMI_MOSI_GPIO_PORT, &GPIO_InitStructure);

    /* Configure USART RTS as alternate function */
    GPIO_InitStructure.GPIO_Pin = HMI_MISO_PIN;
    GPIO_Init(HMI_MISO_GPIO_PORT, &GPIO_InitStructure);

    /* Connect Alternate functions */
    GPIO_PinAFConfig(HMI_SCK_GPIO_PORT,   HMI_SCK_PIN_SOURCE,   HMI_SCK_AF);
    GPIO_PinAFConfig(HMI_MOSI_GPIO_PORT,  HMI_MOSI_PIN_SOURCE,  HMI_MOSI_AF);
    GPIO_PinAFConfig(HMI_MISO_GPIO_PORT,  HMI_MISO_PIN_SOURCE,  HMI_MISO_AF);

    /* Configure HMI_CSN as Digital Output */
    GPIO_InitStructure.GPIO_Pin = HMI_CSN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed; /* 10 MHz */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(HMI_CSN_GPIO_PORT, &GPIO_InitStructure);

    /* Configure HMI_IRQ_IN as Digital Output */
    GPIO_InitStructure.GPIO_Pin = HMI_IRQ_IN_PIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; /* "IN" for HMI, "OUT" for master (us) */
    GPIO_Init(HMI_IRQ_IN_GPIO_PORT, &GPIO_InitStructure);

    /* Configure HMI_IRQ_OUT as Digital Input */
    GPIO_InitStructure.GPIO_Pin = HMI_IRQ_OUT_PIN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; /* "OUT" for HMI, "IN" for master (us) */
    GPIO_Init(HMI_IRQ_OUT_GPIO_PORT, &GPIO_InitStructure);

    /* Default SPI Configuration */
    SPI_StructInit(&SPI_InitStruct);

    /* Configure custom fields */
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; /* TBC */
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_16b;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; // Internal NSS mgmt and pull internal NSS high;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;

    /* Configure SPI module */
    SPI_Init(HMI_COM, &SPI_InitStruct);
    SPI_SSOutputCmd(HMI_COM, ENABLE);

    /* Enable SPI module */
    HMI_CSN_WRITE(HMI_FRAME_IDLE);
    SPI_Cmd(HMI_COM, ENABLE);

}

uint16_t HW_HMI_TxRx(uint16_t value)
{
    /* Send a new 16 bits word */
    HMI_CSN_WRITE(HMI_FRAME_ACTIVE);
    SPI_I2S_SendData16(HMI_COM, value);

    /* Wait until transmit complete */
    while(SPI_I2S_GetFlagStatus(HMI_COM, SPI_I2S_FLAG_TXE) == RESET);

    /* Wait until receive complete */
    while(SPI_I2S_GetFlagStatus(HMI_COM, SPI_I2S_FLAG_RXNE) == RESET);

    /* Wait until SPI is not busy anymore */
    while(SPI_I2S_GetFlagStatus(HMI_COM, SPI_I2S_FLAG_BSY) == SET);

    HMI_CSN_WRITE(HMI_FRAME_IDLE);

    /* Return 16 bit received word */
    return SPI_I2S_ReceiveData16(HMI_COM);
}