#include "grammer.h"

node* root = NULL;
FILE* fp;

char type[][] =
{
	"�ⲿ��������",
	"��������",
	""
};

int IsVarDeclare(keyword t) //�Ƿ������������Ĺؼ���
{
	if (t.kind <= VOID && t.kind >= AUTO)
		return 1;
	else return 0;
}

int IsIdent(keyword t)
{
	if (t.kind == IDENT) return 1;
	else return 0;
}

node* ExternDef(node* root)
{
	keyword w = gettoken(fp);
	//�������� ��ʱ��֧�ֳ�ʼ�� ��֧��extern������
	if (IsVarDeclare(w))
	{
		w = gettoken(fp);
		if(IsIdent(w))

	}
	else if (IsFunDeclare(w))
	{
	}
}
node* ExternDefList(node* root)//�ⲿ��������
{
	node* cur = ExternDef(root);
	while(cur != NULL) 
		ExternDefList(root);
	return root;
}
node* GraAnalyse(FILE* fp_)
{
	fp = fp_;
	keyword w = gettoken(fp);
	if (ExternDefList(root)) return root;
}