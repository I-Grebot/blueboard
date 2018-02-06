/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2016
 * -----------------------------------------------------------------------------
 * @file       syscalls.c
 * @author     Paul
 * @date       Jan 3, 2016
 * @version    V1.0
 * -----------------------------------------------------------------------------
 * @brief
 *   System call stub to route them to hardware
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

/* Includes */
#include "../../2018_T1_R1/include/main.h"

int _write(int file, char *ptr, int len)
// _write_r to be considered!
{
    int DataIdx;

    // Ensure not conflict with the shell
    // TODO: move mutex into serial module
    if(shell_sem_take() == pdPASS) {
      for (DataIdx = 0; DataIdx < len; DataIdx++)
      {
        serial_put((*ptr++));
      }
      shell_sem_give();
      return len;

    } else {
      return -1;
    }

}
