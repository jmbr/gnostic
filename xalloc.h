#ifndef GNOSTIC_XALLOC_H
#define GNOSTIC_XALLOC_H		1
/*
 * xalloc.h -- Error-checking *alloc wrappers.
 * $Id: xalloc.h,v 1.1.1.1 2001/12/07 20:56:20 rwx Exp $
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */


extern void *xmalloc(size_t size);
extern void *xcalloc(size_t nmemb, size_t size);
extern void *xrealloc(void *ptr, size_t size);
extern char *xstrdup(const char *s);
extern void xfree(void *ptr);


#endif /* !GNOSTIC_XALLOC_H */
