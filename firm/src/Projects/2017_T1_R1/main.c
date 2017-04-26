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
robot_t robot;

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
  memset(&robot, 0, sizeof(robot_t));

  /* BlueBoard Initializations */
  bb_init();

  /* Application modules Initialization */

  // Serial is started first to ensure correct print outs
  serial_init();

  dsv_init();

  /* Start software tasks */
  shell_start();

  motion_cs_start();
  motion_traj_start();
  avoidance_start();
  asv_start();
  //dsv_start();

  monitoring_start();
  led_start();

  sequencer_start();


  /* Start FreeRTOS Scheduler */
  vTaskStartScheduler();

  /* Infinite loop */
  for( ;; );

  /* Should never reach this line! */
  return 0;
}
