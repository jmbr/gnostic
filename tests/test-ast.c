/**
 * @file test-ast.c
 * @brief Abstract syntax tree unit test.
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

#include "ast.h"


static void test_ast(void);


int
main(int argc, char *argv[])
{
	test_ast();

	exit(EXIT_SUCCESS);
}

void
test_ast(void)
{
	int status;
	astnode_t tree[3];

	tree[2] = new_astnode(N_ID, NULL, NULL);
	assert(tree[2]);
	tree[1] = new_astnode(N_ID, NULL, NULL);
	assert(tree[1]);
	tree[0] = new_astnode(N_AND, tree[1], tree[2]);
	assert(tree[0]);

	status = delete_ast(tree[0], NULL);
	assert(status == 0);
}
