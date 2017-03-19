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


/* Initialize a DXL Interface with default values.
 * @param itf: Dynamixel Interface to write into
 */
void dxl_init_itf(dxl_interface_t* itf, uint8_t itf_idx)
{
    itf->itf_idx = itf_idx;
    itf->protocol = DXL_V1;
    itf->hw_switch = NULL;
    itf->hw_send_byte = NULL;
    itf->hw_receive_byte = NULL;
    itf->nb_pkt_tx = 0;
    itf->nb_pkt_rx = 0;
    itf->nb_errors = 0;
    itf->status = DXL_STATUS_NO_ERROR;


    itf->return_level = DXL_STATUS_EVERYTHING;
    itf->return_delay_ms = 1;

    // Protocol-dependent values
    if(itf->protocol == DXL_V1) {
        // ..
    }

}

/* Initialize a servo, by model name attach it to a DXL Interface */
void dxl_init_servo(dxl_servo_t* servo, dxl_interface_t* itf, const char* model_name)
{
    servo->model = dxl_find_servo_model_by_name(model_name);
/*
    // Check that model exists
    if(servo->model == NULL)
    {
        // TODO error mgmt
        return;
    }

    // Check that the protocol is correct
    if(servo->model->protocol != itf->protocol)
    {
        // TODO error mgmt
        return;
    }*/

    servo->id = DXL_ID_DEFAULT;
    servo->itf = itf;

    servo->min_position = 0;
    servo->max_position = 1023; // TEMP
    servo->current_position = 0;

#ifdef DXL_DEBUG
    serial_puts("[DXL] Init:"DXL_DEBUG_EOL);
    dxl_print_servo(servo);
#endif

}

/* Basic write access
 * Branch correctly on the dxl_vN protocol version.
 * @param servo: Servo to send the write access
 * @param addr: Register address
 * @param values: Array of values to write
 * @param size: Size of the value array in number of bytes
 * @param reg: True if the write is registered
 */
void dxl_write(dxl_servo_t* servo, uint8_t addr, uint8_t* values, size_t size, bool reg)
{
    if(servo->itf->protocol == DXL_V1) {
        dxl_v1_write(servo, addr, values, size, reg);

    } else if(servo->itf->protocol == DXL_V2) {
        // TODO

    // Error
    } else {
        return;
    }

}

void dxl_reset(dxl_servo_t* servo)
{

    if(servo->itf->protocol == DXL_V1) {
        dxl_v1_reset(servo);

    } else if(servo->itf->protocol == DXL_V2) {
        // TODO

    // Error
    } else {
        return;
    }
}


void dxl_ping(dxl_servo_t* servo)
{
    if(servo->itf->protocol == DXL_V1) {
        dxl_v1_ping(servo);

    } else if(servo->itf->protocol == DXL_V2) {
        // TODO

    // Error
    } else {
        return;
    }
}

void dxl_set_torque(dxl_servo_t* servo, uint8_t torque)
{
    extern const dxl_register_t* dxl_reg_v1_torque_enable;

    dxl_write(servo, dxl_reg_v1_torque_enable->address, &torque, sizeof(torque), false);

}

void dxl_set_position(dxl_servo_t* servo, uint16_t new_position)
{
    // TEMP
    extern const dxl_register_t* dxl_reg_v1_goal_position;
    uint8_t data[2];

    data[0] = (uint8_t) (new_position & 0x00FF);
    data[1] = (uint8_t) (new_position >> 8);

    dxl_write(servo, dxl_reg_v1_goal_position->address, data, sizeof(data), false);

}

void dxl_set_led(dxl_servo_t* servo, uint8_t led)
{
    extern const dxl_register_t* dxl_reg_v1_led;

    dxl_write(servo, dxl_reg_v1_led->address, &led, sizeof(led), false);

}

