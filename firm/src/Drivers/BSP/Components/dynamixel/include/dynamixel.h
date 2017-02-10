/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       dynamixel.h
 * @author     Bebop35
 * @date       Feb 09, 2017
 * -----------------------------------------------------------------------------
 * @brief
 *   General Header for Dynamixel's Digital Servos
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#ifndef __DYNAMIXEL_H_
#define __DYNAMIXEL_H_

#include <stdint.h>
#include <stdlib.h>

 /**
 ********************************************************************************
 **
 **  Types & Enumerations
 **
 ********************************************************************************
 */

/* Communication Protocols */
typedef enum {
    DXL_V1,
    DXL_V2
} dxl_protocol_e;

/* Registers Control Tables */
typedef enum {
    DXL_REG1,   /* Legacy table with V1 Communication (e.g. AX-12) */
    DXL_REG2,   /* Table with PID controls */
    DXL_REG3,   /* Table with PID controls and Torque Control */
    DXL_REG4    /* Table with Control Mode / PID; introduced with V2 Communication */
} dxl_reg_table_e;

/* Area */
typedef enum {
    DXL_EEPROM,  /* Non-Volatile storage */
    DXL_RAM      /* Cleared when powered-off */
} dxl_reg_area_e;

/* Access type */
typedef enum {
    DXL_RW,  /* Read / Write */
    DXL_R    /* Read Only */
} dxl_reg_access_e;

/* Register definition */
typedef struct {
    dxl_reg_area_e area;
    uint8_t address;
    const char* name;
    dxl_reg_access_e access;
    size_t size;            /* Size in Bytes */
    uint16_t value;         /* Registers are either 1 or 2 bytes; big-endian*/
} dxl_register_t;

/* Servo Models */
typedef struct {
    uint16_t model_id;
    const char* name;
    dxl_protocol_e protocol;
    dxl_reg_table_e reg_table;
} dxl_servo_model_t;


#endif /* __DYNAMIXEL_H_ */
