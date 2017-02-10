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
 **  Types & Enumerations
 **
 ********************************************************************************
 */


/* DXL Protocol V1 Packets
 * Double-byte header is not represented,
 * Instruction and status packets only depends on their contents:
 *  - An instruction for the instruction packets (write, read, action etc.)
 *  - Servo's status for the status packets
 */
typedef struct {
    uint8_t id;
    uint8_t length;
    uint8_t content;
    uint8_t parameters[];
    uint8_t checksum;
} dxl_v1_packet_t;


/**
 ********************************************************************************
 **
 **  Framing Constants
 **
 ********************************************************************************
 */



#endif /* ___DXL_PROTOCOL_V1_H_ */
