/*
 * htab.c -- Hash tables for the masses.
 * $Id$
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

#include "htab.h"


#define HTAB_DEFAULT_LEN		797
//#define HTAB_DEFAULT_LEN		6421


struct hnode {
	const void *key;
	const void *value;
	struct hnode *next;
};

struct htab {
	unsigned int len;
	struct hnode **tab;
	htab_cmp cmp;
};


static int hash(const void *key, size_t len);

static struct hnode *new_hnode(const void *key, const void *value);
#define delete_hnode(hn)		free(hn);


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


struct hnode *
new_hnode(const void *key, const void *value)
{
	struct hnode *hn;

	assert(value);

	hn = malloc(sizeof(struct hnode));
	if (!hn)
		return NULL;

	hn->key = key;
	hn->value = value;
	hn->next = NULL;

	return hn;
}


struct htab *
new_htab(htab_cmp cmp)
{
	struct htab *ht;

	if (!cmp)
		return NULL;

	ht = malloc(sizeof(struct htab));
	if (!ht)
		return NULL;

	ht->len = HTAB_DEFAULT_LEN;
	ht->tab = calloc(ht->len, sizeof(struct hnode));
	if (!ht->tab) {
		free(ht);
		return NULL;
	}
	ht->cmp = cmp;

	return ht;
}

void
delete_htab(struct htab *self)
{
	int i;

	assert(self);

	for (i = 0; i < self->len; i++) {
		struct hnode *n, *next;

		for (n = self->tab[i]; n; n = next) {
			next = n->next;
			delete_hnode(n);
		}
	}
	free(self->tab);
	free(self);
}


void *
htab_lookup(struct htab *self, const void *key, size_t len,
	int create, const void *value)
{
	int h;
	struct hnode *n;

	assert(self && key);

	h = hash(key, len) % self->len;

	if (create) {
		n = new_hnode(key, value);
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
