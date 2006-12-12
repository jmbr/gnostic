#ifndef TASKLIST_H
#define TASKLIST_H    1
/**
 * @file tasklist.h
 * @brief List of task structures.
 *
 * @author Juan M. Bello Rivas <jmbr@superadditive.com>
 */


#include "task.h"


struct tasklist {
	struct task *head;
	struct task *tail;
};

typedef int (*tasklist_fn)(struct task *);


/** tasklist constructor.
 * Allocates and initializes tasklist structures.
 *
 * @return A pointer to the allocated structure.
 *
 * @see delete_tasklist
 */
extern struct tasklist *new_tasklist(void);

/** tasklist destructor.
 * Frees all the resources associated to the tasklist.
 *
 * @param self A pointer to the structure.
 * @return 0 on success, -1 on failure.
 *
 * @see new_tasklist
 */
extern int delete_tasklist(struct tasklist *self);

/** tasklist append.
 * Adds a new task at the tail of the list.
 *
 * @param self A pointer to the structure.
 * @param t A pointer to the task to be appended.
 * @return 0 on success, -1 on failure.
 */
extern int tasklist_append(struct tasklist *self, struct task *t);

/** tasklist map function.
 * Applies the function specified by the last argument to each of the tasks
 * contained in the list. If a function returns -1 when it is applied to a
 * given task the whole loop will be canceled.
 *
 * @param self A pointer to the structure.
 * @param fn A ponter to the function to be applied.
 * @return 0 on success, -1 on failure.
 */
extern int tasklist_map(struct tasklist *self, tasklist_fn fn);


#endif /* !TASKLIST_H */
