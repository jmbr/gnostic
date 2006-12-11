#ifndef TASKSET_H
#define TASKSET_H    1
/**
 * @file taskset.h
 * @brief Management of task aggregates.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdio.h>
#endif /* STDC_HEADERS */

#include "task.h"
#include "vars.h"


struct taskset;


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

/** taskset printer.
 * Displays the list of available tasks on stdout.
 *
 * @param self A pointer to the taskset structure.
 * @return 0 on success, -1 on failure.
 */
extern int taskset_print(const struct taskset *self);

/** task accessor.
 * 
 * @param self A pointer to the taskset structure.
 * @param name The name of the task.  If name is NULL then the first task in
 * the set will be returned.
 * @return A pointer to the appropriate task structure on success, NULL
 * otherwise.
 *
 * @see task_exec
 */
extern const struct task *taskset_get_task(const struct taskset *self,
							const char *name);

/** environment variable accessor.
 * Fetches the list of variables declared in a taskfile.
 *
 * @param self A pointer to the taskset structure.
 * @return The head of the list on success, NULL otherwise.
 */
extern const struct var *taskset_get_vars(const struct taskset *self);


#endif /* !TASKSET_H */
