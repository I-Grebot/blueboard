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
**  Main Program
**
********************************************************************************
*/

int main( void )
{
  // BlueBoard Initialization
  bb_init();

  // Serial and shell are started first, so that errors can be displayed
  serial_init();

  if(shell_start() != pdPASS)
  {
    LEDR_WRITE(LEDx_ON); // Indicate error
    while(true);
  }

  // Main sequencer start
  sequencer_start();

  // Start FreeRTOS Scheduler
  vTaskStartScheduler();

  // Infinite loop
  while(true);

  // Should never reach this line!
  return 0;
}
