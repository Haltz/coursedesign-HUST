#ifndef LEX_H_

#define LEX_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

//�ʷ���Ԫ����
enum token_kind
{
	//�ļ�����
	EOF_ = -1,
	//����
	ERROR_TOKEN,
	//��ʶ��
	IDENT,
	//����
	INT,
	FLOAT,
	CHAR,
	VOID,
	LONG,
	SHORT,
	BOOL,
	DOUBLE,
	//�ؼ���
	IF,
	ELSE,
	RETURN,
	WHILE,
	FOR,
	CONTINUE,
	BREAK,
	DEFINE,
	INCLUDE,
	//����
	INT_CONST,
	FLOAT_CONST,
	CHAR_CONST,
	LONG_CONST,
	SHORT_CONST,
	BOOL_CONST,
	DOUBLE_CONST,
	//�����
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
	//������
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
	//�������
	Expres,
	FunUse,
	ARRAY,
	FORMARRAY
};

//�ʷ���Ԫ�ṹ
typedef struct Keyword
{
	char tokentext[20];
	enum token_kind kind;
	int line;
	int change;
} keyword;

//��ȡ�ʷ���Ԫ����
keyword gettoken(FILE *fp); //��ȡ���ʲ������������ͣ�ֵ
//��ȡ�ַ���ͬʱ�������
char readchar(FILE *fp);

#endif // DEBUG
