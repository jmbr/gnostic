#ifndef DEBUG_H
#define DEBUG_H			1
/*!
 * \file debug.h
 * \brief Miscellaneous debugging functions.
 */


/** Debugging printf.
 * If the macro NDEBUG is not set it prints the message to stdout.
 */
extern void dprintf(const char *format, ...);

/** Error printf.
 * Prints a message to stderr.
 */
extern void eprintf(const char *format, ...);

/** Fatal error printf.
 * Prints a message to stderr and exits the program.
 */
extern void fatal_error(const char *format, ...) __attribute__ ((noreturn));


#endif /* DEBUG_H */
