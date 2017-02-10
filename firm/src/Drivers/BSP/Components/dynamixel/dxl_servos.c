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

        /* Model   Name       Com. Protocol   Reg. Table*/
        {   12,    "AX-12"   , DXL_V1,        DXL_REG1}, /* AX-12 / AX-12+ / AX-12A */
        {  300,    "AX-12W"  , DXL_V1,        DXL_REG1},
        {   18,    "AX-18"   , DXL_V1,        DXL_REG1}, /* AX-18F / AX-12A */
        {   10,    "RX-10"   , DXL_V1,        DXL_REG1},
        {   24,    "RX-24"   , DXL_V1,        DXL_REG1}, /* RX-24F */
        {   28,    "RX-28"   , DXL_V1,        DXL_REG1},
        {   64,    "RX-64"   , DXL_V1,        DXL_REG1},
        {  113,    "DX-113"  , DXL_V1,        DXL_REG1},
        {  116,    "DX-116"  , DXL_V1,        DXL_REG1},
        {  117,    "DX-117"  , DXL_V1,        DXL_REG1},
        {  107,    "EX-106"  , DXL_V1,        DXL_REG1}, /* EX-106+ */
        {  360,    "MX-12"   , DXL_V1,        DXL_REG2}, /* MX-12W */
        {   29,    "MX-28"   , DXL_V1,        DXL_REG2}, /* MX-28T / MX-28R / MX-28AT / MX-28AR */
        {  310,    "MX-64"   , DXL_V1,        DXL_REG3}, /* MX-64T / MX-64R / MX-64AT / MX-64AR */
        {  320,    "MX-106"  , DXL_V1,        DXL_REG3}, /* MX-106T / MX-106R */
        {  350,    "XL-320"  , DXL_V2,        DXL_REG4},

};
