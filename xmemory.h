#ifndef XMEMORY_H
#define XMEMORY_H		1
/**
 * @file xmemory.h
 * @brief Error-checking memory allocation wrappers.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */


/*
 * This function fills with zeroes the allocated memory. This should be taken
 * into account if it is going to be replaced by the stock malloc or a
 * different allocator because some programs might rely on the memory being
 * initialized.
 */
extern void *xmalloc(size_t size);

extern void *xcalloc(size_t nmemb, size_t size);

extern void *xrealloc(void *ptr, size_t size);

extern char *xstrdup(const char *s);

/*
 * Be careful with the possible side-effects!
 * Also, take into account xfree doesn't check for NULL pointers passed as
 * parameters. You should use tools such as valgrind to check for those issues.
 */
#define xfree(ptr)	 do { \
	free(ptr); \
	ptr = NULL; \
} while (0)


#endif /* !XMEMORY_H */
