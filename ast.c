/**
 * @file ast.c
 * @brief Abstract syntax tree for dependency expressions.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdio.h>
# include <stdlib.h>
#endif /* STDC_HEADERS */

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */

#include <assert.h>

#include "ast-priv.h"

#include "xmemory.h"


ast_t
new_ast(enum ast_type type, ast_t lhs, ast_t rhs)
{
	struct ast_st *n;

	n = xmalloc(sizeof(struct ast_st));

	n->item = NULL;
	n->type = type;
	n->children[0] = lhs;
	n->children[1] = rhs;

	return n;
}

int
delete_ast(ast_t self, ast_item_dtor dtor)
{
	int i;

	if (!self)
		return -1;

	if (self->item && dtor) {
		int status;

		status = dtor(self->item);
		assert(status == 0);
	}

	for (i = 0; i < AST_MAX_CHILDREN; i++)
		if (self->children[i])
			if (delete_ast(self->children[i], dtor) == -1)
				return -1;

	xfree(self);

	return 0;
}



enum ast_type
ast_get_type(const ast_t self)
{
	if (!self)
		return (enum ast_type) 0;

	return self->type;
}


int
ast_set_item(ast_t self, void *item)
{
	if (!self)
		return -1;

	self->item = item;

	return 0;
}

void *
ast_get_item(const ast_t self)
{
	if (!self)
		return NULL;

	return self->item;
}


ast_t
ast_get_lhs(const ast_t self)
{
	if (!self)
		return NULL;

	return self->children[0];
}

ast_t
ast_get_rhs(const ast_t self)
{
	if (!self)
		return NULL;

	return self->children[1];
}
