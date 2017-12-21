#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<setjmp.h>
#define N 50
#define M 1e-5

double ans=0;
static jmp_buf buff;

void oper(char []);
void bracket(char []);
void unar(char [], char []);
void unary(char []);
void num2str(double, char []);
int  scan(char [], char, char);
int  scan2(char [], char[], char);
void solve(char [], char);
void constants(char []);
void Fill(char [], char [], int, int);
int get(char []);

main (){
char s[N];

printf("%45s\n","KENDOTS CALCULATOR");
while (1){
setjmp(buff);
fputs(">> ",stdout);
get(s);
if (!strcmp(s,"exit")) return 0;
oper(s);
ans=atof(s);
puts(s);
}}


int get(char s[]){
int l;
fgets(s,N,stdin);
l=strlen(s);
if (l>N){
puts("Error: the inputted string is too large");
return 0;
}
if (s[l-1]=='\n') s[--l]='\0';
return l;
}


int scan(char s[], char c, char d){
int i,n;
n=strlen(s);
if (d){
for (i=0; i<n; i++)
if (s[i]==c) return i;
}
else{
for (i=n-1; i>-1; i--)
if (s[i]==c) return i;
}
return -2;
}

int scan2(char s[], char k[], char mode){
int i,j,n,m;
n=strlen(s);
m=strlen(k);
if (mode){
for (i=0; i<n; i++)
if (s[i]==k[0]){
for (j=0; j<m; j++)
if (s[i+j]!=k[j]) break;
if (j==m) return i;
}}
else{
for (i=n-1;i>-1; i--)
if (s[i]==k[0]){
for (j=0; j<m; j++)
if (s[i+j]-k[j]) break;
if (j==m) return i;
}}
return -2;
}


void constants(char s[]){
int m,n,i,l,k,ii;
char t[3][N]={"e","pi","ans"},str[N];
double x;
for (i=0; i<3; i++){
m=scan2(s,t[i],1);
n=strlen(s);


switch (i){
case 0:
strcpy(str,"*10^");
break;
case 1:
x=acos(-1);
num2str(x,str);
break;
case 2:
num2str(ans,str);
break;
}

while (m>-1){
Fill(s,str,m,m+i);
m=scan2(s,t[i],1);
}}}


void unary(char s[]){
int o,i;
char a[]="+-",b[]="--",c[]="-+",d[]="++";
o=scan2(s,a,1)+scan2(s,b,1)+scan2(s,c,1);
while (o>-6){
unar(s,a);
unar(s,b);
unar(s,c);
unar(s,d);
o=scan2(s,a,1)+scan2(s,b,1)+scan2(s,c,1);
}
o=strlen(s);
while (s[0]=='+'){
for (i=0; i<o-1; i++)
s[i]=s[i+1];
s[i]='\0';
o--;
}}

void unar(char s[], char k[2]){
int i,m,n,o;
n=strlen(s);
m=scan2(s,k,1);
while (m>-1){
if (strcmp(k,"**")==0)
s[m]='^';
else{
o=scan(k,'-',1)+scan(k,'+',1);
if (o>0)
s[m]='-';
else
s[m]='+';
}
for (i=m+1; i<n-1; i++)
s[i]=s[i+1];
s[i]='\0';
n--;
m=scan2(s,k,1);
}}


void bracket(char s[]){
char t[N],a[]="**";
int i,m,n,l,k,h;
m=scan(s,')',1);
while (m>0){
l=strlen(s);
for (i=0; i<m;i++)
t[i]=s[i];
t[m]='\0';
n=scan(t,'(',0);

if (n<0) return;
for (i=n+1; i<m; i++)
t[i-n-1]=s[i];
t[i-n-1]='\0';
if (s[m+1]=='^') s[m+1]='<';
oper(t);
k=strlen(t);
h=m-n+1-k;
for (i=m+1; i<l+1; i++)
s[i-h]=s[i];
s[i-h]='\0';
for (i=0; i<k; i++)
s[n+i]=t[i];
m=scan(s,')',1);
}
n=scan(s,'(',1);
if(n>0) return;
unar(s,a);
}

void solve (char s[], char k){
char str1[N],str2[N],str[]=".1234567890";
int i,j,l,n,m,a,b;
double x,y,z=0;
m=scan(s,k,1);
while (m>-1){
n = strlen(s);

if (s[0]=='+'){
n--;
for (i=0; i<n; i++)
s[i]=s[i+1];
s[n]='\0';
}
m=scan(s,k,1);

if ( s[0]=='-') m=scan(s,k,0);

if (m<1) break;

for (i=m-1; i>-1; i--)
if (scan(str,s[i],1)<0){
if ((s[i]=='+' || s[i]=='-') && i>-1 && k!='^') i--;
break;
}

j=m+1;
if (s[j]=='+'){
n--;
for (l=j; l<n; l++)
s[l]=s[l+1];
s[l]='\0';
}
else if (s[j]=='-') j++;

while (scan(str,s[j],1)>=0 && j<n+1)  j++;

for (l=0; l<m-i-1; l++)
str1[l]=s[l+i+1];
str1[l]='\0';

for (l=0; l<j-m-1; l++)
str2[l]=s[l+m+1];
str2[l]='\0';

x=atof(str1);
y=atof(str2);
if (k=='<') k='^';


switch (k){
case '^' :
  if (y<M && x<M && x>-M){
	  if (y>-M)
		  puts("0^0 is an indeterminate form");
	  else
		  puts("Can't Divide by 0");
	  longjmp(buff,1);
  }
  z=pow(x,y);
  break;
case '*' :
  z=x*y;
  break;
case '/' :
  if (y<M && y>-M){
	  puts("Can't Divide by 0");
	  longjmp(buff,1);
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

num2str(z,str1);
a=strlen(str1);

if (z>0){
for (l=a; l>0; l--)
str1[l]=str1[l-1];
str1[0]='+';
str1[++a]='\0';
}

Fill(s,str1,i+1,j-1);
m=scan(s,k,1);
}}


void Fill (char s[], char r[], int a, int b){
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
}


void oper(char s[]){
constants(s);
bracket(s);
solve(s,'<');
unary(s);
solve(s,'^');
solve(s,'/');
solve(s,'*');
unary(s);
solve(s,'-');
solve(s,'+');
}


double abso(double x){
if (x<0) return -x;
return x;
}

void num2str(double n, char s[]){
int x,i,y=-1,k=0,m=1,b,a=0;
double z;
if (n<0){
n=-n;
y++;
s[0]='-';
}

b=(int) n;
z=abso(n-b);
if (b>9) m = (int) log10(n)+1;

while (z>M && m+k<9){
n*=10;
k++;
b=(int) n;
z=abso(n-b);
}
m+=k+y+1;

if (k>0){
a++;
s[m-k]='.';
}

for (i=m+a-1; i>m-k; i--){
x = b%10+48;
s[i] = (char) x;
b= (int) b/10;
}
k=m-k;


for (i=k-1; i>y; i--){
x = b%10+48;
s[i] = (char) x;
b=b/10;
}
s[m+a]='\0';
}
