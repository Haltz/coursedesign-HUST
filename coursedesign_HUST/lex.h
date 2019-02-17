#pragma once

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
} keyword;

enum token_kind
{
	//�ļ�����
	EOF_ = -1,
	//����
	ERROR_TOKEN,
	//��ʶ��
	IDENT,
	//����
	AUTO,
	BOOL,
	CHAR,
	DOUBLE,
	ENUM,
	EXTERN,
	FLOAT,
	INT,
	LONG,
	REGISTRE,
	SHORT,
	SIGNED,
	STATIC,
	STRUCT,
	SWITCH,
	TYPEDEF,
	UNION,
	UNSIGNED,
	VOID,
	//�ؼ��� 
	BREAK,
	CASE,
	CONTINUE,
	DEFAULT,
	DO,
	ELSE,
	FOR,
	GOTO,
	IF,
	RETURN,
	SIZEOF,
	SWITCH,
	WHILE,
	//����
	INT_CONST, FLOAT_CONST, CHAR_CONST,
	//������
	NEGATE, EXCLA, HASHTAG, PERCENT, XOR, AND, ANDAND, ANDEQ, OR, OROR, OREQ, MUL, LP, RP, MINUS, MINUSMINUS, PLUS, PLUSPLUS, ASSIGN, EQUAL, DIV, COMMENT,
	//~       !      #        %       ^     &    &&      &=     |   ||    |=    *    (   )   -      --          +     ++        =       ==     /    //
	PLUSEQ, MINUSEQ, MULEQ, DIVEQ, LCURLY, RCURLY, LSQUARE, RSQUARE, NEGADIV, SINGLE, SINGGLESINGLE, COMMA, SEMMI, QUESTION, DOT, LESS, MORE,
	//+=      -=       *=     /=     {       }       [        ]        \        '       "              ,      ;      ?         .    <     >
	UNEQUAL, XOREQ, LESSEQ, MOREEQ, LMOVE, RMOVE, LMOVEEQ, RMOVEEQ
	//!=       ^=     <=      >=    <<     >>     <<=      >>=   
};

keyword gettoken(FILE* fp);//��ȡ���ʲ������������ͣ�ֵ


#endif // DEBUG

