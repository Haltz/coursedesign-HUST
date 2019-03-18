#include "format.h"
#include "lex.h"
extern keyword gettoken(FILE *fp);
extern ExternDefListNode *GraAnalyse(FILE *fp);
extern int output(ExternDefListNode *root);
extern int format(FILE *fp);
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
		"LCOMMENT",
		"Expres"
	};

FILE *fp;

int main()
{
	printf("请输入文件名:");
	char s[100];
	scanf("%s", s);
	int err = fopen_s(&fp, s, "r");
	if (err != 0)
	{
		printf("The file can't be opened.");
		system("pause");
		return 0;
	}
	printf("Please choose function:\n1:Lex Analyse     2:Grammer Analyse     3:Format\n");
	int choice = 0;
	scanf("%d", &choice);
	if (choice == 1)
	{
		keyword temp;
		temp = gettoken(fp);
		while (temp.kind != ERROR_TOKEN && temp.kind != EOF_)
		{
			printf("%s %s %d\n", token_name[temp.kind], temp.tokentext, temp.line);
			temp = gettoken(fp);
		}
	}
	else if (choice == 2)
		output(GraAnalyse(fp));
	else if (choice == 3)
		format(fp);
	else
	{
		printf("No such choice\n");
		return 0;
	}
	printf("Done\n");
	system("pause");
	return 0;
}