#ifndef VARS_H
#define VARS_H		1
/**
 * @file vars.h
 * @brief Environment variable management.
 *
 * @author Juan M. Bello Rivas <jmbr@superadditive.com>
 */


/** Environment variable declaration.
 */
struct var {
	struct var *next;	/**< Pointer to the next item in the list */
	char *nameval;		/**< Declaration of the form name=value */
};


extern struct var *new_var(char *nameval);
extern int delete_vars(struct var *head);


#endif /* !VARS_H */
