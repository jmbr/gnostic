/*
 * gnostic.c -- Generic Network Scanning Tool.
 */
/*
 * gnostic: adjective 1 of knowledge. 2 having special mystic knowledge.
 * noun (Gnostic) early Christian heretic claiming mystical knowledge.
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

#include "htab.h"
#include "task.h"
#include "graph.h"

#include "version.h"
#include "debug.h"
#include "xalloc.h"


#define PROG_NAME		"gnostic"


struct gnostic {
	htab_t symtab;  	/**< Symbol table */
	graph_t depgraph;	/**< Dependency graph */
	struct task *tasks;	/**< Task list */
};


static void usage(void) __attribute__ ((noreturn));

static struct gnostic *new_gnostic(void);
static int delete_gnostic(struct gnostic *self);

static int gnostic_check_deps(struct gnostic *self);

static void update_environ(int nvars, char *vars[]);
static int gnostic_exec(const struct gnostic *self, const char *name);



int
main(int argc, char *argv[])
{
	struct gnostic *g;
	int status = EXIT_SUCCESS;

	if (argc < 2)
		usage();

	g = new_gnostic();

	g->tasks = tasklist_parse(argv[1], g->symtab, g->depgraph);
	if (!g->tasks) {
		eprintf("gnostic: Unable to parse %s\n", argv[1]);
		delete_gnostic(g);
		exit(EXIT_FAILURE);
	}

	gnostic_check_deps(g);

	if (argc == 2)
		status = tasklist_print(g->tasks, stdout);
	else {
		update_environ(argc - 3, &argv[3]);
		gnostic_exec(g, argv[2]);
	}
	delete_gnostic(g);
	exit((status == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}


void
usage(void)
{
	fprintf(stderr, "%s\n\n", version.v_gnu);
	fprintf(stderr, "Usage: %s <config file> [task] [name=value] ...\n",
		PROG_NAME);

	exit(EXIT_FAILURE);
}


struct gnostic *
new_gnostic(void)
{
	struct gnostic *n;

	n = malloc(sizeof(struct gnostic));
	if (!n)
		return NULL;

	memset(n, 0, sizeof(struct gnostic));

	n->depgraph = new_graph();
	n->symtab = new_htab((htab_cmp) strcmp);

	return n;
}

int
delete_gnostic(struct gnostic *self)
{
	if (!self)
		return -1;

	delete_tasklist(self->tasks);
	delete_htab(self->symtab);
	delete_graph(self->depgraph);
	xfree(self);

	return 0;
}


void
errback(struct task *a, struct task *b)
{
	fatal_error("gnostic: There are circular dependencies between "
		"`%s' and `%s'.\n", task_get_name(a), task_get_name(b));
}

int
gnostic_check_deps(struct gnostic *self)
{
	if (!self)
		return -1;

	assert(self->depgraph);

	self->depgraph = graph_topological_sort(self->depgraph,
						(graph_errback_fn) errback);

	return 0;
}


void
update_environ(int nvars, char *vars[])
{
	int i;

	for (i = 0; i < nvars; i++)
		if (putenv(vars[i]) == -1)
			fatal_error("gnostic: Unable to declare %s", vars[i]);
}


int
gnostic_exec(const struct gnostic *self, const char *name)
{
	int status;
	struct task *t;

	t = htab_lookup_s(self->symtab, name, 0, NULL);
	if (!t)
		fatal_error("gnostic: Unknown task `%s'.\n", name);

	dprintf("gnostic: Executing `%s'.\n", name);
	status = task_exec(t);
	dprintf("gnostic: Task `%s' exited with status %d.\n", name, status);

	return status;
}
