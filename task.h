#ifndef TASK_H
#define TASK_H    1
/**
 * @file task.h
 * @brief Task management module.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
 */


#include "ast.h"


/** Task.
 * Describes a job to be executed at some point by gnostic.
 */
struct task {
	char *name;		/**< Task name */
	char *actions;		/**< Executable payload */
	astnode_t expr;		/**< Dependency expression */
	struct task *prev;	/**< Pointer to the previous task in the list. */
	struct task *next;	/**< Pointer to the next task in the list. */
};


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

/** task printer.
 * Prints the name of the specified task in stdout.
 *
 * @param self A pointer to the structure.
 * @return 0 on success, -1 on failure.
 */
extern int task_print(const struct task *self);

/** task launcher.
 * Executes the given task (but only if its depending tasks are successful).
 *
 * @param self A pointer to the structure.
 * @return 0 on success, -1 on failure.
 */
extern int task_exec(const struct task *self);

/** task dependency checker.
 * Verifies the given task doesn't have any cyclic dependencies.
 *
 * @param self A pointer to the structure
 * @return 0 on success, -1 on failure.
 */
extern int task_check_deps(const struct task *self);


#endif /* !TASK_H */
