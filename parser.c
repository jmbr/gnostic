/*
 * parser.c -- Dependency expression parser.
 * $Id$
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

#include "gnostic.h"

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


int
gnostic_conf_parse(struct gnostic *self, const char *filename)
{
	struct parser_ctx ctx = { NULL, NULL, NULL };

	if (!self)
		return -1;

	ctx.symtab = self->symtab;
	ctx.depgraph = self->depgraph;

	yyin = (filename) ? fopen(filename, "r") : stdin;
	if (!yyin)
		return -1;

	yyparse();
	self->tasks = tasks, tasks = NULL;

	for (ctx.t = self->tasks; ctx.t; ctx.t = ctx.t->next)
		htab_lookup_s(ctx.symtab, ctx.t->name, 1, ctx.t);

	for (ctx.t = self->tasks; ctx.t; ctx.t = ctx.t->next)
		expr_parse(&ctx, ctx.t->expr);

	fclose(yyin);
	return 0;
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
				"defined.\n", (char *) item, ctx->t->name);
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
