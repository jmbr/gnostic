#ifndef TASKSET_H
#define TASKSET_H    1
/*!
 * \file taskset.h
 * \brief This module defines a structure which holds a collection of tasks.
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdio.h>
#endif /* STDC_HEADERS */

#include "task.h"


struct taskset;

/** Environment variable declaration.
 */
struct env_var {
	struct env_var *next;	/**< Pointer to the next item in the list */
	char *v;		/**< Declaration of the form name=value */
};


/** taskset constructor.
 * Allocates and initializes taskset structures.
 *
 * @param filename Name of the configuration file where the tasks are
 * described.
 * @return A pointer to the allocated structure.
 *
 * @see delete_taskset
 */
extern struct taskset *new_taskset(const char *filename);

/** taskset destructor.
 * Frees all the resources associated to the taskset.
 *
 * @param self A pointer to the structure.
 * @return 0 on success, -1 on failure.
 *
 * @see new_taskset
 */
extern int delete_taskset(struct taskset *self);

extern int taskset_print(const struct taskset *self, FILE *fp);

extern const struct task *taskset_get_task(const struct taskset *self,
							const char *name);

extern const struct env_var *taskset_get_env_vars(const struct taskset *self);


#endif /* !TASKSET_H */
