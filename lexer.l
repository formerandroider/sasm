%{
  #include "simpleasm_parser.tab.h"
%}

%option noyywrap

DIGIT [0-9]

%state IN_OPR_ARGS IN_LABEL

%%

<INITIAL>^--(.*)\n
<INITIAL>[a-z]+" "[A-Z]{3} {
    yyless(strlen(yytext)-3);
    yylval.LABEL = malloc(sizeof(char) * (strlen(yytext) + 1));
    sscanf(yytext, "%s", yylval.LABEL);
    BEGIN(IN_LABEL);
    return LABEL;
}
<INITIAL,IN_LABEL>[A-Z]{3} {
    if (yylval.OPERATION != NULL) {
       //yyfree(yylval.OPERATION);
    }
    yylval.OPERATION = yyalloc(sizeof(char) * (strlen(yytext) + 1));
    sprintf(yylval.OPERATION, "%s", yytext);
    BEGIN(IN_OPR_ARGS);
    return OPERATION;
}
<IN_OPR_ARGS>" "
<IN_OPR_ARGS>[a-z]+ {
    if (yylval.IDENTIFIER != NULL) {
       //yyfree(yylval.IDENTIFIER);
    }
    yylval.IDENTIFIER = yyalloc(sizeof(char) * (strlen(yytext) + 1));
    sscanf(yytext, "%s", yylval.IDENTIFIER);
    return IDENTIFIER;
}
<IN_OPR_ARGS>{DIGIT}+ { sscanf(yytext, "%d", &yylval.NUM); return NUM; }
<IN_OPR_ARGS>\n+ {BEGIN(INITIAL);}
[ \t]+
\n+
. { return yytext[0]; }

%%
