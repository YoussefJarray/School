

%{

%}
binaire (0|1)+
exp1 (a|b)+
exp2 (A|B)+
fin (.)+

%%
{binaire} printf("%s: c'est un expression binaire \n",yytext) ;
{exp1} printf("%s: c'est une expression de a et b \n",yytext);
{exp2} printf("%s: c'est une expression de A et B \n",yytext);
{fin} printf("%s: n'est pas valide\n",yytext);

%%

main(){
yylex();
}
