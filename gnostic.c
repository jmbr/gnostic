/**
 * @file gnostic.c
 * @brief Main module.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
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

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif /* HAVE_UNISTD_H */

#ifdef HAVE_SIGNAL_H
# include <signal.h>
#endif /* HAVE_SIGNAL_H */

#include <assert.h>

#include "env.h"
#include "logger.h"
#include "taskset.h"
#include "version.h"


/* FIXME: This is ugly. */
static int exec_first = 0;


static void usage(void) __attribute__ ((noreturn));

static RETSIGTYPE handler(int signum);

static int exec(const struct taskset *tasks, int argc, char *argv[]);


int
main(int argc, char *argv[])
{
	int c, status;
	char *filename;
	struct taskset *tasks;

	if (argc < 2)
		usage();

	(void) signal(SIGINT, handler);

	while ((c = getopt(argc, argv, "hvx")) != -1) {
		switch (c) {
		case 'v':
			log_level = LOG_INFO;
			break;
		case 'x':
			exec_first = 1;
			break;
		case 'h':
		default:
			usage();
			break;
		}
	}

	if (optind >= argc) {
		error("A task file must be specified.\n");
		usage();
	}

	filename = argv[optind++];

	tasks = new_taskset(filename);
	if (!tasks)
		fatal_error("Invalid task file `%s'.\n", filename);

	if ((optind < argc) || exec_first)
		status = exec(tasks, argc, argv);
	else
		status = taskset_print(tasks);
	/* XXX
	status = (optind < argc) ? exec(tasks, argc, argv)
				 : taskset_print(tasks);
				 */

	delete_taskset(tasks);
	exit((status == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}


void
usage(void)
{
	fprintf(stderr, "%s\n\n"
	"Usage: gnostic [option] taskfile [task] [name=value] [name=value] [...]\n"
	"\n"
	"Options:\n"
	"  -h\tPrint this help page\n"
	"  -v\tExplain what is being done\n"
	"  -x\tExecute the first task found.\n"
//	"  -p\tDump attack tree.\n"
	"\n"
	/* XXX Put back some of the examples.
	"Examples:\n"
	*/
	"Email bug reports to %s\n", version.v_gnu, PACKAGE_BUGREPORT);

	exit(EXIT_FAILURE);
}


RETSIGTYPE
handler(int signum)
{
	fatal_error("Interrupted by the user.\n");
}


static void
setup_environ(const struct taskset *tasks, int argc, char *argv[])
{
	int i;
	const struct var *v;
	const int env_decl_start = optind;

	for (v = taskset_get_vars(tasks); v; v = v->next)
		xputenv(v->nameval);

	for (i = env_decl_start; i < argc; i++)
		xputenv(argv[i]);
}

int
exec(const struct taskset *tasks, int argc, char *argv[])
{
	const struct task *t;

	t = taskset_get_task(tasks, exec_first ? NULL : argv[optind]);
	if (!t)
		fatal_error("Unknown task `%s'.\n", argv[optind]);

	setup_environ(tasks, argc, argv);

	xsetenv("GNOSTIC_MAIN", t->name, 1);

	return task_exec(t);
}
