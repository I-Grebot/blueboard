/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       bb_io_mapping.h
 * @author     Paul
 * @date       Jan 3, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   This header describes the I/O mapping of the BlueBoard BSP.
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#ifndef __BB_IO_MAPPING_H
#define __BB_IO_MAPPING_H

/**
********************************************************************************
**
**  RGB LED
**    3x Digital Outputs
**
********************************************************************************
*/

/** @addtogroup BB_LOW_LEVEL_RGB_LED
 * @{
 */

/* Number of instances */
#define LEDn                            ((uint8_t) 3)

/* MCU I/O connected on the LED anodes */
#define LEDx_ON                         (Bit_RESET)
#define LEDx_OFF                        (Bit_SET)

/* LEDR Mapped on PA8 */
#define LEDR_GPIO_PORT                   GPIOA
#define LEDR_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)
#define LEDR_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE)
#define LEDR_PIN                         GPIO_Pin_8
#define LEDR_WRITE(_v)                   GPIO_WriteBit(LEDR_GPIO_PORT, LEDR_PIN, (_v))
#define LEDR_TOGGLE()                    GPIO_ToggleBits(LEDR_GPIO_PORT, LEDR_PIN)

/* LEDG Mapped on PD2 */
#define LEDG_GPIO_PORT                   GPIOD
#define LEDG_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)
#define LEDG_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, DISABLE)
#define LEDG_PIN                         GPIO_Pin_2
#define LEDG_WRITE(_v)                   GPIO_WriteBit(LEDG_GPIO_PORT, LEDG_PIN, (_v))
#define LEDG_TOGGLE()                    GPIO_ToggleBits(LEDG_GPIO_PORT, LEDG_PIN)

/* LEDB Mapped on PD7 */
#define LEDB_GPIO_PORT                   GPIOD
#define LEDB_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)
#define LEDB_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, DISABLE)
#define LEDB_PIN                         GPIO_Pin_7
#define LEDB_WRITE(_v)                   GPIO_WriteBit(LEDB_GPIO_PORT, LEDB_PIN, (_v))
#define LEDB_TOGGLE()                    GPIO_ToggleBits(LEDB_GPIO_PORT, LEDB_PIN)

/**
 * @}
 */

/**
********************************************************************************
**
**  Input Switches
**    3x Digital Inputs
**
********************************************************************************
*/

/** @addtogroup BB_LOW_LEVEL_SWITCHES
 * @{
 */

/* SW1 Mapped on PC14 */
#define SW1_GPIO_PORT				GPIOC
#define SW1_GPIO_CLK_ENABLE()       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define SW1_GPIO_CLK_DISABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE)
#define SW1_PIN                     GPIO_Pin_14
#define SW1_VALUE                 	GPIO_ReadInputDataBit(SW1_GPIO_PORT, SW1_PIN)

/* SW2 Mapped on PC15 */
#define SW2_GPIO_PORT				GPIOC
#define SW2_GPIO_CLK_ENABLE()       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define SW2_GPIO_CLK_DISABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE)
#define SW2_PIN                     GPIO_Pin_15
#define SW2_VALUE                 	GPIO_ReadInputDataBit(SW2_GPIO_PORT, SW2_PIN)

/* SW3 Mapped on PE1 */
#define SW3_GPIO_PORT				GPIOE
#define SW3_GPIO_CLK_ENABLE()       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
#define SW3_GPIO_CLK_DISABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE)
#define SW3_PIN                     GPIO_Pin_1
#define SW3_VALUE                 	GPIO_ReadInputDataBit(SW3_GPIO_PORT, SW3_PIN)

/**
 * @}
 */

/**
********************************************************************************
**
**  Power-Supplies Enables of the PTN78060
**    3x Digital Outputs for VP1 / VP2 / VP3 enables
**
********************************************************************************
*/

/** @addtogroup BB_LOW_LEVEL_POWER_ENABLES
 * @{
 */

/* Number of instances */
#define PWRn                               ((uint8_t) 3)

/* MCU I/O must be tight to VDD to switch the module OFF */
#define PWRx_ON                            (Bit_RESET)
#define PWRx_OFF                           (Bit_SET)

/* VP1_EN Mapped on PB8 */
#define VP1_EN_GPIO_PORT                   GPIOB
#define VP1_EN_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define VP1_EN_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, DISABLE)
#define VP1_EN_PIN                         GPIO_Pin_8

/* VP2_EN Mapped on PB9 */
#define VP2_EN_GPIO_PORT                   GPIOB
#define VP2_EN_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define VP2_EN_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, DISABLE)
#define VP2_EN_PIN                         GPIO_Pin_9

/* VP3_EN Mapped on PE0 */
#define VP3_EN_GPIO_PORT                   GPIOE
#define VP3_EN_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
#define VP3_EN_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE)
#define VP3_EN_PIN                         GPIO_Pin_0

/**
 * @}
 */

/**
********************************************************************************
**
**  Human-Machine Interface (HMI)
**    1x SPI Master Interface (4 wires)
**    1x Digital Input (slave to master interrupt)
**    1x Digital Output (master to slave interrupt)
**
********************************************************************************
*/

/** @addtogroup BB_LOW_LEVEL_HMI
 * @{
 */

