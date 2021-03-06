/**
 * @file task-exec.c
 * @brief Task execution module.
 *
 * @author Juan M. Bello Rivas <jmbr@superadditive.com>
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdio.h>
# include <stdlib.h>
#endif /* STDC_HEADERS */

#ifdef HAVE_STDBOOL_H
# include <stdbool.h>
#else /* !HAVE_STDBOOL_H */
# define bool int
# define true 1
# define false (!true)
#endif /* HAVE_STDBOOL_H */

#ifdef HAVE_STRING_H
# include <string.h>
#elif HAVE_STRINGS_H
# include <strings.h>
#endif /* !HAVE_STRING_H */

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */

#ifdef HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif /* HAVE_SYS_WAIT_H */

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif /* HAVE_UNISTD_H */

#ifndef HAVE_FORK
# error Gnostic is currently supported only in UNIX-like operating systems.
#endif /* HAVE_FORK */

#ifdef HAVE_LIMITS_H
# include <limits.h>
#endif /* HAVE_LIMITS_H */

#include <errno.h>

#include <assert.h>

#include "task.h"
#include "ast.h"

#include "env.h"
#include "logger.h"


/* Beware of the evil strerror and its non-reentrancy! */
#define die(s) fatal_error("%s failed: %s\n", s, strerror(errno))


static char shell[] = "/bin/sh";


static int task_exec_deps(const struct task *self);
static int task_exec_script(const struct task *self);

static bool eval_expr(const ast_t n);



int
task_exec(const struct task *self)
{
	int status = -1;

	assert(self && self->name);

	info("Executing `%s'.\n", self->name);

	if (task_exec_deps(self) == 0)
		status = task_exec_script(self);

	info("Task `%s' exited with status %d\n", self->name, status);

	return status;
}

int
task_exec_deps(const struct task *self)
{
	assert(self);

	if (!self->expr)
		return 0;	/* It's OK. There are no dependencies. */

	return (eval_expr(self->expr) == true) ? 0 : -1;
}



static void
child(int fds[2])
{
	int status;
	char arg[_POSIX_PATH_MAX];
	char *argv[] = { shell, arg, NULL };

	status = snprintf(arg, sizeof(arg), "/dev/fd/%u", fds[0]);
	if ((status >= sizeof(arg)) || (status == -1))
		die("snprintf");

	if (close(fds[1]) == -1)
		die("close");

	if (execv(shell, argv) == -1)
		die("execv");
}

static int
parent(const char *name, int fds[2], pid_t pid, const char *src)
{
	int status;
	size_t srclen = strlen(src);

	if (write(fds[1], src, srclen) != srclen)
		die("write");

	if ((close(fds[0]) == -1) || (close(fds[1])))
		die("close");

	if (waitpid(pid, &status, 0) == -1)
		die("waitpid");

	if (WIFSIGNALED(status)) {
		error("Task `%s' received signal number %d\n",
			name, WTERMSIG(status));
		return -1;
	} else if (!WIFEXITED(status)) {
		error("Task `%s' terminated abnormally.\n", name);
		return -1;
	}
	
	return WEXITSTATUS(status);
}

int
task_exec_script(const struct task *self)
{
	pid_t pid;
	int fds[2], status = -1;

	assert(self);

	xsetenv("GNOSTIC_CURRENT", self->name, 1);

	if (pipe(fds) == -1)
		die("pipe");

	switch ((pid = fork())) {
	case -1:
		die("fork");
	case 0:
		child(fds);
	default:
		status = parent(self->name, fds, pid, self->actions);
	}

	return status;
}



/** Dependency expression evaluator.
 * Traverses the abstract syntax tree of an expression and determines whether
 * it is true or false.
 *
 * @param n The AST node we want to use as root for the evaluation.
 * @returns true on success, false on failure.
 *
 * @see task_exec, task_exec_script
 */
bool
eval_expr(const ast_t n)
{
	int status = false;	

	assert(n);

	switch (ast_get_type(n)) {
	case AST_ID:
		status = (task_exec(ast_get_item(n)) == 0) ? true : false;
		break;
	case AST_AND:
		status = (eval_expr(ast_get_lhs(n)) && eval_expr(ast_get_rhs(n)));
		break;
	case AST_OR:
		status = (eval_expr(ast_get_lhs(n)) || eval_expr(ast_get_rhs(n)));
		break;
	case AST_NOT:
		status = (!eval_expr(ast_get_rhs(n)));
		break;
	default:
		assert(0);
	}

	return status;
}
