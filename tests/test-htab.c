/*
 * test-htab.c -- Unit test for hash tables.
 * $Id$
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdio.h>
# include <stdlib.h>
# ifndef __USE_GNU
#  define __USE_GNU
# endif /* !__USE_GNU */
# include <string.h>
#endif /* STDC_HEADERS */

#include <assert.h>

#include "htab.h"


extern void htab_dump(const struct htab *self);

static void test_htab_ctor_dtor(void);
static void test_htab_basic(void);
static void test_htab_heavy(void);


int
main(int argc, char *argv[])
{
	test_htab_ctor_dtor();
	test_htab_basic();
	test_htab_heavy();

	exit(EXIT_SUCCESS);
}


void
test_htab_ctor_dtor(void)
{
	struct htab *tab;

	tab = new_htab((htab_cmp) strcmp);
	assert(tab);

	delete_htab(tab);
}

void
test_htab_basic(void)
{
	int status;
	struct htab *tab;
	const char *k = "tcp", *v = "root";
	void *value;

	tab = new_htab((htab_cmp) strcmp);
	assert(tab);

	value = htab_lookup(tab, k, strlen(k), 1, v);
	assert(value);

	status = strcmp((char *) value, v);
	assert(status == 0);

	delete_htab(tab);
}

void
test_htab_heavy(void)
{
	void *v;
	FILE *fp;
	int status, i;
	char buf[256], *word = NULL, *frob;
	struct htab *tab;

	fp = fopen("/usr/share/dict/words", "r");
	assert(fp);

	tab = new_htab((htab_cmp) strcmp);
	assert(tab);

	for (i = 0; ; i++) {
		memset(buf, '\0', sizeof(buf));
		fgets(buf, sizeof(buf) - 1, fp);
		if (feof(fp))
			break;
		assert(!ferror(fp));
		buf[strlen(buf) - 1] = '\0';

		word = strdup(buf);
		assert(word);

		frob = strdup(buf);
		assert(frob);

		memfrob(frob, strlen(frob));

		v = htab_lookup(tab, word, strlen(word), 1, frob);
		assert(v);

		status = strcmp((char *) v, frob);
		assert(status == 0);

		//if (i % 1000 == 0) printf("%s --> %s\n", word, (char *) v);
	}

	//htab_dump(tab);

	for (rewind(fp); i; i--) {
		memset(buf, '\0', sizeof(buf));
		fgets(buf, sizeof(buf) - 1, fp);
		if (feof(fp))
			break;
		assert(!ferror(fp));
		buf[strlen(buf) - 1] = '\0';

		word = strdup(buf);
		assert(word);
		frob = strdup(buf);
		assert(frob);
		memfrob(frob, strlen(frob));

		v = htab_lookup(tab, word, strlen(word), 0, NULL);
		assert(v);

		status = strcmp((char *) v, frob);
		//printf("%s --> %s | %s\n", word, (char *) v, frob);
		assert(status == 0);

		free(word);
		free(frob);
	}

	/*
	 * There's a huge memleak here because we lose references to each
	 * hnode's key so we cannot free it. It isn't important for this test
	 * anyway.
	 */
	delete_htab(tab);
	fclose(fp);
}