/* Definitions for HMI SPI connected on SPI4 */
#define HMI_COM                             SPI4
#define HMI_CLK_ENABLE()                    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE)
#define HMI_CLK_DISABLE()                   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, DISABLE)
#define HMI_IRQn                            SPI4_IRQn

/* HMI_SCK Mapped on PE2 */
#define HMI_SCK_GPIO_PORT                   GPIOE
#define HMI_SCK_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
#define HMI_SCK_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE)
#define HMI_SCK_PIN                         GPIO_Pin_2
#define HMI_SCK_AF                          GPIO_AF5_SPI4
#define HMI_SCK_PIN_SOURCE                  GPIO_PinSource2

/* HMI_CSN Mapped on PE4 */
#define HMI_CSN_GPIO_PORT                   GPIOE
#define HMI_CSN_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
#define HMI_CSN_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE)
#define HMI_CSN_PIN                         GPIO_Pin_4
#define HMI_CSN_AF                          GPIO_AF5_SPI4
#define HMI_CSN_PIN_SOURCE                  GPIO_PinSource4
#define HMI_CSN_WRITE(v)                    GPIO_WriteBit(HMI_CSN_GPIO_PORT, HMI_CSN_PIN, (v))

/* HMI_MISO Mapped on PE5 */
#define HMI_MISO_GPIO_PORT                  GPIOE
#define HMI_MISO_GPIO_CLK_ENABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
#define HMI_MISO_GPIO_CLK_DISABLE()         RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE)
#define HMI_MISO_PIN                        GPIO_Pin_5
#define HMI_MISO_AF                         GPIO_AF5_SPI4
#define HMI_MISO_PIN_SOURCE                 GPIO_PinSource5

/* HMI_MOSI Mapped on PE6 */
#define HMI_MOSI_GPIO_PORT                  GPIOE
#define HMI_MOSI_GPIO_CLK_ENABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
#define HMI_MOSI_GPIO_CLK_DISABLE()         RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE)
#define HMI_MOSI_PIN                        GPIO_Pin_6
#define HMI_MOSI_AF                         GPIO_AF5_SPI4
#define HMI_MOSI_PIN_SOURCE                 GPIO_PinSource6

/* HMI_IRQ_IN Mapped on PE3 */
#define HMI_IRQ_IN_GPIO_PORT                GPIOE
#define HMI_IRQ_IN_GPIO_CLK_ENABLE()        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
#define HMI_IRQ_IN_GPIO_CLK_DISABLE()       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE)
#define HMI_IRQ_IN_PIN                      GPIO_Pin_3

/* HMI_IRQ_OUT Mapped on PC13 */
#define HMI_IRQ_OUT_GPIO_PORT               GPIOC
#define HMI_IRQ_OUT_GPIO_CLK_ENABLE()       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define HMI_IRQ_OUT_GPIO_CLK_DISABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE)
#define HMI_IRQ_OUT_PIN                     GPIO_Pin_13

/**
 * @}
 */

/**
********************************************************************************
**
**  End-Stops Sensors
**    8x Digital Inputs
**
********************************************************************************
*/

/** @addtogroup BB_LOW_LEVEL_ENDSTOP_SENSORS
 * @{
 */

/* END1 Mapped on PC0 */
#define END1_GPIO_PORT				      GPIOC
#define END1_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define END1_GPIO_CLK_DISABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE)
#define END1_PIN                    GPIO_Pin_0
#define END1_VALUE                 	GPIO_ReadInputDataBit(END1_GPIO_PORT, END1_PIN)

/* END2 Mapped on PC1 */
#define END2_GPIO_PORT				      GPIOC
#define END2_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define END2_GPIO_CLK_DISABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE)
#define END2_PIN                    GPIO_Pin_1
#define END2_VALUE                 	GPIO_ReadInputDataBit(END2_GPIO_PORT, END2_PIN)

/* END3 Mapped on PB2 */
#define END3_GPIO_PORT				      GPIOB
#define END3_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define END3_GPIO_CLK_DISABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, DISABLE)
#define END3_PIN                    GPIO_Pin_2
#define END3_VALUE                 	GPIO_ReadInputDataBit(END3_GPIO_PORT, END3_PIN)

/* END4 Mapped on PB11 */
#define END4_GPIO_PORT				      GPIOB
#define END4_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define END4_GPIO_CLK_DISABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, DISABLE)
#define END4_PIN                    GPIO_Pin_11
#define END4_VALUE                 	GPIO_ReadInputDataBit(END4_GPIO_PORT, END4_PIN)

/* END5 Mapped on PA15 */
#define END5_GPIO_PORT				      GPIOA
#define END5_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)
#define END5_GPIO_CLK_DISABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE)
#define END5_PIN                    GPIO_Pin_15
#define END5_VALUE                 	GPIO_ReadInputDataBit(END5_GPIO_PORT, END5_PIN)

/* END6 Mapped on PC10 */
#define END6_GPIO_PORT				      GPIOC
#define END6_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define END6_GPIO_CLK_DISABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE)
#define END6_PIN                    GPIO_Pin_10
#define END6_VALUE                 	GPIO_ReadInputDataBit(END6_GPIO_PORT, END6_PIN)

