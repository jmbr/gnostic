%{
/**
 * @file grammar.y
 * @brief Grammar for Gnostic's task files.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
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

#include "tasklist.h"
#include "taskset.h"
#include "ast.h"

#include "err.h"
#include "xmemory.h"


extern int yylex(void);
extern void yyerror(const char *s);


struct var *vars = NULL;
struct tasklist *tasks = NULL;

static char *strconcat(char *s1, char *s2);

static astnode_t do_identifier(char *s);
static astnode_t do_not(astnode_t rhs);
static astnode_t do_or(astnode_t lhs, astnode_t rhs);
static astnode_t do_and(astnode_t lhs, astnode_t rhs);
%}

/*%error-verbose*/

%token AND OR NOT
%token VAR_DECL IDENTIFIER ACTION

%union {
	char *s;
	struct var *v;
	struct task *t;
	struct astnode *n;
}

%left AND OR
%left NOT

%start conf

%type <n> expr 
%type <t> tasks task
%type <v> variable variables
%type <s> IDENTIFIER ACTION action actions VAR_DECL

%%

conf		: tasks
      		| variables tasks {
			vars = $1;
      		}
		;

variables	: variable
	  	| variables variable {
			$2->next = $1;
			$$ = $2;
		}
		;

variable	: VAR_DECL {
	 		$$ = new_var($1);
		}
		;

tasks           : task {
			tasks = new_tasklist();
			tasklist_append(tasks, $1);
		}
		| tasks task {
			tasklist_append(tasks, $2);
		}
                ;

task            : IDENTIFIER ':' expr actions {
			$$ = new_task($1, $3, $4);
		}
		| IDENTIFIER ':' actions {
			$$ = new_task($1, NULL, $3);
		}
                ;

actions		: action
	 	| actions action {
			$$ = strconcat($1, $2);
		}
		;

action		: ACTION {
			$$ = $1;
		}
		;
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


char *
strconcat(char *s1, char *s2)
{
	size_t len1 = strlen(s1), len2 = strlen(s2);

	s1 = xrealloc(s1, len1 + len2 + 1);
	memcpy(s1 + len1, s2, len2 + 1);
	free(s2);

	return s1;
}


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
