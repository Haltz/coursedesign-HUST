#include "format.h"

FILE *fpp;
char ttype[100][20] =
	{
		"ERROR_TOKEN",
		"IDENT",
		"int",
		"float",
		"char",
		"void",
		"long",
		"short",
		"bool",
		"double",
		"IF",
		"ELSE",
		"RETURN",
		"WHILE",
		"FOR",
		"CONTINUE",
		"BREAK",
		"DEFINE",
		"INCLUDE",
		"INT_CONST",
		"FLOAT_CONST",
		"CHAR_CONST",
		"LONG_CONST",
		"SHORT_CONST",
		"BOOL_CONST",
		"DOUBLE_CONST",
		"LCURLY",
		"RCURLY",
		"LSQUARE",
		"RSQUARE",
		"SINGLE",
		"SINGGLESINGLE",
		"COMMA",
		"SEMMI",
		"QUESTION",
		"+",
		"-",
		"*",
		"/",
		"%",
		"&&",
		"||",
		"(",
		")",
		"=",
		">",
		"<",
		"<=",
		">=",
		"==",
		"!=",
		"EXCLA",
		"NEGATE",
		"HASHTAG",
		"PERCENT",
		"XOR",
		"AND",
		"ANDEQ",
		"OR",
		"OREQ",
		"MINUSMINUS",
		"PLUSPLUS",
		"COMMENT",
		"PLUSEQ",
		"MINUSEQ",
		"MULEQ",
		"DIVEQ",
		"NEGADIV",
		"DOT",
		"XOREQ",
		"LMOVE",
		"RMOVE",
		"LMOVEEQ",
		"RMOVEEQ",
		"LCOMMENT",
		"Expres",
		"FUNUSE",
		"ARRAY"};

//根据参数类型名可判断输出内容
int putedn_(ExternDefNode *edn);			//输出回车
int putexp_(Child *root);					//不输出回车
int putcom_(ComposeNode *root, int blank);  //输出回车
int putsen_(SentenceNode *root, int blank); //输出回车
int putblank_(int blank);
int format(FILE *fp)
{
	fpp = fopen("output.txt", "w");
	ExternDefListNode *root = GraAnalyse(fp);
	while (root != NULL && root->edn != NULL)
	{
		putedn_(root->edn);
		root = root->edln;
	}
	return 0;
}

int putedn_(ExternDefNode *edn)
{
	//输出预定义和预编译
	if (edn->di != NULL)
	{
		//输出DEFINE语句
		if (edn->di->kind == DEFINE)
		{
			fprintf(fpp, "#define %s ", edn->di->ident);
			putexp_(edn->di->val);
			fprintf(fpp, "\n");
		}
		//输出INCLUDE语句
		else if (edn->di->kind == INCLUDE)
		{
			fprintf(fpp, "#include %s\n", edn->di->ident);
		}
	}
	//输出注释
	else if (edn->comment != NULL)
	{
		while (edn->comment->c != NULL && strcmp(edn->comment->com, "") != 0)
		{
			fprintf(fpp, "/*%s*/\n", edn->comment->com);
			edn->comment = edn->comment->c;
		}
	}
	//输出外部变量定义
	else if (edn->evd != NULL)
	{
		VarListNode *t = edn->evd;
		fprintf(fpp, "%s ", ttype[edn->kind]);
		fprintf(fpp, "%s", t->ident);
		if (t->initial != NULL)
		{
			fprintf(fpp, " = ");
			putexp_(t->initial);
		}
		t = t->vl;
		while (t != NULL)
		{
			if (strcmp(t->ident, "") != 0)
				fprintf(fpp, ", %s", t->ident);
			if (t->initial != NULL)
			{
				fprintf(fpp, " = ");
				putexp_(t->initial);
			}
			t = t->vl;
		}
		fprintf(fpp, ";\n");
	}
	//输出函数
	else if (edn->fd != NULL)
	{
		//输出函数类型，函数名称
		fprintf(fpp, "%s %s(", ttype[edn->kind], edn->fd->name);
		FormFactorListNode *ffl = edn->fd->ffl;
		//输出形参列表
		if (ffl->kind != ERROR_TOKEN)
		{
			fprintf(fpp, "%s %s", ttype[ffl->kind], ffl->ident);
			ffl = ffl->ffl;
		}
		while (ffl->kind != ERROR_TOKEN && strcmp(ffl->ident, "") != 0)
		{
			fprintf(fpp, ", %s %s", ttype[ffl->kind], ffl->ident);
			ffl = ffl->ffl;
		}
		fprintf(fpp, ")");
		//输出分号
		if (edn->fd->c == NULL)
			fprintf(fpp, ";\n");
		//输出符合语句
		else
		{
			fprintf(fpp, "\n");
			putcom_(edn->fd->c, 0);
		}
	}
	return 0;
}

