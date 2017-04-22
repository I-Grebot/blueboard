/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       bb_monitoring.c
 * @author     Paul
 * @date       Jan 3, 2016
 * -----------------------------------------------------------------------------
 * @brief
 *   Implementation of analog monitoring features
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "blueboard.h"

void bb_mon_init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStruct;
    ADC_InitTypeDef  ADC_InitStructure;

    /* Enable ADC clock so that we can talk to it */
    MON_CLK_ENABLE();

    /* Enable GPIO clocks */
    ADC_CEL1_GPIO_CLK_ENABLE();
    ADC_CEL2_GPIO_CLK_ENABLE();
    ADC_CEL3_GPIO_CLK_ENABLE();
    ADC_CEL4_GPIO_CLK_ENABLE();
    ADC_IBAT_GPIO_CLK_ENABLE();
    ADC_IP1_GPIO_CLK_ENABLE();
    ADC_IP2_GPIO_CLK_ENABLE();
    ADC_IP3_GPIO_CLK_ENABLE();
    ADC_IMOT1_GPIO_CLK_ENABLE();
    ADC_IMOT2_GPIO_CLK_ENABLE();

    /* Common configuration for all Analog channels */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;

    GPIO_InitStructure.GPIO_Pin = ADC_CEL1_PIN;
    GPIO_Init(ADC_CEL1_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = ADC_CEL2_PIN;
    GPIO_Init(ADC_CEL2_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = ADC_CEL3_PIN;
    GPIO_Init(ADC_CEL3_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = ADC_CEL4_PIN;
    GPIO_Init(ADC_CEL4_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = ADC_IBAT_PIN;
    GPIO_Init(ADC_IBAT_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = ADC_IP1_PIN;
    GPIO_Init(ADC_IP1_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = ADC_IP2_PIN;
    GPIO_Init(ADC_IP2_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = ADC_IP3_PIN;
    GPIO_Init(ADC_IP3_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = ADC_IMOT1_PIN;
    GPIO_Init(ADC_IMOT1_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = ADC_IMOT2_PIN;
    GPIO_Init(ADC_IMOT2_GPIO_PORT, &GPIO_InitStructure);

    /* IMPORTANT: populate default values before use */
    ADC_CommonStructInit(&ADC_CommonInitStruct);
    ADC_StructInit(&ADC_InitStructure);

    /* PCLK2 is the APB2 clock (96 MHz)
     * ADCCLK = PCLK2/8 = 96/8 = 12 MHz */
    ADC_CommonInitStruct.ADC_Prescaler          = ADC_Prescaler_Div8;
    ADC_CommonInitStruct.ADC_Mode               = ADC_Mode_Independent;
    ADC_CommonInitStruct.ADC_DMAAccessMode      = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStruct.ADC_TwoSamplingDelay   = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStruct);

    /* Actual ADC Configuration */
    ADC_InitStructure.ADC_Resolution            = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode          = DISABLE;//DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode    = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv      = ADC_Software_Start;//// //
    ADC_InitStructure.ADC_ExternalTrigConvEdge  = ADC_ExternalTrigConvEdge_None;//// //
    ADC_InitStructure.ADC_DataAlign             = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion       = 1;//MONn;
    ADC_Init(MON_ADC, &ADC_InitStructure);

    /* Enable Temperature and Internal VREF channels
     * VBAT Conversion cannot be activated at the same time (channel is shared)
     */
    ADC_TempSensorVrefintCmd(ENABLE);

    /* Switch ADC ON */
    ADC_Cmd(MON_ADC, ENABLE);

}

uint16_t bb_mon_read_channel(uint8_t channel)
{
    /* Configure channel to acquire */
    ADC_RegularChannelConfig(MON_ADC, channel, 1, ADC_SampleTime_480Cycles);

    /* Start conversion and wait for completion */
    ADC_SoftwareStartConv(MON_ADC);
    while(ADC_GetFlagStatus(MON_ADC, ADC_FLAG_EOC) == RESET);

    /* Get converted value */
    return ADC_GetConversionValue(MON_ADC);
}

uint32_t bb_mon_convert_raw_value_to_mv(const uint16_t rawValue)
{
    return (((uint32_t) rawValue) * 1000) / ADC_STEPS_PER_VOLT;
}

int32_t bb_mon_convert_temp_value_to_degree(const uint32_t vsense_mv)
{
    return 25L + ((((int32_t) vsense_mv) - ADC_TEMPERATURE_V25_MV) * 1000 / ADC_TEMPERATURE_AVG_SLOPE_UV_PER_C);
}