/* END7 Mapped on PC11 */
#define END7_GPIO_PORT				      GPIOC
#define END7_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define END7_GPIO_CLK_DISABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE)
#define END7_PIN                    GPIO_Pin_11
#define END7_VALUE                 	GPIO_ReadInputDataBit(END7_GPIO_PORT, END7_PIN)
#define END7_WRITE(v)               GPIO_WriteBit(END7_GPIO_PORT, END7_PIN, (v))

/* END8 Mapped on PC12 */
#define END8_GPIO_PORT				      GPIOC
#define END8_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define END8_GPIO_CLK_DISABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE)
#define END8_PIN                    GPIO_Pin_12
#define END8_VALUE                 	GPIO_ReadInputDataBit(END8_GPIO_PORT, END8_PIN)
#define END8_WRITE(v)               GPIO_WriteBit(END8_GPIO_PORT, END8_PIN, (v))

/**
 * @}
 */

/**
********************************************************************************
**
**  Industrial Sensors
**    10x Digital Inputs
**
********************************************************************************
*/

/** @addtogroup BB_LOW_LEVEL_INDUSTRIAL_SENSORS
 * @{
 */

/* IND1 Mapped on PD15 */
#define IND1_GPIO_PORT				GPIOD
#define IND1_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)
#define IND1_GPIO_CLK_DISABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, DISABLE)
#define IND1_PIN                    GPIO_Pin_15
#define IND1_VALUE                 	(bool)!GPIO_ReadInputDataBit(IND1_GPIO_PORT, IND1_PIN)

/* IND2 Mapped on PD14 */
#define IND2_GPIO_PORT				GPIOD
#define IND2_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)
#define IND2_GPIO_CLK_DISABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, DISABLE)
#define IND2_PIN                    GPIO_Pin_14
#define IND2_VALUE                 	(bool)!GPIO_ReadInputDataBit(IND2_GPIO_PORT, IND2_PIN)

/* IND3 Mapped on PD13 */
#define IND3_GPIO_PORT				GPIOD
#define IND3_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)
#define IND3_GPIO_CLK_DISABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, DISABLE)
#define IND3_PIN                    GPIO_Pin_13
#define IND3_VALUE                 	(bool)!GPIO_ReadInputDataBit(IND3_GPIO_PORT, IND3_PIN)

/* IND4 Mapped on PD12 */
#define IND4_GPIO_PORT				GPIOD
#define IND4_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)
#define IND4_GPIO_CLK_DISABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, DISABLE)
#define IND4_PIN                    GPIO_Pin_12
#define IND4_VALUE                 	(bool)!GPIO_ReadInputDataBit(IND4_GPIO_PORT, IND4_PIN)

/* IND5 Mapped on PD11 */
#define IND5_GPIO_PORT				GPIOD
#define IND5_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)
#define IND5_GPIO_CLK_DISABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, DISABLE)
#define IND5_PIN                    GPIO_Pin_11
#define IND5_VALUE                 	(bool)!GPIO_ReadInputDataBit(IND5_GPIO_PORT, IND5_PIN)

/* IND6 Mapped on PD10 */
#define IND6_GPIO_PORT				GPIOD
#define IND6_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)
#define IND6_GPIO_CLK_DISABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, DISABLE)
#define IND6_PIN                    GPIO_Pin_10
#define IND6_VALUE                 	(bool)!GPIO_ReadInputDataBit(IND6_GPIO_PORT, IND6_PIN)

/* IND7 Mapped on PD9 */
#define IND7_GPIO_PORT				GPIOD
#define IND7_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)
#define IND7_GPIO_CLK_DISABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, DISABLE)
#define IND7_PIN                    GPIO_Pin_9
#define IND7_VALUE                 	(bool)!GPIO_ReadInputDataBit(IND7_GPIO_PORT, IND7_PIN)

/* IND8 Mapped on PD8 */
#define IND8_GPIO_PORT				GPIOD
#define IND8_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)
#define IND8_GPIO_CLK_DISABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, DISABLE)
#define IND8_PIN                    GPIO_Pin_8
#define IND8_VALUE                 	(bool)!GPIO_ReadInputDataBit(IND8_GPIO_PORT, IND8_PIN)

/* IND9 Mapped on PB13 */
#define IND9_GPIO_PORT				GPIOB
#define IND9_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define IND9_GPIO_CLK_DISABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, DISABLE)
#define IND9_PIN                    GPIO_Pin_13
#define IND9_VALUE                 	(bool)!GPIO_ReadInputDataBit(IND9_GPIO_PORT, IND9_PIN)

/* IND10 Mapped on PB10 */
#define IND10_GPIO_PORT				GPIOB
#define IND10_GPIO_CLK_ENABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define IND10_GPIO_CLK_DISABLE()    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, DISABLE)
#define IND10_PIN                   GPIO_Pin_10
#define IND10_VALUE                 (bool)!GPIO_ReadInputDataBit(IND10_GPIO_PORT, IND10_PIN)

/**
 * @}
 */

/**
********************************************************************************
**
**  Quadrature Encoder Sensors
**    4x Digital Inputs for A and B phasis of ENC1 and ENC2
**
********************************************************************************
*/

