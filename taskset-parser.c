/**
 * @file taskset-parser.c
 * @brief Reads a configuration file and transform it into a taskset.
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

#ifdef HAVE_STRING_H
# include <string.h>
#elif HAVE_STRINGS_H
# include <strings.h>
#endif /* !HAVE_STRING_H */

#include <assert.h>

#include "taskset-priv.h"
#include "grammar.h"
#include "ast.h"

#include "logger.h"
#include "xmemory.h"


extern FILE *yyin;

extern struct var *vars;
extern struct tasklist *tasks;


extern int yyparse(void);



static void
move_lists(struct taskset *ts)
{
	ts->vars = vars, vars = NULL;
	ts->tasks = tasks, tasks = NULL;
}

static void
fill_symtab(struct tasklist *tl, hashtab_t symtab)
{
	struct task *t, *s;

	for (t = tl->head; t; t = t->next) {
		s = hashtab_strlookup(symtab, t->name, 0, NULL);
		if (s)
			fatal_error("Task `%s' is defined more than once.\n",
				    t->name);
		task_incref(t);
		hashtab_strlookup(symtab, t->name, 1, t);
	}
}

/*
 * Translates task names found in the abstract syntax tree into pointers to the
 * appropriate task structs.
 */
static void
expr_resolve(hashtab_t symtab, struct task *t)
{
	ast_t i;
	ast_itor_t itor;
	struct task *s;
	char *name;

	assert(t->expr);

	itor = new_ast_itor(t->expr);

	for (i = ast_itor_first(itor); i; i = ast_itor_next(itor)) {
		if (ast_get_type(i) != AST_ID)
			continue;

		name = ast_get_item(i);
		assert(name != NULL);

		s = hashtab_strlookup(symtab, name, 0, NULL);
		if (!s)
			fatal_error("Task `%s' needed by `%s' is not "
				    "defined.\n", name, t->name);

		xfree(name);
		task_incref(s), ast_set_item(i, s);
	}

	delete_ast_itor(itor);
}

int
taskset_parse(struct taskset *self, const char *filename)
{
	struct task *t;

	if (!self)
		return -1;

	yyin = (filename) ? fopen(filename, "r") : stdin;
	if (!yyin)
		return -1;

	yyparse();

	if (!tasks)
		fatal_error("No tasks declared in %s\n", filename);

	move_lists(self);

	fill_symtab(self->tasks, self->symtab);

	for (t = self->tasks->head; t; t = t->next)
		if (t->expr)
			expr_resolve(self->symtab, t);

	return fclose(yyin);
}