int putexp_(Child *root)
{
	if (root == NULL)
		return 0;
	//输出函数调用
	else if (root->op == FunUse)
	{
		fprintf(fpp, "%s(", root->i);
		putexp_(root->l);
		root = root->r;
		while (root->r != NULL)
		{
			fprintf(fpp, ", ");
			putexp_(root->l);
			root = root->r;
		}
		fprintf(fpp, ")");
	}
	//输出空节点
	else if (root->l == NULL && root->r == NULL && strcmp(root->i, "void") == 0 && root->op == 0)
	{
		fprintf(fpp, "");
	}
	//输出表达式
	else if (root->op <= EXCLA && root->op >= PLUS)
	{
		putexp_(root->l);
		fprintf(fpp, "%s", ttype[root->op]);
		putexp_(root->r);
	}
	else
	{
		if(root->op == CHAR_CONST)
			fprintf(fpp, "'%s'", root->i);
	   	else 
			fprintf(fpp, "%s", root->i);
	}
	return 0;
}
int putblank_(int blank)
{
	for (int i = 0; i < blank; i++)
		fprintf(fpp, " ");
	return 0;
}
int putcom_(ComposeNode *root, int blank)
{
	putblank_(blank);
	fprintf(fpp, "{\n");
	//输出局部变量序列
	while (root->lv != NULL && strcmp(root->lv->TypeStatement, "") != 0)
	{
		LocalVarDefNode *t = root->lv;
		putblank_(blank + 3);
		fprintf(fpp, "%s ", t->TypeStatement);
		fprintf(fpp, "%s", t->vln->ident);
		if (t->vln != NULL)
			t->vln = t->vln->vl;
		while (t->vln != NULL)
		{
			fprintf(fpp, ", %s", t->vln->ident);
			t->vln = t->vln->vl;
		}
		fprintf(fpp, ";\n");
		root->lv = root->lv->lvd;
	}
	if (root->sl != NULL)
	{
		while (root->sl != NULL && root->sl->s != NULL)
		{
			SentenceNode *t = root->sl->s;
			putsen_(t, blank + 3);
			root->sl = root->sl->sl;
		}
	}
	putblank_(blank);
	fprintf(fpp, "}\n");
	return 0;
}
int putsen_(SentenceNode *root, int blank)
{
	if (root->kind == COMMENT || root->kind == LCOMMENT)
	{
		while (root->ec1->c != NULL)
		{
			putblank_(blank);
			fprintf(fpp, "/*%s*/\n", root->ec1->com);
			root->ec1 = root->ec1->c;
		}
	}
	else if (root->kind == BREAK)
	{
		putblank_(blank);
		fprintf(fpp, "break;\n");
	}
	else if (root->kind == CONTINUE)
	{
		putblank_(blank);
		fprintf(fpp, "continue;\n");
	}
	else if (root->kind == RETURN)
	{
		putblank_(blank);
		fprintf(fpp, "return ");
		putexp_(root->e1);
		fprintf(fpp, ";\n");
	}
	else if (root->kind == ELSE)
	{
		putblank_(blank);
		fprintf(fpp, "if (");
		putexp_(root->e1);
		fprintf(fpp, ")\n");
		if (root->c1 != NULL)
		{
			putcom_(root->c1, blank + 3);
		}
		else
			putsen_(root->s1, blank + 3);
		putblank_(blank);
		fprintf(fpp, "else\n");
		if (root->c2 != NULL)
		{
			putcom_(root->c2, blank + 3);
		}
		else
			putsen_(root->s2, blank + 3);
	}
	else if (root->kind == IF)
	{
		putblank_(blank);
		fprintf(fpp, "if (");
		putexp_(root->e1);
		fprintf(fpp, ")\n");
		if (root->c1 != NULL)
		{
			putcom_(root->c1, blank + 3);
		}
		else
			putsen_(root->s1, blank + 3);
	}
	else if (root->kind == WHILE)
	{
		putblank_(blank);
		fprintf(fpp, "while(");
		putexp_(root->e1);
		fprintf(fpp, ")\n");
		if (root->c1 != NULL)
		{
			putcom_(root->c1, blank + 3);
		}
		else
			putsen_(root->s1, blank + 3);
	}
	else if (root->kind == FOR)
	{
		putblank_(blank);
		fprintf(fpp, "for(");
		putexp_(root->e1);
		fprintf(fpp, "; ");
		putexp_(root->e2);
		fprintf(fpp, "; ");
		putexp_(root->e3);
		fprintf(fpp, ")\n");
		if (root->c1 != NULL)
		{
			putcom_(root->c1, blank + 3);
		}
		else
			putsen_(root->s1, blank + 3);
	}
	else if (root->kind == Expres)
	{
		putblank_(blank);
		putexp_(root->e1);
		fprintf(fpp, ";\n");
	}
	return 0;
}