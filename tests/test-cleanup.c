/*
 * test-cleanup.c -- Unit test for cleanup stacks.
 * $Id$
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
#endif /* STDC_HEADERS */

#include <assert.h>

#include "cleanup.h"


static void test_cleanup(void);


int
main(int argc, char *argv[])
{
	test_cleanup();

	exit(EXIT_SUCCESS);
}


void
test_cleanup(void)
{
	char *s;
	cleanupstack_t st;

	st = cleanup_setup();
	assert(st);

	for (int i = 0; i < 100; i++) {
		s = strdup("This is a test.");
		assert(s);
		cleanup_push(st, s, NULL);
	}

	cleanup(st);
}