/** @addtogroup BB_LOW_LEVEL_QUAD_ENCODER_SENSORS
 * @{
 */

/* Encoder 1 definitions */
#define ENC1_TIM                        TIM3
#define ENC1_TIM_CLK_ENABLE()           RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE)
#define ENC1_TIM_CLK_DISABLE()          RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE)

/* Encoder 2 definitions */
#define ENC2_TIM                        TIM4
#define ENC2_TIM_CLK_ENABLE()           RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE)
#define ENC2_TIM_CLK_DISABLE()          RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE)

/* ENC1_A Mapped on PB4 (TIM3_CH1) */
#define ENC1_A_GPIO_PORT                GPIOB
#define ENC1_A_GPIO_CLK_ENABLE()        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define ENC1_A_GPIO_CLK_DISABLE()       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, DISABLE)
#define ENC1_A_PIN                      GPIO_Pin_4
#define ENC1_A_AF                       GPIO_AF2_TIM3
#define ENC1_A_PIN_SOURCE               GPIO_PinSource4

/* ENC1_B Mapped on PB5 (TIM3_CH2) */
#define ENC1_B_GPIO_PORT                GPIOB
#define ENC1_B_GPIO_CLK_ENABLE()        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define ENC1_B_GPIO_CLK_DISABLE()       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, DISABLE)
#define ENC1_B_PIN                      GPIO_Pin_5
#define ENC1_B_AF                       GPIO_AF2_TIM3
#define ENC1_B_PIN_SOURCE               GPIO_PinSource5

/* ENC2_A Mapped on PB6 (TIM4_CH1) */
#define ENC2_A_GPIO_PORT                GPIOB
#define ENC2_A_GPIO_CLK_ENABLE()        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define ENC2_A_GPIO_CLK_DISABLE()       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, DISABLE)
#define ENC2_A_PIN                      GPIO_Pin_6
#define ENC2_A_AF                       GPIO_AF2_TIM4
#define ENC2_A_PIN_SOURCE               GPIO_PinSource6

/* ENC2_B Mapped on PB7 (TIM4_CH2) */
#define ENC2_B_GPIO_PORT                GPIOB
#define ENC2_B_GPIO_CLK_ENABLE()        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define ENC2_B_GPIO_CLK_DISABLE()       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, DISABLE)
#define ENC2_B_PIN                      GPIO_Pin_7
#define ENC2_B_AF                       GPIO_AF2_TIM4
#define ENC2_B_PIN_SOURCE               GPIO_PinSource7

/**
 * @}
 */

/**
********************************************************************************
**
**  Main Motors Full H-Bridges (DRV8843)
**    2x Digital Output for Sleep mode and Reset control (MOT_SLEEP/MOT_RESET)
**    2x Digital Outputs for Chopping current adjustment (MOT_I1/MOT_I0)
**    4x Digital Outputs (PWMs) for bridge control (MOT1/2_DIR and MOT1/2_PWM)
**    1x Digital Intput for fault feedback (MOT_FAULT)
**
********************************************************************************
*/

/** @addtogroup BB_LOW_LEVEL_MAIN_MOTORS
 * @{
 */

/* General definitions */
#define MOT_TIM                               TIM1
#define MOT_TIM_CLK_ENABLE()                  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE)
#define MOT_TIM_CLK_DISABLE()                 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, DISABLE)

/* Sleep, Reset and Fault are active low */
#define MOT_SLEEP_ON                          (Bit_RESET)
#define MOT_SLEEP_OFF                         (Bit_SET)
#define MOT_RESET_ON                          (Bit_RESET)
#define MOT_RESET_OFF                         (Bit_SET)
#define MOT_STATUS_FAULT                      (Bit_RESET)
#define MOT_STATUS_NORMAL                     (Bit_SET)

/* MOT_SLEEP Mapped on PE15 */
#define MOT_SLEEP_GPIO_PORT                   GPIOE
#define MOT_SLEEP_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
#define MOT_SLEEP_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE)
#define MOT_SLEEP_PIN                         GPIO_Pin_15
#define MOT_SLEEP_WRITE(_v)                   GPIO_WriteBit(MOT_SLEEP_GPIO_PORT, MOT_SLEEP_PIN, (_v))

/* MOT_RESET Mapped on PE12 */
#define MOT_RESET_GPIO_PORT                   GPIOE
#define MOT_RESET_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
#define MOT_RESET_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE)
#define MOT_RESET_PIN                         GPIO_Pin_12
#define MOT_RESET_WRITE(_v)                   GPIO_WriteBit(MOT_RESET_GPIO_PORT, MOT_RESET_PIN, (_v))

/* MOT_I1 Mapped on PE7 */
#define MOT_I1_GPIO_PORT                      GPIOE
#define MOT_I1_GPIO_CLK_ENABLE()              RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
#define MOT_I1_GPIO_CLK_DISABLE()             RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE)
#define MOT_I1_PIN                            GPIO_Pin_7
#define MOT_I1_WRITE(_v)                      GPIO_WriteBit(MOT_I1_GPIO_PORT, MOT_I1_PIN, (_v))

