#ifndef HASHTAB_H
#define HASHTAB_H		1
/**
 * @file hashtab.h
 * @brief A simple chained hash table data structure.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STRING_H
# include <string.h>
#elif HAVE_STRINGS_H
# include <strings.h>
#endif /* !HAVE_STRING_H */


/**
 * Default hash table length.
 */
#define HASHTAB_DEFAULT_LEN		223	/* Tuned for gnostic */
/*
#define HASHTAB_DEFAULT_LEN		797
#define HASHTAB_DEFAULT_LEN		6421
*/


/** Chained hash table.
 */
typedef struct hashtab *hashtab_t;

/** Comparison function.
 * @returns 0 if the items pointed to by the parameters are equal.
 */
typedef int (*hashtab_cmp)(const void *, const void *);

/** Destructor function.
 * This function is a callback responsible for freeing the resources associated
 * to the item contained in the hash table.
 */
typedef int (*hashtab_dtor)(void *);


/** hashtab constructor.
 * Allocates and initializes hashtab structures.
 *
 * @param len Length of the desired table. A value of 0 means the constructor
 * will create a table of HASHTAB_DEFAULT_LEN buckets.
 * @param cmp Pointer to a function to compare items.
 * @param dtor Pointer to a function in charge of deallocation of values.
 * @return A pointer to the allocated structure.
 *
 * @see delete_hashtab
 */
extern hashtab_t new_hashtab(size_t len, hashtab_cmp cmp, hashtab_dtor dtor);

/** hashtab destructor.
 * Frees all the resources associated to the hashtab.
 *
 * @param self A pointer to the structure.
 * @return 0 on success, -1 on failure.
 *
 * @see new_hashtab
 */
extern int delete_hashtab(hashtab_t self);

/** hashtab lookup/creation.
 * Searches an item by key or creates a new key-value pair.
 *
 * @param self A pointer to the hash table.
 * @param key A pointer to the key to search or create.
 * @param len Length (in bytes) of the key.
 * @param create If true, creates a new entry in the table, else it looks up
 * the specified key.
 * @param value A pointer to the value to store. If create is false value
 * should point to a valid piece of data to be stored. Otherwise it should be
 * set to NULL.
 *
 * @see hashtab_strlookup
 */
extern void *hashtab_lookup(hashtab_t self, const void *key, size_t len,
			    int create, void *value);

/** hashtab string lookup/creation.
 * Same as the previous function, but for strings. The caller doesn't need to
 * provide the function with the key's length.
 */
static inline void *hashtab_strlookup(hashtab_t self, const char *key,
				      int create, void *value)
{
	return hashtab_lookup(self, (void *) key, strlen(key), create, value);
}


#endif /* !HASHTAB_H */
