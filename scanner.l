%{
/**
 * @file scanner.l
 * @brief Scanner for Gnostic's task files.
 *
 * @author Juan M. Bello Rivas <jmbr@superadditive.com>
 */


#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef STDC_HEADERS
# include <stdio.h>
# include <stdlib.h>
#endif /* STDC_HEADERS */

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */

#include <errno.h>

#include "grammar.h"

#include "stack.h"
#include "logger.h"
#include "xmemory.h"


#define YY_NO_UNPUT	1


/* TODO lineno is not enough. When including files their name and current line
 * number should be pushed to the stack for better error reporting.
 */
unsigned int lineno = 1;

stack_t buffers = NULL;
%}

%x INCLUDE

%option noyywrap

IDENTIFIER	[A-Za-z_][A-Za-z1-9\-_]*

%%

include {
	BEGIN(INCLUDE);
}

<INCLUDE>[ \t]+	{
	;
}

<INCLUDE>[^ \t\n]+ {
	if (!buffers) buffers = new_stack();
	stack_push(buffers, YY_CURRENT_BUFFER);

	yyin = fopen(yytext, "r");
	if (!yyin) {
		if (errno == EMFILE) {
			/* XXX This isn't the best way to report the error.
			 * But it is a good enough workaround for the moment.
			 */
			fatal_error("Too many open files. This problem is " \
				    "probably caused by circular include " \
				    "directives.\n");
		} else {
			fatal_error("Unable to open included file `%s'\n", \
				    yytext);
		}
	}

	yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));

	BEGIN(INITIAL);
}

<<EOF>> {
	if (!buffers || stack_peek(buffers) == NULL) {
		delete_stack(buffers);
		yyterminate();
	} else {
		yy_delete_buffer(YY_CURRENT_BUFFER);
		yy_switch_to_buffer(stack_pop(buffers));
	}
}

and {
        return AND;
}

or {
        return OR;
}

not {
        return NOT;
}

{IDENTIFIER}=.*$ {
	++lineno;

        yylval.s = xstrdup(yytext);

        return VAR_DECL;
}

{IDENTIFIER} {
        yylval.s = xstrdup(yytext);

        return IDENTIFIER;
}

^\t.*\n {
	++lineno;

	/*
	 * We discard the initial '\t' but not the comments because losing them
	 * would lead to confusing line numbers in error messages during task
	 * execution.
	 */
        yylval.s = xstrdup(yytext + 1);

        return ACTION;
}

^[ \t]*#.*$ {
	/* Ignore comments */
	++lineno;
}

[\ \t] {
	/* Ignore spaces */
}

\n {
	/* Ignore newlines */
	++lineno;
}

. {
	return yytext[0];
}

%%

void
yyerror(char *s)
{
	fatal_error("%s in line %d\n", s, lineno);
}
