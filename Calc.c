#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include "vars.h"
#define N 50
#define MAX 250
#define eps __DBL_EPSILON__


void oper (char []);
int  calc (char []);
int  assign (char []);
int  solve (char [], char);
int  bracket (char []);
void nospace (char []);
void unar (char [], char []);
void unary (char []);
int  scan (char [], char, char, int);
int  scan2 (char [], char[], char, int);
int  constants (char []);
void func (char [], int, int, char []);
void end (char []);
int  fill (char [], char [], int, int);
void insert (char [], char, int);
int  get (char []);
void handler (int);
char num (char);
char chara (char);
char sign (char);
char bra (char);
void Init ();
void Clear ();



int main (int argc, char * argv[])
{
	char s[MAX];
	Init();
	if (argc>1)
	{
		int i;
		for (i=1; i<argc; i++)
		{
			strcpy(s,argv[i]);
			oper(s);
		}
	}
	else
	{
		printf("%s\n","KENDOTS CALCULATOR");
		signal(SIGINT, handler);
		while (1)
		{
			fputs(">> ",stdout);
			get(s);
			if (!strcmp(s,"exit")) break;
			oper(s);
		}
	}
	Clear();
	return 0;
}


void handler(int sig)
{
	signal(sig, SIG_IGN);
	system("stty cooked echo");
	puts("exit");
	Clear();
	exit(0);
}


void oper (char s[])
{
	nospace(s);
	if (assign(s))
		puts("Variable Assigned");
	else
	{	if (calc(s)==0)
			return;
		var_list[0].value=atof(s);
		puts(s);
		//int n=strlen(s);
	}

}


int calc (char s[])
{
	int n;
	n=constants(s);
	if (n==0) return 0;
	n=bracket(s);
	if (n==0) return 0;
	n=solve(s,'<');
	if (n==0) return 0;
	unary(s);
	n=solve(s,'^');
	if (n==0) return 0;
	n=solve(s,'/');
	if (n==0) return 0;
	solve(s,'*');
	unary(s);
	solve(s,'-');
	solve(s,'+');
	end(s);
	return 1;
}


//assigns a new variable from the user
int assign (char s[])
{
	int i,j,n;
	n=strlen(s);
	for (i=0; i<n; i++)
		if (s[i]=='=') break;
	if (i<n)
	{
		var a;
		char str[N];
		int k=0;
		double x;
		for (j=0; j<i; j++)
			a.name[j]=s[j];
		for (j=i+1; j<n; j++)
			str[k++]=s[j];
		str[k]='\0';
		a.name[i]='\0';
		//search for name

		calc(str);
		x=atof(str);
		a.value=x;
		for (j=0; j<var_count; j++)
			if (strcmp(a.name,var_list[j].name)==0)
				break;
		var_list[j]=a;
		if (j==var_count)
			var_count++;
		return 1;
	}
	return 0;
}


int constants (char s[])
{
	int m,n,i,j,l,k=0,r;
	char str[25];
	n=strlen(s);

	for (i=0; i<n; i++)
		if (s[i]>='a' && s[i]<='z' && s[i]!='e')
		{
			j=i; k=0;
			while (j<n)
			{
				str[k++]=s[j++];
				if (sign(s[j]) || s[j]=='(' || s[j]==')') break;
			}

			str[k]='\0';
			if (s[j]!='(')
			{
				for (j=0; j<var_count; j++)
					if (strcmp(var_list[j].name,str)==0) break;
				if (j==var_count)
				{
					printf("%s is invalid\n",str);
					return 0;
				}
				else
				{
					l=sprintf(str,"%.15g",var_list[j].value);
					str[l+1]=')';
					for (r=l; r>0; r--)
						str[r]=str[r-1];
					str[0]='(';
					str[l+2]='\0';
					n=fill(s,str,i,i+k-1);
				}
			}
		}
	return 1;
}


