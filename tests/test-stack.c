/**
 * @file test-stack.c
 * @brief Stack unit test.
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

#include "stack.h"


static void test_stack(void);


int
main(int argc, char *argv[])
{
	test_stack();

	exit(EXIT_SUCCESS);
}


void
test_stack(void)
{
	int status, i;
	stack_t s;
	
	s = new_stack();
	assert(s);

	for (i = 1; i <= 64; i++)
		stack_push(s, (void *) i);

	while ((i = (int) stack_pop(s)))
		printf("%d\n", i);

	status = delete_stack(s);
	assert(status == 0);
}
