#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
char tokenname[100][100] =
{
	//����
	"ERROR_TOKEN",
	//��ʶ��
	"IDENT",
	//����
	"INT",
	"FLOAT",
	"CHAR",
	//�ؼ���
	"IF",
	"ELSE",
	"RETURN",
	//����
	"INT_CONST",
	"FLOAT_CONST",
	"CHAR_CONST",
	//�����
	"LCURLY", "RCURLY", "LSQUARE", "RSQUARE", "SINGLE", "SINGGLESINGLE", "COMMA", "SEMMI", "QUESTION",
	//{       }       [        ]      '       "              ,      ;      ?         
	//������
	"PLUS", "MINUS", "MUL", "DIV", "LP", "RP", "ASSIGN", "LESS", "MORE", "EQUAL", "UNEQUAL", "EXCLA",
	//+     -     *     /    (   )   =       <     >     ==     !=       #      
	"NEGATE",  "HASHTAG", "PERCENT", "XOR", "AND", "ANDAND", "ANDEQ", "OR", "OROR", "OREQ",   "MINUSMINUS",  "PLUSPLUS",  "COMMENT",
	//!        %        ^     &    &&      &=     |          ||    |=      --           ++         //
	"PLUSEQ", "MINUSEQ", "MULEQ", "DIVEQ", "NEGADIV", "DOT",
	//+=      -=       *=     /=     \        .    
	"XOREQ", "LESSEQ", "MOREEQ", "LMOVE", "RMOVE", "LMOVEEQ", "RMOVEEQ",
	//^=     <=      >=    <<     >>     <<=      >>=   
	//�������
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