/**
 * @file logger.c
 * @brief Miscellaneous error reporting functions.
 *
 * @author Juan M. Bello Rivas <jmbr@superadditive.com>
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
#endif /* STDC_HEADERS */

#include "logger.h"


#define PREFIX		"gnostic: "


enum log_levels log_level = LOG_NOTICE;


#define VFPRINTF(fp, format)	do { \
	va_list ap; \
	\
	va_start(ap, format); \
	vfprintf(fp, format, ap); \
	va_end(ap); \
} while (0)


void
info(const char *format, ...)
{
	if (log_level <= LOG_INFO) {
		fprintf(stdout, PREFIX);
		VFPRINTF(stdout, format);
	}
}


void
error(const char *format, ...)
{
	fprintf(stderr, PREFIX);
	VFPRINTF(stderr, format);
}


void
fatal_error(const char *format, ...)
{
	fprintf(stderr, PREFIX);
	VFPRINTF(stderr, format);

	exit(EXIT_FAILURE);
}
