/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       bb_dio.c
 * @author     Pierrick
 * @date       Apr 28, 2016
 * -----------------------------------------------------------------------------
 * @brief
 *   This module implements the GPIOs hardware functions
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "blueboard.h"

/**
  * @brief  Initialize the digital inputs
  * @param  None
  * @retval None
  */
void bb_dio_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable Port Clock */
	SW1_GPIO_CLK_ENABLE();
	SW2_GPIO_CLK_ENABLE();
	SW3_GPIO_CLK_ENABLE();

    /* Configure switches as input */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed;

    GPIO_InitStructure.GPIO_Pin = SW1_PIN;
    GPIO_Init(SW1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SW2_PIN;
    GPIO_Init(SW2_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SW3_PIN;
    GPIO_Init(SW3_GPIO_PORT, &GPIO_InitStructure);

    /* Configure end stops as input */
    GPIO_InitStructure.GPIO_Pin = END1_PIN;
    GPIO_Init(END1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = END2_PIN;
    GPIO_Init(END2_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = END3_PIN;
    GPIO_Init(END3_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = END4_PIN;
    GPIO_Init(END4_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = END5_PIN;
    GPIO_Init(END5_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = END6_PIN;
    GPIO_Init(END6_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = END7_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_Init(END7_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = END8_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_Init(END8_GPIO_PORT, &GPIO_InitStructure);

    /* Configure end industrial sensors as input */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pin = IND1_PIN;
    GPIO_Init(IND1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = IND2_PIN;
    GPIO_Init(IND2_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = IND3_PIN;
    GPIO_Init(IND3_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = IND4_PIN;
    GPIO_Init(IND4_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = IND5_PIN;
    GPIO_Init(IND5_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = IND6_PIN;
    GPIO_Init(IND6_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = IND7_PIN;
    GPIO_Init(IND7_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = IND8_PIN;
    GPIO_Init(IND8_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = IND9_PIN;
    GPIO_Init(IND9_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = IND10_PIN;
    GPIO_Init(IND10_GPIO_PORT, &GPIO_InitStructure);

}
