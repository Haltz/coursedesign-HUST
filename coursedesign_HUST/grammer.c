#include "grammer.h"

keyword w;
FILE* fp;

int err = 0;

char precede[20][20] =
{
	//+   -   *   /   %    &&  ||  (   )   =   >   <  <=  >=   ==  !=  #
	{'>','>','<','<','<','>','>','<','>',' ','>','>','>','>','>','>','>'},//+
	{'>','>','<','<','<','>','>','<','>',' ','>','>','>','>','>','>','>'},//-
	{'>','>','>','>','>','>','>','<','>',' ','>','>','>','>','>','>','>'},//*
	{'>','>','>','>','>','>','>','<','>',' ','>','>','>','>','>','>','>'},///
	{'>','>','>','>','>','>','>','<','>',' ','>','>','>','>','>','>','>'},//%
	{'<','<','<','<','<','>','>','<','>',' ','<','<','<','<','<','<','>'},//&&
	{'<','<','<','<','<','<','>','<','>',' ','<','<','<','<','<','<','>'},//||
	{'<','<','<','<','<','<','<','<','=',' ','<','<','<','<','<','<','>'},//(
	{'>','>','>','>','>','>','>','>','>',' ','>','>','>','>','>','>','>'},//)
    {'<','<','<','<','<','>','>','<',' ','<','<','<','<','<','<','<','>'},//=
    {'<','<','<','<','<','>','>','<','>',' ','>','>','>','>','>','>','>'},//>
    {'<','<','<','<','<','>','>','<','>',' ','>','>','>','>','>','>','>'},//<
    {'<','<','<','<','<','>','>','<','>',' ','>','>','>','>','>','>','>'},//<=
    {'<','<','<','<','<','>','>','<','>',' ','>','>','>','>','>','>','>'},//>=
    {'<','<','<','<','<','>','>','<','>',' ','<','<','<','<','>','>','>'},//==
    {'<','<','<','<','<','>','>','<','>',' ','<','<','<','<','>','>','>'},//!=
    {'<','<','<','<','<','<','<','<','<',' ','<','<','<','<','<','<','='},//#
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

ComposeNode* Compose();

int IsVarDeclare(keyword t) //�Ƿ������������Ĺؼ���
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
	if ((*pnum) == NULL)
	{
		(*pnum) = to;
		return 0;
	}
	to->head = (*pnum);
	(*pnum) = to;
	return 0;
}
NumStack* NumPop(NumStack** pnum)
{
	if ((*pnum) == NULL)
		return NULL;
	NumStack *res = *pnum;
	(*pnum) = (*pnum)->head;
	res->head = NULL;
	return res;
}

//���ʽ��������endsym�����Ƿֺţ�����ʽ��䣬�����Ƿ�С���ţ���Ϊ����ʱʹ��
Child* Expression(int EndChar, int tot) // if it's function use, tot = 1, else tot = 0
{
	int l = 0, r = 0;
	OpStack* op = (OpStack*)malloc(sizeof(OpStack));
	OpInitiate(op);
	NumStack* num = (NumStack*)malloc(sizeof(NumStack));
	NumInitiate(num);
	int error = 0;
	int flag = 0;
	OpPush(EXCLA, &op);
	while ((w.kind != EXCLA || op->op != EXCLA ) && error == 0)
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
			w = gettoken(fp);
			flag++;
			if (flag == 1 && w.kind == LP)
			{
				w = gettoken(fp);
				Child* f = (Child*)malloc(sizeof(Child));
				strcpy(f->i, to->i);
				f->op = FunUse;
				Child* fcopy = f;
				int count = 0;
				do
				{
					if (count != 0)
						w = gettoken(fp);
					fcopy->l = Expression(RP, 1);
					if (fcopy != f)
						fcopy->op = ERROR_TOKEN;
					if(fcopy->l != NULL)
						fcopy->r = (Child*)malloc(sizeof(Child));
					else
					{
						fcopy->i[0] = '\0';
						fcopy->r = NULL;
						fcopy->op = ERROR_TOKEN;
						break;
					}
					fcopy = fcopy->r;
					count++;
				} while (w.kind == RP);
				//w = gettoken(fp);
				return f;
			}
		}
		else if (w.kind <= EXCLA && w.kind >= PLUS && w.kind != EndChar)
		{
			NumStack* t1, *t2;
			int t;
			Child* p = (Child*)malloc(sizeof(Child));
			switch (precede[op->op - PLUS][w.kind - PLUS])
			{
			case'<':OpPush(w.kind, &op);
				w = gettoken(fp);
				break;
			case'=':
				if (!OpPop(&op))
					error++;
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
				p->r = t1->num;
				p->l = t2->num;
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
		else if (w.kind == EndChar || (w.kind == COMMA && tot == 1))
			w.kind = EXCLA;
		else error = 1;
	}
	if (num->num != NULL && num->head->head == NULL && num->head->num == NULL && op->op == EXCLA && op->head->head == NULL)
	{
		w.kind = EndChar;
		return num->num;
	}
	else
	{
		w.kind = EndChar;
		return NULL;
	}
}

