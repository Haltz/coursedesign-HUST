#include "lex.h"

char readchar(FILE* fp, int* pline);//���ڷ���Ķ�ȡ�ַ�����¼����
int findkey(char* st);//�ж��Ƿ�Ϊ�ؼ���

char key_word[100][30] =
{
	"int",
	"float",
	"char",
	"if",
	"else",
	"return"
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
				else if (ch <= '9'&&ch > '0')
				{
					do
					{
						ans.tokentext[j++] = ch;
					} while ((ch = readchar(fp, pline)) <= '9'&&ch >= '0');
					ans.kind = INT_CONST;
					ungetc(ch, fp);
				}
				else
				{
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
		ungetc(ch, fp);
		if ((ans.kind = findkey(ans.tokentext)) != 0)
		{
			switch (ans.kind)
			{
			case 2:strcpy(ans.tokentext, "int"); break;
			case 3:strcpy(ans.tokentext, "float"); break;
			case 4:strcpy(ans.tokentext, "char"); break;
			case 5:strcpy(ans.tokentext, "if"); break;
			case 6:strcpy(ans.tokentext, "else"); break;
			case 7:strcpy(ans.tokentext, "return"); break;
			}
			return ans;
		}//�ж��Ƿ��ǹؼ���
		else
		{
			//��ʾ��һ����ͨ�ı�ʶ��
			ans.kind = IDENT;
			return ans;
		}
	}
	else switch (ch)
	{
	case '(': ans.kind = LP; strcpy(ans.tokentext, "("); break;
	case ')': ans.kind = RP; strcpy(ans.tokentext, ")"); break;
	case '{': ans.kind = LCURLY; strcpy(ans.tokentext, "{"); break;
	case '}': ans.kind = RCURLY; strcpy(ans.tokentext, "}"); break;
	case '[': ans.kind = LSQUARE; strcpy(ans.tokentext, ch); break;
	case ']': ans.kind = RSQUARE; strcpy(ans.tokentext, ch); break;
	case '~': ans.kind = NEGATE; strcpy(ans.tokentext, ch); break;
	case '#': ans.kind = HASHTAG; strcpy(ans.tokentext, "#"); break;
	case '%': ans.kind = PERCENT; strcpy(ans.tokentext, ch); break;
	case '\\': ans.kind = NEGADIV; strcpy(ans.tokentext, ch); break;
	case '\'': ans.kind = SINGLE; strcpy(ans.tokentext, "'"); break;
	case '\"': ans.kind = SINGGLESINGLE; strcpy(ans.tokentext, ch); break;
	case ',': ans.kind = COMMA; strcpy(ans.tokentext, ","); break;
	case ';': ans.kind = SEMMI; strcpy(ans.tokentext, ";"); break;
	case '?': ans.kind = QUESTION; strcpy(ans.tokentext, ch); break;
	case '.': ans.kind = DOT; strcpy(ans.tokentext, ch); break;
	case '<': if ((ch = readchar(fp, pline)) == '<')
		if ((ch = readchar(fp, pline)) == '=')
		{
			ans.kind = LMOVEEQ;
			strcpy(ans.tokentext, "<<=");
		}
		else { ans.kind = LMOVE; strcpy(ans.tokentext, "<<"); ungetc(ch, fp); }
			  else if (ch == '=') {
				  ans.kind = LESSEQ; strcpy(ans.tokentext, "<=");
			  }
			  else { ans.kind = LESS; strcpy(ans.tokentext, "<"); ungetc(ch, fp); }
			  break;
	case '>': if ((ch = readchar(fp, pline)) == '>')
		if ((ch = readchar(fp, pline)) == '=') { ans.kind = RMOVEEQ; strcpy(ans.tokentext, ">>="); }
		else { ans.kind = RMOVE; strcpy(ans.tokentext, ">>"); ungetc(ch, fp); }
			  else if (ch == '=') { ans.kind = MOREEQ; strcpy(ans.tokentext, ">="); }
			  else { ans.kind = MORE; strcpy(ans.tokentext, ">"); ungetc(ch, fp); }
			  break;
	case '!': if ((ch = readchar(fp, pline)) == '=') { ans.kind = UNEQUAL; strcpy(ans.tokentext, "!="); }
			  else { ans.kind = EXCLA; strcpy(ans.tokentext, "!"); ungetc(ch, fp); }
			  break;
	case '^': if ((ch = readchar(fp, pline)) == '=') { ans.kind = XOREQ; strcpy(ans.tokentext, "^="); }
			  else { ans.kind = XOR; strcpy(ans.tokentext, "^"); ungetc(ch, fp); }
			  break;
	case '&': if ((ch = readchar(fp, pline)) == '&') { ans.kind = ANDAND; strcpy(ans.tokentext, "&&"); }
			  else if (ch == '=') { ans.kind = ANDEQ; strcpy(ans.tokentext, "&="); }
			  else { ans.kind = AND; strcpy(ans.tokentext, "&"); ungetc(ch, fp); }
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
	case '+': if ((ch = readchar(fp, pline)) == '+') { ans.kind = PLUSPLUS; strcpy(ans.tokentext, "++"); }
			  else if (ch == '=') {ans.kind = PLUSEQ; strcpy(ans.tokentext, "+=");}
			  else { ans.kind = PLUS; strcpy(ans.tokentext, "+"); ungetc(ch, fp); }
			  break;
	case '=': if ((ch = readchar(fp, pline)) == '=') { ans.kind = EQUAL; strcpy(ans.tokentext, "==");}
			  else { ans.kind = ASSIGN; strcpy(ans.tokentext, "="); ungetc(ch, fp); }
			  break;
	case '/': if ((ch = readchar(fp, pline)) == '/') { ans.kind = COMMENT; strcpy(ans.tokentext, "//");}
			  else if (ch == '=') { ans.kind = DIVEQ; strcpy(ans.tokentext, "/=");}
			  else { ans.kind = DIV; strcpy(ans.tokentext, "/"); ungetc(ch, fp); }
			  break;
	default: if (feof(fp)) ans.kind = EOF_;
			 else ans.kind = ERROR_TOKEN;
	}
	return ans;
}