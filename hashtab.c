/**
 * @file hashtab.c
 * @brief A simple hash table implementation.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
 */
/* TODO This chained hash table should be replaced by one with linear probing.
 * That would reduce the overhead due to allocating hashnodes and would make
 * iteration *much* simpler.
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdlib.h>
#endif /* STDC_HEADERS */

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */

#include <assert.h>

#include "hashtab.h"

#include "xmemory.h"


struct hashnode {
	const void *key;
	void *value;
	struct hashnode *next;
};

struct hashtab_st {
	size_t len;
	size_t nitems;
	struct hashnode **tab;
	hashtab_cmp cmp;
	hashtab_dtor dtor;
};


static int hash(const void *key, size_t len);

static struct hashnode *new_hashnode(const void *key, void *value);
#define delete_hashnode(hn)		free(hn);


/* DJBX33A hash function */
int
hash(const void *key, size_t len)
{
	int h;
	ssize_t i;
	const unsigned char *p;

	assert(key && len > 0);

	for (h = 0, p = key, i = len; i; i--, p++)
		h = h * 33 + *p;

	return h;
}


struct hashnode *
new_hashnode(const void *key, void *value)
{
	struct hashnode *hn;

	assert(value);

	hn = xmalloc(sizeof(struct hashnode));

	hn->key = key;
	hn->value = value;
	hn->next = NULL;

	return hn;
}


hashtab_t
new_hashtab(size_t len, hashtab_cmp cmp, hashtab_dtor dtor)
{
	struct hashtab_st *ht;

	if (!cmp)
		return NULL;

	ht = xmalloc(sizeof(struct hashtab_st));

	ht->nitems = 0;
	ht->len = (len == 0) ? HASHTAB_DEFAULT_LEN : len;
	ht->tab = xcalloc(ht->len, sizeof(struct hashnode));
	ht->cmp = cmp;
	ht->dtor = dtor;

	return ht;
}

int
delete_hashtab(hashtab_t self)
{
	int i;

	if (!self)
		return -1;

	for (i = 0; i < self->len; i++) {
		struct hashnode *n, *next;

		for (n = self->tab[i]; n; n = next) {
			next = n->next;
			if (self->dtor && (self->dtor(n->value) == -1))
				return -1;
			delete_hashnode(n);
		}
	}
	free(self->tab);
	free(self);

	return 0;
}


void *
hashtab_lookup(hashtab_t self, const void *key, size_t len,
	       int create, void *value)
{
	int h;
	struct hashnode *n;

	assert(self && key);

	h = hash(key, len) % self->len;

	if (create) {
		n = new_hashnode(key, value);
		assert(n);

		n->next = self->tab[h];
		self->tab[h] = n;

		++self->nitems;

		return (void *) n->value;
	}

	for (n = self->tab[h]; n; n = n->next)
		if (self->cmp(n->key, key) == 0)
			return (void *) n->value;

	return NULL;
}

ssize_t
hashtab_get_len(const hashtab_t self)
{
	if (!self)
		return -1;

	return self->nitems;
}
