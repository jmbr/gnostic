/*
 * parser.c -- Dependency expression parser.
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

#include "taskset.h"
#include "taskset-priv.h"
#include "ast.h"
#include "grammar.h"

#include "xalloc.h"
#include "debug.h"


extern FILE *yyin;

extern struct task *tasks;
extern struct env_var *env_vars;


extern int yyparse(void);

/*
 * This function translates task names contained in the abstract syntax tree
 * into pointers to the appropriate task structures.
 */
static void expr_resolve(struct taskset *ts, struct task *t, astnode_t n);



static void
move_lists(struct taskset *ts)
{
	ts->tasks = tasks, tasks = NULL;
	ts->env_vars = env_vars, env_vars = NULL;
}

static void
fill_symtab(struct taskset *ts)
{
	struct task *t;

	for (t = ts->tasks; t; t = task_get_next(t))
		hashtab_strlookup(ts->symtab, task_get_name(t), 1, t);
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

	move_lists(self);

	fill_symtab(self);

	for (t = self->tasks; t; t = task_get_next(t))
		expr_resolve(self, t, task_get_expr(t));

	return fclose(yyin);
}



/** Resolve an ast node containing an identifier.
 * This function changes the content of the astnode, frees the identifier
 * string allocated by the lexer and substitutes it with a pointer to the
 * appropriate task structure.
 */
static void
ident_resolve(struct taskset *ts, struct task *t, astnode_t n)
{
	void *p, *item;

	item = astnode_get_item(n);
	p = hashtab_strlookup(ts->symtab, item, 0, NULL);
	if (!p)
		fatal_error("gnostic: Task `%s' needed by `%s' is not "
				"defined.\n", (char *) item, task_get_name(t));

	graph_insert_edge(ts->depgraph, t, p);

	xfree(item);
	astnode_set_item(n, p);
}

void
expr_resolve(struct taskset *ts, struct task *t, astnode_t n)
{
	if (!n)
		return;

	if (astnode_get_type(n) == N_ID)
		ident_resolve(ts, t, n);

	expr_resolve(ts, t, astnode_get_lhs(n));
	expr_resolve(ts, t, astnode_get_rhs(n));
}
