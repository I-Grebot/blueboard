/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       dxl_registers.c
 * @author     Bebop35
 * @date       Feb 09, 2017
 * -----------------------------------------------------------------------------
 * @brief
 *   Registers definition for the Dynamixel Servos (Protocol v1)
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "dynamixel.h"

/* Control Register Table for Versions 1, 2 and 3 */
dxl_register_t dxl_registers_v1[] = {

/* ---------------------------------------------------------------------------------------------*/
/*   Area         Address     Name                         Access       Size    Value (Default) */
/* ---------------------------------------------------------------------------------------------*/
    {DXL_EEPROM,  0x00,       "model_number"             , DXL_R,        2,       0}
   ,{DXL_EEPROM,  0x02,       "firmware_version"         , DXL_R,        2,       0}
   ,{DXL_EEPROM,  0x03,       "id"                       , DXL_RW,       1,       1}
   ,{DXL_EEPROM,  0x04,       "baudrate"                 , DXL_RW,       1,      34}
   ,{DXL_EEPROM,  0x05,       "return_delay_time"        , DXL_RW,       1,     250}
   ,{DXL_EEPROM,  0x06,       "cw_angle_limit"           , DXL_RW,       2,       0}
   ,{DXL_EEPROM,  0x08,       "ccw_angle_limit"          , DXL_RW,       2,    1023}
   ,{DXL_EEPROM,  0x0B,       "temperature_limit"        , DXL_RW,       1,      80}
   ,{DXL_EEPROM,  0x0C,       "low_voltage_limit"        , DXL_RW,       1,      60}
   ,{DXL_EEPROM,  0x0D,       "high_voltage_limit"       , DXL_RW,       1,     190}
   ,{DXL_EEPROM,  0x0E,       "max_torque"               , DXL_RW,       2,    1023}
   ,{DXL_EEPROM,  0x10,       "status_return_level"      , DXL_RW,       1,       2}
   ,{DXL_EEPROM,  0x11,       "alarm_led"                , DXL_RW,       1,      36}
   ,{DXL_EEPROM,  0x12,       "alarm_shutdown"           , DXL_RW,       1,      36}
/* -- REGISTER TABLE V2/V3 ONLY ----------------------------------------------------------------*/
   ,{DXL_EEPROM,  0x14,       "multi-turn_offset"        , DXL_RW,       2,       0}
   ,{DXL_EEPROM,  0x16,       "resolution_divider"       , DXL_RW,       1,       1}
/* ---------------------------------------------------------------------------------------------*/
   ,{DXL_RAM,     0x18,       "torque_enable"            , DXL_RW,       1,       0}
   ,{DXL_RAM,     0x19,       "led"                      , DXL_RW,       1,       0}
/* -- REGISTER TABLE V1 ONLY --------------------------------------------------------------------*/
   ,{DXL_RAM,     0x1A,       "cw_compliance_margin"     , DXL_RW,       1,       1}
   ,{DXL_RAM,     0x1B,       "ccw_compliance_margin"    , DXL_RW,       1,       0}
   ,{DXL_RAM,     0x1C,       "cw_compliance_slope"      , DXL_RW,       1,      32}
   ,{DXL_RAM,     0x1D,       "ccw_compliance_slope"     , DXL_RW,       1,      32}
/* -- REGISTER TABLE V2/V3 ONLY ----------------------------------------------------------------*/
   ,{DXL_RAM,     0x1A,       "d_gain"                   , DXL_RW,       1,       8}
   ,{DXL_RAM,     0x1B,       "i_gain"                   , DXL_RW,       1,       0}
   ,{DXL_RAM,     0x1C,       "p_gain"                   , DXL_RW,       1,       8}
/* ---------------------------------------------------------------------------------------------*/
   ,{DXL_RAM,     0x1E,       "goal_position"            , DXL_RW,       2,       0}
   ,{DXL_RAM,     0x20,       "moving_speed"             , DXL_RW,       2,       0}
   ,{DXL_RAM,     0x22,       "torque_limit"             , DXL_RW,       2,    1023}
   ,{DXL_RAM,     0x24,       "present_position"         , DXL_R,        2,       0}
   ,{DXL_RAM,     0x26,       "present_velocity"         , DXL_R,        2,       0}
   ,{DXL_RAM,     0x28,       "present_load"             , DXL_R,        2,       0}
   ,{DXL_RAM,     0x2A,       "present_voltage"          , DXL_R,        1,       0}
   ,{DXL_RAM,     0x2B,       "present_temperature"      , DXL_R,        1,       0}
   ,{DXL_RAM,     0x2C,       "registered"               , DXL_R,        1,       0}
   ,{DXL_RAM,     0x2E,       "moving"                   , DXL_R,        1,       0}
   ,{DXL_RAM,     0x2F,       "lock"                     , DXL_RW,       1,       0}
   ,{DXL_RAM,     0x30,       "punch"                    , DXL_RW,       2,      32}
/* -- REGISTER TABLE V3 ONLY -------------------------------------------------------------------*/
   ,{DXL_RAM,     0x44,       "current"                  , DXL_RW,       2,       0}
   ,{DXL_RAM,     0x46,       "torque_control_enable"    , DXL_RW,       1,       0}
   ,{DXL_RAM,     0x47,       "goal_torque"              , DXL_RW,       2,       0}
   ,{DXL_RAM,     0x49,       "goal_acceleration"        , DXL_RW,       1,       0}
/* ---------------------------------------------------------------------------------------------*/
};

