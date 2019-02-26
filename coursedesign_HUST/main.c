#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "grammer.h"
char token_name[100][20] =
{
	"ERROR_TOKEN",
	"IDENT",
	"INT",
	"FLOAT",
	"CHAR",
	"VOID",
	"LONG",
	"SHORT",
	"BOOL",
	"DOUBLE",
	"IF",
	"ELSE",
	"RETURN",
	"WHILE",
	"FOR",
	"CONTINUE",
	"BREAK",
	"DEFINE",
	"INCLUDE",
	"INT_CONST",
	"FLOAT_CONST",
	"CHAR_CONST",
	"LONG_CONST",
	"SHORT_CONST",
	"BOOL_CONST",
	"DOUBLE_CONST",
	"LCURLY",
	"RCURLY",
	"LSQUARE",
	"RSQUARE",
	"SINGLE",
	"SINGGLESINGLE",
	"COMMA",
	"SEMMI",
	"QUESTION",
	"PLUS",
	"MINUS",
	"MUL",
	"DIV",
	"MOD",
	"ANDAND",
	"OROR",
	"LP",
	"RP",
	"ASSIGN",
	"MORE",
	"LESS",
	"LESSEQ",
	"MOREEQ",
	"EQUAL",
	"UNEQUAL",
	"EXCLA",
	"NEGATE",
	"HASHTAG",
	"PERCENT",
	"XOR",
	"AND",
	"ANDEQ",
	"OR",
	"OREQ",
	"MINUSMINUS",
	"PLUSPLUS",
	"COMMENT",
	"PLUSEQ",
	"MINUSEQ",
	"MULEQ",
	"DIVEQ",
	"NEGADIV",
	"DOT",
	"XOREQ",
	"LMOVE",
	"RMOVE",
	"LMOVEEQ",
	"RMOVEEQ",
	"Expres"
};
FILE *fp;
int main()
{
	int err = fopen_s(&fp, "test.c", "r");
	if (err != 0)
	{
		printf("The file can't be opened.");
		system("pause");
		return 0;
	}
	/*
	int line = 0;
	keyword temp;
	temp = gettoken(fp);
	while (temp.kind != ERROR_TOKEN && temp.kind != EOF_)
	{
		printf("%s %s %d\n", token_name[temp.kind], temp.tokentext, temp.line);
		temp = gettoken(fp);
	}*/
	output(GraAnalyse(fp));
	system("pause");
	return 0;
}