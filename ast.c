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
struct astnode {
	void *item;					/**< Payload */
	enum astnode_types type;			/**< Node type */
	struct astnode *children[AST_MAXCHILDREN];	/**< Children */
};


astnode_t
new_astnode(enum astnode_types type, astnode_t lhs, astnode_t rhs)
{
	struct astnode *n;

	n = xmalloc(sizeof(struct astnode));

	n->item = NULL;
	n->type = type;
	n->children[0] = lhs;
	n->children[1] = rhs;

	return n;
}

int
delete_astnode(struct astnode *self)
{
	if (!self)
		return -1;

	xfree(self);

	return 0;
}



enum astnode_types
astnode_get_type(const astnode_t self)
{
	if (!self)
		return (enum astnode_types) 0;

	return self->type;
}


astnode_t
astnode_get_lhs(const astnode_t self)
{
	if (!self)
		return NULL;

	return self->children[0];
}

astnode_t
astnode_get_rhs(const astnode_t self)
{
	if (!self)
		return NULL;

	return self->children[1];
}


int
astnode_set_item(astnode_t self, void *item)
{
	if (!self)
		return -1;

	self->item = item;

	return 0;
}

void *
astnode_get_item(const astnode_t self)
{
	if (!self)
		return NULL;

	return self->item;
}



int
delete_ast(astnode_t self, astnode_item_dtor dtor)
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
