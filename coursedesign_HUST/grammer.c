#include "grammer.h"

node* root = NULL;
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

node* ExternDef()
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
node* ExternDefList()//外部定义序列
{
	if (w.kind == EOF_) return NULL;
	node* root; //生成一个外部定义序列结点root
	root->head = (list*)malloc(sizeof(list));
	root->head->cur = ExternDef(); //处理一个外部定义，得到一棵子树，作为root的第一棵子树
	root->head->next->cur = ExternDefList(); //得到的子树，作为root的第二棵子树
	return root;
}
node* GraAnalyse(FILE* fp_)
{
	fp = fp_;
	w = gettoken(fp);
	if (ExternDefList()) return root;
}