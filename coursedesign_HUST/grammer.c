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

int FreeNode(VarListNode* root)
{
}

VarListNode* VarList()
{
	VarListNode* vl = (VarListNode*)malloc(sizeof(VarListNode));
	vl->ident = w.tokentext;
	w = gettoken(fp);
		if (w.kind != COMMA && w.kind != SEMMI)
			FreeNode(vl);
		if (w.kind == SEMMI)
		{
			w = gettoken(fp);
			return vl;
		}
		w = gettoken(fp);
		if (w.kind != IDENT) FreeNode(vl);
		vl->vl = VarList();
		return vl;
}
//���ô��ӳ���ʱ���ⲿ�������ͺ͵�һ���������ĵ����Ѿ����룬
//������������tokenText0�У���ʱ�ⲿ��������Ĵ������̿ɲο����¡�
ExternVarDefNode* ExternVarDef()
{
	ExternVarDefNode* evd = (ExternVarDefNode*)malloc(sizeof(ExternVarDefNode));
	evd->kind = w.kind;
	evd->vl = VarList();
}

FormFactorNode* FormFactor() //�Ѿ������˵�һ���βε��������� ����ȡ������
{
	FormFactorNode* ff = (FormFactorNode*)malloc(sizeof(FormFactorNode));
	ff->kind = w.kind;
	w = gettoken(fp);
	if (ff->kind != IDENT) return NULL;
	ff->ident = w.tokentext;
	return ff;
}
FormFactorListNode* FormFactorList()
{
	w = gettoken(fp);
	if (w.kind != RP && w.kind != COMMA && !IsVarDeclare(w)) return NULL;
	FormFactorListNode* ffl = (FormFactorListNode*)malloc(sizeof(FormFactorListNode));
	if (w.kind == RP)
	{
		ffl->ff = NULL;
		ffl->ffl = NULL;
		return ffl;
	}
	if (w.kind == COMMA) w = gettoken(fp);
	if (!IsVarDeclare(w)) return NULL;
	ffl->ff = FormFactor();
	ffl->ffl = FormFactorList();
	return ffl;
}

//���ô��ӳ���ʱ�����ĵ�һ�������Ѿ����룬����һ�����ʱ��
//�����������ĵ�һ�����ʣ�ȷ������ʲô���͵���䡣
SentenceNode* Sentence()
{
	SentenceNode* s = (SentenceNode*)malloc(sizeof(SentenceNode));
	switch (w.kind)
	{
	case IF://�����������
		w = gettoken(fp);
		if (w.kind != LP) 
			return NULL;
		expression
	}
}

SentenceListNode* SentenceList()
{
	SentenceListNode* sl = NULL;
	SentenceNode* s = Sentence();
	if (s == NULL)
		return NULL;
	else
	{
		sl->s = s;
		sl->sl = SentenceList();
		return sl;
	}
}

//���ô��ӳ���ʱ���Ѿ������˵���{����������ʱ����������}�������������
ComposeNode* Compose()
{
	ComposeNode* c = (ComposeNode*)malloc(sizeof(ComposeNode));
	w = gettoken(fp);
	if (IsVarDeclare(w))
	{
		c->lv = LocalVarDef();
	}
	else c->lv = NULL;
	c->sl = SentenceList();
	if (w.kind != RCURLY)
		return NULL;
	w = gettoken(fp);
	return c;
}

//���ô��ӳ���ʱ����������ֵ���ͺͺ���������С���ŵĵ����Ѿ����룬������������tokenText0��
FunDefNode* FunDef(keyword copy)
{
	FunDefNode* fd = (FunDefNode*)malloc(sizeof(FunDefNode));
	fd->ReturnType = copy.kind;
	fd->Name = copy.tokentext;
	fd->ffl = FormFactorList();
	w = gettoken(fp);
	if (w.kind == SEMMI) fd->c = NULL;
	else if(w.kind == RCURLY) fd->c = Compose();
}

//���ӳ������һ���ⲿ����Ĵ������ô��ӳ���ʱ���Ѿ�������һ���ⲿ����ĵ�һ�����ʵ�w�С�
//���ӳ�������󣬸պô����ⲿ��������һ�����ţ��������ʻ�û���롣
ExternDefNode* ExternDef() //�����ⲿ�������У���ȷʱ���������������ָ�룬���򷵻�NULL
//
{
	if (!IsVarDeclare(w)) return NULL;
	ExternDefNode* edn = (ExternDefNode*)malloc(sizeof(ExternDefNode));
	edn->kind = w.kind;
	keyword wcopy = w;
	w = gettoken(fp);
	if(w.kind == LP) edn->fd = FunDef(wcopy);
	else edn->vl = VarList();
	return edn;
}

//�﷨��λ<�ⲿ��������>�Ķ��壺
//<�ⲿ��������>���� = <�ⲿ����> <�ⲿ��������> | <�ⲿ����>
//����һ���ݹ鶨�壬���ӳ�����һϵ�е��ⲿ���壬ÿ���ⲿ�������еĽ�㣬���һ��������Ӧһ���ⲿ���壬�ڶ���������Ӧ�������ⲿ���塣
//��һ��Դ�����У�ÿ�γɹ�������һ���ⲿ�������������ļ�������ǣ����﷨�������������ô��ӳ����Ѿ�������һ���ⲿ����ĵ�һ�����ʵ�w�С�
ExternDefListNode* ExternDefList()// �ⲿ��������
{
	if (w.kind = EOF_) return NULL;
	ExternDefListNode* root = (ExternDefListNode*)malloc(sizeof(ExternDefListNode)); //����һ���ⲿ�������н��root
	root->edn = ExternDef(); //����һ���ⲿ���壬�õ�һ����������Ϊroot�ĵ�һ������
	root->edln = ExternDefList(); //�õ�����������Ϊroot�ĵڶ�������
	return root;
}

//����һ�����򣬰����﷨���壺	<����> ����=<�ⲿ��������>
//�﷨��λ<����>���ӳ������£���ɵĹ���������һ���﷨������ָ��ָ�����һ���ⲿ�������еĽ�㡣
int GraAnalyse(FILE* fp_)
{
	fp = fp_;
	w = gettoken(fp);
}