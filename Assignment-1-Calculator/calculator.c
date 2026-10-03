#include <stdio.h>
#include <ctype.h>
#include <string.h>

int precedence(char ch)
{
    if (ch == '+' || ch == '-')
    {
        return 0;
    }
    else
    {
        return 1;
    } 
}

int calculate(int leftOperand, char operator, int rightOperand, int *error)
{ 
    int result = 0;
    if (operator == '+')
    {
      result = leftOperand + rightOperand;
    }
    else if (operator == '-')
    {
      result = leftOperand - rightOperand;
    }
    else if (operator == '*')
    {
      result = leftOperand * rightOperand;
    }
    else
    {
     if (rightOperand == 0)
     {
       *error = 1;      
     }
     else
     {
       result = leftOperand / rightOperand;
     }
}
return result;
}

int main()
{
    char exp[100];
    printf("enter an expression: ");
    fgets(exp , sizeof(exp) , stdin);
   
    printf("%s" , exp);
    
    int numbers[100];
    int top = -1;

    char operators[100];
    int topOp = -1;

     int error = 0;

    for (int i = 0; i < strlen(exp); i++)
    {
     if (isdigit(exp[i]))
     {
         int currNum = 0;
         while (isdigit(exp[i]))
         {
         int digit = exp[i] - '0';
         currNum = currNum*10+digit;
         i++;
         }
         i--;
        top++;
        numbers[top] = currNum;

     }
     else if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/' )
     {

     while (topOp != -1 && precedence(operators[topOp]) >= precedence(exp[i]))
     {
       char currentOperator = operators[topOp];
       topOp--;
       int right = numbers[top];
       top--;
       int left = numbers[top];
       top--;

       int calculationResult = calculate(left, currentOperator, right, &error);
        if(error == 1)
        {
            break;
        }
       top++;
       numbers[top] = calculationResult;
      
       }
        topOp++;
       operators[topOp] = exp[i];
    
    }
    
    else if (exp[i] == ' ' || exp[i] == '\n')
    {
        continue;
    }
    else
    {
        printf("Error: Invalid expression.");
        break;
    }  
    }

while (topOp != -1)
{
  char currentOperator = operators[topOp];
       topOp--;
       int right = numbers[top];
       top--;
       int left = numbers[top];
       top--;
      
       int calculationResult = calculate(left, currentOperator, right, &error);
        if(error == 1)
        {
            break;
        }
       top++;
      numbers[top] = calculationResult;
}
if (error == 0)
{
 printf("answer:%d",numbers[top]);
}
else
{
 printf("Error: Division by zero.");
}
    return 0;
}