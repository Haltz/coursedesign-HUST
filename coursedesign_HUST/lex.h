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
	//定界符
	LCURLY, RCURLY, LSQUARE, RSQUARE, SINGLE, SINGGLESINGLE, COMMA, SEMMI, QUESTION,
	//{       }       [        ]      '       "              ,      ;      ?         
	//标点符号
	  PLUS, MINUS, MUL, DIV, LP, RP, ASSIGN, LESS, MORE, EQUAL, UNEQUAL, EXCLA, 
	//+     -     *     /    (   )   =       <     >     ==     !=       #      
	  NEGATE,  HASHTAG, PERCENT, XOR, AND, ANDAND, ANDEQ, OR, OROR, OREQ,   MINUSMINUS,  PLUSPLUS,  COMMENT,
	//!        %        ^     &    &&      &=     |          ||    |=      --           ++         //
	  PLUSEQ, MINUSEQ, MULEQ, DIVEQ, NEGADIV, DOT, 
	//+=      -=       *=     /=     \        .    
	  XOREQ, LESSEQ, MOREEQ, LMOVE, RMOVE, LMOVEEQ, RMOVEEQ,
	//^=     <=      >=    <<     >>     <<=      >>=   
	//语句类型
	Expres, FunUse
};

keyword gettoken(FILE* fp);//获取单词并返回它的类型，值


#endif // DEBUG

