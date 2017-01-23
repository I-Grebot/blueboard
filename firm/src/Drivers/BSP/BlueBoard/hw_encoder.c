/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       hw_encoder.c
 * @author     Paul
 * @date       Jan 5, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   <DESCRIPTION HERE>
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev: 1464 $
 * $LastChangedBy: Pierrick_Boissard $
 * $LastChangedDate: 2016-05-02 15:38:42 +0200 (lun., 02 mai 2016) $
 * -----------------------------------------------------------------------------
 */

#include "blueboard.h"

static int32_t encoder1_Value, encoder2_Value;
static int16_t encoder1_Old, encoder2_Old;

void HW_ENC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Enable GPIO Clocks */
    ENC1_A_GPIO_CLK_ENABLE();
    ENC1_B_GPIO_CLK_ENABLE();
    ENC2_A_GPIO_CLK_ENABLE();
    ENC2_B_GPIO_CLK_ENABLE();

    /* Configure GPIOs */
    GPIO_InitStruct.GPIO_Mode       = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType      = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd       = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed      = GPIO_Fast_Speed;

    GPIO_InitStruct.GPIO_Pin = ENC1_A_PIN;
    GPIO_PinAFConfig(ENC1_A_GPIO_PORT, ENC1_A_PIN_SOURCE, ENC1_A_AF);
    GPIO_Init(ENC1_A_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = ENC1_B_PIN;
    GPIO_PinAFConfig(ENC1_B_GPIO_PORT, ENC1_B_PIN_SOURCE, ENC1_B_AF);
    GPIO_Init(ENC1_B_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = ENC2_A_PIN;
    GPIO_PinAFConfig(ENC2_A_GPIO_PORT, ENC2_A_PIN_SOURCE, ENC2_A_AF);
    GPIO_Init(ENC2_A_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = ENC2_B_PIN;
    GPIO_PinAFConfig(ENC2_B_GPIO_PORT, ENC2_B_PIN_SOURCE, ENC2_B_AF);
    GPIO_Init(ENC2_B_GPIO_PORT, &GPIO_InitStruct);

    /* Enable Quadrature Encoder timer clocks */
    ENC1_TIM_CLK_ENABLE();
    ENC2_TIM_CLK_ENABLE();

    /* Configure Quadrature Encoder Timers */
    TIM_EncoderInterfaceConfig(ENC1_TIM,
                               TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising);
    TIM_SetAutoreload(ENC1_TIM, 0xFFFFFFFF);

    TIM_EncoderInterfaceConfig(ENC2_TIM,
                               TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising,
                               TIM_ICPolarity_Rising);
    TIM_SetAutoreload(ENC2_TIM, 0xFFFFFFFF);

    /* Cleanup timers */
    HW_ENC_ResetChannels();

    /* Enable Quadrature Encoders */
    TIM_Cmd(ENC1_TIM, ENABLE);
    TIM_Cmd(ENC2_TIM, ENABLE);

}

int32_t HW_ENC_GetChannel(HW_ENC_ChannelTypeDef channel)
{
	int16_t encoder, delta;
    switch(channel)
    {
    case HW_ENC_CHANNEL1:
    	encoder = (int16_t)ENC1_TIM->CNT;
    	delta = encoder - encoder1_Old;
    	encoder1_Old = encoder;
    	encoder1_Value += (int32_t)delta;
        return  encoder1_Value;
        break;

    case HW_ENC_CHANNEL2:
    	encoder = (int16_t)ENC2_TIM->CNT;
    	delta = encoder - encoder2_Old;
    	encoder2_Old = encoder;
    	encoder2_Value += (int32_t)delta;
        return  encoder2_Value;
        break;

    default:
        /* Error */
        return 0;
        break;
    }

}

void HW_ENC_ResetChannels(void)
{
    ENC1_TIM->CNT = 0x00000000 ;
    ENC2_TIM->CNT = 0x00000000 ;
}

