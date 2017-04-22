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

/* Main robot structure containing all operational variables */
RobotTypeDef robot;

/**
********************************************************************************
**
**  Main Program
**
********************************************************************************
*/

int main( void )
{
  /* Globals initialization */
  memset(&robot, 0, sizeof(RobotTypeDef));

  /* BlueBoard Initializations */
  bb_init();

  /* Application modules Initialization */

  // Serial is started first to ensure correct print outs
  serial_init();

  dsv_init();

  /* Apply the Power-Up sequence*/
  bb_power_up();

  /* Start software tasks */
  shell_start();

  motion_cs_start();
  motion_traj_start();
  avoidance_start();
  asv_start();
  //dsv_start();

  mon_start();
  led_start();

  strategy_start();


  /* Start FreeRTOS Scheduler */
  vTaskStartScheduler();


  /* Infinite loop */
  for( ;; );

  /* Should never reach this line! */
  return 0;
}
