#ifndef TASKSET_PRIV_H
#define TASKSET_PRIV_H		1
/*
 * taskset-priv.h -- Private declarations for task collections.
 */

#include "taskset.h"
#include "task.h"
#include "htab.h"
#include "graph.h"


/** An aggregate of tasks.
 */
struct taskset {
	struct env_var *env_vars;	/**< Pointer to the head of a list of
					  env vars */
	struct task *tasks;		/**< Pointer to the head of a list of
					  tasks. */
	htab_t symtab;			/**< Symbol table mapping task names to
					  task structure addresses */
	graph_t depgraph;		/**< Describes the dependencies between
					  task structures. */
};


#endif /* !TASKSET_PRIV_H */
