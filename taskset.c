/*
 * taskset.c -- An aggregate of tasks.
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

#include "taskset.h"
#include "taskset-priv.h"

#include "task.h"
#include "task-priv.h"
#include "htab.h"
#include "graph.h"

#include "xalloc.h"
#include "debug.h"


/* XXX Having this here is not very elegant.
 */
extern struct env_var *env_vars;

extern struct task *tasklist_parse(const char *filename,
					htab_t symtab, graph_t depgraph);


static int delete_tasklist(struct task *self);
static int tasklist_print(const struct task *head, FILE *fp);

static int delete_env_var_list(struct env_var *self);


/** Builds a taskset from a file.
 * Reads, aggregates and checks the validity of tasks coming from a file.
 *
 * @param self A pointer to the structure.
 * @param name Name of the configuration file to parse.
 *
 * @return 0 on success, -1 on failure.
 */
static int taskset_read(struct taskset *self, const char *name);


struct taskset *
new_taskset(const char *filename)
{
	struct taskset *n;

	if (!filename)
		return NULL;

	n = xmalloc(sizeof(struct taskset));

	n->tasks = NULL;
	n->symtab = new_htab((htab_cmp) strcmp);
	n->depgraph = new_graph();

	if (taskset_read(n, filename) == -1) {
		delete_taskset(n);
		n = NULL;
	}

	return n;
}

int
delete_taskset(struct taskset *self)
{
	if (!self)
		return -1;

	delete_env_var_list(self->env_vars);
	delete_tasklist(self->tasks);
	delete_htab(self->symtab);
	delete_graph(self->depgraph);
	xfree(self);

	return 0;
}


int
taskset_print(const struct taskset *self, FILE *fp)
{
	return tasklist_print(self->tasks, fp);
}


int
delete_tasklist(struct task *self)
{
	int status;
	struct task *node, *next;

	if (!self)
		return -1;

	for (node = self; node; node = next) {
		next = task_get_next(node);
		status = delete_task(node);
		assert(status == 0);
	}

	return 0;
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


static void
errback(struct task *a, struct task *b)
{
	fatal_error("gnostic: There are circular dependencies between "
		"`%s' and `%s'.\n", task_get_name(a), task_get_name(b));
}

static int
taskset_verify(struct taskset *self)
{
	if (!self)
		return -1;

	self->depgraph = graph_topological_sort(self->depgraph,
						(graph_errback_fn) errback);

	return 0;
}

static int
taskset_read(struct taskset *self, const char *name)
{
	if (!self || !name)
		return -1;

	self->tasks = tasklist_parse(name, self->symtab, self->depgraph);
	if (!self->tasks)
		return -1;

	self->env_vars = env_vars;

	return taskset_verify(self);
}


const struct task *
taskset_get_task(const struct taskset *self, const char *name)
{
	return htab_lookup_s(self->symtab, name, 0, NULL);
}


const struct env_var *
taskset_get_env_vars(const struct taskset *self)
{
	assert(self);

	return self->env_vars;
}


int
delete_env_var_list(struct env_var *self)
{
	struct env_var *n, *next;

	if (!self)
		return -1;

	for (n = self; n; n = next) {
		next = n->next;
		xfree(n->v);
		xfree(n);
	}

	return 0;
}
