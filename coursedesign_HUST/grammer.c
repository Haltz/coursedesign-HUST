#include "grammer.h"

node* root = NULL;
keyword w;
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

node* ExternDef()
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
node* ExternDefList()//�ⲿ��������
{
	if (w.kind == EOF_) return NULL;
	node* root; //����һ���ⲿ�������н��root
	root->head = (list*)malloc(sizeof(list));
	root->head->cur = ExternDef(); //����һ���ⲿ���壬�õ�һ����������Ϊroot�ĵ�һ������
	root->head->next->cur = ExternDefList(); //�õ�����������Ϊroot�ĵڶ�������
	return root;
}
node* GraAnalyse(FILE* fp_)
{
	fp = fp_;
	w = gettoken(fp);
	if (ExternDefList()) return root;
}