/* MOT_I0 Mapped on PE8 */
#define MOT_I0_GPIO_PORT                      GPIOE
#define MOT_I0_GPIO_CLK_ENABLE()              RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
#define MOT_I0_GPIO_CLK_DISABLE()             RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE)
#define MOT_I0_PIN                            GPIO_Pin_8
#define MOT_I0_WRITE(_v)                      GPIO_WriteBit(MOT_I0_GPIO_PORT, MOT_I0_PIN, (_v))

/* MOT1_PWM (MOT1_IN1) Mapped on PE13 (TIM1_CH3) */
#define MOT1_IN1_GPIO_PORT                   GPIOE
#define MOT1_IN1_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
#define MOT1_IN1_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE)
#define MOT1_IN1_PIN                         GPIO_Pin_13
#define MOT1_IN1_AF                          GPIO_AF1_TIM1
#define MOT1_IN1_PIN_SOURCE                  GPIO_PinSource13
#define MOT1_IN1_SET_PULSE(_v)               TIM_SetCompare3(MOT_TIM, (_v))

/* MOT1_DIR (MOT1_IN2) Mapped on PE14 (TIM1_CH4) */
#define MOT1_IN2_GPIO_PORT                   GPIOE
#define MOT1_IN2_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
#define MOT1_IN2_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE)
#define MOT1_IN2_PIN                         GPIO_Pin_14
#define MOT1_IN2_AF                          GPIO_AF1_TIM1
#define MOT1_IN2_PIN_SOURCE                  GPIO_PinSource14
#define MOT1_IN2_SET_PULSE(_v)               TIM_SetCompare4(MOT_TIM, (_v))

/* MOT2_PWM (MOT2_IN1) Mapped on PE11 (TIM1_CH2) */
#define MOT2_IN1_GPIO_PORT                   GPIOE
#define MOT2_IN1_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
#define MOT2_IN1_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE)
#define MOT2_IN1_PIN                         GPIO_Pin_11
#define MOT2_IN1_AF                          GPIO_AF1_TIM1
#define MOT2_IN1_PIN_SOURCE                  GPIO_PinSource11
#define MOT2_IN1_SET_PULSE(_v)               TIM_SetCompare2(MOT_TIM, (_v))

/* MOT2_DIR (MOT2_IN2) Mapped on PE9 (TIM1_CH1) */
#define MOT2_IN2_GPIO_PORT                   GPIOE
#define MOT2_IN2_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
#define MOT2_IN2_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE)
#define MOT2_IN2_PIN                         GPIO_Pin_9
#define MOT2_IN2_AF                          GPIO_AF1_TIM1
#define MOT2_IN2_PIN_SOURCE                  GPIO_PinSource9
#define MOT2_IN2_SET_PULSE(_v)               TIM_SetCompare1(MOT_TIM, (_v))

/* MOT_FAULT Mapped on PE10 */
#define MOT_FAULT_GPIO_PORT                  GPIOE
#define MOT_FAULT_GPIO_CLK_ENABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE)
#define MOT_FAULT_GPIO_CLK_DISABLE()         RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE)
#define MOT_FAULT_PIN                        GPIO_Pin_10

/**
 * @}
 */

/**
********************************************************************************
**
**  Analog Servomotors
**    8x Digital Outputs (PWMs)
**
********************************************************************************
*/

/** @addtogroup BB_LOW_LEVEL_ANALOG_SERVOS
 * @{
 */

/* Number of instances */
#define ASVn                            ((uint8_t) 8)

/* General definitions
 * ASV1 to ASV4 Mapped on TIM8
 * ASV5 to ASV8 Mapped on TIM5
 */
#define ASV_1_TO_4_TIM                      TIM8
#define ASV_5_TO_8_TIM                      TIM5
#define ASV_1_TO_4_TIM_CLK_ENABLE()         RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE)
#define ASV_1_TO_4_TIM_CLK_DISABLE()        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, DISABLE)
#define ASV_5_TO_8_TIM_CLK_ENABLE()         RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE)
#define ASV_5_TO_8_TIM_CLK_DISABLE()        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, DISABLE)

/* ASV1_PWM Mapped on PC9 (TIM8_CH4) */
#define ASV1_PWM_GPIO_PORT                   GPIOC
#define ASV1_PWM_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define ASV1_PWM_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE)
#define ASV1_PWM_PIN                         GPIO_Pin_9
#define ASV1_PWM_AF                          GPIO_AF3_TIM8
#define ASV1_PWM_PIN_SOURCE                  GPIO_PinSource9
#define ASV1_PWM_SET_PULSE(_v)               TIM_SetCompare4(ASV_1_TO_4_TIM, (_v))

/* ASV2_PWM Mapped on PC8 (TIM8_CH3) */
#define ASV2_PWM_GPIO_PORT                   GPIOC
#define ASV2_PWM_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define ASV2_PWM_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE)
#define ASV2_PWM_PIN                         GPIO_Pin_8
#define ASV2_PWM_AF                          GPIO_AF3_TIM8
#define ASV2_PWM_PIN_SOURCE                  GPIO_PinSource8
#define ASV2_PWM_SET_PULSE(_v)               TIM_SetCompare3(ASV_1_TO_4_TIM, (_v))

