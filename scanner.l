%{
/**
 * @file scanner.l
 * @brief Scanner for gnostic's configuration files.
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


unsigned int cur_line = 1;
%}

%x ACTIONS
%option noyywrap

IDENTIFIER	[A-Za-z]+[A-Za-z0-9\-_]*

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
	++cur_line;

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
	++cur_line;

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
	++cur_line;
}

. {
	return yytext[0];
}

%%

void
yyerror(char *s)
{
	fatal_error("gnostic: %s in line %d\n", s, cur_line);
}
