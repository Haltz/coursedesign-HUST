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
	int line;
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
	INT,
	FLOAT,
	CHAR,
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
	//�����
	LCURLY, RCURLY, LSQUARE, RSQUARE, SINGLE, SINGGLESINGLE, COMMA, SEMMI, QUESTION,
	//{       }       [        ]      '       "              ,      ;      ?         
	//������
	  PLUS, MINUS, MUL, DIV, LP, RP, ASSIGN, LESS, MORE, EQUAL, UNEQUAL, EXCLA, 
	//+     -     *     /    (   )   =       <     >     ==     !=       #      
	  NEGATE,  HASHTAG, PERCENT, XOR, AND, ANDAND, ANDEQ, OR, OROR, OREQ,   MINUSMINUS,  PLUSPLUS,  COMMENT,
	//!        %        ^     &    &&      &=     |          ||    |=      --           ++         //
	  PLUSEQ, MINUSEQ, MULEQ, DIVEQ, NEGADIV, DOT, 
	//+=      -=       *=     /=     \        .    
	  XOREQ, LESSEQ, MOREEQ, LMOVE, RMOVE, LMOVEEQ, RMOVEEQ,
	//^=     <=      >=    <<     >>     <<=      >>=   
	//�������
	Expres, FunUse
};

keyword gettoken(FILE* fp);//��ȡ���ʲ������������ͣ�ֵ


#endif // DEBUG

