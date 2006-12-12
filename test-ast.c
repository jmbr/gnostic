/**
 * @file test-ast.c
 * @brief Abstract syntax tree unit test.
 * 
 * @author Juan M. Bello Rivas <jmbr@superadditive.com>
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

#include "ast.h"


static void test_ast(void);
static void test_ast_itor(void);


int
main(int argc, char *argv[])
{
	test_ast();
	test_ast_itor();

	exit(EXIT_SUCCESS);
}


static ast_t
make_tree(void)
{
	ast_t tree[6];

	tree[5] = new_ast(AST_ID, NULL, NULL);
	assert(tree[5]);
	tree[4] = new_ast(AST_ID, NULL, NULL);
	assert(tree[4]);
	tree[3] = new_ast(AST_OR, tree[4], tree[5]);
	assert(tree[3]);
	tree[2] = new_ast(AST_NOT, NULL, tree[3]);
	assert(tree[2]);
	tree[1] = new_ast(AST_ID, NULL, NULL);
	assert(tree[1]);
	tree[0] = new_ast(AST_AND, tree[1], tree[2]);
	assert(tree[0]);

	return tree[0];
}

void
test_ast(void)
{
	int status;
	ast_t root;

	root = make_tree();

	status = delete_ast(root, NULL);
	assert(status == 0);
}


static void
ast_print(const ast_t self)
{
	char *item;

	if (!self)
		return;

	switch (ast_get_type(self)) {
	case AST_ID:
		item = (char *) ast_get_item(self);
		printf("identifier(\"%s\")\n", item ? item : "(null)");
		break;
	case AST_AND:
		printf("and\n");
		break;
	case AST_OR:
		printf("or\n");
		break;
	case AST_NOT:
		printf("not\n");
		break;
	}
}

void
test_ast_itor(void)
{
	int status;
	ast_t root, n;
	ast_itor_t itor;

	root = make_tree();

	itor = new_ast_itor(root);
	assert(itor);

	for (n = ast_itor_first(itor); n; n = ast_itor_next(itor))
		ast_print(n);

	status = delete_ast_itor(itor);
	assert(status == 0);

	status = delete_ast(root, NULL);
	assert(status == 0);

	/* TODO:
	 * 1. Check if memory is properly deallocated if iteration is
	 * interrupted.
	 * 2. Check whether two iterations in a row work flawlessly or not.
	 */
}
