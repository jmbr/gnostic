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

#include <assert.h>

#include <errno.h>


static void error(const char *f) __attribute__ ((noreturn));


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

	ptr = malloc(size);
	if (!ptr)
		error("malloc");

	memset(ptr, 0, size);

	return ptr;
}


void *
xcalloc(size_t nmemb, size_t size)
{
	void *ptr;

	ptr = calloc(nmemb, size);
	if (!ptr)
		error("calloc");

	return ptr;
}


void *
xrealloc(void *ptr, size_t size)
{
	void *p;

	p = realloc(ptr, size);
	if (!p)
		error("realloc");

	return p;
}


char *
xstrdup(const char *s)
{
	char *p;

	p = strdup(s);
	if (!p)
		error("strdup");

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


void
error(const char *f)
{
	size_t errbuflen = 128;
	char errbuf[errbuflen];

	assert(f);

#ifdef HAVE_STRERROR_R
	/*
	 * We use the incompatible strerror_r provided by glibc.
	 * XXX In the future this should not require glibc to work.
	 */
	fprintf(stderr, "%s: %s\n", f, strerror_r(errno, errbuf, errbuflen));
#else /* !HAVE_STRERROR_R */
	fprintf(stderr, "%s failed (errno = %d)\n", f, errno);
#endif /* HAVE_STRERROR_R */

	exit(EXIT_FAILURE);
}