int bracket (char s[])
{
	char t[N],a[]="**";
	int i,m,n,l,k,h,w;
	double x;
	l=strlen(s);
	for (i=0; i<l; i++)
	{
		if (s[i]=='{' || s[i]=='[') s[i]='(';
		if (s[i]=='}' || s[i]==']') s[i]=')';
	}

	m=0; n=0;
	for (i=0; i<l; i++)
	{
		if (s[i]=='(') n++;
		if (s[i]==')') m++;
	}

	if (m-n)
	{
		puts("Error: Misplacement of bracket(s)");
		return 0;
	}

	m=scan(s,')',1,0);
	while (m>0)
	{
		l=strlen(s);
		n=scan(s,'(',0,m);

		for (i=n+1; i<m; i++)
			t[i-n-1]=s[i];
		t[i-n-1]='\0';
		if (s[m+1]=='^') s[m+1]='<';
		calc(t);

		if (n>0 && (num(s[n-1]) || chara(s[n-1])) )
		{
			w=n-2;
			while (w>=0 && !sign(s[w]) && !bra(s[w]))
				w--;
			k=w+1;
			do
				w++;
			while (num(s[w]));

			if (w>k)
			{
				insert(s,'*',w);
				m++;
				n++;
				w++;
			}

			if (w<n-1)
			{
				func(s,w,n-1,t);
				n=w;
			}
		}

		k=strlen(t);
		h=m-n+1-k;
		for (i=m+1; i<l+1; i++)
			s[i-h]=s[i];
		s[i-h]='\0';
		for (i=0; i<k; i++)
			s[n+i]=t[i];
		m=scan(s,')',1,0);
	}
	n=scan(s,'(',1,0);
	unar(s,a);
	return 1;
}


void end (char s[])
{
	double x=atof(s);
	sprintf(s,"%.15g",x);
}


void nospace(char s[])
{
	int i,j,n;
	n=strlen(s);
	for (i=0; i<n; i++)
		if (s[i]==' ')
		{
			for (j=i; j<n-1; j++)
				s[j]=s[j+1];
			n--;
		}
	for (i=0; i<n; i++)
		if (s[i]>='A' && s[i]<='Z')
			s[i]=s[i]+32;
	s[n]='\0';
}


void unary (char s[])
{
	int o,i;
	char a[]="+-",b[]="--",c[]="-+",d[]="++";
	o=scan2(s,a,1,0)+scan2(s,b,1,0)+scan2(s,c,1,0);
	while (o>-6)
	{
		unar(s,a);
		unar(s,b);
		unar(s,c);
		unar(s,d);
		o=scan2(s,a,1,0)+scan2(s,b,1,0)+scan2(s,c,1,0);
	}
	o=strlen(s);
	while (s[0]=='+')
	{
		for (i=0; i<o-1; i++)
			s[i]=s[i+1];
		s[i]='\0';
		o--;
	}
}

void unar (char s[], char k[2])
{
	int i,m,n,o;
	n=strlen(s);
	m=scan2(s,k,1,0);
	while (m>-1)
	{
		if (strcmp(k,"**")==0)
			s[m]='^';
		else
		{
			o=scan(k,'-',1,0)+scan(k,'+',1,0);
			if (o>0)
				s[m]='-';
			else
				s[m]='+';
		}
		for (i=m+1; i<n-1; i++)
			s[i]=s[i+1];
		s[i]='\0';
		n--;
		m=scan2(s,k,1,0);
	}
}

void func (char s[], int init, int fin, char t[])
{
	int i,l,m,n,k=0,v,u;
	double x=atof(t);
	for (i=init; i<=fin; i++)
		t[k++]=s[i];
	t[k]='\0';
	for (i=0; i<fun_count; i++)
	{
		if (strcmp(fun_list[i].name,t)==0)
			break;
	}

	//if i==fun_count -> error: undefined function 

	x=fun_list[i].f(x);
	sprintf(t,"%.15g",x);
}


