#ifndef LOGGER_H
#define LOGGER_H			1
/**
 * @file logger.h
 * @brief Miscellaneous reporting functions.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
 */
/* TODO Evolve this into a better reporting/logging module. */


extern enum log_levels {
	LOG_DEBUG = 0,
	LOG_INFO,
	LOG_NOTICE,
	LOG_WARNING,
	LOG_ERROR,
} log_level;


/** Information printf.
 * Prints an informational message to stdout.
 */
extern void info(const char *format, ...);

/** Error printf.
 * Prints a message to stderr.
 */
extern void error(const char *format, ...);

/** Fatal error printf.
 * Prints a message to stderr and exits the program.
 */
extern void fatal_error(const char *format, ...) __attribute__ ((noreturn));


#endif /* !LOGGER_H */
