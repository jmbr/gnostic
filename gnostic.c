/*
 * gnostic.c -- Generic Network Scanning Tool.
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

#include "task.h"
#include "task-collection.h"

#include "version.h"
#include "debug.h"


#define PROG_NAME		"gnostic"


static void usage(void) __attribute__ ((noreturn));

static void setup_environ(int nvars, char *vars[],
				const struct task_collection *tasks);

static int exec(const struct task_collection *tasks, const char *name);



int
main(int argc, char *argv[])
{
	int status;
	struct task_collection *tasks;

	if (argc < 2)
		usage();

	tasks = new_task_collection();
	if (task_collection_read(tasks, argv[1]) == -1) {
		eprintf("gnostic: Invalid configuration file `%s'\n", argv[1]);
		delete_task_collection(tasks);
		exit(EXIT_FAILURE);
	}

	if (argc == 2)
		status = task_collection_print(tasks, stdout);
	else {
		setup_environ(argc - 3, &argv[3], tasks);
		status = exec(tasks, argv[2]);
	}

	delete_task_collection(tasks);
	exit((status == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}


void
usage(void)
{
	eprintf("%s\n\n", version.v_gnu);
	eprintf("Usage: %s <config file> [task] [name=value] ...\n", PROG_NAME);

	exit(EXIT_FAILURE);
}


void
setup_environ(int nvars, char *vars[], const struct task_collection *tasks)
{
	int i;
	const struct env_var *var;

	for (var = task_collection_get_vars(tasks); var; var = var->next)
		if (putenv(var->v) == -1)
			fatal_error("gnostic: Unable to declare %s", var->v);

	for (i = 0; i < nvars; i++)
		if (putenv(vars[i]) == -1)
			fatal_error("gnostic: Unable to declare %s", vars[i]);
}


int
exec(const struct task_collection *tasks, const char *name)
{
	int status;
	const struct task *t;

	t = task_collection_get_task(tasks, name);
	if (!t)
		fatal_error("gnostic: Unknown task `%s'.\n", name);

	dprintf("gnostic: Executing `%s'.\n", name);
	status = task_exec(t);
	dprintf("gnostic: Task `%s' exited with status %d.\n", name, status);

	return status;
}
