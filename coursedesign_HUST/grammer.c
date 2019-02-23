#include "grammer.h"

keyword w;
FILE* fp;

int err = 0;

char precede[20][20] =
{
	">><<<> >>>>>",
	">><<<> >>>>>",
	">>>><> >>>>>",
	">>>><> >>>>>",
	"<<<<<= >>>>>",
	">>>>>  >>>>>",
	"<<<<< <<<<<>",
	"<<<<<> >>>>>",
	"<<<<<> >>>>>",
	"<<<<<> <<>>>",
	"<<<<<> <<>>>",
	"<<<<< <<<<<="
};
char type[100][20] =
{
	"ERROR_TOKEN",
	"IDENT",
	"INT",
	"FLOAT",
	"CHAR",
	"IF",
	"ELSE",
	"RETURN",
	"INT_CONST",
	"FLOAT_CONST",
	"CHAR_CONST",
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
	"LP", 
	"RP", 
	"ASSIGN", 
	"LESS", 
	"MORE", 
	"EQUAL", 
	"UNEQUAL", 
	"EXCLA",
	"NEGATE",  
	"HASHTAG", 
	"PERCENT", 
	"XOR", 
	"AND", 
	"ANDAND", 
	"ANDEQ", 
	"OR", 
	"OROR", 
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
	"LESSEQ", 
	"MOREEQ", 
	"LMOVE", 
	"RMOVE", 
	"LMOVEEQ", 
	"RMOVEEQ",
	"Expres"
};

typedef struct OpStack
{
	struct OpStack* head;
	enum token_kind op;
} OpStack;

typedef struct NumStack
{
	struct NumStack* head;
	struct Child* num;
} NumStack;

int IsVarDeclare(keyword t) //是否是声明变量的关键字
{
	if (t.kind <= CHAR && t.kind >= INT)
		return 1;
	else return 0;
}

int IsIdent(keyword t)
{
	if (t.kind == IDENT) return 1;
	else return 0;
}

int IsConst(keyword t)
{
	if (t.kind >= INT_CONST && t.kind <= CHAR_CONST)
		return 1;
	return 0;
}

int FreeNode(VarListNode* root)
{
	if (root == NULL)
		return 1;
	VarListNode* next = root->vl;
	if (!FreeNode(next))
		return 0;
	free(root);
	return 1;
}
int OpInitiate(OpStack* op)
{
	op->head = NULL;
	op->op = 0;
	return 0;
}
int OpPush(int to, OpStack** pnow)
{
	OpStack* t = (OpStack*)malloc(sizeof(OpStack));
	t->op = to;
	t->head = (*pnow);
	(*pnow) = t;
	return 0;
}
int OpPop(OpStack** pnow)
{
	if ((*pnow) == NULL)
		return 0;
	if ((*pnow)->op == EXCLA)
		return 0;
	int res = (*pnow)->op;
	(*pnow) = (*pnow)->head;
	return res;
}
int NumInitiate(NumStack* num)
{
	(num)->num = NULL;
	(num)->head = NULL;
	return 0;
}
int NumPush(NumStack* to, NumStack** pnum)
{
	if ((*pnum)->head == NULL && (*pnum)->num == NULL)
		(*pnum) = to;
	else to->head = (*pnum);
	(*pnum) = to;
	return 0;
}
NumStack* NumPop(NumStack** pnum)
{
	if ((*pnum) == NULL)
		return NULL;
	NumStack *res = *pnum;
	res->head = NULL;
	(*pnum) = (*pnum)->head;
	return res;
}

//表达式结束符号endsym可以是分号，如表达式语句，可以是反小括号，作为条件时使用
Child* Expression(int EndChar)
{
	OpStack* op = (OpStack*)malloc(sizeof(OpStack));
	OpInitiate(op);
	NumStack* num = (NumStack*)malloc(sizeof(NumStack));
	NumInitiate(num);
	int error = 0;
	int flag = 0;
	OpPush(EXCLA, &op);
	//w = gettoken(fp);
	while ((w.kind != EXCLA || op->op != EXCLA )&& error == 0)
	{
		//w = gettoken(fp);
		if (w.kind == IDENT || IsConst(w))
		{
			NumStack* te = (NumStack*)malloc(sizeof(NumStack));
			te->head = NULL;
			Child* to = (Child*)malloc(sizeof(Child));
			strcpy(to->i, w.tokentext);
			to->l = to->r = NULL;
			to->op = 0;
			te->num = to;
			NumPush(te, &num);
		}
		else if (w.kind <= EXCLA && w.kind >= PLUS)
		{
			NumStack* t1, *t2;
			int t;
			Child* p = (Child*)malloc(sizeof(Child));
			switch (precede[op->op - PLUS][w.kind - PLUS])
			{
			case'<':OpPush(w.kind, &op);
				break;
			case'=':if (!OpPop(&op))error++;
				w = gettoken(fp);
				break;
			case'>':
				t1 = NumPop(&num);
				t2 = NumPop(&num);
				t = OpPop(&op);
				if (t1 == NULL)
					error++;
				if (t2 == NULL)
					error++;
				if (t == 0)
					error++;
				p->op = t;
				p->l = t1->num;
				p->r = t2->num;
				p->i[0] = '\0';
				t1->num = p;
				NumPush(t1, &num);
				break;
			default:
				if (w.kind == EndChar)
					w.kind = EXCLA;
				else error = 1;
			}
		}
		else if (w.kind == EndChar)
			w.kind = EXCLA;
		else error = 1;
	}
	if (num != NULL && num->head == NULL && op->op == 0 && op->head == NULL)
		return num->num;
	else return NULL;
}

