/**
 * @file ast-itor.c
 * @brief Abstract syntax tree iterator.
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

#include "ast-priv.h"

#include "stack.h"
#include "xmemory.h"


struct ast_itor_st {
	ast_t root, next;
	stack_t stack;
};


ast_itor_t
new_ast_itor(ast_t root)
{
	struct ast_itor_st *n;

	n = xmalloc(sizeof(struct ast_itor_st));

	n->root = n->next = root;
	n->stack = new_stack();

	return n;
}

int
delete_ast_itor(ast_itor_t self)
{
	ast_t n;

	if (!self)
		return -1;

	while ((n = stack_pop(self->stack)))
		xfree(n);
	if (delete_stack(self->stack) == -1)
		return -1;
	xfree(self);

	return 0;
}



static void
push(ast_itor_t itor, ast_t node)
{
	assert(itor);

	if (node)
		stack_push(itor->stack, node);
}

static ast_t
pop(ast_itor_t itor)
{
	assert(itor);

	return stack_pop(itor->stack);
}


ast_t
ast_itor_first(ast_itor_t self)
{
	ast_t n;

	assert(self);

	n = self->next = self->root;
	push(self, ast_get_rhs(n));
	self->next = ast_get_lhs(n);

	return n;
}

ast_t
ast_itor_next(ast_itor_t self)
{
	ast_t n;

	assert(self);

	if (!self->next) {
		self->next = pop(self);
		if (!self->next)
			return NULL;
	}

	n = self->next;
	push(self, ast_get_rhs(n));
	self->next = ast_get_lhs(n);

	return n;
}
