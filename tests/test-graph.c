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


static char *get_task_name(const struct task *t);


int
main(int argc, char *argv[])
{
	graph *deps;
	//struct graph_itor *itor;
	struct task *p, *q, *r, *s, *t, *u, *v;

	/*
	 * We strdup task names to keep memory checkers happy.
	 */
	p = new_task(NULL, NULL, NULL);
	assert(p);
	p->name = xstrdup("tcp");

	q = new_task(NULL, NULL, NULL);
	assert(q);
	q->name = xstrdup("http");

	r = new_task(NULL, NULL, NULL);
	assert(r);
	r->name = xstrdup("postgres");

	s = new_task(NULL, NULL, NULL);
	assert(s);
	s->name = xstrdup("php");

	t = new_task(NULL, NULL, NULL);
	assert(t);
	t->name = xstrdup("udp");
	
	u = new_task(NULL, NULL, NULL);
	assert(u);
	u->name = xstrdup("onc-rpc");

	v = new_task(NULL, NULL, NULL);
	assert(v);
	v->name = xstrdup("phpnuke");

	deps = new_graph();
	assert(deps);

	graph_insert_edge(deps, p, q);
	graph_insert_edge(deps, p, r);
	graph_insert_edge(deps, q, s);
	graph_insert_edge(deps, p, u);
	graph_insert_edge(deps, t, u);
	graph_insert_edge(deps, s, v);

	deps = graph_topological_sort(deps, NULL);

	graph_print_dot(deps, stdout, (graph_print_fn) get_task_name);

	/*
	itor = new_graph_itor(deps);

	for (p = graph_itor_first(itor); p; p = graph_itor_next(itor)) {
		assert(p);
		task_print(p, stdout);
		printf("\n");
	}

	delete_graph_itor(itor);
	*/
	delete_task(p);
	delete_task(q);
	delete_task(r);
	delete_task(s);
	delete_task(t);
	delete_task(u);
	delete_task(v);
	delete_graph(deps);
	exit(EXIT_SUCCESS);
}


char *
get_task_name(const struct task *t)
{
	assert(t);

	return t->name;
}
