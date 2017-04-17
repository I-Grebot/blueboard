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

/*
 * Control Register Table
 * TODO: V5
 */

const dxl_register_t dxl_registers[] = {

/* -------------------------------------------------------------------------------------------*/
/*   Impl.    Area         Address       Name (no spaces!)            Access      Size  Index */
/* -------------------------------------------------------------------------------------------*/
/* -- REGISTER TABLE COMMON V1/V2/V3/V4 -- */
    {0x0F,    DXL_EEPROM,  0x0000,       "model_number"             , DXL_R,        2}  //  1
   ,{0x0F,    DXL_EEPROM,  0x0002,       "firmware_version"         , DXL_R,        1}  //  2
   ,{0x0F,    DXL_EEPROM,  0x0003,       "id"                       , DXL_RW,       1}  //  3
   ,{0x0F,    DXL_EEPROM,  0x0004,       "baudrate"                 , DXL_RW,       1}  //  4
   ,{0x0F,    DXL_EEPROM,  0x0005,       "return_delay_time"        , DXL_RW,       1}  //  5
   ,{0x0F,    DXL_EEPROM,  0x0006,       "cw_angle_limit"           , DXL_RW,       2}  //  6
   ,{0x0F,    DXL_EEPROM,  0x0008,       "ccw_angle_limit"          , DXL_RW,       2}  //  7
/* -- REGISTER TABLE COMMON V1/V2/V3 -- */
   ,{0x07,    DXL_EEPROM,  0x000B,       "temperature_limit"        , DXL_RW,       1}  //  8
   ,{0x07,    DXL_EEPROM,  0x000C,       "low_voltage_limit"        , DXL_RW,       1}  //  9
   ,{0x07,    DXL_EEPROM,  0x000D,       "high_voltage_limit"       , DXL_RW,       1}  // 10
   ,{0x07,    DXL_EEPROM,  0x000E,       "max_torque"               , DXL_RW,       2}  // 11
   ,{0x07,    DXL_EEPROM,  0x0010,       "status_return_level"      , DXL_RW,       1}  // 12
   ,{0x07,    DXL_EEPROM,  0x0011,       "alarm_led"                , DXL_RW,       1}  // 13
   ,{0x07,    DXL_EEPROM,  0x0012,       "alarm_shutdown"           , DXL_RW,       1}  // 14
/* -- REGISTER TABLE V2/V3 ONLY --*/
   ,{0x06,    DXL_EEPROM,  0x0014,       "multi-turn_offset"        , DXL_RW,       2}  // 15
   ,{0x06,    DXL_EEPROM,  0x0016,       "resolution_divider"       , DXL_RW,       1}  // 16
/* -- REGISTER TABLE V4 ONLY --*/
   ,{0x08,    DXL_EEPROM,  0x0011,       "control_mode"             , DXL_RW,       1}  // 17
   ,{0x08,    DXL_EEPROM,  0x0012,       "temperature_limit"        , DXL_RW,       1}  // 18
   ,{0x08,    DXL_EEPROM,  0x0013,       "low_voltage_limit"        , DXL_RW,       1}  // 19
   ,{0x08,    DXL_EEPROM,  0x0014,       "high_voltage_limit"       , DXL_RW,       1}  // 20
   ,{0x08,    DXL_EEPROM,  0x0015,       "max_torque"               , DXL_RW,       2}  // 21
   ,{0x08,    DXL_EEPROM,  0x0017,       "status_return_level"      , DXL_RW,       1}  // 22
   ,{0x08,    DXL_EEPROM,  0x0018,       "alarm_shutdown"           , DXL_RW,       1}  // 23
/* -- REGISTER TABLE V1 ONLY --*/
   ,{0x01,    DXL_RAM,     0x001A,       "cw_compliance_margin"     , DXL_RW,       1}  // 24
   ,{0x01,    DXL_RAM,     0x001B,       "ccw_compliance_margin"    , DXL_RW,       1}  // 25
   ,{0x01,    DXL_RAM,     0x001C,       "cw_compliance_slope"      , DXL_RW,       1}  // 26
   ,{0x01,    DXL_RAM,     0x001D,       "ccw_compliance_slope"     , DXL_RW,       1}  // 27
/* -- REGISTER TABLE V2/V3 ONLY --*/
   ,{0x06,    DXL_RAM,     0x001A,       "d_gain"                   , DXL_RW,       1}  // 28
   ,{0x06,    DXL_RAM,     0x001B,       "i_gain"                   , DXL_RW,       1}  // 29
   ,{0x06,    DXL_RAM,     0x001C,       "p_gain"                   , DXL_RW,       1}  // 30
/* -- REGISTER TABLE COMMON V1/V2/V3 -- */
   ,{0x07,    DXL_RAM,     0x0018,       "torque_enable"            , DXL_RW,       1}  // 31
   ,{0x07,    DXL_RAM,     0x0019,       "led"                      , DXL_RW,       1}  // 32
   ,{0x07,    DXL_RAM,     0x001E,       "goal_position"            , DXL_RW,       2}  // 33
   ,{0x07,    DXL_RAM,     0x0020,       "moving_speed"             , DXL_RW,       2}  // 34
   ,{0x07,    DXL_RAM,     0x0022,       "torque_limit"             , DXL_RW,       2}  // 35
   ,{0x07,    DXL_RAM,     0x0024,       "present_position"         , DXL_R,        2}  // 36
   ,{0x07,    DXL_RAM,     0x0026,       "present_velocity"         , DXL_R,        2}  // 37
   ,{0x07,    DXL_RAM,     0x0028,       "present_load"             , DXL_R,        2}  // 38
   ,{0x07,    DXL_RAM,     0x002A,       "present_voltage"          , DXL_R,        1}  // 39
   ,{0x07,    DXL_RAM,     0x002B,       "present_temperature"      , DXL_R,        1}  // 40
   ,{0x07,    DXL_RAM,     0x002C,       "registered"               , DXL_R,        1}  // 41
   ,{0x07,    DXL_RAM,     0x002E,       "moving"                   , DXL_R,        1}  // 42
   ,{0x07,    DXL_RAM,     0x002F,       "lock"                     , DXL_RW,       1}  // 43
   ,{0x07,    DXL_RAM,     0x0030,       "punch"                    , DXL_RW,       2}  // 44
/* -- REGISTER TABLE V3 ONLY --*/
   ,{0x04,    DXL_RAM,     0x0044,       "current"                  , DXL_RW,       2}  // 45
   ,{0x04,    DXL_RAM,     0x0046,       "torque_control_enable"    , DXL_RW,       1}  // 46
   ,{0x04,    DXL_RAM,     0x0047,       "goal_torque"              , DXL_RW,       2}  // 47
   ,{0x04,    DXL_RAM,     0x0049,       "goal_acceleration"        , DXL_RW,       1}  // 48
/* -- REGISTER TABLE V4 ONLY --*/
   ,{0x08,    DXL_RAM,     0x0024,       "torque_enable"            , DXL_RW,       1}  // 49
   ,{0x08,    DXL_RAM,     0x0025,       "led"                      , DXL_RW,       1}  // 50
   ,{0x08,    DXL_RAM,     0x0027,       "d_gain"                   , DXL_RW,       1}  // 51
   ,{0x08,    DXL_RAM,     0x0028,       "i_gain"                   , DXL_RW,       1}  // 52
   ,{0x08,    DXL_RAM,     0x0029,       "p_gain"                   , DXL_RW,       1}  // 53
   ,{0x08,    DXL_RAM,     0x0030,       "goal_position"            , DXL_RW,       2}  // 54
   ,{0x08,    DXL_RAM,     0x0032,       "goal_velocity"            , DXL_RW,       2}  // 55
   ,{0x08,    DXL_RAM,     0x0035,       "goal_torque"              , DXL_RW,       2}  // 56
   ,{0x08,    DXL_RAM,     0x0037,       "present position"         , DXL_R,        2}  // 57
   ,{0x08,    DXL_RAM,     0x0039,       "present_velocity"         , DXL_R,        2}  // 58
   ,{0x08,    DXL_RAM,     0x0041,       "present_load"             , DXL_R,        2}  // 59
   ,{0x08,    DXL_RAM,     0x0045,       "present_voltage"          , DXL_R,        1}  // 60
   ,{0x08,    DXL_RAM,     0x0046,       "present_temperature"      , DXL_R,        1}  // 61
   ,{0x08,    DXL_RAM,     0x0047,       "registered"               , DXL_R,        1}  // 62
   ,{0x08,    DXL_RAM,     0x0049,       "moving"                   , DXL_R,        1}  // 63
   ,{0x08,    DXL_RAM,     0x0050,       "hardware_error_status"    , DXL_RW,       1}  // 64
   ,{0x08,    DXL_RAM,     0x0051,       "punch"                    , DXL_RW,       2}  // 65
/* ------------------------------------------------------------------------------------------------*/
};

const size_t dxl_registers_length = sizeof(dxl_registers) / sizeof(dxl_register_t);

/* Named version for functions handlers */
const dxl_register_t* dxl_reg_model_number             = dxl_registers +  0;
const dxl_register_t* dxl_reg_firmware_version         = dxl_registers +  1;
const dxl_register_t* dxl_reg_id                       = dxl_registers +  2;
const dxl_register_t* dxl_reg_baud_rate                = dxl_registers +  3;
const dxl_register_t* dxl_reg_return_delay_time        = dxl_registers +  4;
const dxl_register_t* dxl_reg_cw_angle_limit           = dxl_registers +  5;
const dxl_register_t* dxl_reg_ccw_angle_limit          = dxl_registers +  6;


const dxl_register_t* dxl_reg_v1_torque_enable         = dxl_registers +  31;
const dxl_register_t* dxl_reg_v4_torque_enable         = dxl_registers +  49;

/*
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
*/

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
