/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       dxl_v2.c
 * @author     Bebop35
 * @date       Apr 17, 2017
 * -----------------------------------------------------------------------------
 * @brief
 *   This component implements specific functions of the Dynamixel
 *   Communication Protocol V2
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "dynamixel.h"

 /**
 ********************************************************************************
 **
 **  Framing functions
 **
 ********************************************************************************
 */

// Update the CRC data
uint16_t dxl_v2_compute_crc(uint8_t *data, uint8_t data_size)
{
    extern const uint16_t dxl_v2_crc_table[];
    uint8_t crc_table_idx;
    uint16_t data_idx;
    uint16_t crc_accum = 0;

    for (data_idx = 0; data_idx < data_size; data_idx++)
    {
    	crc_table_idx = ((uint16_t)(crc_accum >> 8) ^ data[data_idx]) & 0xFF;
		crc_accum = (crc_accum << 8) ^ dxl_v2_crc_table[crc_table_idx];
    }

    return crc_accum;
}

/* Send a packet through the given interface
 * Since byte-stuffing is inserted, we build a complete buffer before sending
 * anything. This is also required to know the exact length and CRC of the packet.
 *
 * Update the packet's length and CRC values
 */
void dxl_v2_send_packet(dxl_interface_t* itf, dxl_v2_packet_t* packet)
{
    uint8_t idx_param;
    uint8_t buffer[DXL_V2_MAX_LENGTH];

    uint8_t idx = 0;
    uint8_t len_idx;
    uint8_t header1_cnt;
    uint8_t stuffing_cnt;

    // Header (no Byte-Stuffing)
    buffer[idx++] = DXL_V2_HEADER1;
    buffer[idx++] = DXL_V2_HEADER2;
    buffer[idx++] = DXL_V2_HEADER3;
    buffer[idx++] = DXL_V2_HEADER4; // actually reserved

    // ID (no BS)
    buffer[idx++] = packet->id;

    // Length is not known now, 2 bytes are skipped
    // length index position in the buffer is memorized
    len_idx = idx;
    idx+=2;

    // Instruction (no BS)
    buffer[idx++] = packet->content;

    // Start to add parameters
    header1_cnt = 0;
    stuffing_cnt = 0;
    for (idx_param = 0; idx_param < packet->nb_param; idx_param++)
    {
    	// Check if the triple byte-stuffing value is matched
		if (packet->parameters[idx_param] == DXL_V2_HEADER3 && header1_cnt >=2 ) {
			header1_cnt = 0;
			buffer[idx++] = DXL_V2_HEADER3;
			buffer[idx++] = DXL_V2_HEADER3;
			stuffing_cnt++;

		// No need to add a byte
		} else {

			// Keep track of consecutive header1 values
			if (packet->parameters[idx_param] == DXL_V2_HEADER1) {
				header1_cnt++;
			} else {
				header1_cnt = 0;
			}

			// Still add data to the buffer
			buffer[idx++] = packet->parameters[idx_param];
		}

    }

    // Compute the packet length and add it to the buffer
    packet->length = DXL_V2_MAKE_LENGTH(packet->nb_param) + stuffing_cnt;
    buffer[len_idx]   = (uint8_t) (  packet->length		  & 0xFF);
    buffer[len_idx+1] = (uint8_t) (( packet->length >> 8 ) & 0xFF);

    // Compute CRC, save the buffer length
    packet->crc = dxl_v2_compute_crc(buffer, idx);
    len_idx = idx;

    // Send the buffer
	for(idx = 0; idx < len_idx; idx++)
	{
		// TODO Check for error
		itf->hw_send_byte(itf->itf_idx, buffer[idx]);
	}

	// Finally send the CRC
	itf->hw_send_byte(itf->itf_idx, ((uint8_t)  packet->crc & 0xFF));
	itf->hw_send_byte(itf->itf_idx, ((uint8_t) (packet->crc >> 8U) & 0xFF));

	// Status
    itf->status = DXL_STATUS_NO_ERROR;

    // Stats
    itf->nb_pkt_tx++;

#ifdef DXL_DEBUG
    serial_puts(DXL_DEBUG_PFX" V2 Send:"DXL_DEBUG_EOL);
    dxl_v2_print_packet(packet);
#endif

}

//
// RECEIVE TODO
//



/**
********************************************************************************
**
**  Instructions handlers
**
********************************************************************************
*/

// Ping a servo
void dxl_v2_ping(dxl_servo_t* servo)
{
    dxl_v2_packet_t ping_packet;
    dxl_v2_packet_t status_packet;

    // Build the instruction packet
    ping_packet.id       = servo->id;
    ping_packet.nb_param = 0;
    ping_packet.content  = DXL_V2_INS_PING;

    servo->itf->status = DXL_STATUS_NO_ERROR;

    // Send the ping instruction
    dxl_v2_send_packet(servo->itf, &ping_packet);

    // A status packet is always returned after a ping instruction.
    // However check that id was not broadcast, even if that's a dum operation.
    /*if((servo->id !=  DXL_ID_BROADCAST))
    {
        // Retrieve a status packet, add the UART error flags
        dxl_v1_receive_packet(servo->itf, &status_packet);

        // Get the overall status, add the error flags
        servo->itf->status |= dxl_v1_get_status(&ping_packet, &status_packet, 0);
    }*/

    if(servo->itf->status != DXL_STATUS_NO_ERROR) {
        servo->itf->nb_errors++;

#ifdef DXL_DEBUG
        dxl_print_error(servo->itf->status, servo->itf->protocol);
#endif // DXL_DEBUG
    }
}

