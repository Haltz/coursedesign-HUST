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
ExternDefListNode* ExternDefList()// 外部定义序列
{
	if (w.kind == EOF_) return NULL;
	ExternDefListNode* root = (ExternDefListNode*)malloc(sizeof(ExternDefListNode)); //生成一个外部定义序列结点root
	root->edn = ExternDef(); //处理一个外部定义，得到一棵子树，作为root的第一棵子树
	root->edln = ExternDefList(); //得到的子树，作为root的第二棵子树
	return root;
}
int GraAnalyse(FILE* fp_)
{
	fp = fp_;
	w = gettoken(fp);
}