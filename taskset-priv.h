#ifndef TASKSET_PRIV_H
#define TASKSET_PRIV_H		1
/**
 * @file taskset-priv.h
 * @brief Private declaration of tasksets.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
 */


#include "task.h"
#include "taskset.h"
#include "tasklist.h"
#include "hashtab.h"


/** An aggregate of tasks.
 */
struct taskset {
	struct env_var *env_vars;	/**< Pointer to the head of a list of
					  env vars */
	struct tasklist *tasks;		/**< List of tasks. */
	hashtab_t symtab;		/**< Symbol table mapping task names to
					  task structure addresses */
};


/** Configuration file parser.
 * Fills a taskset structure with the contents specified in the given
 * configuration file..
 *
 * @param self A pointer to the taskset structure.
 * @param filename The name of the file to be parsed or NULL if the caller
 * wants to read from stdin.
 * @returns 0 on success, -1 on failure.
 *
 * @see new_taskset, taskset_read
 */
extern int taskset_parse(struct taskset *self, const char *filename);


#endif /* !TASKSET_PRIV_H */
