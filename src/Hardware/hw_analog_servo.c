/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       hw_analog_servo.c
 * @author     Paul
 * @date       Jan 4, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   <DESCRIPTION HERE>
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev:: 1469                                                                 $
 * $LastChangedBy:: Pierrick_Boissard                                          $
 * $LastChangedDate:: 2016-05-05 09:40:31 +0200 (jeu., 05 mai 2016)            $
 * -----------------------------------------------------------------------------
 */


#include "blueboard.h"

void HW_ASV_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_BaseStruct;
    TIM_OCInitTypeDef TIM_OCStruct;

    /* Enable ASV GPIO Clocks */
    ASV1_PWM_GPIO_CLK_ENABLE();
    ASV2_PWM_GPIO_CLK_ENABLE();
    ASV3_PWM_GPIO_CLK_ENABLE();
    ASV4_PWM_GPIO_CLK_ENABLE();
    ASV5_PWM_GPIO_CLK_ENABLE();
    ASV6_PWM_GPIO_CLK_ENABLE();
    ASV7_PWM_GPIO_CLK_ENABLE();
    ASV8_PWM_GPIO_CLK_ENABLE();

    GPIO_InitStruct.GPIO_Mode       = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType      = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd       = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed      = GPIO_Fast_Speed;

    GPIO_InitStruct.GPIO_Pin = ASV1_PWM_PIN;
    GPIO_PinAFConfig(ASV1_PWM_GPIO_PORT, ASV1_PWM_PIN_SOURCE, ASV1_PWM_AF);
    GPIO_Init(ASV1_PWM_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = ASV2_PWM_PIN;
    GPIO_PinAFConfig(ASV2_PWM_GPIO_PORT, ASV2_PWM_PIN_SOURCE, ASV2_PWM_AF);
    GPIO_Init(ASV2_PWM_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Pin = ASV3_PWM_PIN;
 //   GPIO_PinAFConfig(ASV3_PWM_GPIO_PORT, ASV3_PWM_PIN_SOURCE, ASV3_PWM_AF);
    GPIO_Init(ASV3_PWM_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = ASV4_PWM_PIN;
 //   GPIO_PinAFConfig(ASV4_PWM_GPIO_PORT, ASV4_PWM_PIN_SOURCE, ASV4_PWM_AF);
    GPIO_Init(ASV4_PWM_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode       = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Pin = ASV5_PWM_PIN;
    GPIO_PinAFConfig(ASV5_PWM_GPIO_PORT, ASV5_PWM_PIN_SOURCE, ASV5_PWM_AF);
    GPIO_Init(ASV5_PWM_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = ASV6_PWM_PIN;
    GPIO_PinAFConfig(ASV6_PWM_GPIO_PORT, ASV6_PWM_PIN_SOURCE, ASV6_PWM_AF);
    GPIO_Init(ASV6_PWM_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = ASV7_PWM_PIN;
    GPIO_PinAFConfig(ASV7_PWM_GPIO_PORT, ASV7_PWM_PIN_SOURCE, ASV7_PWM_AF);
    GPIO_Init(ASV7_PWM_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = ASV8_PWM_PIN;
    GPIO_PinAFConfig(ASV8_PWM_GPIO_PORT, ASV8_PWM_PIN_SOURCE, ASV8_PWM_AF);
    GPIO_Init(ASV8_PWM_GPIO_PORT, &GPIO_InitStruct);

    /* Enable Timers Clocks from RCC */
    ASV_1_TO_4_TIM_CLK_ENABLE();
    ASV_5_TO_8_TIM_CLK_ENABLE();

    /* Setup ASV Timers */
    TIM_BaseStruct.TIM_CounterMode          = TIM_CounterMode_Up;
    TIM_BaseStruct.TIM_ClockDivision        = TIM_CKD_DIV1;
    TIM_BaseStruct.TIM_Prescaler            = ASV_TIMER_PRESCALER;
    TIM_BaseStruct.TIM_Period               = ASV_TIMER_PERIOD;
    TIM_BaseStruct.TIM_RepetitionCounter    = 0;
    TIM_TimeBaseInit(ASV_1_TO_4_TIM, &TIM_BaseStruct);
    TIM_TimeBaseInit(ASV_5_TO_8_TIM, &TIM_BaseStruct);

    /* Only valid but required for TIM1/TIM8 */
    TIM_CtrlPWMOutputs(ASV_1_TO_4_TIM, ENABLE);
    TIM_CtrlPWMOutputs(ASV_5_TO_8_TIM, ENABLE);

    TIM_Cmd(ASV_1_TO_4_TIM, ENABLE);
    TIM_Cmd(ASV_5_TO_8_TIM, ENABLE);

    /* Configure Output Compare modules */
    TIM_OCStructInit(&TIM_OCStruct);
    TIM_OCStruct.TIM_OCMode         = TIM_OCMode_PWM1;
    TIM_OCStruct.TIM_OCPolarity     = TIM_OCPolarity_Low;       /* Invert polarity since we have optocouplers */
    TIM_OCStruct.TIM_OutputState    = TIM_OutputState_Enable;
    TIM_OCStruct.TIM_OutputNState   = TIM_OutputState_Enable;   /* Only valid for TIM1/TIM8 */
    TIM_OCStruct.TIM_Pulse          = 0;                        /* Default duty @ 0% */

    TIM_OC1Init(ASV_1_TO_4_TIM, &TIM_OCStruct);
    TIM_OC2Init(ASV_1_TO_4_TIM, &TIM_OCStruct);
    TIM_OC3Init(ASV_1_TO_4_TIM, &TIM_OCStruct);
    TIM_OC4Init(ASV_1_TO_4_TIM, &TIM_OCStruct);
    TIM_OC1Init(ASV_5_TO_8_TIM, &TIM_OCStruct);
    TIM_OC2Init(ASV_5_TO_8_TIM, &TIM_OCStruct);
    TIM_OC3Init(ASV_5_TO_8_TIM, &TIM_OCStruct);
    TIM_OC4Init(ASV_5_TO_8_TIM, &TIM_OCStruct);

    TIM_OC1PreloadConfig(ASV_1_TO_4_TIM, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(ASV_1_TO_4_TIM, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(ASV_1_TO_4_TIM, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(ASV_1_TO_4_TIM, TIM_OCPreload_Enable);
    TIM_OC1PreloadConfig(ASV_5_TO_8_TIM, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(ASV_5_TO_8_TIM, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(ASV_5_TO_8_TIM, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(ASV_5_TO_8_TIM, TIM_OCPreload_Enable);

}

void HW_ASV_SetPwmPulseLength(HW_ASV_ChannelTypeDef ASV_Channel, uint16_t pulseLength)
{

    /* Configure the correct OC depending on channel selection */
    switch(ASV_Channel)
    {
        case HW_ASV_CHANNEL1: ASV1_PWM_SET_PULSE(pulseLength); break;
        case HW_ASV_CHANNEL2: ASV2_PWM_SET_PULSE(pulseLength); break;
        case HW_ASV_CHANNEL3: ASV3_PWM_SET_PULSE(pulseLength); break;
        case HW_ASV_CHANNEL4: ASV4_PWM_SET_PULSE(pulseLength); break;
        case HW_ASV_CHANNEL5: ASV5_PWM_SET_PULSE(pulseLength); break;
        case HW_ASV_CHANNEL6: ASV6_PWM_SET_PULSE(pulseLength); break;
        case HW_ASV_CHANNEL7: ASV7_PWM_SET_PULSE(pulseLength); break;
        case HW_ASV_CHANNEL8: ASV8_PWM_SET_PULSE(pulseLength); break;

        default:
            /* Incorrect, Do nothing */
            break;
    }

}
