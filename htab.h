#ifndef HTAB_H
#define HTAB_H		1
/*
 * htab.h -- Hash tables for the masses.
 * $Id$
 */


struct htab;

typedef struct htab *htab_t;

typedef int (*htab_cmp)(const void *, const void *);


extern struct htab *new_htab(htab_cmp cmp);
extern void delete_htab(struct htab *self);

extern void *htab_lookup(struct htab *self, const void *key, size_t len,
			int create, const void *value);

#define htab_strlookup(s, k, c, v) htab_lookup((s), (k), strlen((k)), (c), (v))


#endif /* !HTAB_H */
