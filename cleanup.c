/*
 * cleanup.c -- Clean-up stacks to ease exception handling.
 * $Id$
 */
/*
 * This module is intended for cleanup stack exception handling (CSEH). It is
 * loosely modeled after the pthread_cleanup_* functions and kazlib's exception
 * handling routines.
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdlib.h>
#endif /* STDC_HEADERS */

#ifdef HAVE_STRING_H
# include <string.h>
#elif HAVE_STRINGS_H
# include <strings.h>
#endif /* !HAVE_STRING_H */

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */

#include <assert.h>

#include "xalloc.h"

#include "cleanup.h"


struct cleanup_st {
	void *v;
	cleanup_fn fn;

	struct cleanup_st *next;
};

struct cleanupstack_st {
	cleanup_t top;
};


cleanupstack_t
cleanup_setup(void)
{
	cleanupstack_t n;

	n = xmalloc(sizeof(struct cleanupstack_st));
	n->top = NULL;

	return n;
}

void
cleanup_push(cleanupstack_t stack, void *v, cleanup_fn fn)
{
	cleanup_t n;

	n = xmalloc(sizeof(struct cleanup_st));

	/*
	 * We don't assert(v) because we might want to push a general
	 * deallocation function which takes no arguments and anyway those
	 * users wanting to check the validity of v will call CLEANUP_PUSH
	 * instead.
	 */
	n->v = v;
	n->fn = (fn) ? fn : free;
	n->next = stack->top;
	stack->top = n;
}

void
cleanup(cleanupstack_t stack)
{
	cleanup_t n, next;

	assert(stack);

	for (n = stack->top; n; n = next) {
		next = n->next;
		n->fn(n->v);
		free(n);
	}

	free(stack);
}
