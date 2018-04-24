/* -----------------------------------------------------------------------------
 * BlueBoard
 * I-Grebot 2017
 * -----------------------------------------------------------------------------
 * @file    debug.h
 * @author  Paul
 * @date    2017/04/20
 * -----------------------------------------------------------------------------
 * @brief
 *    This file contains definitions for the project debug
 * -----------------------------------------------------------------------------
 * Versionning informations
 * Repository: https://github.com/I-Grebot/blueboard.git
 * -----------------------------------------------------------------------------
 */

#ifndef __DEBUG_H
#define __DEBUG_H

/**
********************************************************************************
**
**  Definitions
**
********************************************************************************
*/


/* Define the messages significance */
#define DEBUG_LEVEL_NONE       0
#define DEBUG_LEVEL_CRITICAL  10  // Critical and emergency errors (show-stopper errors)
#define DEBUG_LEVEL_ERROR     20  // Errors
#define DEBUG_LEVEL_WARNING   30  // Warnings
#define DEBUG_LEVEL_INFO      40  // Informations
#define DEBUG_LEVEL_TRACE    100  // Reserved for debug (fully verbose, full spam ahead)

/* Messages prefix */
#define DEBUG_CRITICAL_PFX   "CRITICAL: "
#define DEBUG_ERROR_PFX      "ERROR: "
#define DEBUG_WARNING_PFX    "WARNING: "
#define DEBUG_INFO_PFX       "INFO: "
#define DEBUG_TRACE_PFX      "TRACE: "

/* End Of Line */
#define DEBUG_EOL   "\n\r"


/**
********************************************************************************
**
**  Debug level definition
**
********************************************************************************
*/

/* Define which debug level should be used.
 * The symbol should not be defined if debug is disabled.
 * In this case, the code will be optimized by removing all
 * debug function calls.
 *
 * All debug messages of higher priority are printed.
 * E.g:  in "ERR", Critical errors + Errors are displayed
 *       in "INF", All debug messages are printed
 * */

#define DEBUG_LEVEL     DEBUG_LEVEL_TRACE

/**
********************************************************************************
**
**  Macros
**
********************************************************************************
*/

//
#define DEBUG_PRINTF(...) { if(DEBUG_LEVEL != DEBUG_LEVEL_NONE) { printf(__VA_ARGS__); } }

// Map significance shorthands
#define DEBUG_CRITICAL(...)   { if(DEBUG_LEVEL >= DEBUG_LEVEL_CRITICAL) { DEBUG_PRINTF(DEBUG_CRITICAL_PFX __VA_ARGS__); } }
#define DEBUG_ERROR(...)      { if(DEBUG_LEVEL >= DEBUG_LEVEL_ERROR)    { DEBUG_PRINTF(DEBUG_ERROR_PFX    __VA_ARGS__); } }
#define DEBUG_WARNING(...)    { if(DEBUG_LEVEL >= DEBUG_LEVEL_WARNING)  { DEBUG_PRINTF(DEBUG_WARNING_PFX  __VA_ARGS__); } }
#define DEBUG_INFO(...)       { if(DEBUG_LEVEL >= DEBUG_LEVEL_INFO)     { DEBUG_PRINTF(DEBUG_INFO_PFX     __VA_ARGS__); } }
#define DEBUG_INFO_NOPFX(...) { if(DEBUG_LEVEL >= DEBUG_LEVEL_INFO)     { DEBUG_PRINTF(                   __VA_ARGS__); } }
#define DEBUG_TRACE(...)      { if(DEBUG_LEVEL >= DEBUG_LEVEL_TRACE)    { DEBUG_PRINTF(DEBUG_TRACE_PFX    __VA_ARGS__); } }
#define DEBUG_TRACE_NOPFX(...){ if(DEBUG_LEVEL >= DEBUG_LEVEL_TRACE)    { DEBUG_PRINTF(                   __VA_ARGS__); } }



#endif /* __DEBUG_H */
