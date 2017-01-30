/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       physics_const.h
 * @author     Paul
 * @date       Jan 10, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   This file defines all physical constants like distances, positions, speed
 *   accelerations are time. It concerns the robot(s) as well as the opponent(s)
 *   and playground.
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev:: 1464                                                                 $
 * $LastChangedBy:: Pierrick_Boissard                                          $
 * $LastChangedDate:: 2016-05-02 15:38:42 +0200 (lun., 02 mai 2016)            $
 * -----------------------------------------------------------------------------
 */

#ifndef PHYSICS_CONST_H_
#define PHYSICS_CONST_H_

#include <stdint.h>

/**
********************************************************************************
**
**  Mechanical constants of our robot
**
********************************************************************************
*/

/* Distance between motorized wheels, in millimeters */
#define PHYS_ROBOT_MOTORS_TRACK_MM          ((double)  143.0)

/* Distance between quadrature encoder wheels, in millimeters */
#define PHYS_ROBOT_ENCODERS_TRACK_MM        ((double)  201.5)

/* Nominal Diameter of a quadrature encoder wheel, in millimeters */
#define PHYS_ROBOT_ENCODER_WHEEL_DIAM_MM    ((double)   59.0)

/* Number of impulsions of the quadrature encoder per revolution */
#define PHYS_ROBOT_ENCODER_NB_IMP_PER_REV   ((double) 4096.0)

/* Encoder wheel gains to compensate for forward/backward rotation
 * (adjust sign) and diameter variations */
#define PHYS_ROBOT_ENCODER_LEFT_GAIN        ((double)  1.000)
#define PHYS_ROBOT_ENCODER_RIGHT_GAIN       ((double) -1.000)

/* Centrifugal compensation coefficient. Value TO BE ADJUSTED */
#define PHYS_ROBOT_CENTRIFUGAL_COEF         ((double)   5.0)

/* Quadrature encoders conversion ratio between a number
 * of impulsions and a distance (in mm). Simply divide by the
 * number of impulsions per revolution by the perimeter of a quadrature
 * encoder wheel (nominal value) */
#define PHYS_ROBOT_NB_IMP_PER_MM      (PHYS_ROBOT_ENCODER_NB_IMP_PER_REV \
                                    / (M_PI * PHYS_ROBOT_ENCODER_WHEEL_DIAM_MM))

/**
********************************************************************************
**
**  Motion control filters constants
**
********************************************************************************
*/

/* Control System in Distance filters parameters */
#define PHYS_CS_D_PID_KP                    ((int16_t)   10000)
#define PHYS_CS_D_PID_KI                    ((int16_t)      0)
#define PHYS_CS_D_PID_KD                    ((int16_t)   1000)
#define PHYS_CS_D_PID_MAX_IN                ((int32_t)      0)
#define PHYS_CS_D_PID_MAX_I                 ((int32_t)   5000)
#define PHYS_CS_D_PID_MAX_OUT               ((int32_t)      0)
#define PHYS_CS_D_PID_OUT_SHIFT             ((uint8_t)     12)
#define PHYS_CS_D_PID_DRV_FILTER            ((uint8_t)      4)
#define PHYS_CS_D_QUAD_POS_SPEED            ((uint32_t)  1200)
#define PHYS_CS_D_QUAD_NEG_SPEED            ((uint32_t)  1200)
#define PHYS_CS_D_QUAD_POS_ACCEL            ((uint32_t)     20)
#define PHYS_CS_D_QUAD_NEG_ACCEL            ((uint32_t)     20)

/* Control System in Angle filters parameters */
#define PHYS_CS_A_PID_KP                    ((int16_t)   2500)
#define PHYS_CS_A_PID_KI                    ((int16_t)      0)
#define PHYS_CS_A_PID_KD                    ((int16_t)   1000)
#define PHYS_CS_A_PID_MAX_IN                ((int32_t)      0)
#define PHYS_CS_A_PID_MAX_I                 ((int32_t)   5000)
#define PHYS_CS_A_PID_MAX_OUT               ((int32_t)      0)
#define PHYS_CS_A_PID_OUT_SHIFT             ((uint8_t)     11)
#define PHYS_CS_A_PID_DRV_FILTER            ((uint8_t)      4)
#define PHYS_CS_A_QUAD_POS_SPEED            ((uint32_t)  1200)
#define PHYS_CS_A_QUAD_NEG_SPEED            ((uint32_t)  1200)
#define PHYS_CS_A_QUAD_POS_ACCEL            ((uint32_t)     8)
#define PHYS_CS_A_QUAD_NEG_ACCEL            ((uint32_t)     8)

/* Trajectory Manager parameters */
#define PHYS_TRAJ_D_DEFAULT_SPEED           ((int16_t)    1500)
#define PHYS_TRAJ_A_DEFAULT_SPEED           ((int16_t)    800)
#define PHYS_TRAJ_DEFAULT_WIN_D             ((double)    30.0)
#define PHYS_TRAJ_DEFAULT_WIN_A_DEG         ((double)     5.0)
#define PHYS_TRAJ_DEFAULT_WIN_A_START_DEG   ((double)    30.0)

/* Blocking detection filter constants */
#define PHYS_BD_K1                          ((int32_t)      5)
#define PHYS_BD_K2                          ((int32_t)     40)
#define PHYS_BD_THR                         ((uint32_t)  4000)
#define PHYS_BD_CPT                         ((uint32_t)   100)
#define PHYS_BD_SPD                         ((uint16_t)   150)

/**
********************************************************************************
**
**  Playground definitions
**
********************************************************************************
*/

#endif /* PHYSICS_CONST_H_ */