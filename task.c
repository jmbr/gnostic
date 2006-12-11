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

#include "logger.h"
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


struct task *
new_task(char *name, ast_t expr, char *actions)
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
	(void) delete_ast(self->expr, (ast_item_dtor) task_decref);
	xfree(self);

	return 0;
}

/* XXX task_*ref will be useless once tasklist is removed */

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


static void
check_deps(const struct task *orig, const struct task *prev, ast_t expr)
{
	ast_t n;
	struct task *current;
	ast_itor_t itor;

	assert(orig);
	
	itor = new_ast_itor(expr);

	for (n = ast_itor_first(itor); n; n = ast_itor_next(itor)) {
		if (ast_get_type(n) != AST_ID)
			continue;

		current = ast_get_item(n);

		if (strcmp(orig->name, current->name) == 0)
			fatal_error("Circular dependency between `%s' and"
				    " `%s'.\n", prev ? prev->name : orig->name,
				    current->name);

		if (current->expr)
			check_deps(orig, current, current->expr);
	}

	delete_ast_itor(itor);
}

int
task_check_deps(const struct task *self)
{

	if (!self)
		return -1;

	if (self->expr)
		check_deps(self, NULL, self->expr);

	return 0;
}
