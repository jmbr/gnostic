/*
 * task.c -- Task management module.
 * $Id$
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

#ifdef HAVE_STRING_H
# include <string.h>
#elif HAVE_STRINGS_H
# include <strings.h>
#endif /* !HAVE_STRING_H */

#include <assert.h>

#include "task.h"
#include "ast.h"

#include "xalloc.h"


struct task *
new_task(char *name, astnode_t expr, char *actions)
{
	struct task *n;

	n = xmalloc(sizeof(struct task));

	memset(n, 0, sizeof(struct task));

	n->name = name;
	n->expr = expr;
	n->actions = actions;

	return n;
}

int
delete_task(struct task *self)
{
	if (!self)
		return -1;

	xfree(self->name);
	xfree(self->actions);
	(void) delete_ast(self->expr);
	xfree(self);

	return 0;
}


int
delete_tasklist(struct task *self)
{
	int status;
	struct task *node, *next;

	if (!self)
		return -1;

	for (node = self; node; node = next) {
		next = node->next;
		status = delete_task(node);
		assert(status == 0);
	}

	return 0;
}
