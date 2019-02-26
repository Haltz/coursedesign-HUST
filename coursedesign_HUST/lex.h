#ifndef LEX_H_

#define LEX_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct Keyword
{
	char tokentext[20];
	enum token_kind kind;
	int line;
} keyword;

enum token_kind
{
	//文件结束
	EOF_ = -1,
	//错误
	ERROR_TOKEN,
	//标识符
	IDENT,
	//声明
	INT,
	FLOAT,
	CHAR,
	VOID,
	LONG,
	SHORT,
	BOOL,
	DOUBLE,
	//关键字
	IF,
	ELSE,
	RETURN,
	WHILE,
	FOR,
	CONTINUE,
	BREAK,
	DEFINE,
	INCLUDE,
	//常量
	INT_CONST,
	FLOAT_CONST,
	CHAR_CONST,
	LONG_CONST,
	SHORT_CONST,
	BOOL_CONST,
	DOUBLE_CONST,
	//定界符
	LCURLY,
	RCURLY,
	LSQUARE,
	RSQUARE,
	SINGLE,
	SINGGLESINGLE,
	COMMA,
	SEMMI,
	QUESTION,
	//{       }       [        ]      '       "              ,      ;      ?
	//标点符号
	PLUS,
	MINUS,
	MUL,
	DIV,
	MOD,
	ANDAND,
	OROR,
	LP,
	RP,
	ASSIGN,
	MORE,
	LESS,
	LESSEQ,
	MOREEQ,
	EQUAL,
	UNEQUAL,
	EXCLA,
	//+     -     *     /    (   )   =       <     >     ==     !=       #
	NEGATE,
	HASHTAG,
	PERCENT,
	XOR,
	AND,
	ANDEQ,
	OR,
	OREQ,
	MINUSMINUS,
	PLUSPLUS,
	COMMENT,
	//!        %        ^     &    &&      &=     |          ||    |=      --           ++         //
	PLUSEQ,
	MINUSEQ,
	MULEQ,
	DIVEQ,
	NEGADIV,
	DOT,
	//+=      -=       *=     /=     \        .
	XOREQ,
	LMOVE,
	RMOVE,
	LMOVEEQ,
	RMOVEEQ,
	//^=     <=      >=    <<     >>     <<=      >>=
	LCOMMENT,
	//语句类型
	Expres,
	FunUse
};

keyword gettoken(FILE *fp); //获取单词并返回它的类型，值
//char* getcomment(FILE* fp);

#endif // DEBUG
