#ifndef TASK_H
#define TASK_H    1
/**
 * @file task.h
 * @brief Task management module.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
 */

#include "ast.h"


struct task;


/** task constructor.
 * Allocates and initializes task structures.
 *
 * @param name Name of the task to create.
 * @param expr A pointer to the abstract syntax tree of a dependency expression
 * for this task.
 * @param actions Source code for the payload of the task.
 * @return A pointer to the allocated structure.
 *
 * @see delete_task
 */
extern struct task *new_task(char *name, astnode_t expr, char *actions);

/** task destructor.
 * Frees all the resources associated to the task.
 *
 * @param self A pointer to the structure.
 * @return 0 on success, -1 on failure.
 *
 * @see new_task
 */
extern int delete_task(struct task *self);

/** task launcher.
 * Executes the given task (but only if its depending tasks are successful).
 *
 * @param self A pointer to a task.
 * @return 0 on success, -1 on failure.
 */
extern int task_exec(const struct task *self);

/*@{*/
/**
 * The following accessors are convenient for several reasons but if they ever
 * turn out to become a bottleneck then compile the program with the
 * -finline-functions option.
 */
extern const char *task_get_name(const struct task *self);
extern const char *task_get_actions(const struct task *self);

extern const astnode_t task_get_expr(const struct task *self);

extern struct task *task_get_next(const struct task *self);
extern int task_set_next(struct task *self, struct task *t);

/*@}*/

#endif /* !TASK_H */
