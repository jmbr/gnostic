/**
 * @file test-tasklist.c
 * @brief Tasklist unit test.
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

#include "tasklist.h"

#include "xmemory.h"


static struct tasklist *tl = NULL;


static void test_new_tasklist(void);
static void test_tasklist_append(void);
static void test_tasklist_map(void);
static void test_delete_tasklist(void);


int
main(int argc, char *argv[])
{
	test_new_tasklist();
	test_tasklist_append();
	test_tasklist_map();
	test_delete_tasklist();

	exit(EXIT_SUCCESS);
}


void
test_new_tasklist(void)
{
	assert(!tl);

	tl = new_tasklist();
	assert(tl);

	assert(tl->head == NULL);
	assert(tl->tail == NULL);
}


void
test_delete_tasklist(void)
{
	assert(tl);

	delete_tasklist(tl);
}

#define dup(s)		xstrdup(s)

void
test_tasklist_append(void)
{
	int status;
	struct task *t, *u;

	assert(tl);

	status = tasklist_append(NULL, NULL);
	assert(status == -1);
	status = tasklist_append(tl, NULL);
	assert(status == -1);

	t = new_task(dup("foo"), NULL, dup(""));
	assert(t);

	status = tasklist_append(tl, t);
	assert(status == 0);

	assert(tl->head == t);
	assert(tl->head == tl->tail);

	u = new_task(dup("bar"), NULL, dup(""));
	assert(u);

	status = tasklist_append(tl, u);
	assert(status == 0);

	assert(tl->head == t);
	assert(tl->tail == u);
}

#undef dup

static int
good(struct task *t)
{
	int status;

	assert(t);

	status = task_print(t);
	assert(status == 0);

	return 0;
}

static int
bad(struct task *t)
{
	assert(t);

	return -1;
}

void
test_tasklist_map(void)
{
	int status;

	assert(tl);

	assert(tl->head != tl->tail);

	status = tasklist_map(NULL, NULL);
	assert(status == -1);

	status = tasklist_map(tl, NULL);
	assert(status == -1);

	status = tasklist_map(tl, good);
	assert(status == 0);

	status = tasklist_map(tl, bad);
	assert(status == -1);
}
