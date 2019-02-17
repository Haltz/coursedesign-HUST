#include "lex.h"

char readchar(FILE* fp, int* pline);//���ڷ���Ķ�ȡ�ַ�����¼����
int findkey(char* st);//�ж��Ƿ�Ϊ�ؼ���

char key_word[100][30] =
{
	"auto", "break", "bool", "case", "char", "continue", "default", "do",
	"double", "else", "enum", "extern", "float", "for", "goto", "if",
	"int",  "register", "return", "short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

keyword ans;
static int line = 0;
static int* pline = &line;

char readchar(FILE* fp, int* pline)
{
	char ch = fgetc(fp);
	if (ch == '\n') (*pline) ++;
	return ch;
}

int findkey(char* st)
{
	int dif = 0;
	for (int i = 0; i < 31; i++)  // �˴�31��ֱ������key_word�ĳ��ȣ�ʵ��Ӧ��ʹ�ú�������
		if ((dif = strcmp(st, key_word[i])) == 0)
			return i + 2;
	return 0;
}

keyword gettoken(FILE* fp)
{
	int flag = 0;
	for (int i = 0; i < 20; i++)
		ans.tokentext[i] = '\0';
	char ch;
	//�������ַ�
	while ((ch = readchar(fp, pline)) == ' ' || ch == '\n' || ch == '\t');
	//���ִ� �˴�ʮ�����ƺͰ˽���ֻ֧������
	if (ch <= '9'&&ch >= '0')
	{
		int j = 0;
		ans.tokentext[j++] = ch;
		if (ch == '0')
		{
			ch = readchar(fp, pline);
			//ʮ������
			if (ch == 'x')
			{
				ans.tokentext[j++] = ch;
				ch = readchar(fp, pline);
				//����
				if (ch == '0') { ans.kind = ERROR_TOKEN; ungetc(ch, fp); }
				//δ����
				else
				{
					do
					{
						ans.tokentext[j++] = ch;
					} while ((ch = readchar(fp, pline)) <= '9'&&ch >= '0');
					ans.kind = INT_CONST;
					ungetc(ch, fp);
				}
			}
			//�˽���
			else
			{
				//����
				if (ch == '0')
				{
					ans.kind = ERROR_TOKEN;
					ungetc(ch, fp);
				}
				//δ����
				else
				{
					do
					{
						ans.tokentext[j++] = ch;
					} while ((ch = readchar(fp, pline)) <= '9'&&ch >= '0');
					ans.kind = INT_CONST;
					ungetc(ch, fp);
				}
			}
		}
		//ʮ����
		else
		{
			int fl = 0;//�ж���С����������
			do
			{
				ans.tokentext[j++] = ch;
				if (ch == '.') fl = 1;
			} while ((ch = readchar(fp, pline)) == '.' || (ch <= '9'&&ch >= '0'));
			if (fl == 0) ans.kind = INT_CONST;
			else ans.kind = FLOAT_CONST;
			ungetc(ch, fp);
		}
	}
	//�ַ���
	else if ((ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A')|| ch == '_') //������ַ�
	{
		int j = 0;
		do 
		{
			ans.tokentext[j++] = ch;
		} while ((ch = readchar(fp, pline)) && ((ch <= 'z'&&ch >= 'a') || (ch <= 'Z'&&ch >= 'A') || ch == '_' || (ch <= '9'&&ch >= '0')));
		if ((ans.kind = findkey(ans.tokentext)) != 0) return ans; //�ж��Ƿ��ǹؼ���
		else
		{
			//��ʾ��һ����ͨ�ı�ʶ��
			ans.kind = IDENT;
			return ans;
		}
	}
	else switch (ch)
	{
	case '(': ans.kind = LP; break;
	case ')': ans.kind = RP; break;
	case '{': ans.kind = LCURLY; break;
	case '}': ans.kind = RCURLY; break;
	case '[': ans.kind = LSQUARE; break;
	case ']': ans.kind = RSQUARE; break;
	case '~': ans.kind = NEGATE; break;
	case '#': ans.kind = HASHTAG; break;
	case '%': ans.kind = PERCENT; break;
	case '\\': ans.kind = NEGADIV; break;
	case '\'': ans.kind = SINGLE; break;
	case '\"': ans.kind = SINGGLESINGLE; break;
	case ',': ans.kind = COMMA; break;
	case ';': ans.kind = SEMMI; break;
	case '?': ans.kind = QUESTION; break;
	case '.': ans.kind = DOT; break;
	case '<': if ((ch = readchar(fp, pline)) == '<')
		if ((ch = readchar(fp, pline)) == '=')ans.kind = LMOVEEQ;
		else { ans.kind = LMOVE; ungetc(ch,fp); }
			  else if (ch == '=') ans.kind = LESSEQ;
			       else { ans.kind = LESS; ungetc(ch, fp); }
			  break;
	case '>': if ((ch = readchar(fp, pline)) == '>')
		if ((ch = readchar(fp, pline)) == '=')ans.kind = RMOVEEQ;
		else { ans.kind = RMOVE; ungetc(ch, fp); }
			  else if (ch == '=') ans.kind = MOREEQ;
			  else { ans.kind = MORE; ungetc(ch, fp); }
			  break;
	case '!': if ((ch = readchar(fp, pline)) == '=') ans.kind = UNEQUAL;
			  else { ans.kind = EXCLA; ungetc(ch, fp); }
			  break;
	case '^': if ((ch = readchar(fp, pline)) == '=') ans.kind = XOREQ;
			  else { ans.kind = XOR; ungetc(ch, fp); }
			  break;
	case '&': if ((ch = readchar(fp, pline)) == '&') ans.kind = ANDAND;
			  else if (ch == '=') ans.kind = ANDEQ;
			  else { ans.kind = AND; ungetc(ch, fp); }
			  break;
	case '|': if ((ch = readchar(fp, pline)) == '|') ans.kind = OROR;
			  else if (ch == '=') ans.kind = OREQ;
			  else { ans.kind = OR; ungetc(ch, fp); }
			  break;
	case '*': if ((ch = readchar(fp, pline)) == '=') ans.kind = MULEQ;
			  else { ans.kind = MUL; ungetc(ch, fp); }
			  break;
	case '-': if ((ch = readchar(fp, pline)) == '-') ans.kind = MINUSMINUS;
			  else if (ch == '=') ans.kind = MINUSEQ;
			  else { ans.kind = MINUS; ungetc(ch, fp); }
			  break;
	case '+': if ((ch = readchar(fp, pline)) == '+') ans.kind = PLUSPLUS;
			  else if (ch == '=') ans.kind = PLUSEQ;
			  else { ans.kind = PLUS; ungetc(ch, fp); }
			  break;
	case '=': if ((ch = readchar(fp, pline)) == '=') ans.kind = EQUAL;
			  else { ans.kind = ASSIGN; ungetc(ch, fp); }
			  break;
	case '/': if ((ch = readchar(fp, pline)) == '/') ans.kind = COMMENT;
			  else if (ch == '=') ans.kind = DIVEQ;
			  else { ans.kind = DIV; ungetc(ch, fp); }
			  break;
	default: if (feof(fp)) ans.kind = EOF_;
			 else ans.kind = ERROR_TOKEN;
	}
	return ans;
}