#ifndef AST_PRIV_H
#define AST_PRIV_H		1
/**
 * @file ast-priv.h
 * @brief AST private declarations.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
 */


#include "ast.h"


#define AST_MAX_CHILDREN		2


/** Node of an abstract syntax tree.
 *
 * These nodes compose dependency expressions.
 */
struct ast_st {
	void *item;				/**< Payload */
	enum ast_type type;			/**< Node type */
	ast_t children[AST_MAX_CHILDREN];	/**< Children */
};


#endif /* !AST_PRIV_H */
