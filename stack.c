/**
 * @file stack.c
 * @brief A simple, fast stack data structure.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdlib.h>
#endif /* STDC_HEADERS */

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */

#include <assert.h>

#include "stack.h"

#include "xmemory.h"


#define STACK_DEFAULT_LEN		8


/** A stack (Last-In First-Out) structure.
 */
struct stack_st {
	size_t len;		/**< Stack length (in number of elements) */
	unsigned top;		/**< Index of the top of the stack */
	void **stack;		/**< Array of items */
};


stack_t
new_stack(void)
{
	struct stack_st *n;

	n = xmalloc(sizeof(struct stack_st));

	n->top = 0;
	n->len = STACK_DEFAULT_LEN;
	n->stack = xcalloc(n->len, sizeof(void *));

	return n;
}

int
delete_stack(stack_t self)
{
	if (!self)
		return -1;

	xfree(self->stack);
	xfree(self);

	return 0;
}


static void
stack_grow(stack_t self)
{
	assert(self);

	self->len *= 2;
	self->stack = xrealloc(self->stack, self->len * sizeof(void *));
}

void
stack_push(stack_t self, void *item)
{
	assert(self && item);

	if (self->top == self->len)
		stack_grow(self);

	self->stack[self->top++] = item;
}

void *
stack_pop(stack_t self)
{
	assert(self);

	if (self->top == 0)
		return NULL;

	return self->stack[--self->top];
}

void *
stack_peek(const stack_t self)
{
	assert(self);
	
	if (self->top == 0)
		return NULL;

	return self->stack[self->top - 1];
}
