%{
/**
 * @file scanner.l
 * @brief Scanner for Gnostic's task files.
 *
 * @author Juan M. Bello Rivas <rwx+gnostic@synnergy.net>
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

#include "err.h"
#include "xmemory.h"


#define YY_NO_UNPUT	1

/*
 * lineno is prefered over %option yylineno because the latter sets some
 * warnings during compilation.
 */
unsigned int lineno = 1;
%}

%x ACTIONS

%option noyywrap

IDENTIFIER	[A-Za-z_][A-Za-z1-9\-_]*

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
	++lineno;

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
	++lineno;

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
	++lineno;
}

. {
	return yytext[0];
}

%%

void
yyerror(char *s)
{
	fatal_error("gnostic: %s in line %d\n", s, lineno);
}
