/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       monitoring.c
 * @author     Paul
 * @date       Apr 22, 2016
 * -----------------------------------------------------------------------------
 * @brief
 *   This module performs monitoring tasks (mostly ADC readings)
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

/* Inclusion */
#include "main.h"

/* Local, Private functions */
static void mon_task(void *pvParameters);

/* Main monitoring holders */
mon_cfg_t mon_config;
mon_values_t mon_values;

BaseType_t mon_start(void)
{
  BaseType_t ret;

  // Initialize hardware
  bb_mon_init();

  // Configuration settings
  mon_config.shunt_ibat_mohm = ADC_SHUNT_IBAT_MOHM;
  mon_config.shunt_ip1_mohm  = ADC_SHUNT_IP1_MOHM;
  mon_config.shunt_ip2_mohm  = ADC_SHUNT_IP2_MOHM;
  mon_config.shunt_ip3_mohm  = ADC_SHUNT_IP3_MOHM;

  // Create monitoring task
  ret = xTaskCreate(mon_task, "MONITORING", OS_TASK_STACK_MONITORING, NULL, OS_TASK_PRIORITY_MONITORING, NULL );

  if(ret != pdPASS)
  {
    DEBUG_CRITICAL("Could not start MONITORING task!"DEBUG_EOL);
  } else {
    DEBUG_INFO("Starting MONITORING task"DEBUG_EOL);
  }

  return ret;

}

static void mon_task( void *pvParameters )
{
  TickType_t xNextWakeTime;

  /* Initialize xNextWakeTime - this only needs to be done once. */
  xNextWakeTime = xTaskGetTickCount();

  /* Remove compiler warning about unused parameter. */
  ( void ) pvParameters;

  for( ;; )
  {

    // Read ADC channels and convert values
    mon_values.ibat_ma = mon_config.shunt_ibat_mohm * bb_mon_convert_raw_value_to_mv(bb_mon_read_channel(BB_MON_IBAT)) / 1000;
    mon_values.ip1_ma = mon_config.shunt_ip1_mohm * bb_mon_convert_raw_value_to_mv(bb_mon_read_channel(BB_MON_IP1))  / 1000;
    mon_values.ip2_ma = mon_config.shunt_ip2_mohm * bb_mon_convert_raw_value_to_mv(bb_mon_read_channel(BB_MON_IP2))  / 1000;
    mon_values.ip3_ma = mon_config.shunt_ip3_mohm * bb_mon_convert_raw_value_to_mv(bb_mon_read_channel(BB_MON_IP3))  / 1000;
    mon_values.temp = bb_mon_convert_temp_value_to_degree(bb_mon_convert_raw_value_to_mv(bb_mon_read_channel(BB_MON_VTEMP)));

    vTaskDelayUntil( &xNextWakeTime, pdMS_TO_TICKS(MONITORING_PERIOD_MS));

  } // infinite loop


}
