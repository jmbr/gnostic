#ifndef GRAPH_H
#define GRAPH_H			1
/*
 * graph.h -- Interface for a graph data structure.
 * $Id$
 */


struct vertex;

/* Vertex decorator */
struct vertexnode {
	struct vertex *v;
	struct vertexnode *prev, *next;
};

typedef struct vertices {
	size_t len;
	struct vertexnode *head, *tail;
} graph, *graph_t;

typedef char *(*graph_print_fn)(const void *);

typedef void (*graph_errback_fn)(void *, void *);


/*
 * Vertex methods.
 */
extern void *vertex_get_item(const struct vertex *self);
extern struct vertices *vertex_get_adjacent(const struct vertex *self);
extern struct vertexnode *vertexnode_dup(const struct vertexnode *vn);

/*
 * Vertex stack primitives.
 */
extern void vertices_push(struct vertices *vs, struct vertexnode *vn);
extern struct vertexnode *vertices_pop(struct vertices *vs);
extern struct vertexnode *vertices_peek(struct vertices *vs);


/*
 * Graph constructor and destructor.
 */
extern graph *new_graph(void);
extern void delete_graph(graph *self);

/*
 * The caller must take responsibility for deallocation of items contained in
 * the graph.
 */
extern struct vertexnode *graph_add_vertex(graph *self, void *item);
extern void graph_add_edge(graph *self, struct vertexnode *vp, struct vertexnode *vq);
extern void graph_insert_edge(graph *self, void *a, void *b);

extern void graph_init(graph *self); /* Initialize all vertices */
/* Returns a new graph, the caller must free the old, unsorted one. */
extern graph *graph_topological_sort(graph *self, graph_errback_fn errback);


extern void graph_print(const graph *self, FILE *fp, graph_print_fn fn);
extern void graph_print_dot(const graph *self, FILE *fp, graph_print_fn fn);


#endif /* !GRAPH_H */
