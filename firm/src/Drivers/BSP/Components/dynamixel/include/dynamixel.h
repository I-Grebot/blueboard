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
 **  Constants
 **
 ********************************************************************************
 */

/* Constants definition for the TX or RX mode of an interface */
#define DXL_RX_MODE 0U
#define DXL_TX_MODE 1U

/* Status constants */
#define DXL_STATUS_NO_ERROR             0x0000
#define DXL_STATUS_ERR_TIMEOUT          0x0100  // Returned packet timeout
#define DXL_STATUS_ERR_HEADER           0x0200  // Header error
#define DXL_STATUS_ERR_ID               0x0300  // Returned ID does not match
#define DXL_STATUS_ERR_LENGTH           0x0400  // Packet length error
#define DXL_STATUS_ERR_CHECKSUM         0x0500  // Packet checksum is wrong

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
    DXL_REG4,    /* Table with Control Mode / PID; introduced with V2 Communication */
    DXL_REG5    /* Table with full PID controls, introduced with XM servos */
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


/* Dynamixel Interface definition
 * Define hardware handlers (function pointers) as well as
 * other interface-specific constants
 */
typedef struct {

    // Interface protocol to be used
    dxl_protocol_e protocol;

    // Switch to TX/RX the half-duplex link
    void (* hw_switch)(uint8_t);

    // Send a byte and return error if not successful
    uint8_t (* hw_send_byte)(uint8_t);

    // Receive a byte and return error if not successful
    uint8_t (* hw_receive_byte) (uint8_t*);

    // Flush the receiver (used to make sure its clean before
    // starting to receive actual datas)
    void (* hw_flush) (void);

    // Return level that needs to be remembered to know if
    // something has to be expected.
    uint8_t return_level;

    // Return delay also has to be remembered to set an
    // appropriate timeout.
    uint8_t return_delay_ms;

    // Status that can contain various things but mostly errors
    uint16_t status;

    // Statistics counters
    uint32_t nb_pkt_tx; // Number of transmitted packets
    uint32_t nb_pkt_rx; // Number of received packets (without errors)
    uint32_t nb_errors; // Number of errors

} dxl_interface_t;

/**
********************************************************************************
**
**  Prototypes
**
********************************************************************************
*/

void dxl_init(dxl_interface_t* itf);

#endif /* __DYNAMIXEL_H_ */
