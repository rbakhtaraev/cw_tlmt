%option noyywrap yylineno

%{
#include "parser.h"
int ch;
extern void yyerror (char *);
%}

delim[(){};,]
letter[a-zA-Z]
digit[0-9]
ws[ \t\n]

%x COMM

%%

"/*" { ch += yyleng; BEGIN COMM; }
<COMM>. { ch += yyleng; continue; }
<COMM>\n { ch += yyleng; continue; }
<COMM>"*/" { ch += yyleng; BEGIN INITIAL; }
<COMM><<EOF>> { ch += yyleng; yyerror("Unterminated comment"); BEGIN INITIAL; }
\/\/(.)*(\n|$) { ch += yyleng; }

int|long {
//printf("TYPE (%d, %d): %s\n", yylineno, ch, yytext);
ch += yyleng;
yylval.str = strdup(yytext);
return TYPE;
}

if {
//printf("IFELSE (%d, %d): %s\n", yylineno, ch, yytext);
ch += yyleng;
yylval.str = strdup(yytext);
return IF;
}

print {
//printf("PRINT (%d, %d): %s\n", yylineno, ch, yytext);
ch += yyleng;
yylval.str = strdup(yytext);
return PRINT;
}

scan {
//printf("SCAN (%d, %d): %s\n", yylineno, ch, yytext);
ch += yyleng;
yylval.str = strdup(yytext);
return SCAN;
}

else {
//printf("IFELSE (%d, %d): %s\n", yylineno, ch, yytext);
ch += yyleng;
yylval.str = strdup(yytext);
return ELSE;
}

for {
//printf("KEYWORD (%d, %d): %s\n", yylineno, ch, yytext);
ch += yyleng;
yylval.str = strdup(yytext);
return FOR;
}

while {
//printf("KEYWORD (%d, %d): %s\n", yylineno, ch, yytext);
ch += yyleng;
yylval.str = strdup(yytext);
return WHILE;
}

("_"|{letter})("_"|{letter}|{digit})* {
//printf("IDENTIFIER (%d, %d): %s\n", yylineno, ch, yytext);
ch += yyleng;
yylval.str = strdup(yytext);
return ID;
}

"+"|"-" {
//printf("LOUOPER (%d, %d): %s\n", yylineno, ch, yytext);
ch += yyleng;
yylval.str = strdup(yytext);
return LOUOPER;
}

"*"|"/" {
//printf("HIGHOPER (%d, %d): %s\n", yylineno, ch, yytext);
ch += yyleng;
yylval.str = strdup(yytext);
return HIGHOPER;
}

">"|">="|"=="|"<="|"<"|"!=" {
//printf("LOG (%d, %d): %s\n", yylineno, ch, yytext);
ch += yyleng;
yylval.str = strdup(yytext);
return LOG;
}

[+-]?({digit}*\.{digit}+|{digit}+\.|{digit}+)([eE][+-]?{digit}+)?[flFL]? {
//printf("NUMBER (%d, %d): %s\n", yylineno, ch, yytext);
ch += yyleng;
yylval.str = strdup(yytext);
return NUMBER;

}

"=" {
//printf("ASSIGNATION (%d, %d): %s\n", yylineno, ch, yytext);
ch += yyleng;
yylval.str = strdup(yytext);
return ASSIGN;
}

{delim} {
//printf("DELIMITER (%d, %d): %s\n", yylineno, ch, yytext);
ch += yyleng;
return *yytext;
}

{ws}+ { ch += yyleng; }

. {
yyerror("Unknown character");
ch += yyleng;
}

%%
/*
int main (int argc, char **argv) {
    if (argc < 2) {
        printf("\nNot arguments.\n");
        return -1;
    }
    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        printf("\nCannot open file\n");
        fclose (yyin);
        return 0;
    }
    ch = 1;
    yylineno = 1;
    hashtab_init(hashtab);
    yylex();
    print_hash(hashtab);
    fclose(yyin);
    return 0;
}
*/