// Reset instruction
void dxl_v2_reset(dxl_servo_t* servo)
{}
/*void dxl_v1_reset(dxl_servo_t* servo)
{
    dxl_v1_packet_t reset_packet;

    // Build the instruction packet
    reset_packet.id       = servo->id;
    reset_packet.length   = DXL_V1_MAKE_LENGTH(0);
    reset_packet.content  = DXL_V1_INS_RESET;
    reset_packet.checksum = dxl_v1_compute_checksum(&reset_packet);

    servo->itf->status = DXL_STATUS_NO_ERROR;

    // Send the reset instruction
    dxl_v1_send_packet(servo->itf, &reset_packet);

    // Never check for return status packet

    if(servo->itf->status != DXL_STATUS_NO_ERROR) {
        servo->itf->nb_errors++;
    }
}*/

// Write instruction
// If registered parameter is set, it'll be a reg_write instruction.
void dxl_v2_write(dxl_servo_t* servo, uint8_t address, uint8_t* parameters, size_t nb_param, bool registered)
{
    uint8_t idx_param;
    dxl_v2_packet_t write_packet;
    dxl_v2_packet_t status_packet;

    // Build the packet
    write_packet.id       = servo->id;
    write_packet.nb_param = nb_param;
    write_packet.content  = registered ? DXL_V2_INS_REG_WRITE:DXL_V2_INS_WRITE;
    write_packet.parameters[0] = address;

    for(idx_param=0; idx_param < nb_param; idx_param++) {
        write_packet.parameters[idx_param+1] = parameters[idx_param];
    }

    // Send the instruction
    dxl_v2_send_packet(servo->itf, &write_packet);

    servo->itf->status = DXL_STATUS_NO_ERROR;

    // A status packet is returned only if address is not broadcast and
    // the status return level is set to "all packets".
    /*if((servo->id != DXL_ID_BROADCAST) &&
            servo->itf->return_level == DXL_STATUS_EVERYTHING)
    {
        // Retrieve a status packet, add the UART error flags
        dxl_v1_receive_packet(servo->itf, &status_packet);

        // Get the overall status
        servo->itf->status |= dxl_v1_get_status(&write_packet, &status_packet, 0);
    }*/

    if(servo->itf->status != DXL_STATUS_NO_ERROR) {
        servo->itf->nb_errors++;

#ifdef DXL_DEBUG
        dxl_print_error(servo->itf->status, servo->itf->protocol);
#endif // DXL_DEBUG
    }


}

void dxl_v2_read(dxl_servo_t* servo, uint8_t address, uint8_t* datas, size_t nb_data)
{}

void dxl_v2_action(dxl_servo_t* servo)
{}


/**
********************************************************************************
**
**  Service routines
**
********************************************************************************
*/
void dxl_v2_get_error_str(char* status_str, size_t status_str_len, dxl_status_t status)
{}
/*
{

    if(status & DXL_V1_ERR_INPUT_VOLTAGE) {
        snprintf(status_str, status_str_len, "V1: Input voltage ");
        status_str += strlen(status_str);
    }

    if(status & DXL_V1_ERR_ANGLE_LIMIT) {
        snprintf(status_str, status_str_len, "V1: Angle limit ");
        status_str += strlen(status_str);
    }

    if(status & DXL_V1_ERR_OVERHEATING) {
        snprintf(status_str, status_str_len, "V1: Overheating ");
        status_str += strlen(status_str);
    }

    if(status & DXL_V1_ERR_RANGE) {
        snprintf(status_str, status_str_len, "V1: Range ");
        status_str += strlen(status_str);
    }

    if(status & DXL_V1_ERR_CHECKSUM) {
        snprintf(status_str, status_str_len, "V1: Checksum ");
        status_str += strlen(status_str);
    }

    if(status & DXL_V1_ERR_OVERLOAD) {
        snprintf(status_str, status_str_len, "V1: Overload ");
        status_str += strlen(status_str);
    }

    if(status & DXL_V1_ERR_INSTRUCTION) {
        snprintf(status_str, status_str_len, "V1: Instruction ");
        status_str += strlen(status_str);
    }
}
*/
/**
********************************************************************************
**
**  Debug
**
********************************************************************************
*/
#ifdef DXL_DEBUG

// Print a packet content in the given string
void dxl_v2_print_packet(dxl_v2_packet_t* packet)
{
    char str[100];
    uint8_t idx_param;

    sprintf(str, "  Id:    %02X"DXL_DEBUG_EOL
                 "  Ins:   %02X"DXL_DEBUG_EOL
                 "  Par:   %02X"DXL_DEBUG_EOL
				 "  Len:   %02X"DXL_DEBUG_EOL
                 "  Data: ",
            packet->id, packet->content, packet->nb_param, packet->length);
    serial_puts(str);

    if(packet->nb_param > 0) {
        for(idx_param = 0; idx_param < packet->nb_param ; idx_param++) {
            sprintf(str, "%02X ", packet->parameters[idx_param]);
            serial_puts(str);
        }
    } else {
        serial_puts("(none)");
    }

    sprintf(str, DXL_DEBUG_EOL"  CRC:  %02X"DXL_DEBUG_EOL, packet->crc);
    serial_puts(str);
}

#endif // DXL_DEBUG


