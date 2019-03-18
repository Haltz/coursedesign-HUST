#ifndef GRAMMER_H_
#define GRAMMER_H_

#include "lex.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//ע�ͽڵ�
//comΪע�����ݣ�cΪ��һ��ע��
typedef struct CommentNode
{
	char com[100];
	struct CommentNode *c;
} CommentNode;
//���ʽ�ڵ�
//��������ʽ�������������l == r == NULL����ʶ��Ϊi
//�����������opΪ��������ͣ�i == �գ�l��rΪ��������߱��ʽ
typedef struct Child
{
	struct Child *l;
	struct Child *r;
	char i[20];			//name
	enum token_kind op; //operator
} Child;
//�����б�ڵ�
//identΪ������ʶ����initialΪ��ʼ���ұ߱��ʽ
typedef struct VarListNode
{
	char ident[20];
	Child* initial;
	struct VarListNode *vl;
} VarListNode;
//���ڵ�
//if�������ʽe1 ����ʱ���s1��c1 ������ʱ���s2 c2
//while�������ʽe1 ���s1����c1
typedef struct SentenceNode
{
	struct Child *e1, *e2, *e3; //e2 e3ֻ��forѭ��������
	struct CommentNode *ec1;
	struct CommentNode *ec2;
	struct SentenceNode *s1;
	struct SentenceNode *s2;
	struct ComposeNode *c1;
	struct ComposeNode *c2;
	int kind;
} SentenceNode;
//����б�ڵ�
typedef struct SentenceListNode
{
	struct SentenceNode *s;
	struct SentenceListNode *sl;
} SentenceListNode;
//�ֲ���������ڵ�
//��������Typestatement
typedef struct LocalVarDefNode
{
	char TypeStatement[20];
	struct VarListNode *vln;
	struct LocalVarDefNode *lvd;
	struct CommentNode *comment;
} LocalVarDefNode;
//�������ڵ�
typedef struct ComposeNode
{
	struct LocalVarDefNode *lv;
	struct SentenceListNode *sl;
} ComposeNode;
//�β��б�ڵ�
//�β��������� ident ����kind
typedef struct FormFactorListNode
{
	char ident[20];
	enum token_kind kind;
	struct FormFactorListNode *ffl;
} FormFactorListNode;
//��������ڵ�
//��������name �β��б�ffl �������c
typedef struct FunDefNode
{
	char name[20];
	struct FormFactorListNode *ffl;
	struct ComposeNode *c;
	struct CommentNode *comment;
} FunDefNode;
//�ⲿ��������ڵ�
//����kind �����б�VarlistNode
typedef struct ExternVarDefNode
{
	enum token_kind kind;
	struct VarListNode *vl;
} ExternVarDefNode;
//Ԥ����ڵ�
//define��ֵΪval
//define ��ʶ�� ���� include �ļ���Ϊident
typedef struct Def_includeNode
{
	int kind;
	char ident[20];
	Child *val;
} Def_includeNode;
//�ⲿ��������ڵ�
typedef struct ExternDefNode
{
	enum token_kind kind;
	struct VarListNode *evd;
	struct FunDefNode *fd;
	struct Def_includeNode *di;
	struct CommentNode *comment;
} ExternDefNode;
//�ⲿ���������б�ڵ�
typedef struct ExternDefListNode
{
	struct ExternDefNode *edn;
	struct ExternDefListNode *edln;
} ExternDefListNode;

//�﷨��������
ExternDefListNode *GraAnalyse(FILE *fp_);
//�������
int output(ExternDefListNode *root);

#endif