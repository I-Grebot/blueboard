/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       hw_power.c
 * @author     Paul
 * @date       Jan 3, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   This module handles the PTN78060 power enables
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev:: 1431                                                                 $
 * $LastChangedBy:: paul.m                                                     $
 * $LastChangedDate:: 2016-01-19 22:06:16 +0100 (mar., 19 janv. 2016)          $
 * -----------------------------------------------------------------------------
 */

/* Inclusions */
#include "blueboard.h"

/* Private variables */
static const GPIO_TypeDef* HW_PWR_PORT[PWRn] = {VP1_EN_GPIO_PORT,
                                                VP2_EN_GPIO_PORT,
                                                VP3_EN_GPIO_PORT};

static const uint16_t HW_PWR_PIN[PWRn] = {VP1_EN_PIN,
                                          VP2_EN_PIN,
                                          VP3_EN_PIN};

/**
  * @brief  Configure all VPn power GPIOs (push-pull, no pull-up)
  * @param  None
  * @retval None
  */
void HW_PWR_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    VP1_EN_GPIO_CLK_ENABLE();
    VP2_EN_GPIO_CLK_ENABLE();
    VP3_EN_GPIO_CLK_ENABLE();

    /* Common configuration for all VPn */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed;

    GPIO_InitStructure.GPIO_Pin = VP1_EN_PIN;
    GPIO_Init(VP1_EN_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = VP2_EN_PIN;
    GPIO_Init(VP2_EN_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = VP3_EN_PIN;
    GPIO_Init(VP3_EN_GPIO_PORT, &GPIO_InitStructure);

    /* Default state: Modules OFF */
    HW_PWR_Disable(HW_PWR_VP1);
    HW_PWR_Disable(HW_PWR_VP2);
    HW_PWR_Disable(HW_PWR_VP3);

}

/**
  * @brief  Enable a VPx power-supply
  * @param  power VPx channel to enable
  * @retval None
  */
void HW_PWR_Enable(HW_PWR_TypeDef power)
{
    GPIO_WriteBit((GPIO_TypeDef*) HW_PWR_PORT[power], HW_PWR_PIN[power], PWRx_ON);
}

/**
  * @brief  Disable a VPx power-supply
  * @param  power VPx channel to disable
  * @retval None
  */
void HW_PWR_Disable(HW_PWR_TypeDef power)
{
    GPIO_WriteBit((GPIO_TypeDef*) HW_PWR_PORT[power], HW_PWR_PIN[power], PWRx_OFF);
}
