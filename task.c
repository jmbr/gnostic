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


struct task {
	char *name;
	char *actions;
	astnode_t expr;
	struct task *next;
};


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


inline const char *
task_get_name(const struct task *self)
{
	if (!self)
		return NULL;

	assert(self->name);

	return (const char *) self->name;
}

const char *
task_get_actions(const struct task *self)
{
	if (!self)
		return NULL;

	assert(self->actions);

	return (const char *) self->actions;
}

const astnode_t
task_get_expr(const struct task *self)
{
	if (!self)
		return NULL;

	return (const astnode_t) self->expr;
}

int
task_set_next(struct task *self, struct task *t)
{
	if (!self)
		return -1;

	assert(!self->next);
	self->next = t;

	return 0;
}

inline struct task *
task_get_next(const struct task *self)
{
	assert(self);

	return self->next;
}


int
tasklist_print(const struct task *head, FILE *fp)
{
	const struct task *t;

	if (!head || !fp)
		return -1;

	for (t = head; t; t = task_get_next(t))
		if (fprintf(fp, "%s\n", task_get_name(t)) < 0)
			return -1;

	return 0;
}
