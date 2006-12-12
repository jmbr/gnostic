#ifndef STACK_H
#define STACK_H		1
/**
 * @file stack.h
 * @brief A fast stack abstract data type.
 *
 * @author Juan M. Bello Rivas <jmbr@superadditive.com>
 */


typedef struct stack_st *stack_t;


extern stack_t new_stack(void);
extern int delete_stack(stack_t self);

extern void stack_push(stack_t self, void *item);
extern void *stack_pop(stack_t self);
extern void *stack_peek(const stack_t self);


#endif /* !STACK_H */
