#ifndef CLEANUP_H
#define CLEANUP_H		1
/*
 * cleanup.h -- Clean-up stacks to ease exception handling.
 * $Id$
 */


#define CLEANUP_PUSH(stack, ptr, fn, label)	\
do { \
	if (!ptr) \
		goto label; \
	cleanup_push(stack, ptr, (cleanup_fn) fn); \
} while (0)


typedef void (*cleanup_fn)(void *);

typedef struct cleanup_st *cleanup_t;
typedef struct cleanupstack_st *cleanupstack_t;


extern cleanupstack_t cleanup_setup(void);
extern void cleanup_push(cleanupstack_t stack, void *v, cleanup_fn fn);
extern void cleanup(cleanupstack_t stack);


#endif /* !CLEANUP_H */
