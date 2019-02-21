#pragma once

#ifndef GRAMMER_H_
#define GRAMMER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "lex.h"

typedef struct Child
{
	struct Child *l;
	struct Child *r;
	char i[20];//name
	enum token_kind op;//operator
} Child;
typedef struct VarListNode
{
	char* ident;
	struct VarListNode* vl;
} VarListNode;
typedef struct SentenceNode
{
	struct Child* e1;
	struct SentenceNode* s1;
	struct SentenceNode* s2;
	int kind;
} SentenceNode;
typedef struct SentenceListNode
{
	struct SentenceNode* s;
	struct SentenceListNode* sl;
} SentenceListNode;
typedef struct LocalVarDefNode
{
	char TypeStatement[20];
	char name[20];
	struct LocalVarListNode* vl;
} LocalVarDefNode;
typedef struct LocalVarListNode
{
	struct LocalVarDefNode* lv;
	struct LocalVarListNode* lvl;
} LocalVarListNode;
typedef struct ComposeNode
{
	struct LocalVarListNode* lv;
	struct SentenceListNode* sl;
} ComposeNode;
typedef struct FormFactorNode
{
	enum token_kind kind;
	char ident[20];
} FormFactorNode;
typedef struct FormFactorListNode
{
	struct FormFactorNode* ff;
	struct FormFactorListNode* ffl;
} FormFactorListNode;
typedef struct FunDefNode
{
	char name[20];
	struct FormFactorListNode* ffl;
	struct ComposeNode* c;
} FunDefNode;
typedef struct ExternVarDefNode
{
	enum token_kind kind;
	struct VarListNode* vl;
} ExternVarDefNode;
typedef struct ExternDefNode
{
	enum token_kind kind;
	struct VarListNode* evd;
	struct FunDefNode* fd;
} ExternDefNode;
typedef struct ExternDefListNode
{
	struct ExternDefNode* edn;
	struct ExternDefListNode* edln;
} ExternDefListNode;

int GraAnalyse(FILE* fp_);
int output(ExternDefListNode* root);

#endif