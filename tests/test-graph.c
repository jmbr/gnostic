/*
 * test-graph.c -- Unit test for graphs of tasks.
 * $Id$
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdio.h>
# include <stdlib.h>
#endif /* STDC_HEADERS */

#include <assert.h>

#include "task.h"
#include "graph.h"

#include "xalloc.h"


#define make_task(name)		new_task(xstrdup(name), NULL, xstrdup("#"))


int
main(int argc, char *argv[])
{
	graph *deps;
	struct task *p, *q, *r, *s, *t, *u, *v;
	struct task **list[] = { &p, &q, &r, &s, &t, &u, &v, NULL };

	assert(( p = make_task("tcp") ));
	assert(( q = make_task("http") ));
	assert(( r = make_task("postgres") ));
	assert(( s = make_task("php") ));
	assert(( t = make_task("udp") ));
	assert(( u = make_task("onc-rpc") ));
	assert(( v = make_task("phpnuke") ));

	assert(( deps = new_graph() ));

	graph_insert_edge(deps, p, q);
	graph_insert_edge(deps, p, r);
	graph_insert_edge(deps, q, s);
	graph_insert_edge(deps, p, u);
	graph_insert_edge(deps, t, u);
	graph_insert_edge(deps, s, v);

	deps = graph_topological_sort(deps, NULL);

	graph_print_dot(deps, stdout, (graph_print_fn) task_get_name);

	for (int i = 0; list[i] != NULL; i++)
		delete_task(*list[i]);
	delete_graph(deps);
	exit(EXIT_SUCCESS);
}