VarListNode* VarList()//�Ѿ������˵�һ������ done
{
	if (w.kind != IDENT)
	{
		err++;
		printf("line: %d error: not ident\n", w.line);
	}
	VarListNode* vl = (VarListNode*)malloc(sizeof(VarListNode));
	strcpy(vl->ident, w.tokentext);
	w = gettoken(fp);
	if (w.kind != COMMA && w.kind != SEMMI && w.kind != RP)
	{
		err++;
		printf("line: %d error: lack of comma or semmi or RP\n",w.line);
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
//���ô��ӳ���ʱ���ⲿ�������ͺ͵�һ���������ĵ����Ѿ����룬
//������������tokenText0�У���ʱ�ⲿ��������Ĵ������̿ɲο����¡�done
ExternVarDefNode* ExternVarDef()
{
	ExternVarDefNode* evd = (ExternVarDefNode*)malloc(sizeof(ExternVarDefNode));
	evd->kind = w.kind;
	evd->vl = VarList();
	return evd;
}

FormFactorListNode* FormFactorList()//�Ѿ�������LP done
{
	w = gettoken(fp);
	if (w.kind != RP && !IsVarDeclare(w)) 
		return NULL;
	FormFactorListNode* ffl = (FormFactorListNode*)malloc(sizeof(FormFactorListNode));
	if (w.kind == RP)
	{
		free(ffl);
		return NULL;
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

//���ô��ӳ���ʱ�����ĵ�һ�������Ѿ����룬����һ�����ʱ��
//�����������ĵ�һ�����ʣ�ȷ������ʲô���͵���䡣 done
SentenceNode* Sentence()
{
	SentenceNode* s = (SentenceNode*)malloc(sizeof(SentenceNode));
	switch (w.kind)
	{
	case IF: //�����������
		w = gettoken(fp);
		if (w.kind != LP)
			return NULL;
		w = gettoken(fp);
		s->e1 = Expression(RP, 0);
		w = gettoken(fp);
		if (w.kind != LCURLY)
		{
			s->s1 = Sentence();
			s->c1 = NULL;
		}
		else
		{
			s->s1 = NULL;
			s->c1 = Compose();
			w = gettoken(fp);
		}
		if (w.kind == ELSE)
		{
			w = gettoken(fp);
			if (w.kind == LCURLY)
			{
				s->c2 = Compose();
				s->s2 = NULL;
				w = gettoken(fp);
			}
			else
			{
				s->s2 = Sentence();
				s->c2 = NULL;
			}
			s->kind = ELSE;
		}
		else s->kind = IF;
		break;
	case LP:
		s->kind = Expres;
		s->e1 = Expression(SEMMI, 0);
		w = gettoken(fp);
		break;
	case IDENT:
	case INT_CONST:
	case FLOAT_CONST:
	case CHAR_CONST:
		s->kind = Expres;
		s->e1 = Expression(SEMMI, 0);
		s->s1 = s->s2 = NULL;
		if(s->e1 == NULL)
			return NULL;
		w = gettoken(fp);
		break;
	case RETURN:
		s->kind = RETURN;
		w = gettoken(fp);
		s->e1 = Expression(SEMMI, 0);
		s->s1 = NULL;
		s->s2 = NULL;
		if (s->e1 == NULL)
			return NULL;
		w = gettoken(fp);
		break;
	case WHILE:
		s->kind = WHILE;
		w = gettoken(fp);
		if (w.kind != LP)
		{
			printf("error line %d\n", w.line);
			w = gettoken(fp);
			return NULL;
		}
		w = gettoken(fp);
		s->e1 = Expression(RP, 0);
		w = gettoken(fp);
		if (w.kind != LCURLY)
		{
			s->c1 = NULL;
			s->c2 = NULL;
			s->s2 = NULL;
			s->s1 = Sentence();
		}
		else
		{
			s->s1 = NULL;
			s->s2 = NULL;
			s->c2 = NULL;
			s->c1 = Compose();
		}
		w = gettoken(fp);
		break;
	case FOR:
		s->kind = FOR;
		w = gettoken(fp);
		if (w.kind != LP)
		{
			printf("error line: %d\n", w.line);
			return NULL;
		}
		w = gettoken(fp);
		s->e1 = Expression(SEMMI, 0);
		w = gettoken(fp);
		s->e2 = Expression(SEMMI, 0);
		w = gettoken(fp);
		s->e3 = Expression(RP, 0);
		w = gettoken(fp);
		if (w.kind == LCURLY)
		{
			s->s1 = s->s2 = NULL;
			s->c2 = NULL;
			s->c1 = Compose();
		}
		else
		{
			s->s1 = Sentence();
			s->s2 = NULL;
			s->c1 = s->c2 = NULL;
		}
		w = gettoken(fp);
		break;
	case BREAK:
		s->c1 = s->c2 = NULL;
		s->s1 = s->s2 = NULL;
		s->e1 = s->e2 = s->e3 = NULL;
		s->kind = BREAK;
		w = gettoken(fp);// get ;
		w = gettoken(fp);
		break;
	case CONTINUE:
		s->c1 = s->c2 = NULL;
		s->s1 = s->s2 = NULL;
		s->e1 = s->e2 = s->e3 = NULL;
		s->kind = CONTINUE;
		w = gettoken(fp);// get ;
		w = gettoken(fp);
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
	return c;
}

//���ô��ӳ���ʱ����������ֵ���ͺͺ���������С���ŵĵ����Ѿ����룬������������tokenText0�� done
FunDefNode* FunDef(keyword copy)
{
	FunDefNode* fd = (FunDefNode*)malloc(sizeof(FunDefNode));
	strcpy(fd->name, copy.tokentext);
	fd->ffl = FormFactorList();
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

//���ӳ������һ���ⲿ����Ĵ������ô��ӳ���ʱ���Ѿ�������һ���ⲿ����ĵ�һ�����ʵ�w�С�
//���ӳ�������󣬸պô����ⲿ��������һ�����ţ��������ʻ�û���롣
ExternDefNode* ExternDef() //�����ⲿ�������У���ȷʱ���������������ָ�룬���򷵻�NULL done
{
	ExternDefNode* edn = (ExternDefNode*)malloc(sizeof(ExternDefNode));
	if (w.kind == EXCLA)
	{
		edn->di = (Def_includeNode*)malloc(sizeof(Def_includeNode));
		Def_includeNode* a = edn->di;
		edn->evd = NULL;
		edn->fd = NULL;
		edn->kind = ERROR_TOKEN;
		w = gettoken(fp);
		if (w.kind == INCLUDE)
		{
			a->kind = INCLUDE;
			w = gettoken(fp);
			int temp = w.kind;
			if (w.kind != LESS && w.kind != SINGGLESINGLE)
			{
				printf("error line: %d\n", w.line);
				free(a);
				w = gettoken(fp);
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
					printf("error line: %d\n", w.line);
					w = gettoken(fp);
					return NULL;
				}
				w = gettoken(fp);
			}
			if(!((w.kind == SINGGLESINGLE && temp == SINGGLESINGLE)||(w.kind == MORE && w.kind == LESS)))
			{
				printf("error line: %d\n", w.line);
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
				printf("error line: %d\n", w.line);
				free(edn);
				w = gettoken(fp);
				return NULL;
			}
			strcpy(a->ident, w.tokentext);
			w = gettoken(fp);
			a->val = Expression(SEMMI, 0);
			if (a->val == NULL)
			{
				printf("error line: %d\n", w.line);
				free(edn);
				w = gettoken(fp);
				return NULL;
			}
			w = gettoken(fp);
		}
		return edn;
	}
	if (!IsVarDeclare(w) && w.kind != EOF_)
	{
		printf("error line: %d\n", w.line);
		w = gettoken(fp);
		return NULL;
	}
	edn->kind = w.kind;
	w = gettoken(fp);
	keyword wcopy = w;
	w = gettoken(fp);
	if (w.kind == LP)
	{
		edn->fd = FunDef(wcopy);
		w = gettoken(fp);
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

//�﷨��λ<�ⲿ��������>�Ķ��壺
//<�ⲿ��������>���� = <�ⲿ����> <�ⲿ��������> | <�ⲿ����>
//����һ���ݹ鶨�壬���ӳ�����һϵ�е��ⲿ���壬ÿ���ⲿ�������еĽ�㣬���һ��������Ӧһ���ⲿ���壬�ڶ���������Ӧ�������ⲿ���塣
//��һ��Դ�����У�ÿ�γɹ�������һ���ⲿ�������������ļ�������ǣ����﷨�������������ô��ӳ����Ѿ�������һ���ⲿ����ĵ�һ�����ʵ�w�С�
ExternDefListNode* ExternDefList()// �ⲿ�������� done
{
	if (w.kind == EOF_) return NULL;
	ExternDefListNode* root = (ExternDefListNode*)malloc(sizeof(ExternDefListNode));//����һ���ⲿ�������н��root
	root->edn = ExternDef(); //����һ���ⲿ���壬�õ�һ����������Ϊroot�ĵ�һ������
	root->edln = ExternDefList(); //�õ�����������Ϊroot�ĵڶ�������
	if (root->edn == NULL)
		return NULL;
	return root;
}

//����һ�����򣬰����﷨���壺	<����> ����=<�ⲿ��������>
//�﷨��λ<����>���ӳ������£���ɵĹ���������һ���﷨������ָ��ָ�����һ���ⲿ�������еĽ�㡣
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
	if (c->op == FunUse)
	{
		for (int b = 0; b < blank; b++)
			putchar(' ');
		printf("��������: %s\n", c->i);
		for (int b = 0; b < blank; b++)
			putchar(' ');
		printf("ʵ��:\n");
		putexp(c->l, blank + 2);
		putexp(c->r->l, blank + 2);
	}
	if (c->op <= UNEQUAL && c->op >= PLUS)
	{
		for (int b = 0; b < blank; b++) putchar(' ');
		printf("%s\n", type[c->op]);
		if(c->l != NULL)
			putexp(c->l, blank + 2);
		else
		{
			printf("ERROR!\n");
			return -1;
		}
		if(c->r!=NULL) 
			putexp(c->r, blank + 2);
		else
		{
			printf("ERROR!\n");
			return -1;
		}
	}
	if (c->op == 0)
	{
		for (int b = 0; b < blank; b++) putchar(' ');
		printf("ID��%s\n",c->i);
	}
	return 0;
}
int putsen(SentenceNode* s, int blank)
{
	switch (s->kind)
	{
	case IF:
		for (int i = 0; i < blank; i++)
			putchar(' ');
		printf("IF����\n");
		for (int i = 0; i < blank + 2; i++)
			putchar(' ');
		printf("IF�������ʽ:\n");
		putexp(s->e1, blank + 2);
		for (int i = 0; i < blank + 2; i++)
			putchar(' ');
		printf("THEN:\n");
		if(s->s1 != NULL)
		putsen(s->s1, blank + 2);
		if (s->c1 != NULL)
			putcompose(s->c1, blank + 2);
		break;
	case ELSE:
		for (int b = 0; b < blank; b++) putchar(' ');
		printf("IF-ELSE����:\n");
		for (int b = 0; b < blank + 2; b++) putchar(' ');
		printf("IF�������ʽ\n");
		putexp(s->e1, blank + 2);
		for (int b = 0; b < blank + 2; b++) putchar(' ');
		printf("THEN:\n");
		putsen(s->s1, blank + 2);
		for (int b = 0; b < blank + 2; b++) putchar(' ');
		printf("ELSE:\n");
		putsen(s->s2, blank + 2);
		break;
	case RETURN:
		for (int b = 0; b < blank; b++) putchar(' ');
		printf("RETURN����:\n");
		for (int b = 0; b < blank + 2; b++) putchar(' ');
		printf("���ر��ʽ:\n");
		putexp(s->e1, blank + 4);
		break;
	case Expres:
		for (int b = 0; b < blank; b++) putchar(' ');
		printf("���ʽ:\n");
		putexp(s->e1, blank + 2);
		break;
	case WHILE:
		for (int b = 0; b < blank; b++) putchar(' ');
		printf("WHILEѭ�����:\n");
		for (int b = 0; b < blank + 2; b++) putchar(' ');
		printf("WHILEѭ���������ʽ:\n");
		putexp(s->e1, blank + 4);
		for (int b = 0; b < blank + 2; b++) putchar(' ');
		printf("WHILEѭ����:\n");
		if (s->c1 != NULL)
			putcompose(s->c1, blank + 4);
		if (s->s1 != NULL)
			putsen(s->s1, blank + 4);
		break;
	case FOR:
		for (int b = 0; b < blank; b++) putchar(' ');
		printf("FORѭ�����\n");
		for (int b = 0; b < blank + 2; b++) putchar(' ');
		printf("��ʼ��:\n");
		putexp(s->e1, blank + 4);
		for (int b = 0; b < blank + 2; b++) putchar(' ');
		printf("�������ʽ:\n");
		putexp(s->e2, blank + 4);
		for (int b = 0; b < blank + 2; b++) putchar(' ');
		printf("�仯:\n");
		putexp(s->e3, blank + 4);
		for (int b = 0; b < blank + 2; b++) putchar(' ');
		printf("FORѭ��ѭ����:\n");
		if (s->s1 != NULL)
			putsen(s->s1, blank + 4);
		if (s->c1 != NULL)
			putcompose(s->c1, blank + 4);
		break;
	case BREAK:
		for (int b = 0; b < blank; b++) putchar(' ');
		printf("BREAK���\n");
		break;
	case CONTINUE:
		for (int b = 0; b < blank; b++) putchar(' ');
		printf("CONTINUE���\n");
		break;
	default:
		printf("ERROR!\n");
	}
	return 0;
}
int putcompose(ComposeNode* c, int blank)
{
	if (c->lv != NULL)
	{
		LocalVarDefNode* lvl = c->lv;
		while (lvl->TypeStatement[0] != '\0')
		{
			for (int b = 0; b < blank; b++)
				putchar(' ');
			printf("�ֲ�������������");
			for (int b = 0; b < blank; b++)
				putchar(' ');
			printf("����˵������%s\n", lvl->TypeStatement);
			for (int b = 0; b < blank; b++)
				putchar(' ');
			printf("�������У�");
			putvarlist(lvl->vln);
			putchar('\n');
			lvl = lvl->lvd;
		}
	}
	if (c->sl != NULL)
	{
		for (int b = 0; b < blank; b++)
			putchar(' ');
		printf("������У�\n");
		SentenceListNode* sl = c->sl;
		while (sl != NULL && sl->s != NULL)
		{
			putsen(sl->s, blank + 2);
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
		printf("NO WORD!\n");
		return 0;
	}
	ExternDefListNode* edl = root;
	while(edl!=NULL)
	{
		if(edl->edn->evd != NULL)
		{
			printf("�ⲿ�������壺\n");
			printf("  ����˵������%s\n", type[edl->edn->kind]);
			printf("  �������У�");
			putvarlist(edl->edn->evd);//varlist output
			putchar('\n');
		}
		else if (edl->edn->fd != NULL)
		{
			printf("�ⲿ�������壺\n");
			printf("  ����˵������%s\n", type[edl->edn->kind]);
			printf("  �������ƣ�%s\n", edl->edn->fd->name);
			printf("  �β����У�");
			if (edl->edn->fd->ffl == NULL)
				printf("���β�\n");
			else
			{
				FormFactorListNode* ffl = edl->edn->fd->ffl;
				while (ffl->kind != 0)
				{
					printf("\n    �β����ͣ�%s �β����ƣ�%s\n", type[ffl->kind], ffl->ident);
					ffl = ffl->ffl;
				}
			}
			printf("  ������䣺\n");
			if(edl->edn->fd->c != NULL)
				putcompose(edl->edn->fd->c, 4);
		}
		else if (edl->edn->di != NULL)
		{
			Def_includeNode* t = edl->edn->di;
			if (t->kind == DEFINE)
			{
				printf("#define���:\n");
				printf("��ʶ��:%s\n", t->ident);
				printf("ֵ:\n");
				putexp(t->val, 2);
			}
			if (t->kind == INCLUDE)
			{
				printf("#include���:\n");
				printf("�ļ���:%s\n", t->ident);
			}
		}
		edl = edl->edln;
	}
	return 0;
}