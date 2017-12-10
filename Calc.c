#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define N 50


void oper(char []);
void bracket(char []);
void unar(char [], char []);
void unary(char []);
void num2str(double, char []);
int  scan(char [], char, char);
int  scan2(char [], char[]);
void solve(char [], char);
void constants(char []);

main (){
char s[N];
int l;

printf("%45s\n","KENDOTS CALCULATOR");
while (1){
printf(">> ");
fgets(s,N,stdin);
l = strlen(s);
if (s[l-1]=='\n') s[--l]='\0';
if (!strcmp(s,"exit")) return 0;
oper(s);
printf("%s\a\n",s);
}}

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

int scan2(char s[], char k[]){
int i,j,n,m;
n=strlen(s);
m=strlen(k);
for (i=0; i<n; i++)
if (s[i]==k[0]){
for (j=0; j<m; j++)
if (s[i+j]!=k[j]) break;
if (j==m) return i;
}
return -2;
}


void constants(char s[]){
int m,n,i;
m=scan(s,'e',1);
n=strlen(s);
while (m>-1){
for (i=n+2; i>m+3; i--)
s[i]=s[i-3];
for (i=m; i<m+4; i++)
s[i]="*10^"[i-m];
m=scan(s,'e',1);
n+=3;
s[n]='\0';
}}


void unary(char s[]){
int o,i;
char a[]="+-",b[]="--",c[]="-+",d[]="++";
o=scan2(s,a)+scan2(s,b)+scan2(s,c);
while (o>-6){
unar(s,a);
unar(s,b);
unar(s,c);
unar(s,d);
o=scan2(s,a)+scan2(s,b)+scan2(s,c);
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
m=scan2(s,k);
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
m=scan2(s,k);
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
  z=pow(x,y);
  break;
case '*' :
  z=x*y;
  break;
case '/' :
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

b=j-i-1-a; //difference between original expression and the ans
n-=b;

if (b>0)
for (l=1+i+a; l<n; l++){
s[l]=s[b+l];
}

else if (b<0)
for (l=n-1; l>i+a; l--){
s[l]=s[b+l];
}

for (l=1; l<a+1; l++)
s[i+l]=str1[l-1];

s[n]='\0';
m=scan(s,k,1);
}}


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

double abso (double z){
if (z<0) return -z;
return z;
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

while (z>1e-4 && m+k<9){
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
