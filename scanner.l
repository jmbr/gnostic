%{
/*
 * scanner.l -- Scanner for gnostic's dependency expressions.
 * $Id$
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

#include "grammar.h"

#include "xalloc.h"
#include "debug.h"


#define YY_NO_UNPUT	1


unsigned int nline = 1;
%}

%x ACTIONS
%option noyywrap

IDENTIFIER	[A-Za-z][A-Za-z0-9\-_]+

%%

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
	++nline;
        yylval.s = xstrdup(yytext);
        return VAR_DECL;
}

{IDENTIFIER} {
        yylval.s = xstrdup(yytext);
        return IDENTIFIER;
}

^\t {
        BEGIN(ACTIONS);
}

<ACTIONS>.*\n {
	++nline;
        BEGIN(INITIAL);
        yylval.s = xstrdup(yytext);
        return ACTION;
}

^#.*$ {
	/* Ignore comments */
}

[\ \t] {
	/* Ignore spaces */
}

\n {
	/* Ignore newlines */
	++nline;
}

. {
	return yytext[0];
}

%%

void
yyerror(char *s)
{
	fatal_error("%s in line %d\n", s, nline);
}