/* ASV3_PWM Mapped on PC7 (TIM8_CH2) */
#define ASV3_PWM_GPIO_PORT                   GPIOC
#define ASV3_PWM_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define ASV3_PWM_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE)
#define ASV3_PWM_PIN                         GPIO_Pin_7
#define ASV3_PWM_AF                          GPIO_AF3_TIM8
#define ASV3_PWM_PIN_SOURCE                  GPIO_PinSource7
#define ASV3_PWM_SET_PULSE(_v)               TIM_SetCompare2(ASV_1_TO_4_TIM, (_v))

/* ASV4_PWM Mapped on PC6 (TIM8_CH1) */
#define ASV4_PWM_GPIO_PORT                   GPIOC
#define ASV4_PWM_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define ASV4_PWM_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE)
#define ASV4_PWM_PIN                         GPIO_Pin_6
#define ASV4_PWM_AF                          GPIO_AF3_TIM8
#define ASV4_PWM_PIN_SOURCE                  GPIO_PinSource6
#define ASV4_PWM_SET_PULSE(_v)               TIM_SetCompare1(ASV_1_TO_4_TIM, (_v))

/* ASV5_PWM Mapped on PA0 (TIM5_CH1) */
#define ASV5_PWM_GPIO_PORT                   GPIOA
#define ASV5_PWM_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)
#define ASV5_PWM_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE)
#define ASV5_PWM_PIN                         GPIO_Pin_0
#define ASV5_PWM_AF                          GPIO_AF2_TIM5
#define ASV5_PWM_PIN_SOURCE                  GPIO_PinSource0
#define ASV5_PWM_SET_PULSE(_v)               TIM_SetCompare1(ASV_5_TO_8_TIM, (_v))

/* ASV6_PWM Mapped on PA1 (TIM5_CH2) */
#define ASV6_PWM_GPIO_PORT                   GPIOA
#define ASV6_PWM_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)
#define ASV6_PWM_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE)
#define ASV6_PWM_PIN                         GPIO_Pin_1
#define ASV6_PWM_AF                          GPIO_AF2_TIM5
#define ASV6_PWM_PIN_SOURCE                  GPIO_PinSource1
#define ASV6_PWM_SET_PULSE(_v)               TIM_SetCompare2(ASV_5_TO_8_TIM, (_v))

/* ASV7_PWM Mapped on PA2 (TIM5_CH3) */
#define ASV7_PWM_GPIO_PORT                   GPIOA
#define ASV7_PWM_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)
#define ASV7_PWM_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE)
#define ASV7_PWM_PIN                         GPIO_Pin_2
#define ASV7_PWM_AF                          GPIO_AF2_TIM5
#define ASV7_PWM_PIN_SOURCE                  GPIO_PinSource2
#define ASV7_PWM_SET_PULSE(_v)               TIM_SetCompare3(ASV_5_TO_8_TIM, (_v))

/* ASV8_PWM Mapped on PA3 (TIM5_CH4) */
#define ASV8_PWM_GPIO_PORT                   GPIOA
#define ASV8_PWM_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)
#define ASV8_PWM_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE)
#define ASV8_PWM_PIN                         GPIO_Pin_3
#define ASV8_PWM_AF                          GPIO_AF2_TIM5
#define ASV8_PWM_PIN_SOURCE                  GPIO_PinSource3
#define ASV8_PWM_SET_PULSE(_v)               TIM_SetCompare4(ASV_5_TO_8_TIM, (_v))


/**
 * @}
 */

/**
********************************************************************************
**
**  Digital Servomotors Main channel or channel 1
**    1x Bi-directionnal UART
**
********************************************************************************
*/

/** @addtogroup BB_LOW_LEVEL_DSV_CHAN1
 * @{
 */

/* Definitions for DSV USART connected on USART3 */
#define DSV_COM                         USART3
#define DSV_CLK_ENABLE()                RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE)
#define DSV_CLK_DISABLE()               RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, DISABLE)
#define DSV_IRQn                        USART3_IRQn
#define DSV_ISR                         USART3_IRQHandler

/* DSV_TXRX Mapped on PB10 */
#define DSV_TXRX_GPIO_PORT              GPIOB
#define DSV_TXRX_PIN                    GPIO_Pin_10
#define DSV_TXRX_GPIO_CLK_ENABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define DSV_TXRX_GPIO_CLK_DISABLE()     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, DISABLE)
#define DSV_TXRX_AF                     GPIO_AF7_USART3
#define DSV_TXRX_PIN_SOURCE             GPIO_PinSource10

/**
 * @}
 */

/**
********************************************************************************
**
**  CAN Interface
**    2x Digital Communication Signals (CAN_RX / CAN_TX)
**
********************************************************************************
*/

/** @addtogroup BB_LOW_LEVEL_CAN
 * @{
 */

/* CAN_RX Mapped on PD0 */
/* CAN_TX Mapped on PD1 */

/**
 * @}
 */

/**
********************************************************************************
**
**  RS485 Interface
**    2x Digital Communication Signals (RS485_RX / RS485_TX)
**    2x Digital Outputs for transceiver / receiver enables
**    Could also be used as a 2nd DSV channel
**
********************************************************************************
*/

/** @addtogroup BB_LOW_LEVEL_RS485
 * @{
 */

