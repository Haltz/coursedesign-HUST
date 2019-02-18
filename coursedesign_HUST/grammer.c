#include "grammer.h"

keyword w;
FILE* fp;

char type[][] =
{
	"外部变量定义",
	"函数定义",
	""
};

int IsVarDeclare(keyword t) //是否是声明变量的关键字
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
//调用此子程序时，外部变量类型和第一个变量名的单词已经读入，
//变量名保存在tokenText0中，这时外部变量定义的处理流程可参考如下。
ExternVarDefNode* ExternVarDef()
{
	ExternVarDefNode* evd = (ExternVarDefNode*)malloc(sizeof(ExternVarDefNode));
	evd->kind = w.kind;
	evd->vl = VarList();
}

FormFactorNode* FormFactor() //已经读入了第一个形参的声明部分 不读取标点符号
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

//调用此子程序时，语句的第一个单词已经读入，处理一条语句时，
//根据这条语句的第一个单词，确定处理什么类型的语句。
SentenceNode* Sentence()
{
	SentenceNode* s = (SentenceNode*)malloc(sizeof(SentenceNode));
	switch (w.kind)
	{
	case IF://分析条件语句
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

//调用此子程序时，已经读入了单词{，继续处理时，遇到遇到}，结束复合语句
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

//调用此子程序时，函数返回值类型和函数名，正小括号的单词已经读入，函数名保存在tokenText0中
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

//此子程序完成一个外部定义的处理，调用此子程序时，已经读入了一个外部定义的第一个单词到w中。
//该子程序处理完后，刚好处理到外部定义的最后一个符号，后续单词还没读入。
ExternDefNode* ExternDef() //处理外部定义序列，正确时，返回子树根结点指针，否则返回NULL
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

//语法单位<外部定义序列>的定义：
//<外部定义序列>：： = <外部定义> <外部定义序列> | <外部定义>
//这是一个递归定义，该子程序处理一系列的外部定义，每个外部定义序列的结点，其第一个子树对应一个外部定义，第二棵子树对应后续的外部定义。
//在一个源程序中，每次成功处理完一个外部定义后，如果遇到文件结束标记，则语法分析结束。调用此子程序，已经读入了一个外部定义的第一个单词到w中。
ExternDefListNode* ExternDefList()// 外部定义序列
{
	if (w.kind = EOF_) return NULL;
	ExternDefListNode* root = (ExternDefListNode*)malloc(sizeof(ExternDefListNode)); //生成一个外部定义序列结点root
	root->edn = ExternDef(); //处理一个外部定义，得到一棵子树，作为root的第一棵子树
	root->edln = ExternDefList(); //得到的子树，作为root的第二棵子树
	return root;
}

//对于一个程序，按其语法定义：	<程序> ：：=<外部定义序列>
//语法单位<程序>的子程序如下，完成的功能是生成一棵语法树，根指针指向的是一个外部定义序列的结点。
int GraAnalyse(FILE* fp_)
{
	fp = fp_;
	w = gettoken(fp);
}