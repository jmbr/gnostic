/*
 * parser.c -- Dependency expression parser.
 */

/* XXX This module needs refactoring. */


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

#include "task.h"
#include "graph.h"
#include "htab.h"
#include "ast.h"
#include "scanner.h"
#include "grammar.h"

#include "xalloc.h"
#include "debug.h"


extern FILE *yyin;

extern struct task *tasks;

struct parser_ctx {
	struct task *t;
	htab_t symtab;
	graph_t depgraph;
};


extern int yyparse(void);

static void expr_parse(struct parser_ctx *ctx, astnode_t n);


struct task *
tasklist_parse(const char *filename, htab_t symtab, graph_t depgraph)
{
	struct task *tasklist, *t;
	struct parser_ctx ctx = { NULL, NULL, NULL };

	if (!symtab || !depgraph)
		return NULL;

	yyin = (filename) ? fopen(filename, "r") : stdin;
	if (!yyin)
		return NULL;

	yyparse();
	tasklist = tasks, tasks = NULL;

	for (t = tasklist; t; t = task_get_next(t))
		htab_lookup_s(symtab, task_get_name(t), 1, t);

	ctx.symtab = symtab;
	ctx.depgraph = depgraph;
	for (t = tasklist; t; t = task_get_next(t)) {
		ctx.t = t;
		expr_parse(&ctx, task_get_expr(t));
	}

	fclose(yyin);
	return tasklist;
}


void
expr_parse(struct parser_ctx *ctx, astnode_t n)
{
	void *p, *item;

	if (!n)
		return;

	if (astnode_get_type(n) == N_ID) {
		item = astnode_get_item(n);
		p = htab_lookup_s(ctx->symtab, item, 0, NULL);
		if (!p) {
			fatal_error("gnostic: Task `%s' needed by `%s' is not "
			"defined.\n", (char *) item, task_get_name(ctx->t));
		}

		graph_insert_edge(ctx->depgraph, ctx->t, p);

		/*
		 * This function is responsible from freeing the strings
		 * allocated by the lexer.
		 * Deletion of the new structure pointed to by item will be
		 * performed by delete_tasks.
		 */
		xfree(item);
		astnode_set_item(n, p);
	}

	expr_parse(ctx, astnode_get_lhs(n));
	expr_parse(ctx, astnode_get_rhs(n));
}
