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

#ifdef HAVE_MEMORY_H
# include <memory.h>
#endif /* HAVE_MEMORY_H */

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */

#include <assert.h>

#include <errno.h>


static void die(const char *f) __attribute__ ((noreturn));


void *
xmalloc(size_t size)
{
	void *ptr;

	ptr = malloc(size);
	if (!ptr)
		die("malloc");

	memset(ptr, 0, size);

	return ptr;
}


void *
xcalloc(size_t nmemb, size_t size)
{
	void *ptr;

	ptr = calloc(nmemb, size);
	if (!ptr)
		die("calloc");

	return ptr;
}


void *
xrealloc(void *ptr, size_t size)
{
	void *p;

	p = realloc(ptr, size);
	if (!p)
		die("realloc");

	return p;
}


char *
xstrdup(const char *s)
{
	char *p;

	p = strdup(s);
	if (!p)
		die("strdup");

	return p;
}


void
__xfree(void *ptr)
{
	if (ptr)
		free(ptr);
}


void
die(const char *f)
{
	assert(f);
	assert(errno != 0);

	(void) fprintf(stderr, "%s failed (errno = %d)\n", f, errno);

	exit(EXIT_FAILURE);
}
