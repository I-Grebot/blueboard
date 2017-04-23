/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       monitoring.h
 * @author     Paul
 * @date       Apr 22, 2016
 * -----------------------------------------------------------------------------
 * @brief
 *   Monitoring module definitions
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#ifndef _MONITORING_H
#define _MONITORING_H

#include <stdint.h>

/**
********************************************************************************
**
**  Enumeration & Types
**
********************************************************************************
*/

typedef struct
{

  // TODO: maybe add other parameters (VREF, etc.) -> can be used for calibration

  // Shunt resistors
  uint16_t shunt_ibat_mohm; // Value of the VBAT resistor shunt in milliohm
  uint16_t shunt_ip1_mohm;  // Value of the VP1 resistor shunt in milliohm
  uint16_t shunt_ip2_mohm;  // Value of the VP1 resistor shunt in milliohm
  uint16_t shunt_ip3_mohm;  // Value of the VP1 resistor shunt in milliohm

} mon_cfg_t;

typedef struct
{

  uint32_t ibat_ma; // Battery current in mA
  uint32_t ip1_ma;  // VP1 power-supply current in mA
  uint32_t ip2_ma;  // VP2 power-supply current in mA
  uint32_t ip3_ma;  // VP3 power-supply current in mA

  // Cells: TODO (hardware bug)

  // Motors: TODO

  int32_t temp; // Chip temperature in °C

} mon_values_t;

#endif /* _MONITORING_H */
