#include "grammer.h"

typedef struct RNL
{
	struct RNL *next;
	struct RNL *last;
	char var_name[32];
	enum token_kind var_kind;
} RNL;
RNL *root = NULL, **proot = NULL;

typedef struct OpStack
{
	struct OpStack *head;
	enum token_kind op;
} OpStack;

typedef struct NumStack
{
	struct NumStack *head;
	struct Child *num;
} NumStack;

char precede[20][20] =
	{
		//+     -    *    /    %    &&   ||   (     )   =    >    <    <=   >=   ==  !=    #
		{'>', '>', '<', '<', '<', '>', '>', '<', '>', ' ', '>', '>', '>', '>', '>', '>', '>'}, //+
		{'>', '>', '<', '<', '<', '>', '>', '<', '>', ' ', '>', '>', '>', '>', '>', '>', '>'}, //-
		{'>', '>', '>', '>', '>', '>', '>', '<', '>', ' ', '>', '>', '>', '>', '>', '>', '>'}, //*
		{'>', '>', '>', '>', '>', '>', '>', '<', '>', ' ', '>', '>', '>', '>', '>', '>', '>'}, ///
		{'>', '>', '>', '>', '>', '>', '>', '<', '>', ' ', '>', '>', '>', '>', '>', '>', '>'}, //%
		{'<', '<', '<', '<', '<', '>', '>', '<', '>', ' ', '<', '<', '<', '<', '<', '<', '>'}, //&&
		{'<', '<', '<', '<', '<', '<', '>', '<', '>', ' ', '<', '<', '<', '<', '<', '<', '>'}, //||
		{'<', '<', '<', '<', '<', '<', '<', '<', '=', ' ', '<', '<', '<', '<', '<', '<', '>'}, //(
		{'>', '>', '>', '>', '>', '>', '>', '>', '>', ' ', '>', '>', '>', '>', '>', '>', '>'}, //)
		{'<', '<', '<', '<', '<', '>', '>', '<', ' ', '<', '<', '<', '<', '<', '<', '<', '>'}, //=
		{'<', '<', '<', '<', '<', '>', '>', '<', '>', ' ', '>', '>', '>', '>', '>', '>', '>'}, //>
		{'<', '<', '<', '<', '<', '>', '>', '<', '>', ' ', '>', '>', '>', '>', '>', '>', '>'}, //<
		{'<', '<', '<', '<', '<', '>', '>', '<', '>', ' ', '>', '>', '>', '>', '>', '>', '>'}, //<=
		{'<', '<', '<', '<', '<', '>', '>', '<', '>', ' ', '>', '>', '>', '>', '>', '>', '>'}, //>=
		{'<', '<', '<', '<', '<', '>', '>', '<', '>', ' ', '<', '<', '<', '<', '>', '>', '>'}, //==
		{'<', '<', '<', '<', '<', '>', '>', '<', '>', ' ', '<', '<', '<', '<', '>', '>', '>'}, //!=
		{'<', '<', '<', '<', '<', '<', '<', '<', ' ', '<', '<', '<', '<', '<', '<', '<', '='}, //#
};
char type[100][20] =
	{
		"ERROR_TOKEN",
		"IDENT",
		"INT",
		"FLOAT",
		"CHAR",
		"VOID",
		"LONG",
		"SHORT",
		"BOOL",
		"DOUBLE",
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
		"PLUS",
		"MINUS",
		"MUL",
		"DIV",
		"MOD",
		"ANDAND",
		"OROR",
		"LP",
		"RP",
		"ASSIGN",
		"MORE",
		"LESS",
		"LESSEQ",
		"MOREEQ",
		"EQUAL",
		"UNEQUAL",
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

//用来保存获得的词法单元
keyword w;
//保存文件指针
FILE *fp;
//err：错误个数
//ppp：声明的函数个数
int err = 0, ppp = 0;
//函数声明的数组
char funn[100][100];
//注册表相关结构，保存返回的数组标识符
char retArrName[32];
//保存当前语句开始时的行数
int tline = 0;

//将一个标识符加入注册表，表示该标识符已被使用
void pushRNL(keyword, RNL **);
//获取当前注册表的头指针
RNL *getRNL(RNL *);
//删除当前作用域的所有注册过的标识符
//不包括删除rootRNLcur
void destroyRNL(RNL *);
//获得数组的名称，方便与其他标识符进行比较
//原因在于数组的tokentext为name[]或者name[10]这种形式，无法与标识符进行比较
char *getname(keyword);
//在当前作用域下查询该标识符是否被使用过
int searchRNL(RNL *rootRNLcur, keyword);
//过去单个注释，即以//开头的一行或者以/*开头以*/结尾的一段
char *getsinglecomment();
//获取所有注释，生成注释的链表，防止注释跟随注释
CommentNode *getcomment();
//判断该词法单元是否是类型说明符
int IsVarDeclare(keyword);
//判断该词法单元是否是标识符
int IsIdent(keyword);
//判断该词法单元是否是常数
int IsConst(keyword);
//初始化操作符栈
int OpInitiate(OpStack *);
//将操作符推入栈
int OpPush(int, OpStack **);
//将操作符推出栈
int OpPop(OpStack **);
//初始化操作数栈
int NumInitiate(NumStack *);
//将操作数推入栈
int NumPush(NumStack *, NumStack **);
//将操作数出栈
NumStack *NumPop(NumStack **);

//语法分析程序的开始程序
//对于一个程序，按其语法定义：	<程序> ：：=<外部定义序列>
//语法单位<程序>的子程序如下，完成的功能是生成一棵语法树，根指针指向的是一个外部定义序列的结点。
ExternDefListNode *GraAnalyse(FILE *);
//外部定义序列
//语法单位<外部定义序列>的定义：
//<外部定义序列>：： = <外部定义> <外部定义序列> | <外部定义>
//这是一个递归定义，该子程序处理一系列的外部定义，每个外部定义序列的结点，其第一个子树对应一个外部定义，第二棵子树对应后续的外部定义。
//在一个源程序中，每次成功处理完一个外部定义后，如果遇到文件结束标记，则语法分析结束。调用此子程序，已经读入了一个外部定义的第一个单词到w中。
ExternDefListNode *ExternDefList();
//外部定义
//此子程序完成一个外部定义的处理，调用此子程序时，已经读入了一个外部定义的第一个单词到w中。
//该子程序处理完后，刚好处理到外部定义的最后一个符号，后续单词还没读入。
//处理外部定义序列，正确时，返回子树根结点指针，否则返回NULL 
ExternDefNode *ExternDef();
//外部变量定义
//调用此子程序时，外部变量类型和第一个变量名的单词已经读入，
//变量名保存在tokenText0中，这时外部变量定义的处理流程可参考如下。
ExternVarDefNode *ExternVarDef();
//生成变量列表节点
VarListNode *VarList(RNL *);
//生成函数定义或函数声明节点
//调用此子程序时，函数返回值类型和函数名，正小括号的单词已经读入，函数名保存在tokenText0中 done
//has push fun name into rnl
FunDefNode *FunDef(keyword);
//生成形参序列节点
//已经读入了LP done
FormFactorListNode *FormFactorList(int, RNL *);
//生成复合语句节点
//调用此子程序时，已经读入了单词{，继续处理时，遇到遇到}，结束复合语句
ComposeNode *Compose(RNL *);
//生成局部变量定义节点
LocalVarDefNode *LocalVarDef(RNL *);
//生成语句序列节点
SentenceListNode *SentenceList(RNL *);
//生成语句节点
//调用此子程序时，语句的第一个单词已经读入，处理一条语句时，
//根据这条语句的第一个单词，确定处理什么类型的语句。 
SentenceNode *Sentence(RNL *);
//生成表达式节点
//表达式结束符号endsym可以是分号，如表达式语句，可以是反小括号，作为条件时使用
Child *Expression(int, int, RNL*);

//输出语法树
int output(ExternDefListNode *);
//输出复合语句结构
int putcompose(ComposeNode *, int);
//输出语句结构
int putsen(SentenceNode *, int);
//输出表达式结构
int putexp(Child *, int);
//输出变量列表结构
int putvarlist(VarListNode *);
//输出注释结构
int putcomment(CommentNode *, int);

void pushRNL(keyword w, RNL **leaveRNLcur)
{
	if (*leaveRNLcur == NULL)
	{
		*leaveRNLcur = (RNL *)malloc(sizeof(RNL));
		RNL *temp = *leaveRNLcur;
		temp->next = NULL;
		temp->last = NULL;
		strcpy(temp->var_name, w.tokentext);
		temp->var_kind = w.kind;
		return;
	}
	RNL *bufRNL = *leaveRNLcur;
	RNL *temp = *leaveRNLcur;
	temp->next = (RNL *)malloc(sizeof(RNL));
	temp = temp->next;
	temp->var_kind = w.kind;
	strcpy(temp->var_name, w.tokentext);
	temp->last = bufRNL;
	temp->next = NULL;
}
RNL *getRNL(RNL *RNLcur)
{
	while (RNLcur != NULL && RNLcur->next != NULL)
		RNLcur = RNLcur->next;
	if (RNLcur == NULL)
		return NULL;
	return RNLcur;
}
void destroyRNL(RNL *rootRNLcur) 
{
	RNL *temp = rootRNLcur;
	if (rootRNLcur != NULL)
		rootRNLcur = rootRNLcur->next;
	while (rootRNLcur != NULL)
	{
		RNL *temp = rootRNLcur;
		rootRNLcur = rootRNLcur->next;
		free(temp);
		temp = NULL;
	}
	temp->next = NULL;
	return;
}
char *getname(keyword w)
{
	if (w.kind == FORMARRAY || w.kind == ARRAY)
	{
		int j;
		for (j = 0; j < strlen(w.tokentext); j++)
		{
			if (w.tokentext[j] != '[')
				retArrName[j] = w.tokentext[j];
		}
		retArrName[j] = '\0';
		return retArrName;
	}
	return NULL;
}
int searchRNL(RNL *rootRNLcur, keyword w)
{
	char target[32];
	if (w.kind == FORMARRAY || w.kind == ARRAY)
	{
		int j = 0;
		for (j = 0; w.tokentext[j] != '['; j++)
			target[j] = w.tokentext[j];
		target[j] = '\0';
	}
	else
		strcpy(target, w.tokentext);
	if (rootRNLcur != NULL)
		rootRNLcur = rootRNLcur->next;
	while (rootRNLcur != NULL)
	{
		int ret;
		if (rootRNLcur->var_kind != FORMARRAY && rootRNLcur->var_kind != ARRAY)
			ret = strcmp(rootRNLcur->var_name, target);
		else
		{
			keyword copy;
			copy.kind = rootRNLcur->var_kind;
			strcpy(copy.tokentext, rootRNLcur->var_name);
			getname(copy);
			ret = strcmp(retArrName, target);
		}
		if (ret == 0)
			return 1;
		rootRNLcur = rootRNLcur->next;
	}
	return 0;
}
char *getsinglecomment()
{
	if (err)
		return NULL;
	char *ans = (char *)malloc(sizeof(char) * 100);
	if (w.kind == COMMENT)
	{
		fgets(ans, 100, fp);
		ungetc('\n', fp);
		w = gettoken(fp);
		// get next word
		return ans;
	}
	if (w.kind == LCOMMENT)
	{
		int j = 0;
		char c1 = readchar(fp);
		if (c1 == EOF)
		{
			printf("line:%d 文件结束而注释尚未有右界\n", w.line);
			err++;
			return NULL;
		}
		char c2 = readchar(fp);
		if (c2 == EOF)
		{
			printf("line:%d  文件结束而注释尚未有右界\n", w.line);
			err++;
			return NULL;
		}
		while (!(c1 == '*' && c2 == '/'))
		{
			ans[j++] = c1;
			c1 = c2;
			c2 = readchar(fp);
			if (c2 == EOF)
			{
				printf("line:%d  文件结束而注释尚未有右界\n", w.line);
				err++;
				return NULL;
			}
		}
		ans[j] = '\0';
		w = gettoken(fp);
		// get next word
		return ans;
	}
	return NULL;
}
CommentNode *getcomment()
{
	if (err)
		return NULL;
	CommentNode *c = (CommentNode *)malloc(sizeof(CommentNode));
	CommentNode *tc = c;
	char *t;
	while ((t = getsinglecomment()) != NULL)
	{
		int j = 0;
		while (t[j] != '\0')
		{
			tc->com[j] = t[j];
			j++;
		}
		tc->com[j] = '\0';
		tc->c = (CommentNode *)malloc(sizeof(CommentNode));
		tc = tc->c;
	}
	tc->c = NULL;
	strcpy(tc->com, "");
	return c;
}
int IsVarDeclare(keyword t) 
{
	if (t.kind <= DOUBLE && t.kind >= INT)
		return 1;
	else
		return 0;
}
int IsIdent(keyword t)
{
	if (t.kind == IDENT || t.kind == ARRAY)
		return 1;
	else
		return 0;
}
int IsConst(keyword t)
{
	if (t.kind >= INT_CONST && t.kind <= DOUBLE_CONST)
		return 1;
	return 0;
}
int OpInitiate(OpStack *op)
{
	op->head = NULL;
	op->op = ERROR_TOKEN;
	return 0;
}
int OpPush(int to, OpStack **pnow)
{
	OpStack *t = (OpStack *)malloc(sizeof(OpStack));
	t->op = to;
	t->head = (*pnow);
	(*pnow) = t;
	return 0;
}
int OpPop(OpStack **pnow)
{
	if ((*pnow) == NULL)
		return 0;
	if ((*pnow)->op == EXCLA)
		return 0;
	int res = (*pnow)->op;
	(*pnow) = (*pnow)->head;
	return res;
}
int NumInitiate(NumStack *num)
{
	num->head = NULL;
	num->num = NULL;
	return 0;
}
int NumPush(NumStack *to, NumStack **pnum)
{
	if ((*pnum) == NULL)
	{
		(*pnum) = to;
		return 0;
	}
	to->head = (*pnum);
	(*pnum) = to;
	return 0;
}
NumStack *NumPop(NumStack **pnum)
{
	if ((*pnum) == NULL)
		return NULL;
	NumStack *res = *pnum;
	(*pnum) = (*pnum)->head;
	res->head = NULL;
	return res;
}

// if it's function use, tot = 1, else tot = 0
Child *Expression(int EndChar, int tot, RNL *secroot) 
{
	if (err)
		return NULL;
	int cnt = 0;
	OpStack *op = (OpStack *)malloc(sizeof(OpStack));
	OpInitiate(op);
	NumStack *num = (NumStack *)malloc(sizeof(NumStack));
	NumInitiate(num);
	int error = 0;
	int flag = 0;
	OpPush(EXCLA, &op);
	//int tline = 0;
	while ((w.kind != EXCLA || op->op != EXCLA) && error == 0)
	{
		cnt++;
		if (w.kind == COMMA)
			w.kind = SEMMI;
		if (IsIdent(w) || IsConst(w))
		{
			if (IsIdent(w))
			{
				if (!searchRNL(root, w))
				{
					printf("line %d 变量未定义\n", w.line);
					err++;
					return NULL;
				}
			}
			NumStack *te = (NumStack *)malloc(sizeof(NumStack));
			te->head = NULL;
			Child *to = (Child *)malloc(sizeof(Child));
			strcpy(to->i, w.tokentext);
			to->l = to->r = NULL;
			to->op = w.kind;
			te->num = to;
			NumPush(te, &num);
			if (w.kind == IDENT)
				flag = 1;
			else
				flag = 0;
			w = gettoken(fp);
			if (flag == 1 && w.kind == LP)
			{
				for (int i = 0; i <= ppp; i++)
				{
					if (i == ppp)
					{
						printf("line%d 函数未声明\n", w.line);
						err++;
						return NULL;
					}
					if (strcmp(to->i, funn[i]) == 0)
						break;
				}
				w = gettoken(fp);
				Child *f = (Child *)malloc(sizeof(Child));
				strcpy(f->i, to->i);
				f->op = FunUse;
				Child *fcopy = f;
				int count = 0;
				do
				{
					if (count != 0)
						w = gettoken(fp);
					if (IsIdent(w))
						fcopy->l = Expression(RP, 1, root);
					else
						fcopy->l = NULL;
					if (fcopy != f)
						fcopy->op = ERROR_TOKEN;
					if (fcopy->l != NULL)
						fcopy->r = (Child *)malloc(sizeof(Child));
					else
					{
						fcopy->i[0] = '\0';
						fcopy->r = NULL;
						fcopy->op = ERROR_TOKEN;
						break;
					}
					fcopy = fcopy->r;
					count++;
				} while (w.kind == EXCLA);
				//w = gettoken(fp);
				NumStack *t1 = (NumStack *)malloc(sizeof(NumStack));
				t1->head = NULL;
				t1->num = f;
				NumPop(&num);
				NumPush(t1, &num);
			}
		}
		else if (w.kind <= EXCLA && w.kind >= PLUS)
		{
			NumStack *n = (NumStack *)malloc(sizeof(NumStack));
			Child *t1 = NULL, *t2 = NULL;
			int t;
			OpStack *xxx;
			Child *p = (Child *)malloc(sizeof(Child));
			switch (precede[op->op - PLUS][w.kind - PLUS])
			{
				//higher priority
			case '<':
				xxx = (OpStack *)malloc(sizeof(OpStack));
				xxx->op = w.kind;
				xxx->head = op;
				op = xxx;
				w = gettoken(fp);
				free(n);
				free(p);
				break;
				//erase right paren
			case '=':
				if (op->head->head == NULL)
					error++;
				xxx = op;
				if (error == 0)
					op = op->head;
				free(xxx);
				free(n);
				free(p);
				w = gettoken(fp);
				break;
				//lower priority
			case '>':
				if (num->num != NULL)
				{
					t1 = num->num;
					num = num->head;
				}
				else
					error++;
				if (num->num != NULL)
				{
					t2 = num->num;
					num = num->head;
				}
				else
					error++;
				xxx = op;
				if (op->op != EXCLA)
				{
					t = op->op;
					op = op->head;
				}
				else
					error++;
				if (t == 0)
					error++;
				if (error == 0)
					free(xxx);
				p->op = t;
				if (error == 0)
					p->r = t1;
				if (error == 0)
					p->l = t2;
				p->i[0] = '\0';
				n->num = p;
				n->head = num;
				num = n;
				break;
			default:
				if (w.kind == EndChar)
					w.kind = EXCLA;
				else
					error = 1;
			}
		}
		else if (w.kind == EndChar || (w.kind == SEMMI && tot == 1))
			w.kind = EXCLA;
		else
			error++;
	}
	//special case for void value
	if (cnt == 1 && error == 0)
	{
		num->num = (Child *)malloc(sizeof(Child));
		num->num->l = num->num->r = NULL;
		strcpy(num->num->i, "void");
		num->num->op = 0;
		return num->num;
	}
	//whether only one num is left
	if (num->num != NULL && num->head->head == NULL && num->head->num == NULL && op->op == EXCLA && op->head->head == NULL && error == 0)
	{
		return num->num;
	}
	else
	{
		return NULL;
	}
}

VarListNode *VarList(RNL *secroot) //已经读入了第一个变量 done 会读入下一个字符 显示了错误
{
	if (err)
		return NULL;
	if ((!IsIdent(w)) && w.kind != ARRAY)
	{
		err++;
		printf("line %d 错误的标识符\n", w.line);
		return NULL;
	}
	VarListNode *vl = (VarListNode *)malloc(sizeof(VarListNode));
	strcpy(vl->ident, w.tokentext);
	if (secroot == root)
	{
		if (searchRNL(secroot, w))
		{
			printf("line %d 重复定义变量\n", w.line);
			err++;
			return NULL;
		}
		RNL *pushed = getRNL(secroot);
		pushRNL(w, &pushed);
	}
	w = gettoken(fp);
	//处理初始化
	if (w.kind == ASSIGN)
	{
		w = gettoken(fp);
		vl->initial = Expression(SEMMI, 0, root);
		if (vl->initial == NULL)
			return NULL;
		else if (strcmp(vl->initial->i, "void") == 0)
		{
			printf("line %d 初始化错误\n", w.line);
			err++;
			return NULL;
		}
		if (strcmp(w.tokentext, ",") == 0)
			w.kind = COMMA;
		else if (strcmp(w.tokentext, ";") == 0)
			w.kind = SEMMI;
		if (err)
			return NULL;
	}
	else
		vl->initial = NULL;
	if (w.kind != COMMA && w.kind != SEMMI)
	{
		err++;
		printf("line %d 结束符错误\n", tline);
		return NULL;
	}
	if (w.kind == SEMMI)
	{
		w = gettoken(fp);
		vl->vl = NULL;
		return vl;
	}
	w = gettoken(fp);
	if (searchRNL(secroot, w))
	{
		printf("line %d 重复定义变量\n", w.line);
		err++;
		return NULL;
	}
	RNL *tpushed = getRNL(secroot);
	pushRNL(w, &tpushed);
	vl->vl = VarList(secroot);
	if (vl->vl == NULL)
		return NULL;
	return vl;
}

ExternVarDefNode *ExternVarDef()
{
	tline = w.line;
	if (err)
		return NULL;
	ExternVarDefNode *evd = (ExternVarDefNode *)malloc(sizeof(ExternVarDefNode));
	evd->kind = w.kind;
	evd->vl = VarList(root);
	if (evd->vl == NULL)
		return NULL;
	return evd;
}

FormFactorListNode *FormFactorList(int stop, RNL *secroot) 
{
	if (err)
		return NULL;
	w = gettoken(fp);
	if (w.kind != RP && !IsVarDeclare(w))
	{
		if (stop == 1)
		{
			printf("line%d  形参列表错误\n ", w.line);
			err++;
			return NULL;
		}
		return NULL;
	}
	FormFactorListNode *ffl = (FormFactorListNode *)malloc(sizeof(FormFactorListNode));
	if (w.kind == RP)
	{
		ffl->kind = 0;
		ffl->ffl = NULL;
		strcpy(ffl->ident, "VOID");
		return ffl;
	}
	else
	{
		ffl->kind = w.kind;
		w = gettoken(fp);
		if (!IsIdent(w) && w.kind != FORMARRAY)
		{
			if (stop == 1)
			{
				err++;
				printf("line%d 形参列表错误\n", w.line);
				return NULL;
			}
			return NULL;
		}
		if (searchRNL(secroot, w))
		{
			printf("line:%d  重复定义标识符\n", w.line);
			err++;
			return NULL;
		}
		RNL *pushed = getRNL(secroot);
		pushRNL(w, &pushed);
		strcpy(ffl->ident, w.tokentext);
	}
	w = gettoken(fp);
	if (w.kind == COMMA)
	{
		ffl->ffl = FormFactorList(stop + 1, secroot);
		if (ffl->ffl == NULL)
		{
			if (stop == 1)
			{
				err++;
				printf("line%d  形参列表错误\n", w.line);
				return NULL;
			}
			return NULL;
		}
		return ffl;
	}
	else if (w.kind == RP)
	{
		ffl->ffl = (FormFactorListNode *)malloc(sizeof(FormFactorListNode));
		ffl->ffl->kind = 0;
		ffl->ffl->ffl = NULL;
		return ffl;
	}
	else
	{
		if (stop == 1)
		{
			err++;
			printf("line%d  形参列表错误\n", w.line);
			return NULL;
		}
		return NULL;
	}
	return ffl;
}


SentenceNode *Sentence(RNL *secroot)
{
	if (err > 0)
		return NULL;
	int tline = w.line;
	SentenceNode *s = (SentenceNode *)malloc(sizeof(SentenceNode));
	switch (w.kind)
	{
	case IF: //分析条件语句
		w = gettoken(fp);
		if (w.kind != LP)
		{
			printf("line%d\n  没有左括弧\n", tline);
			err++;
			return NULL;
		}
		else
		{
			w = gettoken(fp);
			s->e1 = Expression(RP, 0, root);
			if (s->e1 == NULL)
			{
				err++;
				printf("line%d  条件表达式错误\n", w.line);
				return NULL;
			}
		}
		w = gettoken(fp);
		s->ec1 = getcomment();
		if (w.kind != LCURLY)
		{
			s->s1 = Sentence(secroot);
			s->c1 = NULL;
		}
		else
		{
			s->s1 = NULL;
			RNL *temp = getRNL(secroot);
			s->c1 = Compose(temp);
			destroyRNL(temp);
			w = gettoken(fp);
		}
		if (w.kind == ELSE)
		{
			w = gettoken(fp);
			s->ec2 = getcomment(); //else+comment
			if (w.kind == LCURLY)
			{
				RNL *temp = getRNL(secroot);
				s->c2 = Compose(temp);
				destroyRNL(temp);
				s->s2 = NULL;
				w = gettoken(fp);
			}
			else
			{
				s->s2 = Sentence(secroot);
				s->c2 = NULL;
			}
			s->kind = ELSE;
		}
		else
			s->kind = IF;
		break;
	case LP:
		s->kind = Expres;
		s->e1 = Expression(SEMMI, 0, secroot);
		if (s->e1 == NULL)
		{
			err++;
			printf("line%d 表达式错误\n", tline);
			return NULL;
		}
		w = gettoken(fp);
		break;
	case IDENT:
	case ARRAY:
	case INT_CONST:
	case FLOAT_CONST:
	case CHAR_CONST:
	case LONG_CONST:
	case DOUBLE_CONST:
	case SHORT_CONST:
		s->kind = Expres;
		s->e1 = Expression(SEMMI, 0, secroot);
		if (s->e1 == NULL)
		{
			err++;
			printf("line%d  表达式错误\n", tline);
			return NULL;
		}
		s->s1 = s->s2 = NULL;
		w = gettoken(fp);
		break;
	case RETURN:
		s->kind = RETURN;
		w = gettoken(fp);
		s->e1 = Expression(SEMMI, 0, secroot);
		if (s->e1 == NULL)
		{
			err++;
			printf("line%d  表达式错误\n", tline);
			return NULL;
		}
		s->s1 = NULL;
		s->s2 = NULL;
		w = gettoken(fp);
		break;
	case WHILE:
		s->kind = WHILE;
		s->e2 = s->e3 = NULL;
		w = gettoken(fp);
		if (w.kind != LP)
		{
			err++;
			printf("line%d  没有左括弧\n", tline);
			return NULL;
		}
		else
		{
			w = gettoken(fp);
			s->e1 = Expression(RP, 0, root);
			if (s->e1 == NULL)
			{
				err++;
				printf("line%d  条件表达式错误\n", tline);
				return NULL;
			}
		}
		w = gettoken(fp);
		s->ec1 = getcomment();
		s->ec2 = getcomment();
		if (w.kind != LCURLY)
		{
			s->c1 = NULL;
			s->c2 = NULL;
			s->s2 = NULL;
			s->s1 = Sentence(secroot);
		}
		else
		{
			s->s1 = NULL;
			s->s2 = NULL;
			s->c2 = NULL;
			RNL *temp = getRNL(secroot);
			s->c1 = Compose(temp);
			destroyRNL(temp);
			w = gettoken(fp);
		}
		break;
	case FOR:
		s->kind = FOR;
		w = gettoken(fp);
		if (w.kind != LP)
		{
			err++;
			printf("line%d  没有左括弧\n", tline);
			return NULL;
		}
		else
		{
			w = gettoken(fp);
			s->e1 = Expression(SEMMI, 0, secroot);
			if (s->e1 == NULL)
			{
				printf("line%d  条件表达式错误\n", tline);
				err++;
				return NULL;
			}
			else
			{
				w = gettoken(fp);
				s->e2 = Expression(SEMMI, 0, secroot);
				if (s->e2 == NULL)
				{
					printf("line%d  条件表达式错误\n", tline);
					err++;
					s->e3 = NULL;
				}
				else
				{
					w = gettoken(fp);
					s->e3 = Expression(RP, 0, root);
					if (s->e3 == NULL)
					{
						printf("line%d  条件表达式错误\n", tline);
						err++;
						return NULL;
					}
					else
						w = gettoken(fp);
				}
			}
		}
		s->ec1 = getcomment();
		s->ec2 = getcomment();
		if (w.kind == LCURLY)
		{
			s->s1 = s->s2 = NULL;
			s->c2 = NULL;
			RNL *temp = getRNL(secroot);
			s->c1 = Compose(temp);
			destroyRNL(temp);
			w = gettoken(fp);
			//w = gettoken(fp);
		}
		else
		{
			s->s1 = Sentence(secroot);
			s->s2 = NULL;
			s->c1 = s->c2 = NULL;
		}
		break;
	case BREAK:
		s->c1 = s->c2 = NULL;
		s->s1 = s->s2 = NULL;
		s->e1 = s->e2 = s->e3 = NULL;
		s->kind = BREAK;
		w = gettoken(fp); // get ;
		if (w.kind != SEMMI)
		{
			printf("line:%d 没有分号结束\n", tline);
			err++;
			return NULL;
		}
		w = gettoken(fp);
		break;
	case CONTINUE:
		s->c1 = s->c2 = NULL;
		s->s1 = s->s2 = NULL;
		s->e1 = s->e2 = s->e3 = NULL;
		s->kind = CONTINUE;
		w = gettoken(fp); // get ;
		if (w.kind != SEMMI)
		{
			printf("line:%d 没有分号结束\n", tline);
			err++;
			return NULL;
		}
		w = gettoken(fp);
		break;
	case COMMENT:
	case LCOMMENT:
		s->ec1 = getcomment();
		s->ec2 = NULL;
		s->kind = COMMENT;
		s->c1 = s->c2 = NULL;
		s->e1 = s->e2 = s->e3 = NULL;
		s->s1 = s->s2 = NULL;
		break;
	default:
		if (w.kind != RCURLY)
		{
			err++;
			printf("line%d  没有右大括号\n", tline);
			return NULL;
		}
		return NULL;
	}
	return s;
}

SentenceListNode *SentenceList(RNL *secroot) //done
{
	if (err)
		return NULL;
	SentenceNode *s = Sentence(secroot);
	if (s == NULL)
		return NULL;
	else
	{
		SentenceListNode *sl = (SentenceListNode *)malloc(sizeof(SentenceListNode));
		sl->s = s;
		sl->sl = SentenceList(secroot);
		return sl;
	}
}

LocalVarDefNode *LocalVarDef(RNL *secroot)
{
	tline = w.line;
	if (err)
		return NULL;
	LocalVarDefNode *lvd = (LocalVarDefNode *)malloc(sizeof(LocalVarDefNode));
	strcpy(lvd->TypeStatement, w.tokentext);
	w = gettoken(fp);
	if (!IsIdent(w))
	{
		printf("line:%d\n", w.line);
		err++;
		return NULL;
	}
	if (searchRNL(secroot, w))
	{
		printf("line %d 重复定义变量\n", w.line);
		err++;
		return NULL;
	}
	RNL *pushed = getRNL(secroot);
	pushRNL(w, &pushed);
	lvd->vln = VarList(secroot);
	if (IsVarDeclare(w))
		lvd->lvd = LocalVarDef(secroot);
	else
	{
		lvd->lvd = (LocalVarDefNode *)malloc(sizeof(LocalVarDefNode));
		lvd->lvd->TypeStatement[0] = '\0';
		lvd->lvd->lvd = NULL;
		lvd->lvd->vln = NULL;
	}
	return lvd;
}
ComposeNode *Compose(RNL *secroot)
{
	if (err)
		return NULL;
	ComposeNode *c = (ComposeNode *)malloc(sizeof(ComposeNode));
	w = gettoken(fp);
	if (IsVarDeclare(w))
	{
		c->lv = LocalVarDef(secroot);
	}
	else
		c->lv = NULL;
	if (err > 0)
		return NULL;
	c->sl = SentenceList(secroot);
	if (w.kind != RCURLY)
	{
		printf("line%d 没有右大括号\n", w.line);
		err++;
		w = gettoken(fp);
	}
	return c;
}
FunDefNode *FunDef(keyword copy)
{
	if (err)
		return NULL;
	FunDefNode *fd = (FunDefNode *)malloc(sizeof(FunDefNode));
	strcpy(fd->name, copy.tokentext);
	strcpy(funn[ppp++], copy.tokentext);
	RNL *formfact = getRNL(root);
	pushRNL(copy, &formfact);
	formfact = getRNL(root);
	fd->ffl = FormFactorList(1, formfact);
	destroyRNL(formfact);
	if (fd->ffl != NULL)
		w = gettoken(fp);
	else
	{
		return NULL;
	}
	fd->comment = getcomment();
	if (w.kind == SEMMI)
		fd->c = NULL;
	else if (w.kind == LCURLY)
	{
		RNL *temp = getRNL(root);
		fd->c = Compose(temp);
		destroyRNL(temp);
	}
	else
	{
		printf("line:%d  不符合条件的字符\n", w.line);
		err++;
		return NULL;
	}
	return fd;
}


ExternDefNode *ExternDef() 
{
	if (err)
		return NULL;
	ExternDefNode *edn = (ExternDefNode *)malloc(sizeof(ExternDefNode));
	edn->comment = NULL;
	edn->di = NULL;
	edn->evd = NULL;
	edn->fd = NULL;
	edn->kind = 0;
	if (w.kind == COMMENT || w.kind == LCOMMENT)
	{
		edn->comment = getcomment();
		edn->kind = COMMENT;
		return edn;
	}
	if (w.kind == EXCLA)
	{
		edn->di = (Def_includeNode *)malloc(sizeof(Def_includeNode));
		Def_includeNode *a = edn->di;
		edn->kind = ERROR_TOKEN;
		w = gettoken(fp);
		if (w.kind == INCLUDE)
		{
			a->kind = INCLUDE;
			w = gettoken(fp);
			int temp = w.kind;
			if (w.kind != LESS && w.kind != SINGGLESINGLE)
			{
				printf("line:%d  错误的定界符\n", w.line);
				free(a);
				err++;
				return NULL;
			}
			w = gettoken(fp);
			strcpy(a->ident, w.tokentext);
			w = gettoken(fp);
			if (w.kind == DOT)
			{
				w = gettoken(fp);
				if (strcmp(w.tokentext, "h") != 0)
				{
					free(edn->di);
					free(edn);
					printf("line:%d\n  没有.h\n", w.line);
					err++;
					return NULL;
				}
				w = gettoken(fp);
			}
			if (!((w.kind == SINGGLESINGLE && temp == SINGGLESINGLE) || (w.kind == MORE && temp == LESS)))
			{
				printf("line:%d\n  错误的定界符", w.line);
				err++;
				free(edn);
				w = gettoken(fp);
				return NULL;
			}
			a->val = NULL;
			w = gettoken(fp);
		}
		else if (w.kind == DEFINE)
		{
			a->kind = DEFINE;
			w = gettoken(fp);
			if (w.kind != IDENT)
			{
				printf("line:%d\n  错误的标识符", w.line);
				free(edn);
				w = gettoken(fp);
				err++;
				return NULL;
			}
			strcpy(a->ident, w.tokentext);
			if (searchRNL(root, w))
			{
				printf("line:%d\n  重复定义标识符", w.line);
				free(edn);
				err++;
				return NULL;
			}
			pushRNL(w, &root);
			w = gettoken(fp);
			int tline = w.line;
			a->val = Expression(SEMMI, 0, root);
			if (a->val == NULL)
			{
				if (w.line != tline)
				{
					printf("line:%d  表达式错误\n ", tline);
				}
				else
					printf("line:%d  表达式错误\n ", w.line);
				free(edn);
				err++;
				return NULL;
			}
			w = gettoken(fp);
		}
		else
		{
			printf("line:%d  错误的外部变量定义\n", w.line);
			free(edn);
			err++;
			return NULL;
		}
		return edn;
	}
	if (!IsVarDeclare(w) && w.kind != EOF_)
	{
		printf("line:%d  错误的类型声明\n", w.line);
		err++;
		return NULL;
	}
	edn->kind = w.kind;
	tline = w.line;
	w = gettoken(fp);
	if (w.kind != IDENT)
	{
		printf("line%d  错误的标识符\n", w.line);
		err++;
		return NULL;
	}
	if (searchRNL(root, w))
	{
		printf("line:%d\n  重复定义标识符", w.line);
		free(edn);
		err++;
		return NULL;
	}
	if (root == NULL)
		pushRNL(w, &root);
	else
	{
		RNL *temp = getRNL(root);
		pushRNL(w, &temp);
	}
	keyword wcopy = w;
	w = gettoken(fp);
	if (w.kind == LP)
	{
		edn->fd = FunDef(wcopy);
		if (edn->fd == NULL)
			return NULL;
		w = gettoken(fp);
		edn->evd = NULL;
	}
	else
	{
		edn->evd = (VarListNode *)malloc(sizeof(VarListNode));
		strcpy(edn->evd->ident, wcopy.tokentext);
		edn->evd->vl = NULL;
		edn->fd = NULL;
		if (w.kind == ASSIGN)
		{
			w = gettoken(fp);
			edn->evd->initial = Expression(SEMMI, 0, root);
			if (edn->evd->initial == NULL)
				return NULL;
			else if (strcmp(edn->evd->initial->i, "void") == 0)
			{
				printf("line %d 初始化错误\n", w.line);
				err++;
				return NULL;
			}
			if (strcmp(w.tokentext, ",") == 0)
				w.kind = COMMA;
			if (strcmp(w.tokentext, ";") == 0)
				w.kind = SEMMI;
		}
		else
			edn->evd->initial = NULL;
		if (w.kind == COMMA)
		{
			w = gettoken(fp);
			edn->evd->vl = VarList(root);
		}
		else if (w.kind == SEMMI)
			w = gettoken(fp);
		else
		{
			printf("line%d  少了结束符号\n", tline);
			err++;
			return NULL;
		}
	}
	return edn;
}


ExternDefListNode *ExternDefList() // 外部定义序列 done
{
	if (err)
		return NULL;
	if (w.kind == EOF_)
		return NULL;
	ExternDefListNode *root = (ExternDefListNode *)malloc(sizeof(ExternDefListNode)); //生成一个外部定义序列结点root
	root->edn = ExternDef();														  //处理一个外部定义，得到一棵子树，作为root的第一棵子树
	while (root->edn == NULL && w.kind != EOF_)
	{
		root->edn = ExternDef();
	}
	root->edln = ExternDefList(); //得到的子树，作为root的第二棵子树
	return root;
}


ExternDefListNode *GraAnalyse(FILE *fp_)
{
	fp = fp_;
	w = gettoken(fp);
	ExternDefListNode *root = ExternDefList();
	fclose(fp);
	return root;
}
int putcomment(CommentNode *c, int blank)
{
	while (strcmp(c->com, "") != 0)
	{
		for (int b = 0; b < blank; b++)
			putchar(' ');
		printf("注释:");
		printf("%s\n", c->com);
		c = c->c;
	}
	return 0;
}
int putvarlist(VarListNode *v)
{
	if (v == NULL)
		return -1;
	printf("%s ", v->ident);
	if (v->initial != NULL)
	{
		printf("初始化值为 ");
		putexp(v->initial, 0);
	}
	v = v->vl;
	while (v != NULL && v->ident[0] != '\0')
	{
		printf(",%s", v->ident);
		if (v->initial != NULL)
		{
			printf("初始化值为 ");
			putexp(v->initial, 0);
		}
		v = v->vl;
	}
	return 0;
}
int putexp(Child *c, int blank)
{
	if (c == NULL)
		return 0;
	if (c->op == FunUse)
	{
		Child *cc = c;
		for (int b = 0; b < blank; b++)
			putchar(' ');
		printf("函数调用: %s\n", c->i);
		for (int b = 0; b < blank; b++)
			putchar(' ');
		printf("实参:\n");
		while (cc->l != NULL)
		{
			putexp(cc->l, blank + 2);
			printf("\n");
			cc = cc->r;
		}
	}
	if (c->op <= UNEQUAL && c->op >= PLUS)
	{
		for (int b = 0; b < blank; b++)
			putchar(' ');
		printf("%s\n", type[c->op]);
		if (c->l != NULL)
		{
			putexp(c->l, blank + 2);
			printf("\n");
		}
		else
		{
			printf("ERROR!\n");
			return -1;
		}
		if (c->r != NULL)
		{
			putexp(c->r, blank + 2);
			printf("\n");
		}
		else
		{
			printf("ERROR!\n");
			return -1;
		}
	}
	if (c->op == ARRAY || (c->op >= IDENT && c->op <= DOUBLE) || (c->op >= INT_CONST && c->op <= DOUBLE_CONST))
	{
		for (int b = 0; b < blank; b++)
			putchar(' ');
		printf("ID:%s 种类:%s", c->i, type[c->op]);
	}
	if (c->op == 0 && c->l == NULL && c->r == NULL)
	{
		for (int b = 0; b < blank; b++)
			putchar(' ');
		printf("VOID\n");
	}
	return 0;
}
int putsen(SentenceNode *s, int blank)
{
	switch (s->kind)
	{
	case COMMENT:
		putcomment(s->ec1, blank);
		break;
	case IF:
		for (int i = 0; i < blank; i++)
			putchar(' ');
		printf("IF类型\n");
		for (int i = 0; i < blank + 2; i++)
			putchar(' ');
		printf("IF条件表达式:\n");
		putexp(s->e1, blank + 2);
		printf("\n");
		if (s->ec1)
			putcomment(s->ec1, blank + 2);
		for (int i = 0; i < blank + 2; i++)
			putchar(' ');
		printf("THEN:\n");
		if (s->s1 != NULL)
			putsen(s->s1, blank + 2);
		if (s->c1 != NULL)
			putcompose(s->c1, blank + 2);
		break;
	case ELSE:
		for (int b = 0; b < blank; b++)
			putchar(' ');
		printf("IF-ELSE类型:\n");
		for (int b = 0; b < blank + 2; b++)
			putchar(' ');
		printf("IF条件表达式\n");
		putexp(s->e1, blank + 2);
		printf("\n");
		if (s->ec1)
			putcomment(s->ec1, blank + 2);
		for (int b = 0; b < blank + 2; b++)
			putchar(' ');
		printf("THEN:\n");
		if (s->s1 != NULL)
			putsen(s->s1, blank + 2);
		else if (s->c1 != NULL)
			putcompose(s->c1, blank + 2);
		for (int b = 0; b < blank + 2; b++)
			putchar(' ');
		printf("ELSE:\n");
		if (s->ec2)
			putcomment(s->ec2, blank + 2);
		if (s->s2 != NULL)
			putsen(s->s2, blank + 2);
		else if (s->c2 != NULL)
			putcompose(s->c2, blank + 2);
		break;
	case RETURN:
		for (int b = 0; b < blank; b++)
			putchar(' ');
		printf("RETURN类型:\n");
		for (int b = 0; b < blank + 2; b++)
			putchar(' ');
		printf("返回表达式:\n");
		putexp(s->e1, blank + 4);
		printf("\n");
		break;
	case Expres:
		for (int b = 0; b < blank; b++)
			putchar(' ');
		printf("表达式:\n");
		putexp(s->e1, blank + 2);
		printf("\n");
		break;
	case WHILE:
		for (int b = 0; b < blank; b++)
			putchar(' ');
		printf("WHILE循环语句:\n");
		for (int b = 0; b < blank + 2; b++)
			putchar(' ');
		printf("WHILE循环条件表达式:\n");
		putexp(s->e1, blank + 4);
		printf("\n");
		if (s->ec1)
			putcomment(s->ec1, blank);
		for (int b = 0; b < blank + 2; b++)
			putchar(' ');
		printf("WHILE循环体:\n");
		if (s->c1 != NULL)
			putcompose(s->c1, blank + 4);
		if (s->s1 != NULL)
			putsen(s->s1, blank + 4);
		break;
	case FOR:
		for (int b = 0; b < blank; b++)
			putchar(' ');
		printf("FOR循环语句\n");
		for (int b = 0; b < blank + 2; b++)
			putchar(' ');
		printf("初始化:\n");
		putexp(s->e1, blank + 4);
		printf("\n");
		for (int b = 0; b < blank + 2; b++)
			putchar(' ');
		printf("条件表达式:\n");
		putexp(s->e2, blank + 4);
		printf("\n");
		for (int b = 0; b < blank + 2; b++)
			putchar(' ');
		printf("变化:\n");
		putexp(s->e3, blank + 4);
		if (s->ec1)
			putcomment(s->ec2, blank);
		for (int b = 0; b < blank + 2; b++)
			putchar(' ');
		printf("FOR循环循环体:\n");
		if (s->s1 != NULL)
			putsen(s->s1, blank + 4);
		if (s->c1 != NULL)
			putcompose(s->c1, blank + 4);
		break;
	case BREAK:
		for (int b = 0; b < blank; b++)
			putchar(' ');
		printf("BREAK语句\n");
		break;
	case CONTINUE:
		for (int b = 0; b < blank; b++)
			putchar(' ');
		printf("CONTINUE语句\n");
		break;
	default:
		printf("ERROR!\n");
	}
	return 0;
}
int putcompose(ComposeNode *c, int blank)
{
	if (c->lv != NULL)
	{
		LocalVarDefNode *lvl = c->lv;
		while (lvl->TypeStatement[0] != '\0')
		{
			for (int b = 0; b < blank; b++)
				putchar(' ');
			printf("局部变量定义序列:");
			for (int b = 0; b < blank; b++)
				putchar(' ');
			printf("类型说明符:%s\n", lvl->TypeStatement);
			for (int b = 0; b < blank; b++)
				putchar(' ');
			printf("变量序列:");
			putvarlist(lvl->vln);
			putchar('\n');
			lvl = lvl->lvd;
		}
	}
	if (c->sl != NULL)
	{
		for (int b = 0; b < blank; b++)
			putchar(' ');
		printf("语句序列 \n");
		SentenceListNode *sl = c->sl;
		while (sl != NULL && sl->s != NULL)
		{
			putsen(sl->s, blank + 2);
			sl = sl->sl;
		}
	}
	return 0;
}
int output(ExternDefListNode *root)
{
	if (err)
		return 0;
	if (root == NULL)
	{
		return 0;
	}
	ExternDefListNode *edl = root;
	while (edl != NULL)
	{
		if (edl->edn == NULL)
			break;
		if (edl->edn->evd != NULL)
		{
			printf("外部变量定义：\n");
			printf("  类型说明符 %s\n", type[edl->edn->kind]);
			printf("  变量序列");
			putvarlist(edl->edn->evd); //varlist output
			putchar('\n');
		}
		else if (edl->edn->fd != NULL)
		{
			printf("外部函数定义：\n");
			printf("  类型说明符 %s\n", type[edl->edn->kind]);
			printf("  函数名称 %s\n", edl->edn->fd->name);
			printf("  形参序列\n");
			if (edl->edn->fd->ffl != NULL)
			{
				FormFactorListNode *ffl = edl->edn->fd->ffl;
				while (ffl->kind != 0)
				{
					printf("    形参类型 %s 形参名称 %s\n", type[ffl->kind], ffl->ident);
					ffl = ffl->ffl;
				}
			}
			if (edl->edn->fd->c != NULL)
			{
				printf("  复合语句：\n");
				putcompose(edl->edn->fd->c, 4);
			}
		}
		else if (edl->edn->di != NULL)
		{
			Def_includeNode *t = edl->edn->di;
			if (t->kind == DEFINE)
			{
				printf("#define语句:\n");
				printf("标识符:%s\n", t->ident);
				printf("值:\n");
				putexp(t->val, 2);
				printf("\n");
			}
			if (t->kind == INCLUDE)
			{
				printf("#include语句:\n");
				printf("文件名:%s\n", t->ident);
			}
		}
		else if (edl->edn->comment != NULL)
		{
			putcomment(edl->edn->comment, 0);
		}
		edl = edl->edln;
	}
	return 0;
}