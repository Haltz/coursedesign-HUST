#pragma once

#ifndef GRAMMER_H_
#define GRAMMER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "lex.h"

typedef struct Node
{
	int current;
	char descrption[100];
	node* next;
} node;

int GraAnalyse(FILE* fp_);

#endif