#ifndef SCANNER_H
#define SCANNER_H		1
/*
 * scanner.h -- Interface for the dependency expression lexical analyzer.
 * $Id$
 */


typedef struct yy_buffer_state *YY_BUFFER_STATE;


extern int yylex(void);
extern void yyerror(const char *s);
extern YY_BUFFER_STATE yy_scan_string(const char *str);
extern void yy_delete_buffer (YY_BUFFER_STATE b);


#endif /* !SCANNER_H */
