#include<stdio.h>
#include<ctype.h>
#include<string.h>


int precedence(char ch){
    if(ch=='+' ||ch=='-'){
        return 0;
    }
    else{
        return 1;
    }
}

int calculate(int a,char op,int b,int *error){
if(op=='+'){
return a+b;
}
else if(op=='-'){
return a-b;
}
else if(op=='*'){
return a*b;
}
else {
    if(b==0){
     *error=1;      
        return 0;
    }
    else{
      return a/b;
    }
}
}

int main(){
    char exp[100];
    printf("enter an expression: ");
   fgets(exp,sizeof(exp),stdin);
   
    printf("%s",exp);
    
    int numbers[100];
    int top=-1;

    char operators[100];
    int top_op=-1;

     int error=0;

    for(int i=0;i<strlen(exp);i++){
     if(isdigit(exp[i])){
         int curr_num=0;
         while(isdigit(exp[i])){
         int digit=exp[i]-'0';
         curr_num=curr_num*10+digit;
         i++;
         }
         i--;
        top++;
        numbers[top]=curr_num;

     }
     else if(exp[i]=='+' || exp[i]=='-' || exp[i]=='*' ||exp[i]=='/' ){


     while(top_op != -1 && precedence(operators[top_op]) >= precedence(exp[i])){
       char op=operators[top_op];
       top_op--;
       int right=numbers[top];
       top--;
       int left=numbers[top];
       top--;

       int x=calculate(left,op,right,&error);
        if(error==1){
            break;
        }
       top++;
       numbers[top]=x;
      
       }
        top_op++;
       operators[top_op]=exp[i];
    
    }
    

    else if(exp[i]==' ' || exp[i]=='\n'){
        continue;
    }
    else{
        printf("Error: invalid expression ");
        break;
    }

    
    }

while(top_op != -1)
{
  char op=operators[top_op];
       top_op--;
       int right=numbers[top];
       top--;
       int left=numbers[top];
       top--;
      
       int x=calculate(left,op,right,&error);
        if(error==1){
            break;
        }
       top++;
       numbers[top]=x;

}
if(error==0){
printf("answer:%d",numbers[top]);
}
else{
printf("Error: division by zero");
}
    return 0;
}