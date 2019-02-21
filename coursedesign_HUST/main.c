#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
char tokenname[100][100] =
{
	//错误
	"ERROR_TOKEN",
	//标识符
	"IDENT",
	//声明
	"INT",
	"FLOAT",
	"CHAR",
	//关键字
	"IF",
	"ELSE",
	"RETURN",
	//常量
	"INT_CONST",
	"FLOAT_CONST",
	"CHAR_CONST",
	//定界符
	"LCURLY", "RCURLY", "LSQUARE", "RSQUARE", "SINGLE", "SINGGLESINGLE", "COMMA", "SEMMI", "QUESTION",
	//{       }       [        ]      '       "              ,      ;      ?         
	//标点符号
	"PLUS", "MINUS", "MUL", "DIV", "LP", "RP", "ASSIGN", "LESS", "MORE", "EQUAL", "UNEQUAL", "EXCLA",
	//+     -     *     /    (   )   =       <     >     ==     !=       #      
	"NEGATE",  "HASHTAG", "PERCENT", "XOR", "AND", "ANDAND", "ANDEQ", "OR", "OROR", "OREQ",   "MINUSMINUS",  "PLUSPLUS",  "COMMENT",
	//!        %        ^     &    &&      &=     |          ||    |=      --           ++         //
	"PLUSEQ", "MINUSEQ", "MULEQ", "DIVEQ", "NEGADIV", "DOT",
	//+=      -=       *=     /=     \        .    
	"XOREQ", "LESSEQ", "MOREEQ", "LMOVE", "RMOVE", "LMOVEEQ", "RMOVEEQ",
	//^=     <=      >=    <<     >>     <<=      >>=   
	//语句类型
	"Expres"
};
FILE *fp;
int main()
{
	int err = fopen_s(&fp, "test.txt", "r");
	if (err != 0)
	{
		printf("The file can't be opened.");
		system("pause");
		return 0;
	}
	int line = 0;
	keyword temp;
	temp = gettoken(fp);
	/*
	while (temp.kind != ERROR_TOKEN && temp.kind != EOF_)
	{
		printf("%s %s %d\n", tokenname[temp.kind], temp.tokentext, line);
		temp = gettoken(fp);
	}
	*/
	system("pause");
	return 0;
}