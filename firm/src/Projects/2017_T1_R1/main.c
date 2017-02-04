/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2017
 * -----------------------------------------------------------------------------
 * @file       main.c
 * @author     Paul
 * @date       Jan 23, 2017
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   Main module of IgreBot's 2017 Robot #1 Firmware
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

/* General Header */
#include "main.h"

/**
********************************************************************************
**
**  Global Variables
**
********************************************************************************
*/



/**
********************************************************************************
**
**  Main Program
**
********************************************************************************
*/

int main( void )
{
    /* BlueBoard Initializations */
    bb_init();

    /* Application modules Initialization */
    serial_init();
    dsv_init();

    /* Apply the Power-Up sequence*/
    bb_power_up();

    /* Start software tasks */

    //OS_CreateDebugTask();
    OS_SHL_Start();
    //OS_CreateLedTask();
    OS_CreateMotionTask();
    OS_CreateAvoidanceTask();
    OS_CreateASVTask();
    OS_CreateDSVTask();

    /*OS_CreateStrategyTask();

    */

    /* Start FreeRTOS Scheduler */
    vTaskStartScheduler();


    /* Infinite loop */
    for( ;; );

    /* Should never reach this line! */
    return 0;
}
