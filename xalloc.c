/*
 * xalloc.c -- Error-checking *alloc wrappers.
 * $Id: xalloc.c,v 1.1.1.1 2001/12/07 20:56:20 rwx Exp $
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdio.h>
# include <stdlib.h>
#endif /* STDC_HEADERS */

#ifdef HAVE_STRING_H
# include <string.h>
#elif HAVE_STRINGS_H
# include <strings.h>
#endif /* !HAVE_STRING_H */

#ifdef HAVE_MEMORY_H
# include <memory.h>
#endif /* HAVE_MEMORY_H */

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */

#include <errno.h>

#include <assert.h>

#include "debug.h"


/*
 * This function fills with zeroes the allocated memory. This should be taken
 * into account if it is going to be replaced by the stock malloc or a
 * different allocator because some programs might rely on the memory being
 * initialized.
 */
void *
xmalloc(size_t size)
{
	void *ptr;

	if (!(ptr = malloc(size)))
		fatal_error("malloc: %s", strerror(errno));

	memset(ptr, 0, size);

	return ptr;
}


void *
xcalloc(size_t nmemb, size_t size)
{
	void *ptr;

	if (!(ptr = calloc(nmemb, size)))
		fatal_error("calloc: %s", strerror(errno));

	return ptr;
}


void *
xrealloc(void *ptr, size_t size)
{
	void *p;

	if (!(p = realloc(ptr, size)))
		fatal_error("realloc: %s", strerror(errno));

	return p;
}


char *
xstrdup(const char *s)
{
	char *p;

	if (!(p = strdup(s)))
		fatal_error("strdup: %s", strerror(errno));

	return p;
}


void
xfree(void *ptr)
{
	if (ptr) {
		free(ptr);
		ptr = NULL;
	}
}
