/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       beacons.c
 * @author     Paul
 * @date       01/05/2017
 * -----------------------------------------------------------------------------
 * @brief
 *   Beacons V3 module
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "main.h"

// Globals
TaskHandle_t handle_task_beacons;

// Local and static functions
BaseType_t beacons_init(void);
static void beacons_task(void *pvParameters);

/**
********************************************************************************
**
**  Initializations
**
********************************************************************************
*/

// Main init for beacons
BaseType_t beacons_start(void)
{
  BaseType_t ret;

  ret = beacons_init();

  if(ret == pdPASS)
  {
    // Start beacons task
    ret = sys_create_task(beacons_task, "BEACONS", OS_TASK_STACK_BEACONS, NULL, OS_TASK_PRIORITY_BEACONS, &handle_task_beacons );

  } else {
    DEBUG_CRITICAL("Error while initializing Beacons module"DEBUG_EOL);
  }

  return ret;

}

// Beacons initialization
BaseType_t beacons_init(void)
{

  // Beacons interface is shared with HMI SPI
  // bb_hmi_init() is supposed to be already launched

}

/**
********************************************************************************
**
**  Beacons task
**
********************************************************************************
*/

static void beacons_task( void *pvParameters )
{
  TickType_t next_wake_time = xTaskGetTickCount();

  // Remove compiler warnings
  (void) pvParameters;

  for( ;; )
  {

    // TBC
    vTaskDelayUntil( &next_wake_time, OS_BEACONS_PERIOD_MS);
  }


}



