/*
 * exec.c -- Implements task execution.
 * $Id$
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
# error gnostic is currently supported only in UNIX-like operating systems.
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


/** Execution context.
* Packs together a set of structures needed throughout the execution stage.
*/
struct exec_ctx {
	char **envp;
	const struct task *t;
};


static int exec_script(struct exec_ctx *ctx);
static int eval_expression(struct exec_ctx *ctx, astnode_t n);



int
execute(const struct task *t, char **envp)
{
	int status;
	struct exec_ctx ctx = { envp, t };

	if (!t)
		return -1;

        if (t->expr) {
		dprintf("gnostic: Trying to satisfy dependencies for `%s'.\n",
			t->name);
		if (!eval_expression(&ctx, t->expr)) {
			dprintf("gnostic: Unable to satisfy dependencies "
				"for `%s'.\n", t->name);
			return -1;
		}
		dprintf("gnostic: Done with dependencies for `%s'.\n", t->name);
        }

	dprintf("gnostic: Executing `%s'.\n", t->name);
	status = exec_script(&ctx);
	dprintf("gnostic: Task `%s' exited with status %d.\n", t->name, status);

	return status;
}



static void
child(int fds[2], char **envp)
{
	char arg[_POSIX_PATH_MAX];
	char *argv[] = { "/bin/sh", arg, NULL };

	close(fds[1]);

	sprintf(arg, "/dev/fd/%u", fds[0]);

	if (execve("/bin/sh", argv, envp) == -1)
		fatal_error("gnostic: execve: %s\n", strerror(errno));
}

static int
parent(int fds[2], pid_t pid, const char *src)
{
	int status;
	size_t srclen;

	srclen = strlen(src);
	if (write(fds[1], src, srclen) != srclen)
		fatal_error("gnostic: write: %s\n", strerror(errno));

	close(fds[0]), close(fds[1]);

	if (waitpid(pid, &status, 0) == -1)
		fatal_error("gnostic: waitpid: %s\n", strerror(errno));

	if (!WIFEXITED(status))
		return -1;
	
	return WEXITSTATUS(status);
}

int
exec_script(struct exec_ctx *ctx)
{
	pid_t pid;
	int fds[2], status = -1;

	assert(ctx && ctx->t->actions);

	if (pipe(fds) == -1)
		fatal_error("gnostic: Unable to create pipe.\n");

	switch ((pid = fork())) {
	case -1:
		fatal_error("gnostic: Unable to fork: %s\n", strerror(errno));
	case 0:
		child(fds, ctx->envp);
	default:
		status = parent(fds, pid, ctx->t->actions);
	}

	return status;
}



/** Executes a dependency expression.
 * @return 1 on success, 0 on failure.
 */
int
eval_expression(struct exec_ctx *ctx, astnode_t n)
{
	int status = 0;	

	assert(ctx);

	switch (astnode_get_type(n)) {
	case N_ID:
		status = (execute(astnode_get_item(n), ctx->envp) == 0)
			? 1 : 0;
		break;
	case N_AND:
		status = (eval_expression(ctx, astnode_get_lhs(n))
			&& eval_expression(ctx, astnode_get_rhs(n)));
		break;
	case N_OR:
		status = (eval_expression(ctx, astnode_get_lhs(n))
			|| eval_expression(ctx, astnode_get_rhs(n)));
		break;
	case N_NOT:
		status = (!eval_expression(ctx, astnode_get_rhs(n)));
		break;
	default:
		assert(0);
	}

	return status;
}
