/*
 * graph.c -- Implementation of a graph data structure describing dependencies
 * between tasks.
 * $Id$
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdio.h>
# include <stdlib.h>
#endif /* STDC_HEADERS */

#ifdef HAVE_STRING_H
# include <string.h>
#elif HAVE_STRINGS_H
# include <strings.h>
#endif /* !HAVE_STRING_H */

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */

#include <assert.h>

#include "graph.h"
#include "graph-priv.h"

#include "debug.h"
#include "xalloc.h"


/* Elapsed time for timestamping vertices during search */
/* This is absolutely non-reentrant */
static int elapsed;


static struct vertex *new_vertex(void *item);
static void delete_vertex(struct vertex *self);
/* xxx - This function is very expensive if there is a large number of probes */
static struct vertex *vertex_lookup(struct vertices *vs, void *item, int append);

struct vertexnode *new_vertexnode(struct vertex *v);
static void delete_vertexnode(struct vertexnode *vn);
static void vertexnode_unlink(struct vertexnode *vn);

static void vertices_prepend(struct vertices *vs, struct vertexnode *vn);
static void vertices_append(struct vertices *vs, struct vertexnode *vn);
static void delete_vertices(struct vertices *vs);

static void graph_dfs(graph *self, struct vertices *vs, graph_errback_fn errback);
static void dfs_visit(struct vertex *u, struct vertices *vs, graph_errback_fn errback);



struct vertex *
new_vertex(void *item)
{
	struct vertex *v;

	assert(item);

	v = xmalloc(sizeof(struct vertex));

	v->item = item;

	v->adjacent = xmalloc(sizeof(struct vertices));

	return v;
}

void
delete_vertex(struct vertex *self)
{
	assert(self);

	delete_vertices(self->adjacent);
	free(self);
}


struct vertex *
vertex_lookup(struct vertices *vs, void *item, int append)
{
	struct vertexnode *vn;

	assert(vs && item);

	for (vn = vs->head; vn; vn = vn->next)
		if (vn->v->item == item)
			return vn->v;

	if (append) {
		vn = new_vertexnode(new_vertex(item));
		vertices_append(vs, vn);
		return vn->v;
	}
	return NULL;
}


void *
vertex_get_item(const struct vertex *self)
{
	assert(self);
	
	return self->item;
}


struct vertices *
vertex_get_adjacent(const struct vertex *self)
{
	assert(self);

	return self->adjacent;
}



struct vertexnode *
new_vertexnode(struct vertex *v)
{
	struct vertexnode *vn;

	assert(v);

	vn = xmalloc(sizeof(struct vertexnode));
	vn->v = v;

	return vn;
}

void
delete_vertexnode(struct vertexnode *vn)
{
	assert(vn);

	vertexnode_unlink(vn);
	free(vn);
}


void
vertexnode_unlink(struct vertexnode *vn)
{
	assert(vn);

	if (vn->prev)
		vn->prev->next = vn->next;
	if (vn->next)
		vn->next->prev = vn->prev;

	vn->prev = vn->next = NULL;
}


struct vertexnode *
vertexnode_dup(const struct vertexnode *vn)
{
	assert(vn);

	return new_vertexnode(vn->v);
}



void
vertices_prepend(struct vertices *vs, struct vertexnode *vn)
{
	assert(vs && vn);

	if (vs->len == 0)
		vs->head = vs->tail = vn;
	else {
		vn->prev = NULL;
		vn->next = vs->head;
		vs->head->prev = vn;
		vs->head = vn;
	}

	++vs->len;
}

void
vertices_append(struct vertices *vs, struct vertexnode *vn)
{
	assert(vs && vn);

	if (vs->len == 0)
		vs->head = vs->tail = vn;
	else {
		vn->prev = vs->tail;
		vn->next = NULL;
		vs->tail->next = vn;
		vs->tail = vn;
	}

	++vs->len;
}


void
vertices_push(struct vertices *vs, struct vertexnode *vn)
{
	assert(vs && vn);

	/* The append method already increases the len field */
	vertices_append(vs, vn);
}

struct vertexnode *
vertices_peek(struct vertices *vs)
{
	assert(vs);

	return vs->head;
}