VarListNode* VarList()//已经读入了第一个变量 done
{
	if (w.kind != IDENT)
	{
		err++;
		printf("line: %d error: not ident\n", w.line);
	}
	VarListNode* vl = (VarListNode*)malloc(sizeof(VarListNode));
	strcpy(vl->ident, w.tokentext);
	w = gettoken(fp);
	if (w.kind != COMMA && w.kind != SEMMI)
	{
		err++;
		printf("line: %d error: lack of comma or semmi\n",w.line);
		return NULL;
	}
	if (w.kind == SEMMI)
	{
		w = gettoken(fp);
		vl->vl = NULL;
		return vl;
	}
	w = gettoken(fp);
	vl->vl = VarList();
	return vl;
}
//调用此子程序时，外部变量类型和第一个变量名的单词已经读入，
//变量名保存在tokenText0中，这时外部变量定义的处理流程可参考如下。done
ExternVarDefNode* ExternVarDef()
{
	ExternVarDefNode* evd = (ExternVarDefNode*)malloc(sizeof(ExternVarDefNode));
	evd->kind = w.kind;
	evd->vl = VarList();
	return evd;
}

FormFactorListNode* FormFactorList()//已经读入了LP done
{
	w = gettoken(fp);
	if (w.kind != RP && !IsVarDeclare(w)) 
		return NULL;
	FormFactorListNode* ffl = (FormFactorListNode*)malloc(sizeof(FormFactorListNode));
	if (w.kind == RP)
	{
		ffl->ffl = (FormFactorListNode*)malloc(sizeof(FormFactorListNode));
		ffl->ffl->kind = 0;
		ffl->ffl->ffl = NULL;
		return ffl;
	}
	else
	{
		ffl->kind = w.kind;
		w = gettoken(fp);
		if (w.kind != IDENT)
		{
			err++;
			printf("line: %d error: not ident",w.line);
			free(ffl);
			return NULL;
		}
		strcpy(ffl->ident, w.tokentext);
	}
	w = gettoken(fp);
	if (w.kind == COMMA)
	{
		ffl->ffl = FormFactorList();
		if (ffl->ffl == NULL)
			return NULL;
		return ffl;
	}
	else if (w.kind == RP)
	{
		ffl->ffl = (FormFactorListNode*)malloc(sizeof(FormFactorListNode));
		ffl->ffl->kind = 0;
		ffl->ffl->ffl = NULL;
		return ffl;
	}
	else
	{
		err++;
		printf("line: %d error: wrong endchar",w.line);
		return NULL;
	}
	return ffl;
}

//调用此子程序时，语句的第一个单词已经读入，处理一条语句时，
//根据这条语句的第一个单词，确定处理什么类型的语句。 done
SentenceNode* Sentence()
{
	SentenceNode* s = (SentenceNode*)malloc(sizeof(SentenceNode));
	switch (w.kind)
	{
	case IF: //分析条件语句
		w = gettoken(fp);
		if (w.kind != LP)
			return NULL;
		s->e1 = Expression(RP);
		s->s1 = Sentence();
		w = gettoken(fp);
		if (w.kind == ELSE)
		{
			w = gettoken(fp);
			s->s2 = Sentence();
			s->kind = ELSE;
		}
		else s->kind = IF;
		break;
	case LP:
		s->kind = Expres;
		s->e1 = Expression(SEMMI);
		break;
	case IDENT:
	case INT_CONST:
	case FLOAT_CONST:
	case CHAR_CONST:
		s->kind = Expres;
		s->e1 = Expression(SEMMI);
		if (s->e1 != NULL)
		{
			w = gettoken(fp);
		}
		else return NULL;
		break;
	case RETURN:
		s->kind = RETURN;
		s->e1 = Expression(SEMMI);
		s->s1 = NULL;
		s->s2 = NULL;
		if (s->e1 == NULL)
			return NULL;
		break;
	default:
		return NULL;
	}
	return s;
}

