/*
 * debug.c -- Miscellaneous debugging features.
 * $Id: debug.c,v 1.1.1.1 2001/12/07 20:56:20 rwx Exp $
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
#endif /* STDC_HEADERS */


void
dprintf(const char *format, ...)
{
#ifndef NDEBUG
	va_list ap;

	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);
#else /* NDEBUG */

#endif /* !NDEBUG */
}

void
eprintf(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);
}


void
fatal_error(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}
