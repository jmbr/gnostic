#ifndef TASK_H
#define TASK_H    1
/*
 * \file task.h
 * \brief Task management module.
 */

#include "ast.h"


struct task;


extern struct task *new_task(char *name, astnode_t expr, char *actions);
extern int delete_task(struct task *self);

extern const char *task_get_name(const struct task *self);
extern const char *task_get_actions(const struct task *self);
extern const astnode_t task_get_expr(const struct task *self);

extern int task_set_next(struct task *self, struct task *t);
extern struct task *task_get_next(const struct task *self);

/** Task launcher.
 * Executes the given task (but only if its depending tasks are successful).
 *
 * @param self A pointer to a task.
 *
 * @return 0 on success, -1 on failure.
 */
extern int task_exec(const struct task *self);


#endif /* !TASK_H */
