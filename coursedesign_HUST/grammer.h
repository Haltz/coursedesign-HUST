#ifndef GRAMMER_H_
#define GRAMMER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "lex.h"

typedef struct CommentNode
{
	char com[100];
	struct CommentNode *c;
} CommentNode;
typedef struct Child
{
	struct Child *l;
	struct Child *r;
	char i[20];			//name
	enum token_kind op; //operator
} Child;
typedef struct VarListNode
{
	char ident[20];
	struct VarListNode *vl;
} VarListNode;
typedef struct SentenceNode
{
	struct Child *e1, *e2, *e3; //e2 e3只有for循环才有用
	struct CommentNode *ec1;
	struct CommentNode *ec2;
	struct SentenceNode *s1;
	struct SentenceNode *s2;
	struct ComposeNode *c1;
	struct ComposeNode *c2;
	int kind;
} SentenceNode;
typedef struct SentenceListNode
{
	struct SentenceNode *s;
	struct SentenceListNode *sl;
} SentenceListNode;
typedef struct LocalVarDefNode
{
	char TypeStatement[20];
	struct VarListNode *vln;
	struct LocalVarDefNode *lvd;
	struct CommentNode *comment;
} LocalVarDefNode;
typedef struct ComposeNode
{
	struct LocalVarDefNode *lv;
	struct SentenceListNode *sl;
} ComposeNode;
typedef struct FormFactorListNode
{
	char ident[20];
	enum token_kind kind;
	struct FormFactorListNode *ffl;
} FormFactorListNode;
typedef struct FunDefNode
{
	char name[20];
	struct FormFactorListNode *ffl;
	struct ComposeNode *c;
	struct CommentNode *comment;
} FunDefNode;
typedef struct ExternVarDefNode
{
	enum token_kind kind;
	struct VarListNode *vl;
} ExternVarDefNode;
typedef struct Def_includeNode
{
	int kind;
	char ident[20];
	Child *val;
} Def_includeNode;
typedef struct ExternDefNode
{
	enum token_kind kind;
	struct VarListNode *evd;
	struct FunDefNode *fd;
	struct Def_includeNode *di;
	struct CommentNode *comment;
} ExternDefNode;
typedef struct ExternDefListNode
{
	struct ExternDefNode *edn;
	struct ExternDefListNode *edln;
} ExternDefListNode;

ExternDefListNode *GraAnalyse(FILE *fp_);
int output(ExternDefListNode *root);

#endif