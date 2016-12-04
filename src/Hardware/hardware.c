/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       hardware.c
 * @author     Paul
 * @date       Jan 4, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   This is the top-level module of hardware functions.
 *
 *   Hardware peripherals resources usage:
 *      o TIM1                  for [MOT] Main Motors DIR and PWM channels 1 to 4
 *      o TIM3 / TIM4           for [QUA] Quadrature Encoders channels A (1) and B (2)
 *      o TIM5 / TIM8           for [ASV] Analog Servos PWM channels 1 to 8
 *      o SPI4                  for [HMI] Human Machine Interface
 *      o CAN1                  for [CAN] CAN bus Interface
 *      o USART1                for [DBG] Debug USART
 *      o USART2                for [RS4] RS485 bus Interface
 *      o USART3                for [DSV] Digital Servo bus Interface
 *      o ADC1 / TIM2 / DMA2    for [MON] Analog monitoring (Scan-mode with DMA, auto)
 *      o USB_OTG               for [USB] USB OTG High-Speed 2.0 Interface
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev: 1458 $
 * $LastChangedBy: Pierrick_Boissard $
 * $LastChangedDate: 2016-04-29 11:29:31 +0200 (ven., 29 avr. 2016) $
 * -----------------------------------------------------------------------------
 */

/* Inclusions */
#include "blueboard.h"

/* Local private hardware configuration handlers */
static USART_InitTypeDef Debug_Config, Dsv_Config;

/**
  * @brief  Configure all hardware peripheral structures that could be
  *         changed depending on application or usage.
  *         This function is private so it can only be called from this
  *         module.
  * @param  None
  * @retval None
  */
static void HW_ConfigAll(void)
{
   /*
    * Configure the Debug UART init structure:
    *   8 bits length + 1 stop bit, no parity
    *   Baudrate 115200 kbps
    */
    Debug_Config.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    Debug_Config.USART_BaudRate            = 115200;
    Debug_Config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    Debug_Config.USART_Parity              = USART_Parity_Even;
    Debug_Config.USART_StopBits            = USART_StopBits_1;
    Debug_Config.USART_WordLength          = USART_WordLength_9b;


    /*
     * Configure the Digital Servo UART init structure:
     *   8 bits length + 1 stop bit, no parity
     *   Baudrate 57600 kbps
     */
     Dsv_Config.USART_Mode                = USART_Mode_Tx;
     Dsv_Config.USART_BaudRate            = 57600;
     Dsv_Config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
     Dsv_Config.USART_Parity              = USART_Parity_No;
     Dsv_Config.USART_StopBits            = USART_StopBits_1;
     Dsv_Config.USART_WordLength          = USART_WordLength_8b;
}

/**
  * @brief  Initialize all hardware modules, also call the main
  *         HW_ConfigAll() function.
  * @param  None
  * @retval None
  */
void HW_InitAll(void)
{
    /* System Config */
    HW_CPU_CACHE_Enable();
    HW_SystemClock_Config();

    /* HW Modules Configurations */
    HW_ConfigAll();

    /* HW Modules Initializations */
    HW_PWR_Init();
    HW_LED_Init();
    HW_ENC_Init();
    HW_MOT_Init();
    HW_ASV_Init();
    HW_MON_Init();
    HW_HMI_Init();
    HW_Digital_Input_Init();

    HW_DBG_Init(&Debug_Config);
    HW_DSV_Init(&Dsv_Config);

    /* Set Interrupt group priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
}

/**
  * @brief  Apply the power-up sequence
  * @param  None
  * @retval None
  */
void HW_PowerUp(void)
{
    int i = 0;

    /* TEMPORARY */

    //MOT_SLEEP_WRITE(MOT_SLEEP_OFF);
    //MOT_RESET_WRITE(MOT_RESET_ON);

    /* Enable powers */
    for(i = 10000000; i; i--);
    HW_PWR_Enable(HW_PWR_VP1);
    for(i = 10000000; i; i--);
    HW_PWR_Enable(HW_PWR_VP2);
    for(i = 10000000; i; i--);
    HW_PWR_Enable(HW_PWR_VP3);
    for(i = 50000000; i; i--);

    /* Enable H-bridges power */
  //  MOT_RESET_WRITE(MOT_RESET_OFF);

}

/**
  * @brief  Apply the power-down sequence
  * @param  None
  * @retval None
  */
void HW_PowerDown(void)
{
    /* Disable H-bridges */
    MOT_SLEEP_WRITE(MOT_SLEEP_ON);
    MOT_RESET_WRITE(MOT_RESET_ON);

    /* Disable Power */
    HW_PWR_Disable(HW_PWR_VP1);
    HW_PWR_Disable(HW_PWR_VP2);
    HW_PWR_Disable(HW_PWR_VP3);
}

