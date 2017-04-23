/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       aversive_config.h
 * @author     Paul
 * @date       Jan 9, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   This file goes along with aversive.c and defines target specific items.
 *   It must be adapted depending on the platform.
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: http://svn2.assembla.com/svn/paranoid_android/
 * -----------------------------------------------------------------------------
 * $Rev: 1464 $
 * $LastChangedBy: Pierrick_Boissard $
 * $LastChangedDate: 2016-05-02 15:38:42 +0200 (lun., 02 mai 2016) $
 * -----------------------------------------------------------------------------
 */

#ifndef AVERSIVE_CONFIG_H_
#define AVERSIVE_CONFIG_H_

/* Errors definitions (included in errno.h) */
#include <sys/errno.h>

/* Debug definitions */
#include "debug.h"

/* Hardware constants */
#define F_CPU   SYSCLK_VALUE    /* Unused in the code */

/* ASM Utilities (unused in the code) */
#define nop() do {} while(0)
#define nothing() do {} while(0)
#define cli() do {} while(0)
#define sei() do {} while(0)
#define reset() do {} while(0)

#define GLOBAL_IRQ_ARE_MASKED() (/* tbu */)
#define IRQ_LOCK(flags)   do {} while(0)//{ (void) flags; __asm__ volatile ("cpsie i"); }
#define IRQ_UNLOCK(flags) do {} while(0)//{ (void) flags; __asm__ volatile ("cpsid i"); }

/* PID Filters configuration:
 * The derivate term can be filtered to remove the noise. This value
 * is the maximum sample count to keep in memory to do this
 * filtering.
 */
#define PID_DERIVATE_FILTER_MAX_SIZE 4


/* Uncomment to configure incremental encoders for the motors as well */
//#define CONFIG_MODULE_ROBOT_SYSTEM_MOT_AND_EXT

/* Uncomment to activate the centrifugal force compensation */
//#define CONFIG_MODULE_COMPENSATE_CENTRIFUGAL_FORCE

/* tbu */
#define DO_CS     1  /* control system */
#define DO_RS     2  /* robot system (angle/distance) */
#define DO_POS    4  /* process position */
#define DO_BD     8  /* process blocking detection */
#define DO_POWER  32 /* disable brake */

#define DO_STATUS 64 /* status events*/

// Mutexes handlers
extern void vLockEncoderAngle(void);
extern void vLockEncoderDistance(void);
extern void vLockAngleConsign(void);
extern void vLockDistanceConsign(void);
extern void vLockRobotPosition(void);
extern void vUnlockEncoderAngle(void);
extern void vUnlockEncoderDistance(void);
extern void vUnlockAngleConsign(void);
extern void vUnlockDistanceConsign(void);
extern void vUnlockRobotPosition(void);

#endif /* AVERSIVE_CONFIG_H_ */
