/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot
 * -----------------------------------------------------------------------------
 * @file       dynamixel.c
 * @author     Bebop35
 * @date       Feb 09, 2017
 * -----------------------------------------------------------------------------
 * @brief
 *   This component implements the control of Dynamixel Servos
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#include "dynamixel.h"

/* Initialize a DXL Interface with default values */
void dxl_init(dxl_interface_t* itf)
{
    itf->protocol = DXL_V1;
    itf->hw_switch = NULL;
    itf->hw_send_byte = NULL;
    itf->hw_receive_byte = NULL;
    itf->return_level = 0;
    itf->return_delay_ms = 0;
    itf->nb_pkt_tx = 0;
    itf->nb_pkt_rx = 0;
    itf->nb_errors = 0;
    itf->status = DXL_STATUS_NO_ERROR;
}
