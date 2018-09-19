#ifndef VARS_H
#define VARS_H

#include<stdlib.h>
#include<math.h>
#include<string.h>
#define fun_num 14

typedef double (*Fun) ();

typedef struct var
{
	char name[20];
	double value;
}var;

typedef struct vfun
{
	char name[20];
	Fun f;
}vfun;


vfun fun_list[fun_num];
var * var_list;
char (*logs)[256];
int var_count=0, fun_count=0, line=0;


void Init()
{
	logs=malloc(100*sizeof(char *));
	var_list = malloc(100*sizeof(var));

	strcpy(var_list[var_count].name,"ans");
	var_list[var_count++].value=0;

	strcpy(var_list[var_count].name,"pi");
	var_list[var_count++].value=3.14159265359;

	strcpy(fun_list[fun_count].name,"exp");
	fun_list[fun_count++].f=exp;

	strcpy(fun_list[fun_count].name,"ln");
	fun_list[fun_count++].f=log;

	strcpy(fun_list[fun_count].name,"log");
	fun_list[fun_count++].f=log;

	strcpy(fun_list[fun_count].name,"log10");
	fun_list[fun_count++].f=log10;

	strcpy(fun_list[fun_count].name,"sqrt");
	fun_list[fun_count++].f=sqrt;

	strcpy(fun_list[fun_count].name,"sin");
	fun_list[fun_count++].f=sin;

	strcpy(fun_list[fun_count].name,"cos");
	fun_list[fun_count++].f=cos;

	strcpy(fun_list[fun_count].name,"tan");
	fun_list[fun_count++].f=tan;

	strcpy(fun_list[fun_count].name,"asin");
	fun_list[fun_count++].f=asin;

	strcpy(fun_list[fun_count].name,"acos");
	fun_list[fun_count++].f=acos;

	strcpy(fun_list[fun_count].name,"atan");
	fun_list[fun_count++].f=atan;

	strcpy(fun_list[fun_count].name,"sinh");
	fun_list[fun_count++].f=sinh;

	strcpy(fun_list[fun_count].name,"cosh");
	fun_list[fun_count++].f=cosh;

	strcpy(fun_list[fun_count].name,"tanh");
	fun_list[fun_count++].f=tanh;
}


void Clear()
{
	free(logs);
	free(var_list);
}


#endif 
