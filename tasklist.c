/**
 * @file tasklist.c
 * @brief List of task structures.
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

#ifdef HAVE_STRING_H
# include <string.h>
#elif HAVE_STRINGS_H
# include <strings.h>
#endif /* !HAVE_STRING_H */

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */

#include <assert.h>

#include "tasklist.h"

#include "xmemory.h"


#define tasklist_is_empty(self) ((self)->head == NULL && (self)->tail == NULL)


struct tasklist *
new_tasklist(void)
{
	struct tasklist *n;

	n = xmalloc(sizeof(struct tasklist));

	n->head = n->tail = NULL;

	return n;
}

int
delete_tasklist(struct tasklist *self)
{
	int status;
	struct task *node, *next;

	if (!self)
		return -1;

	for (node = self->head; node; node = next) {
		next = node->next;
		status = task_decref(node);
		assert(status == 0);
	}

	xfree(self);

	return 0;
}


int
tasklist_append(struct tasklist *self, struct task *t)
{
	if (!self || !t)
		return -1;

	if (tasklist_is_empty(self)) {
		self->head = self->tail = t;
	} else {
		t->prev = self->tail;
		t->next = NULL;
		self->tail->next = t;
		self->tail = t;
	}

	return 0;
}


/*
 * XXX Should be a foldr-like function instead of map
 * Perhaps its name should be changed to tasklist_apply instead?
 */
int
tasklist_map(struct tasklist *self, tasklist_fn fn)
{
	int status;
	struct task *t;

	if (!self || !fn)
		return -1;

	for (t = self->head, status = 0; (t && status == 0); t = t->next)
		status = fn(t);

	return status;
}
