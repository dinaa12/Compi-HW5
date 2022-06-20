%{

/* Declarations section */
#include <stdio.h>
#include "semanticTypes.h"
#include "hw3_output.hpp"
#include "parser.tab.hpp"

%}

%option yylineno
%option noyywrap
relop_compare										(<=|>=|<|>)
relop_equal                                         (==|!=)
whitespace											([\t\n\r ])

%%

void												return VOID;
int													return INT;
byte												return BYTE;
b													return B;
bool												return BOOL;
auto												return AUTO;

and													return AND;
or													return OR;
not													return NOT;
true												return TRUE;
false												return FALSE;

return												return RETURN;
if													return IF;
else												return ELSE;
while												return WHILE;
break												return BREAK;
continue											return CONTINUE;

;													return SC;
,													return COMMA;
\(													return LPAREN;
\)													return RPAREN;
\{													return LBRACE;
\}													return RBRACE;
=													return ASSIGN;

{relop_equal}										yylval = new STRelopEqual("RELOP_EQUAL", yytext); return RELOP_EQUAL;
{relop_compare}										yylval = new STRelopCompare("RELOP_COMPARE", yytext); return RELOP_COMPARE;
\+  											    yylval = new STBinopAdd("BINOP_ADD", yytext); return BINOP_ADD;
\-  											    yylval = new STBinopSub("BINOP_SUB", yytext); return BINOP_SUB;
\*  											    yylval = new STBinopMUL("BINOP_MUL", yytext); return BINOP_MUL;
\/  											    yylval = new STBinopDiv("BINOP_DIV", yytext); return BINOP_DIV;

[a-zA-Z][a-zA-Z0-9]*						        yylval = new STID("ID", yytext); return ID;

0|[1-9][0-9]*         					            yylval = new STNum("NUM", yytext); return NUM;
\"([^\n\r\"\\]|\\[rnt"\\])+\"					    yylval = new STString("STRING", yytext); return STRING;

\/\/[^\r\n]*[\r|\n|\r\n]?                           ; /* ignore comment */

{whitespace}										;
.													{ output::errorLex(yylineno); exit(0); }

%%