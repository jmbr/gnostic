/**
 * @file vars.c
 * @brief Environment variable management.
 *
 * @author Juan M. Bello Rivas <rwx@synnergy.net>
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdlib.h>
#endif /* STDC_HEADERS */

#include "vars.h"

#include "xmemory.h"


/*
 * delete_vars is responsible for freeing nameval (which, btw is supposed to
 * come from an strduped string read by the lexer).
 */
struct var *
new_var(char *nameval)
{
	struct var *v;

	v = xmalloc(sizeof(struct var));
	v->nameval = nameval;

	return v;
}


int
delete_vars(struct var *head)
{
	struct var *n, *next;

	if (!head)
		return -1;

	for (n = head; n; n = next) {
		next = n->next;
		xfree(n->nameval);
		xfree(n);
	}

	return 0;
}
