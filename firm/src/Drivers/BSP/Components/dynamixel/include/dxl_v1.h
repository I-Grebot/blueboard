/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       dxl_v1.c
 * @author     Bebop35
 * @date       Feb 09, 2017
 * -----------------------------------------------------------------------------
 * @brief
 *   Dynamixel's Digital Servo Protocol v1 framing description
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#ifndef ___DXL_PROTOCOL_V1_H_
#define ___DXL_PROTOCOL_V1_H_

#include "dynamixel.h"


/**
 ********************************************************************************
 **
 **  Framing Constants & Macros
 **
 ********************************************************************************
 */

/* Packet Header */
#define DXL_V1_HEADER            0xFF

/* Broadcast ID used to talk to all servos connected on the bus */
#define DXL_V1_ID_BROADCAST      0xFE

/* Minimum length of a packet */
#define DXL_V1_PACKET_MIN_LENGTH 2U

/* Calculate the length of a packet based on the number of parameters */
#define DXL_V1_MAKE_LENGTH(_n_param) ((_n_param) + DXL_V1_PACKET_MIN_LENGTH)

/* Calculate the baudrate register value based on the required BPS */
#define DXL_V1_MAKE_BAUDRATE(_br_bps) (2000000/((_br_bps)+1))

/* Maximum number of parameters in a packet */
#define DXL_V1_MAX_PARAMETERS   8U

/* Instructions identifiers */
#define DXL_V1_INS_PING          0x01 // Nb. Param: 0
#define DXL_V1_INS_READ          0x02 // Nb. Param: 2
#define DXL_V1_INS_WRITE         0x03 // Nb. Param: 2
#define DXL_V1_INS_REG_WRITE     0x04 // Nb. Param: 0
#define DXL_V1_INS_ACTION        0x05 // Nb. Param: 0
#define DXL_V1_INS_RESET         0x06 // Nb. Param: 0
#define DXL_V1_INS_SYNC_WRITE    0x83 // Nb. Param: N
#define DXL_V1_INS_BULK_READ     0x92 // Nb. Param: N

/**
 ********************************************************************************
 **
 **  Data content constants
 **
 ********************************************************************************
 */

/* Status answer level definition */
#define DXL_V1_STATUS_NO_AWNSER   0x00 // Except for PING command
#define DXL_V1_STATUS_READ_ONLY   0x01 // Only when a READ command is issued
#define DXL_V1_STATUS_EVERYTHING  0x02

/* Error flags definition */
#define DXL_V1_ERR_INPUT_VOLTAGE    1U
#define DXL_V1_ERR_ANGLE_LIMIT      2U
#define DXL_V1_ERR_OVERHEATING      4U
#define DXL_V1_ERR_RANGE            8U
#define DXL_V1_ERR_CHECKSUM        16U
#define DXL_V1_ERR_OVERLOAD        32U
#define DXL_V1_ERR_INSTRUCTION     64U

 /**
 ********************************************************************************
 **
 **  Types & Enumerations
 **
 ********************************************************************************
 */


/* DXL Communication Protocol V1 Packets
 * Double-byte header is not represented,
 * Instruction and status packets only depends on their contents:
 *  - An instruction for the instruction packets (write, read, action etc.)
 *  - Servo's status for the status packets
 */
typedef struct {
    uint8_t id;
    uint8_t length;
    uint8_t content;
    uint8_t parameters[DXL_V1_MAX_PARAMETERS];
    uint8_t checksum;
} dxl_v1_packet_t;


/**
********************************************************************************
**
**  Prototypes
**
********************************************************************************
*/

/* Hardware and low-level routines */
uint8_t dxl_v1_compute_checksum(dxl_v1_packet_t* packet);
void dxl_v1_send_packet(dxl_interface_t* itf, dxl_v1_packet_t* packet);
void dxl_v1_receive_packet(dxl_interface_t* itf, dxl_v1_packet_t* packet);
uint16_t dxl_v1_get_status(dxl_v1_packet_t* instruction_packet,
                           dxl_v1_packet_t* status_packet,
                           uint8_t expected_param_length);

/* Instructions */
void dxl_v1_ping(dxl_interface_t* itf, uint8_t id);
void dxl_v1_write(dxl_interface_t* itf, uint8_t id, uint8_t address, uint8_t* parameters,
                  size_t nb_param, bool registered);

#endif /* ___DXL_PROTOCOL_V1_H_ */