int solve (char s[], char k)
{
	char str1[N],str2[N],str[]=".1234567890";
	int i,j,l,n,m,a,b;
	double x,y,z=0;
	m=scan(s,k,1,0);
	while (m>-1)
	{
		n = strlen(s);

		if (s[0]=='+')
		{
			n--;
			for (i=0; i<n; i++)
				s[i]=s[i+1];
			s[n]='\0';
		}
		m=scan(s,k,1,0);

		if (s[0]=='-') 
			m=scan(s,k,0,0);
		if (m<1) break;

		if (k=='-' && s[m-1]=='e')
			break;

		for (i=m-1; i>-1; i--)
		{

			if (i>0 && s[i-1]=='e') i-=2;
			else if (scan(str,s[i],1,0)<0)
			{
				if ((s[i]=='+' || s[i]=='-') && i>-1 && k!='^') i--;
				break;
			}
		}

		j=m+1;
		if (s[j]=='+')
		{
			n--;
			for (l=j; l<n; l++)
				s[l]=s[l+1];
			s[l]='\0';
		}

		else if (s[j]=='-') j++;

		while (scan(str,s[j],1,0)>=0 && j<n+1)
		{
			j++;
			if (s[j]=='e') j+=2;
		}


		for (l=0; l<m-i-1; l++)
			str1[l]=s[l+i+1];
		str1[l]='\0';

		for (l=0; l<j-m-1; l++)
			str2[l]=s[l+m+1];
		str2[l]='\0';

		x=atof(str1);
		y=atof(str2);
		if (k=='<') k='^';


		switch (k)
		{
			case '^' :
				if (y<eps && x<eps && x>-eps)
				{
					if (y>-eps)
						puts("0^0 is an indeterminate form");
					else
						puts("Can't Divide by 0");
					return 0;
				}

				z=pow(x,y);
				break;
			case '*' :
				z=x*y;
				break;
			case '/' :
				if (y<eps && y>-eps)
				{
					puts("Can't Divide by 0");
					return 0;
				}
				z=x/y;
				break;
			case '+' :
				z=x+y;
				break;
			case '-' :
				z=x-y;
				break;
		}

		sprintf(str1,"%.15g",z);
		a=strlen(str1);

		if (z>0)
		{
			for (l=a; l>0; l--)
				str1[l]=str1[l-1];
			str1[0]='+';
			str1[++a]='\0';
		}

		fill(s,str1,i+1,j-1);
		m=scan(s,k,1,0);
	}
	return 1;
}


int scan (char s[], char c, char mode, int r)
{
	int i,n;
	n=strlen(s);
	if (mode)
	{
		for (i=r; i<n; i++)
			if (s[i]==c) return i;
	}
	else
	{
		if (r==0) r=n-1;
		for (i=r; i>-1; i--)
			if (s[i]==c) return i;
	}
	return -2;
}


int scan2 (char s[], char k[], char mode, int r)
{
	int i,j,n,m;
	n=strlen(s);
	m=strlen(k);
	if (mode)
	{
		for (i=r; i<n; i++)
			if (s[i]==k[0])
			{
				for (j=0; j<m; j++)
					if (s[i+j]!=k[j]) break;
				if (j==m) return i;
			}
	}
	else
	{
		if (!r) r=n-1;
		for (i=r;i>-1; i--)
			if (s[i]==k[0])
			{
				for (j=0; j<m; j++)
					if (s[i+j]-k[j]) break;
				if (j==m) return i;
			}
	}
	return -2;
}


int fill (char s[], char r[], int a, int b)
{
	int i,z,l,n,m;
	n=strlen(s);
	m=strlen(r);
	l=b-a+1-m;
	n-=l;

	if (l>0)
		for (i=a+m; i<n; i++)
			s[i]=s[i+l];

	else if (l<0)
		for (i=n-1; i>m+a-1; i--)
			s[i]=s[i+l];

	for (i=0; i<m; i++)
		s[a+i]=r[i];

	s[n]='\0';
	return n;
}