const size_t dxl_registers_v1_length = sizeof(dxl_registers_v1) / sizeof(dxl_register_t);

/* Control Register Table for Version 4 */
dxl_register_t dxl_registers_v4[] = {

/* ---------------------------------------------------------------------------------------------*/
/*   Area         Address     Name                         Access       Size    Value (Default) */
/* ---------------------------------------------------------------------------------------------*/
    {DXL_EEPROM,  0x00,       "Model Number"             , DXL_R,        2,       0}
   ,{DXL_EEPROM,  0x02,       "Firmware Version"         , DXL_R,        2,       0}
   ,{DXL_EEPROM,  0x03,       "ID"                       , DXL_RW,       1,       1}
   ,{DXL_EEPROM,  0x04,       "Baud-Rate"                , DXL_RW,       1,      34}
   ,{DXL_EEPROM,  0x05,       "Return Delay Time"        , DXL_RW,       1,     250}
   ,{DXL_EEPROM,  0x06,       "CW Angle Limit"           , DXL_RW,       2,       0}
   ,{DXL_EEPROM,  0x08,       "CCW Angle Limit"          , DXL_RW,       2,    1023}
   ,{DXL_EEPROM,  0x11,       "Control Mode"             , DXL_RW,       1,       2}
   ,{DXL_EEPROM,  0x12,       "Temperature Limit"        , DXL_RW,       1,      65}
   ,{DXL_EEPROM,  0x13,       "Low Voltage Limit"        , DXL_RW,       1,      60}
   ,{DXL_EEPROM,  0x14,       "High Voltage Limit"       , DXL_RW,       1,      90}
   ,{DXL_EEPROM,  0x15,       "Max Torque"               , DXL_RW,       2,    1023}
   ,{DXL_EEPROM,  0x17,       "Status Return Level"      , DXL_RW,       1,       0}
   ,{DXL_EEPROM,  0x18,       "Alarm Shutdown"           , DXL_RW,       1,       0}
   ,{DXL_RAM,     0x24,       "Torque Enable"            , DXL_RW,       1,       0}
   ,{DXL_RAM,     0x25,       "LED"                      , DXL_RW,       1,       0}
   ,{DXL_RAM,     0x27,       "D Gain"                   , DXL_RW,       0,       8}
   ,{DXL_RAM,     0x28,       "I Gain"                   , DXL_RW,       0,       0}
   ,{DXL_RAM,     0x29,       "P Gain"                   , DXL_RW,      32,       8}
   ,{DXL_RAM,     0x30,       "Goal Position"            , DXL_RW,       2,       0}
   ,{DXL_RAM,     0x32,       "Goal Velocity"            , DXL_RW,       2,       0}
   ,{DXL_RAM,     0x35,       "Goal Torque"              , DXL_RW,       2,       0}
   ,{DXL_RAM,     0x37,       "Present Position"         , DXL_R,        2,       0}
   ,{DXL_RAM,     0x39,       "Present Velocity"         , DXL_R,        2,       0}
   ,{DXL_RAM,     0x41,       "Present Load"             , DXL_R,        2,       0}
   ,{DXL_RAM,     0x45,       "Present Voltage"          , DXL_R,        1,       0}
   ,{DXL_RAM,     0x46,       "Present Temperature"      , DXL_R,        1,       0}
   ,{DXL_RAM,     0x47,       "Registered"               , DXL_R,        1,       0}
   ,{DXL_RAM,     0x49,       "Moving"                   , DXL_R,        1,       0}
   ,{DXL_RAM,     0x50,       "Hardware Error Status"    , DXL_RW,       1,       0}
   ,{DXL_RAM,     0x51,       "Punch"                    , DXL_RW,       2,      32}
/* ---------------------------------------------------------------------------------------------*/
};
/* Control Register Table for Version 5 */
dxl_register_t dxl_registers_v5[] = {
        /* TODO */
};

