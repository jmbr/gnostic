/**
 * @file task.c
 * @brief Task management module.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
 *
 * Tasks are at the heart of gnostic. They describe how something should be
 * done and when. Tasks are read from gnostic configuration files and stored in
 * a taskset for future invocation.
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

#include "task-priv.h"

#include "err.h"
#include "xmemory.h"


struct task *
new_task(char *name, astnode_t expr, char *actions)
{
	struct task *n;

	n = xmalloc(sizeof(struct task));

	n->next = NULL;
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



const char *
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

struct task *
task_get_next(const struct task *self)
{
	assert(self);

	return self->next;
}



/*
 * We provide an extra parameter prev. This is useful because the alternative
 * is to have a pointer in each astnode to its parent.
 */
static int
check_deps(const struct task *cur, const struct task *prev, astnode_t n)
{
	int status = 0;
	struct task *u;

	assert(cur);

	if (!n)
	       return 0;

	switch (astnode_get_type(n)) {
	case N_ID:
		u = astnode_get_item(n);

		/* FIXME Improve readability. */
		if (cur != u)
			status = check_deps(cur, u, task_get_expr(u));
		else
			fatal_error("gnostic: There are circular dependencies "
				    "between " "`%s' and `%s'.\n",
				    task_get_name(cur),
				    task_get_name((prev) ? prev : cur));
		break;
	case N_AND:
	case N_OR:
		status = check_deps(cur, NULL, astnode_get_lhs(n))
			+ check_deps(cur, NULL, astnode_get_rhs(n));
		break;
	case N_NOT:
		status = check_deps(cur, NULL, astnode_get_rhs(n));
		break;
	default:
		assert(0);
	}

	return status;
}

int
task_check_deps(const struct task *self)
{
	astnode_t n;

	n = task_get_expr(self);
	if (!n)
		return 0;

	return (check_deps(self, NULL, n) == 0) ? 0 : -1;
}


int
task_print(const struct task *self)
{
	if (!self)
		return -1;

	if (fprintf(stdout, "%s\n", self->name) < 0)
		return -1;

	return 0;
}
