#include "lex.h"

keyword gettoken(FILE *fp);

char readchar(FILE *fp); //用于方便的读取字符并记录行数
int findkey(char *st);   //判断是否为关键字

//行数计数器
int line = 1;

//关键字种类
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
	for (int i = 0; i < 31; i++) // 此处31是直接数出key_word的长度，实际应该使用函数计算
		if ((dif = strcmp(st, key_word[i])) == 0)
			return i + 2;
	return 0;
}

keyword gettoken(FILE *fp)
{
	keyword ans;
	for (int i = 0; i < 20; i++)
		ans.tokentext[i] = '\0';
	char ch;
	//跳过空字符
	while ((ch = readchar(fp)) == ' ' || ch == '\n' || ch == '\t')
	{
		if (ch == '\n')
			ans.change = 1;
		else
			ans.change = 0;
	}
	//数字串 此处十六进制和八进制只支持整数
	if (ch <= '9' && ch >= '0')
	{
		int j = 0;
		if (ch == '0')
		{
			ans.tokentext[j++] = ch;
			ch = readchar(fp);
			//十六进制
			if (ch == 'x')
			{
				ans.tokentext[j++] = ch;
				ch = readchar(fp);
				//出错
				if (ch == '0')
				{
					ans.kind = ERROR_TOKEN;
					printf("word line: %d 0x后不应该跟随0\n", line);
					ungetc(ch, fp);
				}
				//未出错
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
			//八进制
			else
			{
				//出错
				if (ch == '0' || (ch <= '9' && ch >= '8'))
				{
					ans.kind = ERROR_TOKEN;
					printf("word line: %d\n  八进制下含有错误的字符\n", line);
					ungetc(ch, fp);
				}
				//未出错
				else if (ch <= '7' && ch > '0')
				{
					do
					{
						ans.tokentext[j++] = ch;
					} while ((ch = readchar(fp)) <= '7' && ch >= '0');
					ans.kind = INT_CONST;
					if (ch == '8' || ch == '9')
					{
						printf("word line: %d\n  八进制下含有错误的字符\n", line);
						ans.kind = ERROR_TOKEN;
					}
					else
						ungetc(ch, fp);
				}
				else
				{
					ans.kind = INT_CONST;
					ans.tokentext[j] = '\0';
					ungetc(ch, fp);
				}
			}
		}
		//十进制
		else
		{
			int fl = 0; //判断是小数还是整数
			do
			{
				ans.tokentext[j++] = ch;
				if (ch == '.')
					fl++;
			} while ((ch = readchar(fp)) == '.' || (ch <= '9' && ch >= '0'));
			if (fl == 0)
				ans.kind = INT_CONST;
			else if (fl == 1)
				ans.kind = DOUBLE_CONST;
			if (ch == 'L' && ans.kind == INT_CONST)
				ans.kind = LONG_CONST;
			else if (ch == 'S' && ans.kind == INT_CONST)
				ans.kind = SHORT_CONST;
			else if (ch == 'f' && ans.kind == DOUBLE_CONST)
				ans.kind = FLOAT_CONST;
			else
				ungetc(ch, fp);
			if (fl > 1)
			{
				printf("word line: %d\n 包含多个点产生错误\n", line);
				ans.kind = ERROR_TOKEN;
			}
		}
	}
	//字符串
	else if ((ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A') || ch == '_') //如果是字符
	{
		int j = 0;
		do
		{
			ans.tokentext[j++] = ch;
		} while ((ch = readchar(fp)) && ((ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A') || ch == '_' || (ch <= '9' && ch >= '0')));
		if (ch == '\n')
			line--;
		ungetc(ch, fp);
		if ((ans.kind = findkey(ans.tokentext)) != ERROR_TOKEN)
		{
			strcpy(ans.tokentext, key_word[ans.kind - 2]);
			ans.line = line;
			return ans;
		} //判断是否是关键字
		else
		{
			ch = readchar(fp);
			if (ch == '[')
			{
				ans.tokentext[j++] = '[';
				//cp = readchar(fp);
				ch = readchar(fp);
				keyword wp;
				if (ch != ']')
				{
					ungetc(ch, fp);
					wp = gettoken(fp);
				}
				else
				{
					ans.kind = FORMARRAY;
					ans.tokentext[j++] = ']';
					ans.tokentext[j] = '\0';
					return ans;
				}
				if (wp.kind == IDENT || wp.kind == INT_CONST || wp.kind == LONG_CONST || wp.kind == SHORT_CONST || wp.kind == ARRAY)
				{
					for (int i = 0; i < strlen(wp.tokentext); i++)
						ans.tokentext[j++] = wp.tokentext[i];
					ch = readchar(fp);
					if (ch != ']')
					{
						ans.kind = ERROR_TOKEN;
						printf("word line: %d\n 应该以]作为数组的结束字符\n", line);
						strcpy(ans.tokentext, "");
						return ans;
					}
					else
					{
						ans.tokentext[j++] = ']';
						ans.tokentext[j++] = '\0';
						ans.kind = ARRAY;
						return ans;
					}
				}
			}
			//表示是一个普通的标识符
			else
			{
				ungetc(ch, fp);
				ans.kind = IDENT;
			}
		}
	}
	else
		//ch为运算符或特殊字符
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
			ans.kind = MOD;
			strcpy(ans.tokentext, "%");
			break;
		case '\\':
			ans.kind = NEGADIV;
			strcpy(ans.tokentext, "\\");
			break;
		case '\'':
			ch = readchar(fp);
			if (ch == '\\')
				ch = fgetc(fp);
			ans.tokentext[0] = ch;
			ans.tokentext[1] = '\0';
			ch = fgetc(fp);
			if (ch != '\'')
			{
				printf("word line: %d\n  字符的定界符错误\n", line);
				ans.kind = ERROR_TOKEN;
			}
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
				printf("word line: %d\n 错误的未知字符\n", line);
				ans.kind = ERROR_TOKEN;
			}
		}
		if (ch == '\n')
			line--;
	if (ans.kind == ERROR_TOKEN)
		printf("word line: %d\n", line);
	ans.line = line;
	return ans;
}