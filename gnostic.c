/**
 * @file gnostic.c
 * @brief Generic Network Scanning Tool.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
 */
/*
 * gnostic: adjective 1 of knowledge. 2 having special mystic knowledge.
 * noun (Gnostic) early Christian heretic claiming mystical knowledge.
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdio.h>
# include <stdlib.h>
#endif /* STDC_HEADERS */

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */

#include <assert.h>

#include "taskset.h"

#include "version.h"
#include "err.h"


#define PROG_NAME		"gnostic"


static void usage(void) __attribute__ ((noreturn));

static int exec(const struct taskset *tasks, int argc, char *argv[]);



int
main(int argc, char *argv[])
{
	int status;
	struct taskset *tasks;

	if (argc < 2)
		usage();

	tasks = new_taskset(argv[1]);
	if (!tasks) {
		eprintf("gnostic: Invalid configuration file `%s'\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	status = (argc == 2)
			? taskset_print(tasks)
			: exec(tasks, argc, argv);

	delete_taskset(tasks);
	exit((status == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}


void
usage(void)
{
	eprintf("%s\n\n", version.v_gnu);
	eprintf("Usage: %s <config file> [task] [name=value] ...\n", PROG_NAME);

	exit(EXIT_FAILURE);
}


static void
xputenv(char *string)
{
	if (putenv(string) == -1)
		fatal_error("gnostic: Unable to declare %s", string);
}

static void
setup_environ(const struct taskset *tasks, int argc, char *argv[])
{
	int i;
	const struct env_var *var;
	const int env_decl_start = 3;

	for (var = taskset_get_env_vars(tasks); var; var = var->next)
		xputenv(var->v);

	for (i = env_decl_start; i < argc; i++)
		xputenv(argv[i]);
}

int
exec(const struct taskset *tasks, int argc, char *argv[])
{
	const struct task *t;

	t = taskset_get_task(tasks, argv[2]);
	if (!t)
		fatal_error("gnostic: Unknown task `%s'.\n", argv[2]);

	setup_environ(tasks, argc, argv);

	return task_exec(t);
}