/* Named version for functions handlers */
const dxl_register_t* dxl_reg_v1_model_number             = dxl_registers_v1 +  0;
const dxl_register_t* dxl_reg_v1_firmware_version         = dxl_registers_v1 +  1;
const dxl_register_t* dxl_reg_v1_id                       = dxl_registers_v1 +  2;
const dxl_register_t* dxl_reg_v1_baud_rate                = dxl_registers_v1 +  3;
const dxl_register_t* dxl_reg_v1_return_delay_time        = dxl_registers_v1 +  4;
const dxl_register_t* dxl_reg_v1_cw_angle_limit           = dxl_registers_v1 +  5;
const dxl_register_t* dxl_reg_v1_ccw_angle_limit          = dxl_registers_v1 +  6;
const dxl_register_t* dxl_reg_v1_temperature_limit        = dxl_registers_v1 +  7;
const dxl_register_t* dxl_reg_v1_low_voltage_limit        = dxl_registers_v1 +  8;
const dxl_register_t* dxl_reg_v1_high_voltage_limit       = dxl_registers_v1 +  9;
const dxl_register_t* dxl_reg_v1_max_torque               = dxl_registers_v1 + 10;
const dxl_register_t* dxl_reg_v1_status_return_level      = dxl_registers_v1 + 11;
const dxl_register_t* dxl_reg_v1_alarm_led                = dxl_registers_v1 + 12;
const dxl_register_t* dxl_reg_v1_alarm_shutdown           = dxl_registers_v1 + 13;
const dxl_register_t* dxl_reg_v1_multi_turn_offset        = dxl_registers_v1 + 14;
const dxl_register_t* dxl_reg_v1_resolution_divider       = dxl_registers_v1 + 15;
const dxl_register_t* dxl_reg_v1_torque_enable            = dxl_registers_v1 + 16;
const dxl_register_t* dxl_reg_v1_led                      = dxl_registers_v1 + 17;
const dxl_register_t* dxl_reg_v1_cw_compliance_margin     = dxl_registers_v1 + 18;
const dxl_register_t* dxl_reg_v1_ccw_compliance_margin    = dxl_registers_v1 + 19;
const dxl_register_t* dxl_reg_v1_cw_compliance_slope      = dxl_registers_v1 + 20;
const dxl_register_t* dxl_reg_v1_ccw_compliance_slope     = dxl_registers_v1 + 21;
const dxl_register_t* dxl_reg_v1_d_gain                   = dxl_registers_v1 + 22;
const dxl_register_t* dxl_reg_v1_i_gain                   = dxl_registers_v1 + 23;
const dxl_register_t* dxl_reg_v1_p_gain                   = dxl_registers_v1 + 24;
const dxl_register_t* dxl_reg_v1_goal_position            = dxl_registers_v1 + 25;
const dxl_register_t* dxl_reg_v1_moving_speed             = dxl_registers_v1 + 26;
const dxl_register_t* dxl_reg_v1_torque_limit             = dxl_registers_v1 + 27;
const dxl_register_t* dxl_reg_v1_present_position         = dxl_registers_v1 + 28;
const dxl_register_t* dxl_reg_v1_present_velocity         = dxl_registers_v1 + 29;
const dxl_register_t* dxl_reg_v1_present_load             = dxl_registers_v1 + 30;
const dxl_register_t* dxl_reg_v1_present_voltage          = dxl_registers_v1 + 31;
const dxl_register_t* dxl_reg_v1_present_temperature      = dxl_registers_v1 + 32;
const dxl_register_t* dxl_reg_v1_registered               = dxl_registers_v1 + 33;
const dxl_register_t* dxl_reg_v1_moving                   = dxl_registers_v1 + 34;
const dxl_register_t* dxl_reg_v1_lock                     = dxl_registers_v1 + 35;
const dxl_register_t* dxl_reg_v1_punch                    = dxl_registers_v1 + 36;
const dxl_register_t* dxl_reg_v1_current                  = dxl_registers_v1 + 37;
const dxl_register_t* dxl_reg_v1_torque_control_enable    = dxl_registers_v1 + 38;
const dxl_register_t* dxl_reg_v1_goal_torque              = dxl_registers_v1 + 39;
const dxl_register_t* dxl_reg_v1_goal_acceleration        = dxl_registers_v1 + 40;