SentenceListNode* SentenceList() //done
{
	SentenceNode* s = Sentence();
	if (s == NULL)
		return NULL;
	else
	{
		SentenceListNode* sl = (SentenceListNode*)malloc(sizeof(SentenceListNode));
		sl->s = s;
		sl->sl = SentenceList();
		w = gettoken(fp);
		return sl;
	}
}

LocalVarDefNode* LocalVarDef()
{
	LocalVarDefNode* lvd = (LocalVarDefNode*)malloc(sizeof(LocalVarDefNode));
	strcpy(lvd->TypeStatement, w.tokentext);
	w = gettoken(fp);
	if (w.kind != IDENT)
		return NULL;
	lvd->vln = VarList();
	if (IsVarDeclare(w))
		lvd->lvd = LocalVarDef();
	else
	{
		lvd->lvd = (LocalVarDefNode*)malloc(sizeof(LocalVarDefNode));
		lvd->lvd->TypeStatement[0] = '\0';
		lvd->lvd->lvd = lvd->lvd->vln = NULL;
	}
	return lvd;
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
	{
		err++;
		printf("line: %d error: endchar should be }\n", w.line);
		return NULL;
	}
	w = gettoken(fp);
	return c;
}

//调用此子程序时，函数返回值类型和函数名，正小括号的单词已经读入，函数名保存在tokenText0中 done
FunDefNode* FunDef(keyword copy)
{
	FunDefNode* fd = (FunDefNode*)malloc(sizeof(FunDefNode));
	strcpy(fd->name, copy.tokentext);
	fd->ffl = FormFactorList();
	if (fd->ffl == NULL)
		return NULL;
	w = gettoken(fp);
	if (w.kind == SEMMI) fd->c = NULL;
	else if (w.kind == LCURLY) fd->c = Compose();
	else
	{
		err++;
		printf("line: %d error: wrong endchar\n", w.line);
		return NULL;
	}
	return fd;
}

//此子程序完成一个外部定义的处理，调用此子程序时，已经读入了一个外部定义的第一个单词到w中。
//该子程序处理完后，刚好处理到外部定义的最后一个符号，后续单词还没读入。
ExternDefNode* ExternDef() //处理外部定义序列，正确时，返回子树根结点指针，否则返回NULL done
{
	if (!IsVarDeclare(w) && w.kind != EOF_)
	{
		err++;
		printf("line: %d  error: VarDeclare\n", w.line);
		return NULL;
	}
	ExternDefNode* edn = (ExternDefNode*)malloc(sizeof(ExternDefNode));
	edn->kind = w.kind;
	w = gettoken(fp);
	keyword wcopy = w;
	w = gettoken(fp);
	if (w.kind == LP)
	{
		edn->fd = FunDef(wcopy);
		edn->evd = NULL;
	}
	else
	{
		edn->evd = (VarListNode*)malloc(sizeof(VarListNode));
		strcpy(edn->evd->ident, wcopy.tokentext);
		edn->evd->vl = NULL;
		edn->fd = NULL;
		if (w.kind == COMMA)
		{
			w = gettoken(fp);
			edn->evd->vl = VarList();
		}
		else if (w.kind == SEMMI)
			w = gettoken(fp);
		else return NULL;
	}
	return edn;
}

//语法单位<外部定义序列>的定义：
//<外部定义序列>：： = <外部定义> <外部定义序列> | <外部定义>
//这是一个递归定义，该子程序处理一系列的外部定义，每个外部定义序列的结点，其第一个子树对应一个外部定义，第二棵子树对应后续的外部定义。
//在一个源程序中，每次成功处理完一个外部定义后，如果遇到文件结束标记，则语法分析结束。调用此子程序，已经读入了一个外部定义的第一个单词到w中。
ExternDefListNode* ExternDefList()// 外部定义序列 done
{
	if (w.kind == EOF_) return NULL;
	ExternDefListNode* root = (ExternDefListNode*)malloc(sizeof(ExternDefListNode));//生成一个外部定义序列结点root
	root->edn = ExternDef(); //处理一个外部定义，得到一棵子树，作为root的第一棵子树
	root->edln = ExternDefList(); //得到的子树，作为root的第二棵子树
	if (root->edn == NULL)
		return NULL;
	return root;
}

