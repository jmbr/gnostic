#ifndef TASK_H
#define TASK_H    1
/*
 * task.h -- Task management module.
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdio.h>
#endif /* STDC_HEADERS */

#include "ast.h"


struct task;


extern struct task *new_task(char *name, astnode_t expr, char *actions);
extern int delete_task(struct task *self);

extern const char *task_get_name(const struct task *self);
extern const char *task_get_actions(const struct task *self);
extern const astnode_t task_get_expr(const struct task *self);

extern int task_set_next(struct task *self, struct task *t);
extern struct task *task_get_next(const struct task *self);

extern int tasklist_print(const struct task *head, FILE *fp);
extern int delete_tasklist(struct task *self);


#endif /* !TASK_H */
