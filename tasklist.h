#ifndef TASKLIST_H
#define TASKLIST_H    1
/**
 * @file tasklist.h
 * @brief List of task structures.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
 */


#include "task.h"


struct tasklist {
	struct task *head;
	struct task *tail;
};

typedef int (*tasklist_fn)(const struct task *);


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

extern void tasklist_append(struct tasklist *self, struct task *t);

extern int tasklist_map(struct tasklist *self, tasklist_fn fn);


#endif /* !TASKLIST_H */
