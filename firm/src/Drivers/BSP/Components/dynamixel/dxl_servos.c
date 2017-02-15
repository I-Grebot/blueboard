/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       dxl_servos.c
 * @author     Bebop35
 * @date       Feb 09, 2017
 * -----------------------------------------------------------------------------
 * @brief
 *   Implements Dynamixel Servo Specific functions
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "dynamixel.h"


const dxl_servo_model_t dxl_servo_models[] = {

        /* ID      Model          Com. Protocol     Reg. Table  */
        // AX Series
        {   12,    "AX-12"          , DXL_V1,        DXL_REG1}, /* AX-12 / AX-12+ / AX-12A */
        {  300,    "AX-12W"         , DXL_V1,        DXL_REG1},
        {   18,    "AX-18"          , DXL_V1,        DXL_REG1}, /* AX-18F / AX-18A */
        // EX Series
        {  107,    "EX-106"         , DXL_V1,        DXL_REG1}, /* EX-106+ */
        // RX Series
        {   10,    "RX-10"          , DXL_V1,        DXL_REG1},
        {   24,    "RX-24"          , DXL_V1,        DXL_REG1}, /* RX-24F */
        {   28,    "RX-28"          , DXL_V1,        DXL_REG1},
        {   64,    "RX-64"          , DXL_V1,        DXL_REG1},
        // DX Series
        {  113,    "DX-113"         , DXL_V1,        DXL_REG1},
        {  116,    "DX-116"         , DXL_V1,        DXL_REG1},
        {  117,    "DX-117"         , DXL_V1,        DXL_REG1},
        // MX Series
        {  360,    "MX-12"          , DXL_V1,        DXL_REG2}, /* MX-12W */
        {   29,    "MX-28"          , DXL_V1,        DXL_REG2}, /* MX-28T / MX-28R / MX-28AT / MX-28AR */
        {  310,    "MX-64"          , DXL_V1,        DXL_REG3}, /* MX-64T / MX-64R / MX-64AT / MX-64AR */
        {  320,    "MX-106"         , DXL_V1,        DXL_REG3}, /* MX-106T / MX-106R */
        // XL Series
        {  350,    "XL320"          , DXL_V2,        DXL_REG4},
        // XH Series
        { 1000,    "XH430-W350"     , DXL_V2,        DXL_REG5}, /* XH430-W350-T / XH430-W350-R */
        { 1010,    "XH430-W210"     , DXL_V2,        DXL_REG5}, /* XH430-W210-T / XH430-W210-R */
        // XM Series
        { 1020,    "XM430-W350"     , DXL_V2,        DXL_REG5}, /* XM430-W350-T / XM430-W350-R */
        { 1030,    "XM430-W210"     , DXL_V2,        DXL_REG5}  /* XM430-W210-T / XM430-W210-R */
};
