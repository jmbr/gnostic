#ifndef AST_H
#define AST_H    1
/**
 * @file ast.h
 * @brief Abstract syntax tree for dependency expressions.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
 */


/** Node types.
 */
enum astnode_types {
	AST_ID,		/**< Identifier */
	AST_AND,	/**< Boolean and */
	AST_OR,		/**< Boolean or */
	AST_NOT,	/**< Boolean not */
};

typedef struct astnode *astnode_t;

typedef int (*astnode_item_dtor)(void *);


/** astnode constructor.
 * Allocates and initializes astnode structures.
 *
 * @return A pointer to the allocated structure.
 *
 * @see delete_astnode
 */
extern astnode_t new_astnode(enum astnode_types type,
				astnode_t lhs, astnode_t rhs);

/** astnode destructor.
 * Frees all the resources associated to the astnode.
 *
 * @param self A pointer to the structure.
 * @return 0 on success, -1 on failure.
 *
 * @see new_astnode
 */
extern int delete_astnode(astnode_t self);

/** Node child accessor.
 * Returns a pointer to the left child of the specified node.
 *
 * @param self A pointer to the node.
 * @return An astnode_t on success, NULL on failure.
 *
 * @see astnode_get_rhs, new_astnode
 */
extern astnode_t astnode_get_lhs(const astnode_t self);

/** Node child accessor.
 * Returns a pointer to the right child of the specified node.
 *
 * @param self A pointer to the node.
 * @return An astnode_t on success, NULL on failure.
 *
 * @see astnode_get_lhs, new_astnode
 */
extern astnode_t astnode_get_rhs(const astnode_t self);

/** Node type accessor.
 * Informs about the given node's type.
 *
 * @param self A pointer to the node.
 * @return The node type on success, 0 on failure.
 * 
 * @see new_astnode
 */
extern enum astnode_types astnode_get_type(const astnode_t self);

/** Item mutator.
 * Attaches an item to a given node.
 * Note that delete_astnode won't free the memory associated to the item
 * pointer. It is the caller's responsibility to do that.
 *
 * @param self A pointer to the node.
 * @param item A pointer to the item going to be referenced.
 * @return 0 on success, -1 on failure.
 *
 * @see ast_get_item, delete_astnode
 */
extern int astnode_set_item(astnode_t self, void *item);

/** Item accessor.
 * Fetches the item attached to a node.
 *
 * @param self A pointer to the node.
 * @return A pointer to the item on success, NULL otherwise.
 *
 * @see ast_set_item
 */
extern void *astnode_get_item(const astnode_t self);


/** Abstract syntax tree destructor.
 * Frees all the resources associated to the AST.
 *
 * @param self A pointer to the structure.
 * @param astnode_item_dtor Destructor callback. Frees the resources associated
 * with an item.
 * @return 0 on success, -1 on failure.
 */
extern int delete_ast(astnode_t self, astnode_item_dtor dtor);


#endif /* !AST_H */