/* Definitions for RS485 USART connected on USART2 */
#define RS485_COM                         USART2
#define RS485_CLK_ENABLE()                RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE)
#define RS485_CLK_DISABLE()               RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE)
#define RS485_IRQn                        USART2_IRQn
#define RS485_ISR                         USART2_IRQHandler

/* RS485_RX Mapped on PD6 */
#define RS485_RX_GPIO_PORT                GPIOD
#define RS485_RX_PIN                      GPIO_Pin_6
#define RS485_RX_GPIO_CLK_ENABLE()        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)
#define RS485_RX_GPIO_CLK_DISABLE()       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, DISABLE)
#define RS485_RX_AF                       GPIO_AF7_USART2
#define RS485_RX_PIN_SOURCE               GPIO_PinSource6

/* RS485_TX Mapped on PD5 */
#define RS485_TX_GPIO_PORT                GPIOD
#define RS485_TX_PIN                      GPIO_Pin_5
#define RS485_TX_GPIO_CLK_ENABLE()        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)
#define RS485_TX_GPIO_CLK_DISABLE()       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, DISABLE)
#define RS485_TX_AF                       GPIO_AF7_USART2
#define RS485_TX_PIN_SOURCE               GPIO_PinSource5

/* RS485_DE Mapped on PD4 */
#define RS485_DE_GPIO_PORT                GPIOD
#define RS485_DE_PIN                      GPIO_Pin_4
#define RS485_DE_GPIO_CLK_ENABLE()        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)
#define RS485_DE_GPIO_CLK_DISABLE()       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, DISABLE)
#define RS485_DE_WRITE(_v)                GPIO_WriteBit(RS485_DE_GPIO_PORT, RS485_DE_PIN, (_v))

/* RS485_NRE Mapped on PD3 */
#define RS485_NRE_GPIO_PORT               GPIOD
#define RS485_NRE_PIN                     GPIO_Pin_3
#define RS485_NRE_GPIO_CLK_ENABLE()       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE)
#define RS485_NRE_GPIO_CLK_DISABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, DISABLE)
#define RS485_NRE_WRITE(_v)               GPIO_WriteBit(RS485_NRE_GPIO_PORT, RS485_NRE_PIN, (_v))

/**
 * @}
 */

/**
********************************************************************************
**
**  Debug UART Interface [USART1]
**    2x Digital Communication Signals (UART_RX / UART_TX)
**    2x Optionnal Handshake Signals (UART_RTS / UART_CTS)
**
********************************************************************************
*/

/** @addtogroup BB_LOW_LEVEL_DEBUG_UART
 * @{
 */

/* Definitions for Debug USART connected on USART1 */
#define DBG_COM                         USART1
#define DBG_CLK_ENABLE()                RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE)
#define DBG_CLK_DISABLE()               RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, DISABLE)
#define DBG_IRQn                        USART1_IRQn
#define DBG_ISR                         USART1_IRQHandler

/* UART_RX Mapped on PA10 */
#define DBG_RX_GPIO_PORT                GPIOA
#define DBG_RX_PIN                      GPIO_Pin_10
#define DBG_RX_GPIO_CLK_ENABLE()        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)
#define DBG_RX_GPIO_CLK_DISABLE()       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE)
#define DBG_RX_AF                       GPIO_AF7_USART1
#define DBG_RX_PIN_SOURCE               GPIO_PinSource10

/* UART_TX Mapped on PA9 */
#define DBG_TX_GPIO_PORT                GPIOA
#define DBG_TX_PIN                      GPIO_Pin_9
#define DBG_TX_GPIO_CLK_ENABLE()        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)
#define DBG_TX_GPIO_CLK_DISABLE()       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE)
#define DBG_TX_AF                       GPIO_AF7_USART1
#define DBG_TX_PIN_SOURCE               GPIO_PinSource9

/* UART_RTS Mapped on PA12 */
#define DBG_RTS_GPIO_PORT               GPIOA
#define DBG_RTS_PIN                     GPIO_Pin_12
#define DBG_RTS_GPIO_CLK_ENABLE()       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)
#define DBG_RTS_GPIO_CLK_DISABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE)
#define DBG_RTS_AF                      GPIO_AF7_USART1
#define DBG_RTS_PIN_SOURCE              GPIO_PinSource12

/* UART_CTS Mapped on PA11 */
#define DBG_CTS_GPIO_PORT               GPIOA
#define DBG_CTS_PIN                     GPIO_Pin_11
#define DBG_CTS_GPIO_CLK_ENABLE()       RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)
#define DBG_CTS_GPIO_CLK_DISABLE()      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE)
#define DBG_CTS_AF                      GPIO_AF7_USART1
#define DBG_CTS_PIN_SOURCE              GPIO_PinSource11

/**
 * @}
 */

/**
********************************************************************************
**
**  USB OTG Interface
**    2x Digital Communication Signals (USB_DP / USB_DM)
**
********************************************************************************
*/

/** @addtogroup BB_LOW_LEVEL_USB
 * @{
 */

/* USB_DP Mapped on PB15 */
/* USB_DM Mapped on PB14 */

/**
 * @}
 */

/**
********************************************************************************
**
**  Analog Monitoring
**    4x Analog Inputs for battery cells voltage
**    1x Analog Inputs for total current consumption (IBAT)
**    3x Analog Inputs for power supplies current consumption (IP1/IP2/IP3)
**    2x Analog Inputs for main motors current consumption (IMOT1/IMOT2)
**
********************************************************************************
*/

