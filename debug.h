#ifndef ERR_H
#define ERR_H			1
/**
 * @file err.h
 * @brief Miscellaneous error reporting functions.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
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


#endif /* !ERR_H */
