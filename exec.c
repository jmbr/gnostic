/*
 * exec.c -- Implements task execution.
 */
/*
 * "Within the armor is the butterfly and within the butterfly is the signal
 * from another star." --Philip K. Dick
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdio.h>
# include <stdlib.h>
#endif /* STDC_HEADERS */

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

#include "exec.h"
#include "ast.h"
#include "task.h"

#include "debug.h"


/* Here we want to be on the safe side with regard to strerror */
#if (HAVE_STRERROR && !_REENTRANT)
#define die(s) fatal_error("gnostic: %s failed: %s\n", s, strerror(errno))
#else /* !HAVE_STRERROR || _REENTRANT */
#define die(s) fatal_error("gnostic: %s failed: %d\n", s, errno)
#endif /* HAVE_STRERROR */


static int exec_deps(const struct task *t);
static int exec_script(const struct task *t);

static int eval_expression(const astnode_t n);



int
execute(const struct task *t)
{
	int status;
	const char *name;

	if (!t)
		return -1;

	name = task_get_name(t);
	assert(name);

	dprintf("gnostic: Trying to satisfy dependencies for `%s'.\n", name);
	if (exec_deps(t) == -1) {
		dprintf("gnostic: Unable to satisfy dependencies for `%s'.\n", name);
		return -1;
	}
	dprintf("gnostic: Done with dependencies for `%s'.\n", name);

	dprintf("gnostic: Executing `%s'.\n", name);
	status = exec_script(t);
	dprintf("gnostic: Task `%s' exited with status %d.\n", name, status);

	return status;
}

int
exec_deps(const struct task *t)
{
	astnode_t expr;

	assert(t);

	expr = task_get_expr(t);

        if (!expr)
		return 0;

	return (eval_expression(expr)) ? 0 : -1;
}



static void
child(int fds[2])
{
	char arg[_POSIX_PATH_MAX];
	char *argv[] = { "/bin/sh", arg, NULL };

	close(fds[1]);

	sprintf(arg, "/dev/fd/%u", fds[0]);

	if (execv("/bin/sh", argv) == -1)
		die("execv");
}

static int
parent(int fds[2], pid_t pid, const char *src)
{
	int status;
	size_t srclen;

	srclen = strlen(src);
	if (write(fds[1], src, srclen) != srclen)
		die("write");

	close(fds[0]), close(fds[1]);

	if (waitpid(pid, &status, 0) == -1)
		die("waitpid");

	if (!WIFEXITED(status))
		return -1;
	
	return WEXITSTATUS(status);
}

int
exec_script(const struct task *t)
{
	pid_t pid;
	int fds[2], status = -1;

	assert(t);

	if (pipe(fds) == -1)
		die("pipe");

	switch ((pid = fork())) {
	case -1:
		die("fork");
	case 0:
		child(fds);
	default:
		status = parent(fds, pid, task_get_actions(t));
	}

	return status;
}



/** Executes a dependency expression.
 * @return 1 on success, 0 on failure.
 */
int
eval_expression(const astnode_t n)
{
	int status = 0;	

	assert(n);

	switch (astnode_get_type(n)) {
	case N_ID:
		status = (execute(astnode_get_item(n)) == 0)
			? 1 : 0;
		break;
	case N_AND:
		status = (eval_expression(astnode_get_lhs(n))
			&& eval_expression(astnode_get_rhs(n)));
		break;
	case N_OR:
		status = (eval_expression(astnode_get_lhs(n))
			|| eval_expression(astnode_get_rhs(n)));
		break;
	case N_NOT:
		status = (!eval_expression(astnode_get_rhs(n)));
		break;
	default:
		assert(0);
	}

	return status;
}
