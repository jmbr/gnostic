/*
 * gnostic.c -- Generic Network Scanning Tool.
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

#include "gnostic.h"

#include "htab.h"
#include "task.h"
#include "graph.h"
#include "exec.h"

#include "version.h"
#include "debug.h"
#include "xalloc.h"


#define PROG_NAME		"gnostic"


static void usage(void) __attribute__ ((noreturn));

static int gnostic_check_deps(struct gnostic *self);



int
main(int argc, char *argv[])
{
	struct task *t;
	struct gnostic *g;
	int status = EXIT_SUCCESS;

	if (argc < 2)
		usage();

	g = new_gnostic();

	if (gnostic_conf_parse(g, argv[1]) == -1) {
		eprintf("gnostic: Unable to parse %s\n", argv[1]);
		delete_gnostic(g);
		exit(EXIT_FAILURE);
	}

	gnostic_check_deps(g);

	if (argc == 2) {
		for (t = g->tasks; t; t = t->next)
			printf("%s\n", t->name);
	} else {
		t = htab_lookup_s(g->symtab, argv[2], 0, NULL);
		if (!t)
			fatal_error("gnostic: Unknown task `%s'.\n", argv[2]);

		status = execute(t, &argv[3]);
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
			"`%s' and `%s'.\n", a->name, b->name);
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
