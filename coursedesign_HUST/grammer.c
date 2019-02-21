#include "grammer.h"

keyword w;
FILE* fp;

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

//���ʽ��������endsym�����Ƿֺţ�����ʽ��䣬�����Ƿ�С���ţ���Ϊ����ʱʹ��
Child* Expression(int EndChar)
{
	OpStack* op = (OpStack*)malloc(sizeof(OpStack));
	OpInitiate(&op);
	NumStack* num = (NumStack*)malloc(sizeof(NumStack));
	NumInitiate(&num);
	OpInitiate(&op);
	OpPush(EXCLA, &op);
	int error = 0;
	while (w.kind != EXCLA && op->op != EXCLA && error == 0)
	{
		if (w.kind == IDENT || IsConst(w))
		{
			NumStack* te = (NumStack*)malloc(sizeof(NumStack));
			Child* to = (Child*)malloc(sizeof(Child));
			strcpy(to->i, w.tokentext);
			to->l = to->r = NULL;
			to->op = 0;
			te->num = to;
			NumPush(te, &num);
		}
		else if (w.kind <= 31 && w.kind >= 20)
		{
			NumStack* t1, *t2;
			int t;
			Child* p = (Child*)malloc(sizeof(Child));
			switch (precede[op->op][w.kind])
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
					w.kind == EXCLA;
				else error = 1;
			}
		}
		else if (w.kind == EndChar)
			w.kind = EXCLA;
		else error = 1;
	}
	if (num != NULL && num->head == NULL && op->op == EXCLA)
		return num->num;
	else return NULL;
}

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
int show(char* s)
{
	while (*s != '\0')
		printf("%c", s++);
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
int OpInitiate(OpStack** op)
{
	if (*op == NULL)
		return 0;
	OpInitiate(&((*op)->head));
	(*op)->head = NULL;
	(*op)->op = ERROR_TOKEN;
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
int NumInitiate(NumStack** num)
{
	if (*num == NULL)
		return 0;
	NumInitiate(&((*num)->head));
	(*num)->num = NULL;
	(*num)->head = NULL;
	return 0;
}
int NumPush(OpStack* to, NumStack** pnum)
{
	to->head = (*pnum);
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
	case IF: //�����������
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
	default:
		return NULL;
	}
	return s;
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
	return ExternDefList();
}