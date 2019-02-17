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
	//文件结束
	EOF_ = -1,
	//错误
	ERROR_TOKEN,
	//标识符
	IDENT,
	//声明
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
	//关键字 
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
	//常量
	INT_CONST, FLOAT_CONST, CHAR_CONST,
	//标点符号
	NEGATE, EXCLA, HASHTAG, PERCENT, XOR, AND, ANDAND, ANDEQ, OR, OROR, OREQ, MUL, LP, RP, MINUS, MINUSMINUS, PLUS, PLUSPLUS, ASSIGN, EQUAL, DIV, COMMENT,
	//~       !      #        %       ^     &    &&      &=     |   ||    |=    *    (   )   -      --          +     ++        =       ==     /    //
	PLUSEQ, MINUSEQ, MULEQ, DIVEQ, LCURLY, RCURLY, LSQUARE, RSQUARE, NEGADIV, SINGLE, SINGGLESINGLE, COMMA, SEMMI, QUESTION, DOT, LESS, MORE,
	//+=      -=       *=     /=     {       }       [        ]        \        '       "              ,      ;      ?         .    <     >
	UNEQUAL, XOREQ, LESSEQ, MOREEQ, LMOVE, RMOVE, LMOVEEQ, RMOVEEQ
	//!=       ^=     <=      >=    <<     >>     <<=      >>=   
};

keyword gettoken(FILE* fp);//获取单词并返回它的类型，值


#endif // DEBUG

