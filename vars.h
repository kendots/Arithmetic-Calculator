#ifndef VARS_H
#define VARS_H

#include<stdlib.h>


typedef struct var
{
	char name[20];
	double value;
}var;

typedef struct vfun
{
	char name[20];
	char va[5][20];
	char fun[50];
}vfun;

var  * var_list;
vfun * fun_list;
int var_count=2, fun_count=0;


#endif 
