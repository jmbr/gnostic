/**
 * @file test-grammar.c
 * @brief Grammar unit test. Checks whether ASTs are well-formed after the task
 * file parsing stage.
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

#include "grammar.h"
#include "tasklist.h"
#include "ast.h"



extern FILE *yyin;

extern struct tasklist *tasks;


extern int yyparse(void);

static void test_grammar(void);


int
main(int argc, char *argv[])
{
	test_grammar();

	exit(EXIT_SUCCESS);
}


static inline void
indent(unsigned depth)
{
	for (unsigned i = 0; i < depth; i++)
		printf("    ");
}

static void
print_expr(const ast_t n, unsigned depth)
{
	if (!n)
		return;

	switch (ast_get_type(n)) {
	case AST_ID:
		indent(depth), printf("%s\n", (char *) ast_get_item(n));
		break;
	case AST_AND:
		indent(depth), printf("and\n");
		print_expr(ast_get_lhs(n), depth + 1);
		print_expr(ast_get_rhs(n), depth + 1);
		break;
	case AST_OR:
		indent(depth), printf("or\n");
		print_expr(ast_get_lhs(n), depth + 1);
		print_expr(ast_get_rhs(n), depth + 1);
		break;
	case AST_NOT:
		indent(depth), printf("not\n");
		print_expr(ast_get_rhs(n), depth + 1);
		break;
	default:
		assert(0);
	}
}

void
test_grammar(void)
{
	yyin = fopen("expressions", "r");
	assert(yyin);

	yyparse();

	/* Memory deallocation is not really important for this test and anyway
	 * calling delete_tasklist would be wrong because the names haven't
	 * been translated to pointers to task structures.
	 */
	for (struct task *t = tasks->head; t; t = t->next) {
		printf("%s:\n", t->name);
		print_expr(t->expr, 1);
		printf("\n");
	}
}
