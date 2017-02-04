/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       bb_motor.c
 * @author     Paul
 * @date       Jan 5, 2016
 * -----------------------------------------------------------------------------
 * @brief
 *   This module handles main motors hardware
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "blueboard.h"


void bb_mot_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_BaseStruct;
    TIM_OCInitTypeDef TIM_OCStruct;

  //  MOT_SLEEP_GPIO_CLK_ENABLE();
  //  MOT_RESET_GPIO_CLK_ENABLE();
  //  MOT_I1_GPIO_CLK_ENABLE();
  //  MOT_I0_GPIO_CLK_ENABLE();
    MOT1_IN1_GPIO_CLK_ENABLE();
    MOT1_IN2_GPIO_CLK_ENABLE();
    MOT2_IN1_GPIO_CLK_ENABLE();
    MOT2_IN2_GPIO_CLK_ENABLE();
  //  MOT_FAULT_GPIO_CLK_ENABLE();

    /* Common configuration for all General-Purpose Outputs */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;

    //   GPIO_InitStructure.GPIO_Pin = MOT_SLEEP_PIN;
    //   GPIO_Init(MOT_SLEEP_GPIO_PORT, &GPIO_InitStructure);

    //    GPIO_InitStructure.GPIO_Pin = MOT_RESET_PIN;
    //    GPIO_Init(MOT_RESET_GPIO_PORT, &GPIO_InitStructure);

    //    GPIO_InitStructure.GPIO_Pin = MOT_I1_PIN;
    //    GPIO_Init(MOT_I1_GPIO_PORT, &GPIO_InitStructure);

    //    GPIO_InitStructure.GPIO_Pin = MOT_I0_PIN;
    //    GPIO_Init(MOT_I0_GPIO_PORT, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = MOT1_IN2_PIN;
        GPIO_Init(MOT1_IN2_GPIO_PORT, &GPIO_InitStructure);

        GPIO_InitStructure.GPIO_Pin = MOT2_IN2_PIN;
        GPIO_Init(MOT2_IN2_GPIO_PORT, &GPIO_InitStructure);

    /* Alternate function (PWM) */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

    GPIO_InitStructure.GPIO_Pin = MOT1_IN1_PIN;
    GPIO_Init(MOT1_IN1_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(MOT1_IN1_GPIO_PORT, MOT1_IN1_PIN_SOURCE, MOT1_IN1_AF);

    //    GPIO_InitStructure.GPIO_Pin = MOT1_IN2_PIN;
    //    GPIO_Init(MOT1_IN2_GPIO_PORT, &GPIO_InitStructure);
    //    GPIO_PinAFConfig(MOT1_IN2_GPIO_PORT, MOT1_IN2_PIN_SOURCE, MOT1_IN2_AF);

    GPIO_InitStructure.GPIO_Pin = MOT2_IN1_PIN;
    GPIO_Init(MOT2_IN1_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(MOT2_IN1_GPIO_PORT, MOT2_IN1_PIN_SOURCE, MOT2_IN1_AF);

    //    GPIO_InitStructure.GPIO_Pin = MOT2_IN2_PIN;
    //    GPIO_Init(MOT2_IN2_GPIO_PORT, &GPIO_InitStructure);
    //    GPIO_PinAFConfig(MOT2_IN2_GPIO_PORT, MOT2_IN2_PIN_SOURCE, MOT2_IN2_AF);

    /* Fault is an Open-Drain output so we need a pull-up */
    //    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    //    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    //    GPIO_InitStructure.GPIO_Pin = MOT_FAULT_PIN;
    //    GPIO_Init(MOT_FAULT_GPIO_PORT, &GPIO_InitStructure);

    /* Enable Timers Clocks from RCC */
    MOT_TIM_CLK_ENABLE();

    /* Setup Motors Timers */
    TIM_BaseStruct.TIM_CounterMode          = TIM_CounterMode_Up;
    TIM_BaseStruct.TIM_ClockDivision        = TIM_CKD_DIV1;
    TIM_BaseStruct.TIM_Prescaler            = MOT_TIMER_PRESCALER;
    TIM_BaseStruct.TIM_Period               = MOT_TIMER_PERIOD;
    TIM_BaseStruct.TIM_RepetitionCounter    = 0;
    TIM_TimeBaseInit(MOT_TIM, &TIM_BaseStruct);

    /* Only valid but required for TIM1/TIM8 */
    TIM_CtrlPWMOutputs(MOT_TIM, ENABLE);

    /* Enable Motor Timer */
    TIM_Cmd(MOT_TIM, ENABLE);

    /* Configure Output Compare modules */
    TIM_OCStructInit(&TIM_OCStruct);
    TIM_OCStruct.TIM_OCMode         = TIM_OCMode_PWM1;
    TIM_OCStruct.TIM_OutputState    = TIM_OutputState_Enable;
    TIM_OCStruct.TIM_OutputNState   = TIM_OutputState_Enable;   /* Only valid for TIM1/TIM8 */
    TIM_OCStruct.TIM_Pulse          = 0;                        /* Default duty @ 0% */

    TIM_OC1Init(MOT_TIM, &TIM_OCStruct);
    TIM_OC2Init(MOT_TIM, &TIM_OCStruct);
    TIM_OC3Init(MOT_TIM, &TIM_OCStruct);
    TIM_OC4Init(MOT_TIM, &TIM_OCStruct);

    TIM_OC1PreloadConfig(MOT_TIM, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(MOT_TIM, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(MOT_TIM, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(MOT_TIM, TIM_OCPreload_Enable);

    /* Ensure the module is in Reset and Sleep mode by default */
    //    MOT_SLEEP_WRITE(MOT_SLEEP_ON);
    //    MOT_RESET_WRITE(MOT_RESET_ON);

    /* Setup default bridge current */
    //    BB_MOT_SetBridgeCurrent(BB_MOT_CURRENT_100);
}

void bb_mot_set_bridge_current(BB_MOT_CurrentTypeDef current)
{
    switch(current) {
        case BB_MOT_CURRENT_100:
            MOT_I1_WRITE(Bit_RESET);
            MOT_I0_WRITE(Bit_RESET);
            break;

        case BB_MOT_CURRENT_71:
            MOT_I1_WRITE(Bit_RESET);
            MOT_I0_WRITE(Bit_SET);
            break;

        case BB_MOT_CURRENT_38:
            MOT_I1_WRITE(Bit_SET);
            MOT_I0_WRITE(Bit_RESET);
            break;

        default:
        case BB_MOT_CURRENT_0:
            MOT_I1_WRITE(Bit_SET);
            MOT_I0_WRITE(Bit_SET);
            break;
    }
}

/**
  * @brief  Set a channel motor speed. DRV8843's PWM function:
  *
  *     xIN1            xIN2            Function
  * ------------------------------------------------------------
  *     PWM              0              Forward PWM, fast decay
  *      1              PWM             Forward PWM, slow decay
  *      0              PWM             Reverse PWM, fast decay
  *     PWM              1              Reverse PWM, slow decay
  *
  * @param  channel: Motor channel to adjust
  * @param  speed: Signed input value, the sign indicates backward or forward rotation
  * @param  fastDecay: If this param is enabled, fast decay mode is applied.
  * @retval None
  */
void bb_mot_set_motor_speed(BB_MOT_ChannelTypeDef channel, int16_t speed, FunctionalState fastDecay)
{
    uint16_t xIN1_PWM;
    uint16_t xIN2_PWM;

    /* Clamp speed value to 100% */
    if(speed > MOT_TIMER_PERIOD) {
        speed = MOT_TIMER_PERIOD;
    } else if(speed < -MOT_TIMER_PERIOD) {
        speed = -MOT_TIMER_PERIOD;
    }

    /* Forward rotation */
    if(speed > 0) {
 //       if(fastDecay == ENABLE) {
            xIN1_PWM = (uint16_t) speed;
            xIN2_PWM = 0x0000;           /* Meaning constant 0 */
//        } else {
//            xIN1_PWM = MOT_TIMER_PERIOD; /* Meaning constant 1 */
//            xIN2_PWM = (uint16_t) speed;
//        }

    /* Reverse rotation */
    } else {
//       if(fastDecay == ENABLE) {
//            xIN1_PWM = 0x0000;           /* Meaning constant 0 */
//            xIN2_PWM = (uint16_t) -speed;
//        } else {
            xIN1_PWM = (uint16_t) -speed;
            xIN2_PWM = MOT_TIMER_PERIOD; /* Meaning constant 1 */
//        }

    }

    /* Setup new PWM Compare value for the selected channel */
    switch(channel) {
        case BB_MOT_CHANNEL1:
            MOT1_IN1_SET_PULSE(xIN1_PWM);
            //MOT1_IN2_SET_PULSE(xIN2_PWM);
            GPIO_WriteBit(MOT1_IN2_GPIO_PORT, MOT1_IN2_PIN, xIN2_PWM);
            break;

        case BB_MOT_CHANNEL2:
            MOT2_IN1_SET_PULSE(xIN1_PWM);
            //MOT2_IN2_SET_PULSE(xIN2_PWM);
            GPIO_WriteBit(MOT2_IN2_GPIO_PORT, MOT2_IN2_PIN, xIN2_PWM);
            break;

        default:
            /* Incorrect channel: do noting */
            break;
    }

}

/* Shorthand for fast-decay PWM */
void bb_mot_set_motor_speed_fast_decay(BB_MOT_ChannelTypeDef channel, int32_t speed)
{
    bb_mot_set_motor_speed(channel, (int16_t) speed, ENABLE);
}
