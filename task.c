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

#include "task.h"

#include "err.h"
#include "xmemory.h"


/** task destructor.
 * Frees all the resources associated to the task.
 *
 * @param self A pointer to the structure.
 * @return 0 on success, -1 on failure.
 *
 * @see new_task
 */
static int delete_task(struct task *self);

static int check_deps(const struct task *cur, const struct task *prev,
		      astnode_t n);


struct task *
new_task(char *name, astnode_t expr, char *actions)
{
	struct task *n;

	assert(name && actions);

	n = xmalloc(sizeof(struct task));

	n->refs = 1;
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

	assert(self->refs == 0);

	xfree(self->name);
	xfree(self->actions);
	(void) delete_ast(self->expr, (astnode_item_dtor) task_decref);
	xfree(self);

	return 0;
}

int
task_incref(struct task *self)
{
	if (!self)
		return -1;

	assert(self->refs > 0);

	++self->refs;

	return 0;
}

int
task_decref(struct task *self)
{
	if (!self)
		return -1;

	assert(self->refs > 0);

	--self->refs;

	if (self->refs == 0)
		return delete_task(self);

	return 0;
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


/*
 * TODO Using an AST iterator would *greatly* clarify this piece of code
 */

static int
check_ident(const struct task *cur, const struct task *prev, astnode_t n)
{
	const struct task *u; 

	assert(cur && n);

	u = astnode_get_item(n);

	if (cur == u)
		fatal_error("gnostic: There are circular dependencies between "
			    "`%s' and `%s'.\n", cur->name,
			    (prev) ? prev->name : cur->name);

	return check_deps(cur, u, u->expr);
}

int
check_deps(const struct task *cur, const struct task *prev, astnode_t n)
{
	int status = 0;

	assert(cur);

	if (!n)
	       return 0;

	switch (astnode_get_type(n)) {
	case AST_ID:
		status = check_ident(cur, prev, n);
		break;
	case AST_AND:
	case AST_OR:
		status = check_deps(cur, NULL, astnode_get_lhs(n))
			 + check_deps(cur, NULL, astnode_get_rhs(n));
		break;
	case AST_NOT:
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
	return (check_deps(self, NULL, self->expr) == 0) ? 0 : -1;
}
