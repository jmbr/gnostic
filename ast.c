/*
 * ast.c -- Abstract syntax tree for dependency expressions.
 * $Id$
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

#include "xalloc.h"


/** Nodes from an abstract syntax tree.
 * It is the user's responsibility to free any memory pointed to by the item
 * field.
 */
struct astnode {
	enum astnode_types type;	/**< Node type */
	void *item;			/**< Payload */
	astnode_t lhs;			/**< Left hand side */
	astnode_t rhs;			/**< Right hand side */
};


astnode_t
new_astnode(enum astnode_types type, astnode_t lhs, astnode_t rhs)
{
	struct astnode *n;

	n = malloc(sizeof(struct astnode));
	if (!n)
		return NULL;

	memset(n, 0, sizeof(struct astnode));

	n->type = type;
	n->lhs = lhs;
	n->rhs = rhs;

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

	return self->lhs;
}

astnode_t
astnode_get_rhs(const astnode_t self)
{
	if (!self)
		return NULL;

	return self->rhs;
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
delete_ast(astnode_t self)
{
	if (!self)
		return -1;

	/* Remember: self->item should not be freed by this function. */

	if (self->lhs)
		delete_ast(self->lhs);
	if (self->rhs)
		delete_ast(self->rhs);

	xfree(self);

	return 0;
}
