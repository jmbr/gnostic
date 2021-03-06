/**
 * @file test-hashtab.c
 * @brief Hash table unit test.
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

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif /* HAVE_UNISTD_H */

#include <sys/stat.h>
#include <fcntl.h>

#include <signal.h>

#include <sys/mman.h>

#include <assert.h>

#include "hashtab.h"

#include "xmemory.h"


static int fd;
static char *m;
static size_t len;

static hashtab_t ht = NULL;


static void test_hashtab_lookup(void);
static void test_hashtab_itor(void);


int
main(int argc, char *argv[])
{
	test_hashtab_lookup();
	test_hashtab_itor();

	exit(EXIT_SUCCESS);
}


static void
map(void)
{
	int status;
	struct stat st;

	fd = open("/usr/share/dict/words", O_RDONLY);
	assert(fd != -1);

	status = fstat(fd, &st);
	assert(status == 0);

	len = (size_t) st.st_size + 1;

	m = mmap(0, len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, (off_t) 0);
	assert(m != MAP_FAILED);

	*(m + len - 1) = '\0'; /* Turn the mmapped file into a valid string */
}

static void
unmap(void)
{
	int status;

	status = munmap(m, len);
	assert(status == 0);

	status = close(fd);
	assert(status == 0);
}

static inline char *
get_word(char *text)
{
	char *start;
	static char *p = NULL;

	start = p = (p) ? p : text;

	p = strchr(p, '\n');
	if (!p)
		return NULL;

	*p++ = '\0';

	return start;
}

static void
dict_set(void)
{
	char *v, *w;

	map();

	for (w = get_word(m); w; w = get_word(NULL)) {
		if (strlen(w) == 0)
			continue;
		v = xstrdup(w);
		hashtab_strlookup(ht, w, 1, v);
	}

	unmap();
}

static void
dict_get(void)
{
	char *v, *w;

	map();

	for (w = get_word(m); w; w = get_word(NULL)) {
		if (strlen(w) == 0)
			continue;
		v = hashtab_strlookup(ht, w, 0, NULL);
	}

	unmap();
}

int
strfree(char *s)
{
	free(s);
	
	return 0;
}

void
test_hashtab_lookup(void)
{
	int status;
	
	assert(!ht);

	ht = new_hashtab(6421, (hashtab_cmp) strcmp, (hashtab_dtor) strfree);

	dict_set();
	dict_get();

	status = delete_hashtab(ht);
	assert(status == 0);
	ht = NULL;
}

void
test_hashtab_itor(void)
{
#if 0
	char *s;
	int status;
	hashtab_itor_t hi;

	assert(!ht);

	ht = new_hashtab(0, (hashtab_cmp) strcmp, NULL);
	assert(ht);

	hi = new_hashtab_itor(ht);
	assert(hi);

	hashtab_strlookup(ht, "a", 1, xstrdup("aaa"));
	/*
	hashtab_strlookup(ht, "b", 1, xstrdup("bbb"));
	hashtab_strlookup(ht, "c", 1, xstrdup("ccc"));
	*/

	for (s = hashtab_itor_first(hi); s; s = hashtab_itor_next(hi)) {
		printf("%s\n", s);
		xfree(s);
	}

	free(hi);
	/*
	status = delete_hashtab_itor(itor);
	assert(status == 0);
	*/

	status = delete_hashtab(ht);
	assert(status == 0);
	ht = NULL;
#endif
}
