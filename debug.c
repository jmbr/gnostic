/**
 * @file err.c
 * @brief Miscellaneous error reporting functions.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
#endif /* STDC_HEADERS */


#define VFPRINTF(fp, format)	do { \
	va_list ap; \
	\
	va_start(ap, format); \
	vfprintf(fp, format, ap); \
	va_end(ap); \
} while (0)



void
dprintf(const char *format, ...)
{
#ifndef NDEBUG
	VFPRINTF(stdout, format);
#endif /* !NDEBUG */
}

void
eprintf(const char *format, ...)
{
	VFPRINTF(stderr, format);
}


void
fatal_error(const char *format, ...)
{
	VFPRINTF(stderr, format);

	exit(EXIT_FAILURE);
}