/** @addtogroup BB_LOW_LEVEL_MONITORING
 * @{
 */

/* Number of channels to monitor */
#define MONn                                ((uint8_t) 10+2) /* 10 external channels + VTEMP + VREF */

/* All monitoring I/Os are mapped on ADC1 */
#define MON_ADC                             ADC1
#define MON_CLK_ENABLE()                    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE)
#define MON_CLK_DISABLE()                   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE)

/* ADC_CEL1 Mapped on PC4 (ADC1_IN14) */
#define ADC_CEL1_GPIO_PORT                  GPIOC
#define ADC_CEL1_GPIO_CLK_ENABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define ADC_CEL1_GPIO_CLK_DISABLE()         RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE)
#define ADC_CEL1_PIN                        GPIO_Pin_4
#define ADC_CEL1_CHANNEL                    ADC_Channel_14

/* ADC_CEL2 Mapped on PC5 (ADC1_IN15) */
#define ADC_CEL2_GPIO_PORT                  GPIOC
#define ADC_CEL2_GPIO_CLK_ENABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define ADC_CEL2_GPIO_CLK_DISABLE()         RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE)
#define ADC_CEL2_PIN                        GPIO_Pin_5
#define ADC_CEL2_CHANNEL                    ADC_Channel_15

/* ADC_CEL3 Mapped on PB0 (ADC1_IN8) */
#define ADC_CEL3_GPIO_PORT                  GPIOB
#define ADC_CEL3_GPIO_CLK_ENABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define ADC_CEL3_GPIO_CLK_DISABLE()         RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, DISABLE)
#define ADC_CEL3_PIN                        GPIO_Pin_0
#define ADC_CEL3_CHANNEL                    ADC_Channel_8

/* ADC_CEL4 Mapped on PB1 (ADC1_IN9) */
#define ADC_CEL4_GPIO_PORT                  GPIOB
#define ADC_CEL4_GPIO_CLK_ENABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE)
#define ADC_CEL4_GPIO_CLK_DISABLE()         RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, DISABLE)
#define ADC_CEL4_PIN                        GPIO_Pin_1
#define ADC_CEL4_CHANNEL                    ADC_Channel_9

/* ADC_IBAT Mapped on PC2 (ADC1_IN12) */
#define ADC_IBAT_GPIO_PORT                  GPIOC
#define ADC_IBAT_GPIO_CLK_ENABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define ADC_IBAT_GPIO_CLK_DISABLE()         RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE)
#define ADC_IBAT_PIN                        GPIO_Pin_2
#define ADC_IBAT_CHANNEL                    ADC_Channel_12

/* ADC_IP1 Mapped on PC3 (ADC1_IN13) */
#define ADC_IP1_GPIO_PORT                   GPIOC
#define ADC_IP1_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE)
#define ADC_IP1_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE)
#define ADC_IP1_PIN                         GPIO_Pin_3
#define ADC_IP1_CHANNEL                     ADC_Channel_13

/* ADC_IP2 Mapped on PA4 (ADC1_IN4) */
#define ADC_IP2_GPIO_PORT                   GPIOA
#define ADC_IP2_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)
#define ADC_IP2_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE)
#define ADC_IP2_PIN                         GPIO_Pin_4
#define ADC_IP2_CHANNEL                     ADC_Channel_4

/* ADC_IP3 Mapped on PA5 (ADC1_IN5) */
#define ADC_IP3_GPIO_PORT                   GPIOA
#define ADC_IP3_GPIO_CLK_ENABLE()           RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)
#define ADC_IP3_GPIO_CLK_DISABLE()          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE)
#define ADC_IP3_PIN                         GPIO_Pin_5
#define ADC_IP3_CHANNEL                     ADC_Channel_5

/* ADC_IMOT1 Mapped on PA6 (ADC1_IN6) */
#define ADC_IMOT1_GPIO_PORT                 GPIOA
#define ADC_IMOT1_GPIO_CLK_ENABLE()         RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)
#define ADC_IMOT1_GPIO_CLK_DISABLE()        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE)
#define ADC_IMOT1_PIN                       GPIO_Pin_6
#define ADC_IMOT1_CHANNEL                   ADC_Channel_6

/* ADC_IMOT2 Mapped on PA7 (ADC1_IN7) */
#define ADC_IMOT2_GPIO_PORT                 GPIOA
#define ADC_IMOT2_GPIO_CLK_ENABLE()         RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE)
#define ADC_IMOT2_GPIO_CLK_DISABLE()        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE)
#define ADC_IMOT2_PIN                       GPIO_Pin_7
#define ADC_IMOT2_CHANNEL                   ADC_Channel_7

/* Temperature, VREF and VBAT channels are hard-wired to dedicated channels */
#define ADC_TEMP_CHANNEL                    ADC_Channel_Vbat        /* Shared with VBAT */
#define ADC_VREF_CHANNEL                    ADC_Channel_Vrefint
#define ADC_VBAT_CHANNEL                    ADC_Channel_Vbat
/**
 * @}
 */

#endif /* __BB_IO_MAPPING_H */
