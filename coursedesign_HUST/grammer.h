#pragma once

#ifndef GRAMMER_H_
#define GRAMMER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "lex.h"

typedef struct SentenceNode
{

} SentenceNode;
typedef struct SentenceListNode
{
	SentenceNode* s;
	SentenceListNode* sl;
} SentenceListNode;
typedef struct LocalVarDefNode
{
	char* TypeStatement;
	VarListNode* vl;
} LocalVarDefNode;
typedef struct LocalVarListNode
{
	LocalVarDefNode* lv;
	LocalVarListNode* lvl;
} LocalVarListNode;
typedef struct ComposeNode
{
	LocalVarListNode* lv;
	SentenceListNode* sl;
} ComposeNode;
typedef struct FormFactorNode
{
	enum token_kind kind;
	char* ident;
} FormFactorNode;
typedef struct FormFactorListNode
{
	FormFactorNode* ff;
	FormFactorListNode* ffl;
} FormFactorListNode;
typedef struct FunDefNode
{
	enum token_kind ReturnType;
	char* Name;
	FormFactorListNode* ffl;
	ComposeNode* c;
} FunDefNode;
/*typedef struct VarNode
{
	char* ident;
} VarNode;*/
typedef struct VarListNode
{
	char* ident;
	VarListNode* vl;
} VarListNode;
typedef struct ExternVarDefNode
{
	enum token_kind kind;
	VarListNode* vl;
} ExternVarDefNode;
typedef struct ExternDefNode
{
	enum token_kind kind;
	ExternVarDefNode* vl;
	FunDefNode* fd;
} ExternDefNode;
typedef struct ExternDefListNode
{
	ExternDefNode* edn;
	ExternDefListNode* edln;
} ExternDefListNode;

int GraAnalyse(FILE* fp_);

#endif