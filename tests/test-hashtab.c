/*
 * test-hashtab.c -- Unit test for hash tables.
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

#ifndef __USE_GNU
# define __USE_GNU
#endif /* !__USE_GNU */

#ifdef HAVE_STRING_H
# include <string.h>
#elif HAVE_STRINGS_H
# include <strings.h>
#endif /* !HAVE_STRING_H */

#include <assert.h>

#include "hashtab.h"


static void test_hashtab_ctor_dtor(void);
static void test_hashtab_basic(void);
static void test_hashtab_heavy(void);


int
main(int argc, char *argv[])
{
	test_hashtab_ctor_dtor();
	test_hashtab_basic();
	test_hashtab_heavy();

	exit(EXIT_SUCCESS);
}


void
test_hashtab_ctor_dtor(void)
{
	struct hashtab *tab;

	tab = new_hashtab(0, (hashtab_cmp) strcmp);
	assert(tab);

	delete_hashtab(tab);
}

void
test_hashtab_basic(void)
{
	int status;
	struct hashtab *tab;
	const char *k = "tcp", *v = "root";
	void *value;

	tab = new_hashtab(0, (hashtab_cmp) strcmp);
	assert(tab);

	value = hashtab_lookup(tab, k, strlen(k), 1, v);
	assert(value);

	status = strcmp((char *) value, v);
	assert(status == 0);

	delete_hashtab(tab);
}

void
test_hashtab_heavy(void)
{
	void *v;
	FILE *fp;
	int status, i;
	char buf[256], *word = NULL, *frob;
	struct hashtab *tab;

	fp = fopen("/usr/share/dict/words", "r");
	assert(fp);

	tab = new_hashtab(6421, (hashtab_cmp) strcmp);
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

		v = hashtab_lookup(tab, word, strlen(word), 1, frob);
		assert(v);

		status = strcmp((char *) v, frob);
		assert(status == 0);

		//if (i % 1000 == 0) printf("%s --> %s\n", word, (char *) v);
	}

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

		v = hashtab_lookup(tab, word, strlen(word), 0, NULL);
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
	delete_hashtab(tab);
	fclose(fp);
}
