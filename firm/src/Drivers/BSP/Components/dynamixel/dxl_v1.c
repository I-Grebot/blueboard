/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       dxl_v1.c
 * @author     Bebop35
 * @date       Feb 15, 2017
 * -----------------------------------------------------------------------------
 * @brief
 *   This component implements specific functions of the Dynamixel
 *   Communication Protocol V1
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "dxl_v1.h"

 /**
 ********************************************************************************
 **
 **  Framing functions
 **
 ********************************************************************************
 */

/* Return the checksum of a packet */
uint8_t dxl_v1_compute_checksum(dxl_v1_packet_t* packet)
{
    uint8_t i;
    uint8_t checksum;

    // Init checksum with the 'static' values
    checksum = packet->id + packet->content + packet->length;

    // Add parameters value to checksum
    if(packet->length > 2) {
        for(i=0; i<(packet->length)-2; i++) {
            checksum += (packet->parameters)[i];
        }
    }

    return ~checksum;
}

/* Send a packet through the given interface */
void dxl_v1_send_packet(dxl_interface_t* itf, dxl_v1_packet_t* packet)
{
    uint8_t idx_param;

    // Set uart2 half duplex to TX mode
    itf->hw_switch(DXL_TX_MODE);

    itf->hw_send_byte(DXL_V1_HEADER);
    itf->hw_send_byte(DXL_V1_HEADER);
    itf->hw_send_byte(packet->id);
    itf->hw_send_byte(packet->length);
    itf->hw_send_byte(packet->content);

    // Parameters
    if(packet->length > DXL_V1_PACKET_MIN_LENGTH) {
        for(idx_param = 0; idx_param < packet->length-DXL_V1_PACKET_MIN_LENGTH ; idx_param++) {
            itf->hw_send_byte(packet->parameters[idx_param]);
        }
    }

    // Checksum
    itf->hw_send_byte(packet->checksum);

    // Get back to RX mode
    itf->hw_switch(DXL_RX_MODE);

    itf->status = DXL_STATUS_NO_ERROR;

    // Stats
    itf->nb_pkt_tx++;
}

/* Receive a packet from the given interface.
 * This function must be reentrant in order to work with potential
 * concurrent multi-interfaces.*/
void dxl_v1_receive_packet(dxl_interface_t* itf, dxl_v1_packet_t* packet)
{
    uint8_t idx_param;
    uint8_t rx_buffer;
    uint16_t status;

    // Initialize values
    packet->id      = -1;   // Because nobody can have this ID
    packet->length  = 0;
    packet->content = 0;

    for(idx_param=0; idx_param < DXL_V1_MAX_PARAMETERS; idx_param++)
        packet->parameters[idx_param] = 0;
    idx_param = 0;

    // Initialize the status buffer
    status = DXL_STATUS_NO_ERROR;

    // Flush RX before receiving anything
    itf->hw_flush();

    do {
        status = itf->hw_receive_byte(&rx_buffer);
    } while((rx_buffer != DXL_V1_HEADER) && (!(status))); /* TEMP FIXME */

    /* Detect a timeout error */
    if(status) {
      itf->status = DXL_STATUS_ERR_TIMEOUT;
      itf->nb_errors++;
      return;
    }

    // Wait for the 2nd header and retrieve it
    status |= itf->hw_receive_byte(&rx_buffer);
    if(rx_buffer != DXL_V1_HEADER) {
        itf->status = DXL_STATUS_ERR_HEADER;
        itf->nb_errors++;
        return;
    }

    // Wait for the id and retrieve it
    status |= itf->hw_receive_byte(&rx_buffer);
    packet->id = rx_buffer;

    // Wait for the length and retrieve it
    status |= itf->hw_receive_byte(&rx_buffer);
    packet->length = rx_buffer;

    // Wait for the status and retrieve it
    status |= itf->hw_receive_byte(&rx_buffer);
    packet->content = rx_buffer;

    // Retrieve parameters, length must be greater than a given value
    if(packet->length > DXL_V1_PACKET_MIN_LENGTH) {
        for(idx_param=0; idx_param < packet->length-DXL_V1_PACKET_MIN_LENGTH ; idx_param++)
        {
            // Wait for the parameter and retrieve it
            status |= itf->hw_receive_byte(&rx_buffer);
            (packet->parameters)[idx_param] = rx_buffer;
        }
    }

    // Wait for the checksum and retrieve it
    status |= itf->hw_receive_byte(&rx_buffer);
    packet->checksum = rx_buffer & 0xFF;

    // TODO: check received checksum

    // End of RX
    itf->status = status;
    itf->nb_pkt_rx++;
}

// Gather all status: from DXL_V1 but also from the interface layer
uint16_t dxl_v1_get_status(dxl_v1_packet_t* instruction_packet,
                           dxl_v1_packet_t* status_packet,
                           uint8_t expected_param_length)
{
    uint16_t status;

    // Retrieve hardware status from DXL_V1
    status = (status_packet->content) & 0xFF;

    // Check that the status packet id matches the sender
    if(status_packet->id != instruction_packet->id)
        status |= DXL_STATUS_ERR_ID;

    // Check that received parameter length is correct
    if(status_packet->length != DXL_V1_MAKE_LENGTH(expected_param_length)) {
        status |= DXL_STATUS_ERR_LENGTH;
    }

    // Check that checksum is correct, this is not relevant if length is incorrect
    if(status_packet->checksum != dxl_v1_compute_checksum(status_packet)) {
        status |= DXL_STATUS_ERR_CHECKSUM;
    }

    return status;
}

/**
********************************************************************************
**
**  Instructions handlers
**
********************************************************************************
*/

// Ping a servo from its ID
void dxl_v1_ping(dxl_interface_t* itf, uint8_t id)
{
    uint16_t status;
    dxl_v1_packet_t ping_packet;
    dxl_v1_packet_t status_packet;

    // Build the instruction packet
    ping_packet.id       = id;
    ping_packet.length   = DXL_V1_MAKE_LENGTH(0);
    ping_packet.content  = DXL_V1_INS_PING;
    ping_packet.checksum = dxl_v1_compute_checksum(&ping_packet);

    status = DXL_STATUS_NO_ERROR;

    // Send the ping instruction
    dxl_v1_send_packet(itf, &ping_packet);

    // A status packet is always returned after a ping instruction.
    // However check that id was not broadcast, even if that's a dum operation.
    if((id !=  DXL_V1_ID_BROADCAST))
    {
        // Retrieve a status packet, add the UART error flags
        dxl_v1_receive_packet(itf, &status_packet);

        // Get the overall status, add the error flags
        itf->status |= dxl_v1_get_status(&ping_packet, &status_packet, 0);
    }

    if(status != DXL_STATUS_NO_ERROR) {
        itf->nb_errors++;
    }
}