struct vertexnode *
vertices_pop(struct vertices *vs)
{
	struct vertexnode *vn;

	assert(vs);

	vn = vertices_peek(vs);
	if (!vn)
		return NULL;

	vs->head = vn->next;
	vertexnode_unlink(vn);

	--vs->len;

	return vn;
}


void
delete_vertices(struct vertices *vs)
{
	struct vertexnode *vn, *next;

	assert(vs);

	for (vn = vs->head; vn; vn = next, vs->len--) {
		next = vn->next;
		delete_vertexnode(vn);
	}

	free(vs);
}



graph *
new_graph(void)
{
	graph *g;

	g = xmalloc(sizeof(graph));
	memset(g, 0, sizeof(graph));

	return g;
}

void
delete_graph(struct vertices *self)
{
	struct vertexnode *vn, *next;

	assert(self);

	for (vn = self->head; vn; vn = next, self->len--) {
		next = vn->next;
		delete_vertex(vn->v);
		delete_vertexnode(vn);
	}

	assert(self->len == 0);

	free(self);
}


struct vertexnode *
graph_add_vertex(graph *self, void *item)
{
	struct vertexnode *vn;

	assert(self && item);

	vn = new_vertexnode(new_vertex(item));

	vertices_append(self, vn);

	return vn;
}

void
graph_add_edge(graph *self, struct vertexnode *vp, struct vertexnode *vq)
{
	assert(self && vp && vq);

	vertices_append(vp->v->adjacent, vq);
}

void
graph_insert_edge(graph *self, void *a, void *b)
{
	struct vertex *u, *v;

	assert(self && a && b);

	u = vertex_lookup(self, a, 1);
	v = vertex_lookup(self, b, 1);

	vertices_append(u->adjacent, new_vertexnode(v));
}



void
graph_print(const graph *self, FILE *fp, graph_print_fn fn)
{
	struct vertex *u;
	struct vertexnode *vn;

	assert(self && fp && fn);

	for (vn = self->head; vn; vn = vn->next) {
		u = vn->v;

		fprintf(fp, "%s\n", fn(u->item));
	}
}

void
graph_print_dot(const graph *self, FILE *fp, graph_print_fn fn)
{
	struct vertex *u, *v;
	struct vertexnode *vn, *an;

	assert(self && fp && fn);

	fprintf(fp, "digraph G {\n\trankdir=LR;\n\n");

	for (vn = self->head; vn; vn = vn->next) {
		u = vn->v;

		for (an = u->adjacent->head; an; an = an->next) {
			v = an->v;

			fprintf(fp, "\t\"%s\" -> \"%s\";\n",
				fn(u->item), fn(v->item));
		}
	}

	fprintf(fp, "}\n");
}



void
graph_init(graph *self)
{
	struct vertex *u;
	struct vertexnode *vn;

	assert(self);

	for (vn = self->head; vn; vn = vn->next) {
		u = vn->v;

		u->color = white;
		u->pred = NULL;
	}

	elapsed = 0;
}

graph *
graph_topological_sort(graph *self, graph_errback_fn errback)
{
	struct vertices *vs;

	assert(self);

	vs = new_graph();
	
	graph_dfs(self, vs, errback);

	delete_vertices(self);

	return vs;
}

void
graph_dfs(graph *self, struct vertices *vs, graph_errback_fn errback)
{
	struct vertex *u;
	struct vertexnode *vn;

	assert(self);

	graph_init(self);

	for (vn = self->head; vn; vn = vn->next) {
		u = vn->v;

		if (u->color == white)
			dfs_visit(u, vs, errback);
	}
}



void
dfs_visit(struct vertex *u, struct vertices *vs, graph_errback_fn errback)
{
	struct vertex *v;
	struct vertexnode *vn;

	assert(u);

	u->color = gray;
	u->discovered = ++elapsed;

	for (vn = u->adjacent->head; vn; vn = vn->next) {
		v = vn->v;

		if (v->color == white) {
			v->pred = u;
			dfs_visit(v, vs, errback);
		} else if (v->color == gray) {
			if (!errback)
				fatal_error("Cycle detected between "
					"%p and %p!\n", u->item, v->item);
			errback(u->item, v->item);
		}
	}

	u->color = black;
	u->finished = ++elapsed;

	if (vs) vertices_prepend(vs, new_vertexnode(u));
}
