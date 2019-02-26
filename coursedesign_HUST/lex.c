#include "lex.h"

char readchar(FILE *fp); //���ڷ���Ķ�ȡ�ַ�����¼����
int findkey(char *st);   //�ж��Ƿ�Ϊ�ؼ���

char key_word[][30] =
	{
		"int",
		"float",
		"char",
		"void",
		"long",
		"short",
		"bool",
		"double",
		"if",
		"else",
		"return",
		"while",
		"for",
		"continue",
		"break",
		"define",
		"include"};

keyword ans;
int line = 1;

char readchar(FILE *fp)
{
	char ch = fgetc(fp);
	if (ch == '\n')
		line++;
	return ch;
}

int findkey(char *st)
{
	int dif = 0;
	for (int i = 0; i < 31; i++) // �˴�31��ֱ������key_word�ĳ��ȣ�ʵ��Ӧ��ʹ�ú�������
		if ((dif = strcmp(st, key_word[i])) == 0)
			return i + 2;
	return 0;
}

keyword gettoken(FILE *fp)
{
	int flag = 0;
	for (int i = 0; i < 20; i++)
		ans.tokentext[i] = '\0';
	char ch;
	//�������ַ�
	while ((ch = readchar(fp)) == ' ' || ch == '\n' || ch == '\t')
		;
	//���ִ� �˴�ʮ�����ƺͰ˽���ֻ֧������
	if (ch <= '9' && ch >= '0')
	{
		int j = 0;
		if (ch == '0')
		{
			ans.tokentext[j++] = ch;
			ch = readchar(fp);
			//ʮ������
			if (ch == 'x')
			{
				ans.tokentext[j++] = ch;
				ch = readchar(fp);
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
					} while ((ch = readchar(fp)) <= '9' && ch >= '0');
					ans.kind = INT_CONST;
					ungetc(ch, fp);
				}
			}
			//�˽���
			else
			{
				//����
				if (ch == '0' || (ch <= '9'&&ch >= '8'))
				{
					ans.kind = ERROR_TOKEN;
					ungetc(ch, fp);
				}
				//δ����
				else if (ch <= '7' && ch > '0')
				{
					do
					{
						ans.tokentext[j++] = ch;
					} while ((ch = readchar(fp)) <= '7' && ch >= '0');
					ans.kind = INT_CONST;
					if (ch == '8' || ch == '9')
						ans.kind = ERROR_TOKEN;
					else ungetc(ch, fp);
				}
				else
				{
					ans.kind = INT_CONST;
					ans.tokentext[j] = '0';
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
				if (ch == '.') fl++;
			} while ((ch = readchar(fp)) == '.' || (ch <= '9'&&ch >= '0'));
			if (fl == 0) ans.kind = INT_CONST;
			else if(fl == 1) ans.kind = FLOAT_CONST;
			if (ch == 'L' && ans.kind == INT_CONST)
				ans.kind = LONG_CONST;
			else if(ch=='S' &&  ans.kind == INT_CONST)
				ans.kind = SHORT_CONST;
			else ungetc(ch, fp);
		}
	}
	//�ַ���
	else if ((ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A') || ch == '_') //������ַ�
	{
		int j = 0;
		do
		{
			ans.tokentext[j++] = ch;
		} while ((ch = readchar(fp)) && ((ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A') || ch == '_' || (ch <= '9' && ch >= '0')));
		if (ch == '\n')
			line--;
		ungetc(ch, fp);
		if ((ans.kind = findkey(ans.tokentext)) != 0)
		{
			strcpy(ans.tokentext, key_word[ans.kind - 2]);
			ans.line = line;
			return ans;
		} //�ж��Ƿ��ǹؼ���
		else
		{
			//��ʾ��һ����ͨ�ı�ʶ��
			ans.kind = IDENT;
			ans.line = line;
			return ans;
		}
	}
	else
		switch (ch)
		{
		case '(':
			ans.kind = LP;
			strcpy(ans.tokentext, "(");
			break;
		case ')':
			ans.kind = RP;
			strcpy(ans.tokentext, ")");
			break;
		case '{':
			ans.kind = LCURLY;
			strcpy(ans.tokentext, "{");
			break;
		case '}':
			ans.kind = RCURLY;
			strcpy(ans.tokentext, "}");
			break;
		case '[':
			ans.kind = LSQUARE;
			strcpy(ans.tokentext, "[");
			break;
		case ']':
			ans.kind = RSQUARE;
			strcpy(ans.tokentext, "]");
			break;
		case '~':
			ans.kind = NEGATE;
			strcpy(ans.tokentext, "~");
			break;
		case '#':
			ans.kind = EXCLA;
			strcpy(ans.tokentext, "#");
			break;
		case '%':
			ans.kind = PERCENT;
			strcpy(ans.tokentext, "%");
			break;
		case '\\':
			ans.kind = NEGADIV;
			strcpy(ans.tokentext, "\\");
			break;
		case '\'':
			ch = fgetc(fp);
			if (ch == '\\')
				ch = fgetc(fp);
			ans.tokentext[0] = ch;
			ans.tokentext[1] = '\0';
			ch = fgetc(fp);
			if (ch != '\'')
				ans.kind = ERROR_TOKEN;
			else
				ans.kind = CHAR_CONST;
			break;
		case '\"':
			ans.kind = SINGGLESINGLE;
			strcpy(ans.tokentext, "\"");
			break;
		case ',':
			ans.kind = COMMA;
			strcpy(ans.tokentext, ",");
			break;
		case ';':
			ans.kind = SEMMI;
			strcpy(ans.tokentext, ";");
			break;
		case '?':
			ans.kind = QUESTION;
			strcpy(ans.tokentext, "?");
			break;
		case '.':
			ans.kind = DOT;
			strcpy(ans.tokentext, ".");
			break;
		case '<':
			if ((ch = readchar(fp)) == '<')
				if ((ch = readchar(fp)) == '=')
				{
					ans.kind = LMOVEEQ;
					strcpy(ans.tokentext, "<<=");
				}
				else
				{
					ans.kind = LMOVE;
					strcpy(ans.tokentext, "<<");
					ungetc(ch, fp);
				}
			else if (ch == '=')
			{
				ans.kind = LESSEQ;
				strcpy(ans.tokentext, "<=");
			}
			else
			{
				ans.kind = LESS;
				strcpy(ans.tokentext, "<");
				ungetc(ch, fp);
			}
			break;
		case '>':
			if ((ch = readchar(fp)) == '>')
				if ((ch = readchar(fp)) == '=')
				{
					ans.kind = RMOVEEQ;
					strcpy(ans.tokentext, ">>=");
				}
				else
				{
					ans.kind = RMOVE;
					strcpy(ans.tokentext, ">>");
					ungetc(ch, fp);
				}
			else if (ch == '=')
			{
				ans.kind = MOREEQ;
				strcpy(ans.tokentext, ">=");
			}
			else
			{
				ans.kind = MORE;
				strcpy(ans.tokentext, ">");
				ungetc(ch, fp);
			}
			break;
		case '!':
			if ((ch = readchar(fp)) == '=')
			{
				ans.kind = UNEQUAL;
				strcpy(ans.tokentext, "!=");
			}
			else
			{
				ans.kind = HASHTAG;
				strcpy(ans.tokentext, "!");
				ungetc(ch, fp);
			}
			break;
		case '^':
			if ((ch = readchar(fp)) == '=')
			{
				ans.kind = XOREQ;
				strcpy(ans.tokentext, "^=");
			}
			else
			{
				ans.kind = XOR;
				strcpy(ans.tokentext, "^");
				ungetc(ch, fp);
			}
			break;
		case '&':
			if ((ch = readchar(fp)) == '&')
			{
				ans.kind = ANDAND;
				strcpy(ans.tokentext, "&&");
			}
			else if (ch == '=')
			{
				ans.kind = ANDEQ;
				strcpy(ans.tokentext, "&=");
			}
			else
			{
				ans.kind = AND;
				strcpy(ans.tokentext, "&");
				ungetc(ch, fp);
			}
			break;
		case '|':
			if ((ch = readchar(fp)) == '|')
			{
				ans.kind = OROR;
				strcpy(ans.tokentext, "||");
			}
			else if (ch == '=')
			{
				ans.kind = OREQ;
				strcpy(ans.tokentext, "|=");
			}
			else
			{
				ans.kind = OR;
				strcpy(ans.tokentext, "|");
				ungetc(ch, fp);
			}
			break;
		case '*':
			if ((ch = readchar(fp)) == '=')
			{
				ans.kind = MULEQ;
				strcpy(ans.tokentext, "*=");
			}
			else
			{
				ans.kind = MUL;
				strcpy(ans.tokentext, "*");
				ungetc(ch, fp);
			}
			break;
		case '-':
			if ((ch = readchar(fp)) == '-')
			{
				ans.kind = MINUSMINUS;
				strcpy(ans.tokentext, "--");
			}
			else if (ch == '=')
			{
				ans.kind = MINUSEQ;
				strcpy(ans.tokentext, "-=");
			}
			else
			{
				ans.kind = MINUS;
				strcpy(ans.tokentext, "-");
				ungetc(ch, fp);
			}
			break;
		case '+':
			if ((ch = readchar(fp)) == '+')
			{
				ans.kind = PLUSPLUS;
				strcpy(ans.tokentext, "++");
			}
			else if (ch == '=')
			{
				ans.kind = PLUSEQ;
				strcpy(ans.tokentext, "+=");
			}
			else
			{
				ans.kind = PLUS;
				strcpy(ans.tokentext, "+");
				ungetc(ch, fp);
			}
			break;
		case '=':
			if ((ch = readchar(fp)) == '=')
			{
				ans.kind = EQUAL;
				strcpy(ans.tokentext, "==");
			}
			else
			{
				ans.kind = ASSIGN;
				strcpy(ans.tokentext, "=");
				ungetc(ch, fp);
			}
			break;
		case '/':
			if ((ch = readchar(fp)) == '/')
			{
				ans.kind = COMMENT;
				strcpy(ans.tokentext, "//");
			}
			else if (ch == '*')
			{
				ans.kind = LCOMMENT;
				strcpy(ans.tokentext, "/*");
			}
			else if (ch == '=')
			{
				ans.kind = DIVEQ;
				strcpy(ans.tokentext, "/=");
			}
			else
			{
				ans.kind = DIV;
				strcpy(ans.tokentext, "/");
				ungetc(ch, fp);
			}
			break;
		default:
			if (feof(fp))
			{
				ans.kind = EOF_;
				strcpy(ans.tokentext, "EOF");
			}
			else
			{
				ans.kind = ERROR_TOKEN;
			}
		}
	if (ans.kind == ERROR_TOKEN)
		printf("word line: %d\n",line);
	ans.line = line;
	return ans;
}