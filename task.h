#ifndef TASK_H
#define TASK_H    1
/*
 * task.h -- Task management module.
 * $Id$
 */



#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdio.h>
#endif /* STDC_HEADERS */

#include "ast.h"


struct task {
	char *name;
	char *actions;
	astnode_t expr;

	struct task *next;
};

struct tasklist {
	size_t len;
	struct task *head;
};


extern struct task *new_task(char *name, astnode_t expr, char *actions);
extern int delete_task(struct task *self);

extern int delete_tasklist(struct task *self);


#endif /* !TASK_H */