//对于一个程序，按其语法定义：	<程序> ：：=<外部定义序列>
//语法单位<程序>的子程序如下，完成的功能是生成一棵语法树，根指针指向的是一个外部定义序列的结点。
ExternDefListNode* GraAnalyse(FILE* fp_)
{
	fp = fp_;
	w = gettoken(fp);
	return ExternDefList();
}

int putvarlist(VarListNode* v)
{
	if (v == NULL)
		return -1;
	printf("%s", v->ident);
	v = v->vl;
	while (v != NULL && v->ident[0] != '\0')
	{
		printf(",%s", v->ident);
		v = v->vl;
	}
	return 0;
}
int putexp(Child* c, int blank)
{
	if (c == NULL)
		return 0;
	if (c->op <= UNEQUAL && c->op >= PLUS)
	{
		for (int b = 0; b < blank; b++) putchar(' ');
		printf("%s\n", type[c->op]);
		if(c->l!=NULL)
			putexp(c->l, blank + 1);
		else
		{
			printf("ERROR!\n");
			return -1;
		}
		if(c->r!=NULL) 
			putexp(c->r, blank + 1);
		else
		{
			printf("ERROR!\n");
			return -1;
		}
	}
	if (c->op == 0)
	{
		for (int b = 0; b < blank; b++) putchar(' ');
		printf("ID：%s\n",c->i);
	}
	return 0;
}
int putsen(SentenceNode* s, int blank)
{
	switch (s->kind)
	{
	case IF:
		for (int b = 0; b < blank; b++) putchar(' ');
		printf("IF:\n");
		putexp(s->e1, blank+1);
		for (int b = 0; b < blank; b++) putchar(' ');
		printf("THEN:\n");
		putsen(s->s1, blank+1);
		break;
	case ELSE:
		for (int b = 0; b < blank; b++) putchar(' ');
		printf("IF:\n");
		putexp(s->e1, blank + 1);
		for (int b = 0; b < blank; b++) putchar(' ');
		printf("THEN:\n");
		putsen(s->s1, blank + 1);
		for (int b = 0; b < blank; b++) putchar(' ');
		printf("ELSE:\n");
		putsen(s->s2, blank + 1);
		break;
	case RETURN:
		for (int b = 0; b < blank; b++) putchar(' ');
		printf("RETURN:\n");
		putexp(s->e1, blank + 1);
		break;
	case Expres:
		for (int b = 0; b < blank; b++) putchar(' ');
		printf("Expression:\n");
		putexp(s->e1, blank + 1);
		break;
	default:
		printf("ERROR!\n");
	}
	return 0;
}
int putcompose(ComposeNode* c)
{
	if (c->lv != NULL)
	{
		LocalVarDefNode* lvl = c->lv;
		while (lvl->TypeStatement[0] != '\0')
		{
			printf("  类型说明符：%s\n", lvl->TypeStatement);
			printf("  变量序列：");
			putvarlist(lvl->vln);
			putchar('\n');
			lvl = lvl->lvd;
		}
	}
	if (c->sl != NULL)
	{
		printf("  语句序列：\n");
		SentenceListNode* sl = c->sl;
		while (sl!=NULL && sl->s != NULL)
		{
			putsen(sl->s, 3);
			sl = sl->sl;
		}
	}
	return 0;
}
int output(ExternDefListNode* root)
{
	int blank = 0;
	if (root == NULL)
	{
		printf("ERROR!\n");
		return 0;
	}
	printf("外部变量定义：\n");
	ExternDefListNode* edl = root;
	while(edl!=NULL)
	{
		if(edl->edn->evd != NULL)
		{
			printf("外部变量定义：\n");
			printf(" 类型说明符：%s\n", type[edl->edn->kind]);
			printf(" 变量序列：");
			putvarlist(edl->edn->evd);//varlist output
			putchar('\n');
		}
		else if (edl->edn->fd != NULL)
		{
			printf("外部函数定义：\n");
			printf(" 类型说明符：%s\n", type[edl->edn->kind]);
			printf(" 函数名称：%s\n", edl->edn->fd->name);
			printf(" 形参序列：\n");
			if (edl->edn->fd->ffl->kind == 0)
				printf("  无形参\n");
			else
			{
				FormFactorListNode* ffl = edl->edn->fd->ffl;
				while (ffl->kind != 0)
				{
					printf("  形参类型：%s 形参名称：%s\n", type[ffl->kind], ffl->ident);
					ffl = ffl->ffl;
				}
			}
			printf(" 语句序列：\n");
			if(edl->edn->fd->c != NULL)
				putcompose(edl->edn->fd->c);
		}
		edl = edl->edln;
	}
	return 0;
}