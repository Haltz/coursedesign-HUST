#include "grammer.h"

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

ExternDefListNode* ExternDefList()
{
}
ExternDefNode* ExternDef()
{
	if (!IsVarDeclare(w)) return NULL;
	ExternDefNode* edn = (ExternDefNode*)malloc(sizeof(ExternDefNode));
	edn->fd = FunDefFun();
	edn->vl = VarListFun();
	return edn;
}
ExternDefListNode* ExternDefList()// �ⲿ��������
{
	if (w.kind == EOF_) return NULL;
	ExternDefListNode* root = (ExternDefListNode*)malloc(sizeof(ExternDefListNode)); //����һ���ⲿ�������н��root
	root->edn = ExternDef(); //����һ���ⲿ���壬�õ�һ����������Ϊroot�ĵ�һ������
	root->edln = ExternDefList(); //�õ�����������Ϊroot�ĵڶ�������
	return root;
}
int GraAnalyse(FILE* fp_)
{
	fp = fp_;
	w = gettoken(fp);
}