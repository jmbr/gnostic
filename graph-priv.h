#ifndef GRAPH_PRIV_H
#define GRAPH_PRIV_H		1
/*
 * graph-priv.h -- Private side of the graph data structure.
 * $Id$
 */


/*
 * The graph is represented as a collection of adjacency lists.
 */

enum vertex_colors {
	white = 0,			/* Not visited */
	gray,				/* In progress */
	black				/* Visited */
};

struct vertex {
	void *item;			/* Stored item */
	struct vertices *adjacent;	/* Adjacency list */
	enum vertex_colors color;	/* Color (for exploration) */
	int discovered, finished;	/* Timestamps */
	struct vertex *pred;		/* Link to its predecessor */
};

/*
 * Vertices will have to be placed at different lists simultaneously. That's
 * why we can't just put a prev and next pointer into the vertex structure. We
 * have to do that outside of it. We need vertexnodes.
 */


#endif /* !GRAPH_PRIV_H */
