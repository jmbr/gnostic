#ifndef TASK_COLLECTION_H
#define TASK_COLLECTION_H    1
/*!
 * \file task-collection.h
 * \brief An aggregate of tasks.
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdio.h>
#endif /* STDC_HEADERS */

#include "task.h"


struct task_collection;

/** Environment variable declaration.
 */
struct env_var {
	char *v;		/**< Declaration of the form name=value */
	struct env_var *next;	/**< Pointer to the next item in the list */
};


/** task_collection constructor.
 * Allocates and initializes task_collection structures.
 *
 * @return A pointer to the allocated structure.
 *
 * @see delete_task_collection
 */
extern struct task_collection *new_task_collection(void);

/** task_collection destructor.
 * Frees all the resources associated to the task_collection.
 *
 * @param self A pointer to the structure.
 * @return 0 on success, -1 on failure.
 *
 * @see new_task_collection
 */
extern int delete_task_collection(struct task_collection *self);


/** Builds a task_collection from a file.
 * Reads, aggregates and checks the validity of tasks coming from a file.
 *
 * @param self A pointer to the structure.
 *
 * @return 0 on success, -1 on failure.
 */
extern int task_collection_read(struct task_collection *self, const char *name);

extern int task_collection_print(const struct task_collection *self, FILE *fp);

extern const struct task *
task_collection_get_task(const struct task_collection *self, const char *name);

extern const struct env_var *
task_collection_get_vars(const struct task_collection *self);


#endif /* !TASK_COLLECTION_H */
