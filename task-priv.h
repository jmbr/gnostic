#ifndef TASK_PRIV_H
#define TASK_PRIV_H		1
/*!
 * \file task-priv.h
 * \brief Private-side of task structures.
 */


#include "task.h"
#include "ast.h"


/** Gnostic task.
 * Describes a job to be executed at some point by gnostic.
 */
struct task {
	char *name;		/**< Task name */
	char *actions;		/**< Executable payload */
	astnode_t expr;		/**< Dependency expression */
	struct task *next;	/**< Pointer to the next task in the list. */
};


#endif /* !TASK_PRIV_H */
