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
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>

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

/* Generic Fail / Pass error code status */
#define DXL_PASS    0U
#define DXL_FAIL    1U

/* Status constants */
#define DXL_STATUS_NO_ERROR             0x0000
#define DXL_STATUS_ERR_TIMEOUT          0x0100  // Returned packet timeout
#define DXL_STATUS_ERR_HEADER           0x0200  // Header error
#define DXL_STATUS_ERR_ID               0x0300  // Returned ID does not match
#define DXL_STATUS_ERR_LENGTH           0x0400  // Packet length error
#define DXL_STATUS_ERR_CHECKSUM         0x0500  // Packet checksum is wrong

/* LEDs color */
#define DXL_LED_OFF    0U
#define DXL_LED_RED    1U
#define DXL_LED_GREEN  2U
#define DXL_LED_BLUE   4U
#define DXL_LED_YELLOW  (DXL_LED_RED   | DXL_LED_GREEN)
#define DXL_LED_CYAN    (DXL_LED_GREEN | DXL_LED_BLUE)
#define DXL_LED_MAGENTA (DXL_LED_RED   | DXL_LED_BLUE)
#define DXL_LED_WHITE   (DXL_LED_RED   | DXL_LED_GREEN | DXL_LED_BLUE)

/* Motor Mode */
#define DXL_WHEEL_MODE   1U
#define DXL_JOIN_MODE    2U

/* Some IDs constants */
#define DXL_ID_BROADCAST      0xFE
#define DXL_ID_DEFAULT        0x00

/* Status answer level definition */
#define DXL_STATUS_NO_AWNSER   0x00 // Except for PING command
#define DXL_STATUS_READ_ONLY   0x01 // Only when a READ command is issued
#define DXL_STATUS_EVERYTHING  0x02

/* Defined when debug mode is required */
//#define DXL_DEBUG

#define DXL_DEBUG_EOL "\n\r" // End-Of-Line character for debug

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


/* Pin mode for half-duplex communication */
typedef enum {
    DXL_MODE_TX = 0U,
    DXL_MODE_RX = 1U
} dxl_switch_mode_e;

/* Dynamixel Interface definition
 * Define hardware handlers (function pointers) as well as
 * other interface-specific constants
 */
typedef struct {

    // Unique index of the interface
    uint8_t itf_idx;

    // Interface protocol to be used
    dxl_protocol_e protocol;

    // Switch to TX/RX the half-duplex link
    void (* hw_switch)(uint8_t chan_idx, dxl_switch_mode_e mode);

    // Send a byte and return error if not successful
    // An error is a non-zero code
    uint8_t (* hw_send_byte)(uint8_t chan_idx, uint8_t tx_data);

    // Receive a byte and return error if not successful
    // An error is a non-zero code
    uint8_t (* hw_receive_byte) (uint8_t chan_idx, const uint8_t* rx_data);

    // Flush the receiver (used to make sure its clean before
    // starting to receive actual datas)
    uint8_t (* hw_flush) (uint8_t chan_idx);

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

/* Dynamixel Servo control structure
 * Useful to handle dynamic values for each servo
 */
typedef struct {

    // Servo model. Contains protocol and register versions.
    dxl_servo_model_t* model;

    // Interface on which the servo is attached
    dxl_interface_t* itf;

    // Unique ID of the servo on the interface
    uint8_t id;

    // Boundaries
    uint16_t min_position;
    uint16_t max_position;

    // Temporary variables
    uint16_t current_position;

} dxl_servo_t;


/**
********************************************************************************
**
**  Prototypes
**
********************************************************************************
*/

// Servo Models
const dxl_servo_model_t* dxl_find_servo_model_by_name(const char* name);


void dxl_init_itf(dxl_interface_t* itf, uint8_t itf_idx);

void dxl_write(dxl_servo_t* servo, uint8_t addr, uint8_t* values, size_t size, bool reg);
void dxl_reset(dxl_servo_t* servo);
void dxl_ping(dxl_servo_t* servo);

void dxl_set_position(dxl_servo_t* servo, uint16_t new_position);

/* Debug */
#ifdef DXL_DEBUG

extern long serial_puts(const char* str);

void dxl_print_servo(dxl_servo_t* servo);


#endif // DXL_DEBUG

#endif /* __DYNAMIXEL_H_ */
