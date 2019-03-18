#ifndef GRAMMER_H_
#define GRAMMER_H_

#include "lex.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//注释节点
//com为注释内容，c为下一条注释
typedef struct CommentNode
{
	char com[100];
	struct CommentNode *c;
} CommentNode;
//表达式节点
//二叉树形式，若无运算符，l == r == NULL，标识符为i
//若有运算符，op为运算符类型，i == 空，l，r为运算符两边表达式
typedef struct Child
{
	struct Child *l;
	struct Child *r;
	char i[20];			//name
	enum token_kind op; //operator
} Child;
//变量列表节点
//ident为变量标识符，initial为初始化右边表达式
typedef struct VarListNode
{
	char ident[20];
	Child* initial;
	struct VarListNode *vl;
} VarListNode;
//语句节点
//if条件表达式e1 满足时语句s1或c1 不满足时语句s2 c2
//while条件表达式e1 语句s1或者c1
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
//语句列表节点
typedef struct SentenceListNode
{
	struct SentenceNode *s;
	struct SentenceListNode *sl;
} SentenceListNode;
//局部变量定义节点
//类型声明Typestatement
typedef struct LocalVarDefNode
{
	char TypeStatement[20];
	struct VarListNode *vln;
	struct LocalVarDefNode *lvd;
	struct CommentNode *comment;
} LocalVarDefNode;
//复合语句节点
typedef struct ComposeNode
{
	struct LocalVarDefNode *lv;
	struct SentenceListNode *sl;
} ComposeNode;
//形参列表节点
//形参类型名称 ident 类型kind
typedef struct FormFactorListNode
{
	char ident[20];
	enum token_kind kind;
	struct FormFactorListNode *ffl;
} FormFactorListNode;
//函数定义节点
//函数名称name 形参列表ffl 复合语句c
typedef struct FunDefNode
{
	char name[20];
	struct FormFactorListNode *ffl;
	struct ComposeNode *c;
	struct CommentNode *comment;
} FunDefNode;
//外部变量定义节点
//类型kind 参数列表VarlistNode
typedef struct ExternVarDefNode
{
	enum token_kind kind;
	struct VarListNode *vl;
} ExternVarDefNode;
//预编译节点
//define赋值为val
//define 标识符 或者 include 文件名为ident
typedef struct Def_includeNode
{
	int kind;
	char ident[20];
	Child *val;
} Def_includeNode;
//外部变量定义节点
typedef struct ExternDefNode
{
	enum token_kind kind;
	struct VarListNode *evd;
	struct FunDefNode *fd;
	struct Def_includeNode *di;
	struct CommentNode *comment;
} ExternDefNode;
//外部变量定义列表节点
typedef struct ExternDefListNode
{
	struct ExternDefNode *edn;
	struct ExternDefListNode *edln;
} ExternDefListNode;

//语法分析函数
ExternDefListNode *GraAnalyse(FILE *fp_);
//输出函数
int output(ExternDefListNode *root);

#endif