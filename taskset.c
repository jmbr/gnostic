/**
 * @file taskset.c
 * @brief Management of task aggregates.
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

#ifdef HAVE_STRING_H
# include <string.h>
#elif HAVE_STRINGS_H
# include <strings.h>
#endif /* !HAVE_STRING_H */

#include <assert.h>

#include "taskset-priv.h"

#include "err.h"
#include "xmemory.h"


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
	n->symtab = new_hashtab(0, (hashtab_cmp) strcmp, (hashtab_dtor) task_decref);

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

	delete_vars(self->vars);
	delete_tasklist(self->tasks);
	delete_hashtab(self->symtab);
	xfree(self);

	return 0;
}



static int
taskset_verify(struct taskset *self)
{
	if (!self)
		return -1;

	return tasklist_map(self->tasks, (tasklist_fn) task_check_deps);
}

static int
taskset_read(struct taskset *self, const char *name)
{
	if (!self || !name)
		return -1;

	if (taskset_parse(self, name) == -1)
		return -1;

	return taskset_verify(self);
}


int
taskset_print(const struct taskset *self)
{
	if (!self)
		return -1;

	return tasklist_map(self->tasks, (tasklist_fn) task_print);
}


const struct task *
taskset_get_task(const struct taskset *self, const char *name)
{
	return hashtab_strlookup(self->symtab, name, 0, NULL);
}


const struct var *
taskset_get_vars(const struct taskset *self)
{
	assert(self);

	return self->vars;
}
