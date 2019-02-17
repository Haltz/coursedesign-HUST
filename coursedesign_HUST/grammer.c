#include "grammer.h"

node* root = NULL;
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

node* ExternDef(node* root)
{
	keyword w = gettoken(fp);
	//变量声明 暂时不支持初始化 不支持extern等修饰
	if (IsVarDeclare(w))
	{
		w = gettoken(fp);
		if(IsIdent(w))

	}
	else if (IsFunDeclare(w))
	{
	}
}
node* ExternDefList(node* root)//外部定义序列
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