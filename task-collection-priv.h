#ifndef TASK_COLLECTION_PRIV_H
#define TASK_COLLECTION_PRIV_H		1
/*
 * task-collection-priv.h -- Private declarations for task collections.
 */

#include "task-collection.h"
#include "task.h"
#include "htab.h"
#include "graph.h"


/** Aggregates tasks.
 */
struct task_collection {
	struct env_var *env_vars;	/**< Pointer to the first node of a
					  list of env vars */
	struct task *tasks;		/**< Pointer to the first node of a
					  list of tasks. */
	htab_t symtab;			/**< Symbol table mapping task names to
					  task structure addresses */
	graph_t depgraph;		/**< Describes the dependencies between
					  task structures. */
};


#endif /* !TASK_COLLECTION_PRIV_H */
