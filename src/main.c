/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       main.c
 * @author     Paul
 * @date       Jan 2, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   Main module of BlueBoard project
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev: 1469 $
 * $LastChangedBy: Pierrick_Boissard $
 * $LastChangedDate: 2016-05-05 09:40:31 +0200 (jeu., 05 mai 2016) $
 * -----------------------------------------------------------------------------
 */

/* General Header */
#include "blueboard.h"

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
    /* Configure the hardware */
    HW_InitAll();

    /* Apply the Power-Up sequence*/
    HW_PowerUp();

    /* Start software tasks */

    //OS_CreateDebugTask();
    OS_SHL_Start();
    OS_CreateLedTask();
    /*OS_CreateMotionTask();
    OS_CreateStrategyTask();
    OS_CreateAvoidanceTask();
    OS_CreateASVTask();*/
    //OS_CreateDSVTask();


    /* Start FreeRTOS Scheduler */
    vTaskStartScheduler();


    /* Infinite loop */
    for( ;; );

    /* Should never reach this line! */
    return 0;
}
