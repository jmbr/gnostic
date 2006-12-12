/**
 * @file test-XXX.c
 * @brief XXX unit test.
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


static void test_foo(void);
static void test_bar(void);
static void test_baz(void);


int
main(int argc, char *argv[])
{
	test_foo();
	test_bar();
	test_baz();

	exit(EXIT_SUCCESS);
}


void
test_foo(void)
{
	assert(0);
}


void
test_bar(void)
{
	assert(0);
}


void
test_baz(void)
{
	assert(0);
}
