#ifndef XALLOC_H
#define XALLOC_H		1
/*
 * xalloc.h -- Error-checking *alloc wrappers.
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

/* Be careful with the possible side-effects. */
#define xfree(ptr)	 do { assert(ptr); free(ptr); ptr = NULL; } while (0)


#endif /* !XALLOC_H */
