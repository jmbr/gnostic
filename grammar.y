%{
/*
 * grammar.y -- Grammar for gnostic's configuration files.
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdlib.h>
#endif /* STDC_HEADERS */

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */

#ifdef HAVE_STRING_H
# include <string.h>
#elif HAVE_STRINGS_H
# include <strings.h>
#endif /* !HAVE_STRING_H */

#include <assert.h>

#include "task.h"
#include "task-collection.h"
#include "ast.h"
#include "scanner.h"

#include "xalloc.h"
#include "debug.h"


struct task *tasks = NULL;
struct env_var *env_vars = NULL;


static astnode_t do_identifier(char *s);
static astnode_t do_not(astnode_t rhs);
static astnode_t do_or(astnode_t lhs, astnode_t rhs);
static astnode_t do_and(astnode_t lhs, astnode_t rhs);
%}

%token AND OR NOT
%token VAR_DECL IDENTIFIER ACTION

%union {
	char *s;
	struct task *t;
	struct env_var *v;
	struct astnode *n;
}

%left AND OR
%left NOT

%start conf

%type <t> tasks task
%type <n> dependencies expr 
%type <s> IDENTIFIER ACTION action actions VAR_DECL
%type <v> variable variables

%%

conf		: tasks {
      			tasks = $1;
		}
      		| variables tasks {
			env_vars = $1;
      			tasks = $2;
      		}
		;

variables	: variable
	  	| variables variable {
			$2->next = $1;
			$$ = $2;
		}
		;

variable	: VAR_DECL {
	 		struct env_var *var;

	 		var = xmalloc(sizeof(struct env_var));
			var->v = $1;

			$$ = var;
		}
		;

tasks           : task
		| tasks task {
			task_set_next($2, $1);
			$$ = $2;
		}
                ;

task            : IDENTIFIER ':' dependencies actions {
			$$ = new_task($1, $3, $4);
		}
                ;

actions		: action
	 	| actions action {
			size_t len1 = strlen($1), len2 = strlen($2);

			$1 = xrealloc($1, len1 + len2 + 1);
			memcpy($1 + len1, $2, len2 + 1);
			free($2);
			$$ = $1;
		}
		;

action		: ACTION {
			$$ = $1;
		}
		;

dependencies	: expr {
      			$$ = $1;
		}
		| /* Empty */ {
			$$ = NULL;
		}
		;

expr		: '(' expr ')' {
      			$$ = $2;
      		}
      		| expr AND expr {
			$$ = do_and($1, $3);
		}
      		| expr OR expr {
			$$ = do_or($1, $3);
		}
		| NOT expr %prec NOT {
			$$ = do_not($2);
		}
		| IDENTIFIER {
			$$ = do_identifier($1);
		}
		;

%%

astnode_t
do_identifier(char *s)
{
	astnode_t n;

	assert(s);

	n = new_astnode(N_ID, NULL, NULL);
	astnode_set_item(n, (void *) s);

	return n;
}

astnode_t
do_not(astnode_t rhs)
{
	return new_astnode(N_NOT, NULL, rhs);
}

astnode_t
do_and(astnode_t lhs, astnode_t rhs)
{
	return new_astnode(N_AND, lhs, rhs);
}

astnode_t
do_or(astnode_t lhs, astnode_t rhs)
{
	return new_astnode(N_OR, lhs, rhs);
}
