#ifndef AST_H
#define AST_H    1
/**
 * @file ast.h
 * @brief Abstract syntax tree for dependency expressions.
 *
 * @author Juan M. Bello Rivas <jmbr@superadditive.com>
 */


/** Node types.
 */
enum ast_type {
	AST_ID,		/**< Identifier */
	AST_AND,	/**< Boolean and */
	AST_OR,		/**< Boolean or */
	AST_NOT,	/**< Boolean not */
};

typedef struct ast_st *ast_t;

typedef struct ast_itor_st *ast_itor_t;

/** AST item destructor callback */
typedef int (*ast_item_dtor)(void *);


/** ast constructor.
 * Allocates and initializes ast structures.
 *
 * @return A pointer to the allocated structure.
 *
 * @see delete_ast
 */
extern ast_t new_ast(enum ast_type type, ast_t lhs, ast_t rhs);

/** ast destructor.
 * Frees all the resources associated to the AST.
 *
 * @param self A pointer to the structure.
 * @param ast_item_dtor Destructor callback. Frees the resources associated
 * with an item.
 * @return 0 on success, -1 on failure.
 */
extern int delete_ast(ast_t self, ast_item_dtor dtor);

/** Node type accessor.
 * Informs about the given node's type.
 *
 * @param self A pointer to the node.
 * @return The node type on success, 0 on failure.
 * 
 * @see new_ast
 */
extern enum ast_type ast_get_type(const ast_t self);

/** Item mutator.
 * Attaches an item to a given node.
 * Note that delete_ast won't free the memory associated to the item
 * pointer. It is the caller's responsibility to do that.
 *
 * @param self A pointer to the node.
 * @param item A pointer to the item going to be referenced.
 * @return 0 on success, -1 on failure.
 *
 * @see ast_get_item, delete_ast
 */
extern int ast_set_item(ast_t self, void *item);

/** Item accessor.
 * Fetches the item attached to a node.
 *
 * @param self A pointer to the node.
 * @return A pointer to the item on success, NULL otherwise.
 *
 * @see ast_set_item
 */
extern void *ast_get_item(const ast_t self);

/** Node child accessor.
 * Returns a pointer to the left child of the specified node.
 *
 * @param self A pointer to the node.
 * @return An ast_t on success, NULL on failure.
 *
 * @see ast_get_rhs, new_ast
 */
extern ast_t ast_get_lhs(const ast_t self);

/** Node child accessor.
 * Returns a pointer to the right child of the specified node.
 *
 * @param self A pointer to the node.
 * @return An ast_t on success, NULL on failure.
 *
 * @see ast_get_lhs, new_ast
 */
extern ast_t ast_get_rhs(const ast_t self);


/** AST preorder iterator constructor.
 * Creates an instance of a preorder iterator for ASTs.
 *
 * @param root The root of the abstract syntax tree to be traversed.
 * @return A pointer to an iterator.
 *
 * @see delete_ast_itor
 */
extern ast_itor_t new_ast_itor(ast_t root);

/** AST preorder iterator destructor.
 * Frees the resources associated to an iterator.
 *
 * @param self A pointer to the structure.
 * @return 0 on success, -1 on failure.
 *
 * @see new_ast_itor
 */
extern int delete_ast_itor(ast_itor_t self);

/** AST iterator initializer.
 * Returns the first node of the AST. This is suitable for the initialization
 * part of a for loop.
 *
 * @param self A pointer to the structure.
 * @return A pointer to the root of the AST.
 *
 * @see ast_itor_next
 */
extern ast_t ast_itor_first(ast_itor_t self);

/** AST iterator update function.
 * Returns the next node in a preorder traversal of the AST.
 *
 * @param self A pointer to the structure.
 * @return A pointer to the current node of the AST.
 *
 * @see ast_itor_first
 */
extern ast_t ast_itor_next(ast_itor_t self);


#endif /* !AST_H */