const dxl_register_t* dxl_reg_v4_model_number             = dxl_registers_v4 +  0;
const dxl_register_t* dxl_reg_v4_firmware_version         = dxl_registers_v4 +  1;
const dxl_register_t* dxl_reg_v4_id                       = dxl_registers_v4 +  2;
const dxl_register_t* dxl_reg_v4_baud_rate                = dxl_registers_v4 +  3;
const dxl_register_t* dxl_reg_v4_return_delay_time        = dxl_registers_v4 +  4;
const dxl_register_t* dxl_reg_v4_cw_angle_limit           = dxl_registers_v4 +  5;
const dxl_register_t* dxl_reg_v4_ccw_angle_limit          = dxl_registers_v4 +  6;
const dxl_register_t* dxl_reg_v4_control_mode             = dxl_registers_v4 +  7;
const dxl_register_t* dxl_reg_v4_temperature_limit        = dxl_registers_v4 +  8;
const dxl_register_t* dxl_reg_v4_low_voltage_limit        = dxl_registers_v4 +  9;
const dxl_register_t* dxl_reg_v4_high_voltage_limit       = dxl_registers_v4 + 10;
const dxl_register_t* dxl_reg_v4_max_torque               = dxl_registers_v4 + 11;
const dxl_register_t* dxl_reg_v4_status_return_level      = dxl_registers_v4 + 12;
const dxl_register_t* dxl_reg_v4_alarm_shutdown           = dxl_registers_v4 + 13;
const dxl_register_t* dxl_reg_v4_torque_enable            = dxl_registers_v4 + 14;
const dxl_register_t* dxl_reg_v4_led                      = dxl_registers_v4 + 15;
const dxl_register_t* dxl_reg_v4_d_gain                   = dxl_registers_v4 + 16;
const dxl_register_t* dxl_reg_v4_i_gain                   = dxl_registers_v4 + 17;
const dxl_register_t* dxl_reg_v4_p_gain                   = dxl_registers_v4 + 18;
const dxl_register_t* dxl_reg_v4_goal_position            = dxl_registers_v4 + 19;
const dxl_register_t* dxl_reg_v4_goal_velocity            = dxl_registers_v4 + 20;
const dxl_register_t* dxl_reg_v4_goal_torque              = dxl_registers_v4 + 21;
const dxl_register_t* dxl_reg_v4_present_position         = dxl_registers_v4 + 22;
const dxl_register_t* dxl_reg_v4_present_velocity         = dxl_registers_v4 + 23;
const dxl_register_t* dxl_reg_v4_present_load             = dxl_registers_v4 + 24;
const dxl_register_t* dxl_reg_v4_present_voltage          = dxl_registers_v4 + 25;
const dxl_register_t* dxl_reg_v4_present_temperature      = dxl_registers_v4 + 26;
const dxl_register_t* dxl_reg_v4_registered               = dxl_registers_v4 + 27;
const dxl_register_t* dxl_reg_v4_moving                   = dxl_registers_v4 + 28;
const dxl_register_t* dxl_reg_v4_hardware_error_status    = dxl_registers_v4 + 29;
const dxl_register_t* dxl_reg_v4_punch                    = dxl_registers_v4 + 30;



/*
 * Converts an area type into a printable string
 */
const char* dxl_get_area_as_string(const dxl_reg_area_e area)
{
    switch(area)
    {
        case DXL_EEPROM: return "EEPROM";
        case DXL_RAM: 	 return "RAM";
        default:     	 return "NC"; // Error
    }
}

/*
 * Converts an access type into a printable string
 */
const char* dxl_get_access_as_string(const dxl_reg_access_e acc)
{
    switch(acc)
    {
        case DXL_RW: return "RW";
        case DXL_R:  return "R";
        default:     return "NC"; // Error
    }
}
