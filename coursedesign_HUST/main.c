#include <stdio.h>
#include <stdlib.h>
#include "lex.h"

FILE *fp;

int main()
{
	int err = fopen_s(&fp, "D:\\test.txt", "r");
	if (err != 0)
	{
		printf("The file can't be opened.");
		system("pause");
		return 0;
	}
	int line = 0;
	keyword temp;
	temp = gettoken(fp, &line);
	while (temp.kind != ERROR_TOKEN && temp.kind != EOF_)
	{
		printf("%d %s %d\n", temp.kind, temp.tokentext, line);
		temp = gettoken(fp, &line);
	}
	system("pause");
	return 0;
}