void insert (char s[], char c, int k)
{
	int i,n;
	n=strlen(s);
	for (i=n; i>k; i--)
		s[i]=s[i-1];
	s[k]=c;
	s[n+1]='\0';
}


int get (char * s)
{
	char c,t[N],use=1;
	int siz=0,i,j,k,cur_hor=0,cur_ver=line;
	system("stty cbreak -echo");
	while (1)
	{
		if (use)
			c=getchar();
		use=1;

		if (c=='\n') 
			break;

		else if(c==127)
		{
			//BackSpace
			if (cur_hor)
			{
				k=0;
				for (j=cur_hor; j<siz; j++)
				{
					s[j-1]=s[j];
					t[k++]=s[j];
				}

				t[k]=' ';
				t[k+1]='\0';
				s[--siz]='\0';
				putchar('\b');
				fputs(t,stdout);

				for (j=0; j<=k; j++)
					putchar('\b');
				cur_hor--;
			}
			else putchar('\a');
		}

		else if (c=='\033')
		{
			c=getchar();
			if (c=='[')
			{
				switch (getchar())
				{
					case 'A': //Arrow Up
						if (cur_ver>0)
						{
							s[siz]='\0';
							if (cur_ver==line)
								strcpy(logs[line],s);
							cur_ver--;
							for (i=0; i<siz; i++)
								putchar('\b');
							strcpy(s,logs[cur_ver]);
							fputs(s,stdout);
							cur_hor=strlen(s);
							for (i=cur_hor; i<siz; i++)
								putchar(' ');
							for (i=cur_hor; i<siz; i++)
								putchar('\b');
							siz=cur_hor;
						}
						else
							fputs("\a",stdout);
						break;
					case 'B': //Arrow Down
						if (cur_ver<line)
						{
							cur_ver++;
							s[siz]='\0';
							for (i=0; i<siz; i++)
								putchar('\b');
							strcpy(s,logs[cur_ver]);
							fputs(s,stdout);
							cur_hor=strlen(s);
							for (i=cur_hor; i<siz; i++)
								putchar(' ');
							for (i=cur_hor; i<siz; i++)
								putchar('\b');
							siz=cur_hor;
						}
						else
							fputs("\a",stdout);
						break;
					case 'C': //Arrow Right
						if (cur_hor<siz)
							putchar(s[cur_hor++]);
						else
							fputs("\a",stdout);
						break;
					case 'D': //Arrow Left
						if (cur_hor)
						{
							fputs("\b",stdout);
							cur_hor--;
						}
						else
							fputs("\a",stdout);
						break;
					case '3': //Delete
						getchar();
						c=127;
						putchar(s[cur_hor++]);
						use=0;
						break;
					default : fputs("Invalid",stdout);
				}
			}
			else
				use=0;
		}

		else
		{
			for (j=siz; j>cur_hor; j--)
			{
				s[j]=s[j-1];
			}

			s[cur_hor]=c;
			putchar(c);
			siz++; cur_hor++;
			for (j=cur_hor; j<siz; j++)
				putchar(s[j]);
			for (j=0; j<siz-cur_hor; j++)
				putchar('\b');

		}
	}
	system("stty cooked echo");
	puts("");
	s[siz]='\0';
	strcpy(logs[line],s);
	line++;
	return siz;
}


char num (char c)
{
	if (c>='0' && c<='9')
		return 1;
	return 0;
}


char chara (char c)
{
	if (c>='a' && c<='z') 
		return 1;
	return 0;
}


char sign (char c)
{
	if (c=='+' || c=='-' || c=='*' || c=='/' || c=='^' || c=='<') 
		return 1;
	return 0;
}


char bra (char c)
{
	if (c=='(' || c==')')
		return 1;
	return 0;
}
