#ifndef EXEC_H
#define EXEC_H		1
/*
 * exec.h -- Task execution module.
 * $Id$
 */


#include "task.h"


extern int execute(const struct task *t, char **envp);


#endif /* !EXEC_H */
