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

#include "ast.h"

#include "xmemory.h"


#define AST_MAXCHILDREN		2


/** Node of an abstract syntax tree.
 *
 * These nodes conform the structure that will be traversed during evaluation
 * of dependency expressions.
 */
struct ast {
	void *item;				/**< Payload */
	enum ast_types type;			/**< Node type */
	struct ast *children[AST_MAXCHILDREN];	/**< Children */
};


ast_t
new_ast(enum ast_types type, ast_t lhs, ast_t rhs)
{
	struct ast *n;

	n = xmalloc(sizeof(struct ast));

	n->item = NULL;
	n->type = type;
	n->children[0] = lhs;
	n->children[1] = rhs;

	return n;
}



enum ast_types
ast_get_type(const ast_t self)
{
	if (!self)
		return (enum ast_types) 0;

	return self->type;
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

	for (i = 0; i < AST_MAXCHILDREN; i++)
		if (self->children[i])
			if (delete_ast(self->children[i], dtor) == -1)
				return -1;

	xfree(self);

	return 0;
}
