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

#include "err.h"
#include "xmemory.h"


extern FILE *yyin;

extern struct var *vars;
extern struct tasklist *tasks;


extern int yyparse(void);

/*
 * This function translates task names contained in the abstract syntax tree
 * into pointers to the appropriate task structures.
 */
static void expr_resolve(hashtab_t symtab, struct task *t, astnode_t n);



static void
move_lists(struct taskset *ts)
{
	ts->vars = vars, vars = NULL;
	ts->tasks = tasks, tasks = NULL;
}

static void
fill_symtab(struct tasklist *tl, hashtab_t symtab)
{
	struct task *t;

	for (t = tl->head; t; t = t->next)
		hashtab_strlookup(symtab, t->name, 1, t);
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

	fill_symtab(self->tasks, self->symtab);

	for (t = self->tasks->head; t; t = t->next)
		expr_resolve(self->symtab, t, t->expr);

	return fclose(yyin);
}



/** Resolve an ast node containing an identifier.
 * This function changes the content of the astnode, frees the identifier
 * string allocated by the lexer and substitutes it with a pointer to the
 * appropriate task structure.
 */
static void
ident_resolve(hashtab_t symtab, struct task *t, astnode_t n)
{
	void *p, *item;

	item = astnode_get_item(n);
	p = hashtab_strlookup(symtab, item, 0, NULL);
	if (!p)
		fatal_error("gnostic: Task `%s' needed by `%s' is not "
			    "defined.\n", (char *) item, t->name);

	xfree(item);
	astnode_set_item(n, p);
}

void
expr_resolve(hashtab_t symtab, struct task *t, astnode_t n)
{
	if (!n)
		return;

	if (astnode_get_type(n) == N_ID)
		ident_resolve(symtab, t, n);

	expr_resolve(symtab, t, astnode_get_lhs(n));
	expr_resolve(symtab, t, astnode_get_rhs(n));
}
