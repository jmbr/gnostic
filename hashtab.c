/**
 * @file hashtab.c
 * @brief A simple hash table implementation.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
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


struct hashnode {
	const void *key;
	const void *value;
	struct hashnode *next;
};

struct hashtab {
	size_t len;
	struct hashnode **tab;
	hashtab_cmp cmp;
};


static int hash(const void *key, size_t len);

static struct hashnode *new_hashnode(const void *key, const void *value);
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
new_hashnode(const void *key, const void *value)
{
	struct hashnode *hn;

	assert(value);

	hn = malloc(sizeof(struct hashnode));
	if (!hn)
		return NULL;

	hn->key = key;
	hn->value = value;
	hn->next = NULL;

	return hn;
}


struct hashtab *
new_hashtab(size_t len, hashtab_cmp cmp)
{
	struct hashtab *ht;

	if (!cmp)
		return NULL;

	ht = malloc(sizeof(struct hashtab));
	if (!ht)
		return NULL;

	ht->len = (len == 0) ? HASHTAB_DEFAULT_LEN : len;
	ht->tab = calloc(ht->len, sizeof(struct hashnode));
	if (!ht->tab) {
		free(ht);
		return NULL;
	}
	ht->cmp = cmp;

	return ht;
}

void
delete_hashtab(struct hashtab *self)
{
	int i;

	assert(self);

	for (i = 0; i < self->len; i++) {
		struct hashnode *n, *next;

		for (n = self->tab[i]; n; n = next) {
			next = n->next;
			delete_hashnode(n);
		}
	}
	free(self->tab);
	free(self);
}


void *
hashtab_lookup(struct hashtab *self, const void *key, size_t len,
	int create, const void *value)
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
		return (void *) n->value;
	}

	for (n = self->tab[h]; n; n = n->next)
		if (self->cmp(n->key, key) == 0)
			return (void *) n->value;

	return NULL;
}
