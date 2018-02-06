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

#include "../../2018_T1_R1/include/main.h"

// Globals
TaskHandle_t handle_task_beacons;

// Local and static functions
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

void beacons_write_reg(uint8_t add, int16_t data)
{
  uint8_t wr_data[3];
  uint8_t rd_data[3];

  wr_data[0] = (add & 0x7F); // Command
  wr_data[1] = data >> 8;
  wr_data[2] = data & 0xFF;

  memset(&rd_data, 0, sizeof(rd_data));

  bb_hmi_tx_rx(3, wr_data, rd_data);

}

int16_t beacons_read_reg(uint8_t add)
{
  uint8_t wr_data[3];
  uint8_t rd_data[3];

  memset(&wr_data, 0, sizeof(wr_data));
  wr_data[0] = 0x80 | (add & 0x7F); // Command

  bb_hmi_tx_rx(3, wr_data, rd_data);

  return (int16_t) (rd_data[1] << 8U) + rd_data[2];

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

  uint16_t i = 0;

  // Remove compiler warnings
  (void) pvParameters;

  for( ;; )
  {

    //bb_hmi_tx_rx(i++);
    beacons_write_reg(0x55, i++);
    beacons_read_reg(0);
    beacons_read_reg(12);
    //vTaskDelayUntil( &next_wake_time, pdMS_TO_TICKS(OS_BEACONS_PERIOD_MS));
    vTaskDelayUntil( &next_wake_time, pdMS_TO_TICKS(100));
  }


}



