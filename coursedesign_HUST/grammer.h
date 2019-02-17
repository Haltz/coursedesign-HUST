#pragma once

#ifndef GRAMMER_H_
#define GRAMMER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "lex.h"

typedef struct Sentence
{

} Sentence;
typedef struct SentenceList
{
	Sentence* s;
	SentenceList* sl;
} SentenceList;
typedef struct LocalVarDef
{
	char* TypeStatement;
	VarList* vl;
} LocalVarDef;
typedef struct LocalVarList
{
	LocalVarDef* lv;
	LocalVarList* lvl;
} LocalVarList;
typedef struct Compose
{
	LocalVarList* lv;
	SentenceList* sl;
} Compose;
typedef struct FormFactor
{
	char* TypeStatement;
	char* ident;
} FormFactor;
typedef struct FormFactorList
{
	FormFactor* ff;
	FormFactorList* ffl;
} FormFactorList;
typedef struct FunDef
{
	char* TypeStatement;
	char* Name;
	FormFactorList* ffl;
	Compose* c;
} FunDef;
typedef struct Var
{
	char* ident;
} Var;
typedef struct VarList
{
	Var* v;
	VarList* vl;
} VarList;
typedef struct ExternVarDef
{
	char* TypeStatement;
	VarList* vl;
} ExternVarDef;
typedef struct ExternDefNode
{
	ExternVarDef* vl;
	FunDef* fd;
} ExternDefNode;
typedef struct ExternDefListNode
{
	ExternDefNode* edn;
	ExternDefListNode* edln;
} ExternDefListNode;

int GraAnalyse(FILE* fp_);

#endif