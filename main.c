#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAXSIZE 101 //constant maximum size of the stack.

typedef union
{
    float fData;
    char  cData;
} Item;

typedef struct
{
    Item items[MAXSIZE];    // array of unions.
    int top;                // used to detect the current top of the stack.
} Stack;

Stack * initialize()        // constructor for the stack to allocate a memory in the heap and initialize top by zero.
{
    Stack* s = (Stack*)malloc(sizeof(Stack));
    s->top = 0;
    return s;
}

int isEmpty(Stack *s)       // used to check whether the stack is empty or not.
{
    // if current top is zero then the stack is empty
    return s->top==0?1:0;
}

int isFull(Stack* s)        // used to check whether the stack is full or not.
{
    // if current top is equal to the max size then the stack is full.
    return s->top==MAXSIZE?1:0;
}

Item top(Stack *s)          // returns the item at the top pf the stack.
{
    return s->items[s->top-1];
}

Item pop(Stack *s)          // pop the item on top from the stack and returns it.
{

    s->top--;
    return s->items[s->top];
}

void push(Stack *s, Item val)   // insert a new item into the stack.
{

    s->items[s->top] = val;
    s->top++;

}

float evaluateOperation(char O,float oper1,float oper2) // take an operator and two operands and evaluate the operator on them.
{
    switch(O)
    {
    case '+' :
        return oper1 + oper2;
    case '-' :
        return oper1 - oper2;
    case '*' :
        return oper1 * oper2;
    case '/' :
        return oper1 / oper2;

    case '^' :
        return pow(oper1,oper2);
    default:
        return 0;
    }
}



int isOperator(char c)  // checks if the parameter is an operator or not by checking all possible operators.
{
    if(c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%' || c == '(' || c == ')' )
        return 1;
    else
        return 0;

}

int isNegative(char* token) // checks if the string is a negative number or not.
{
    if(token[0] == '-' && token[1]!='\0')   // if the number is negative then it starts with '-' sign followed by any character.
        return 1;                           // as if it is an operator then it's followed by a null character due to strtok() function.
    else
        return 0;
}

int precedence(char oper)   // used to determine the operators with higher precedence.
{
    switch(oper)
    {
    case '^':
        return 2;
    case '*':
    case '/':
        return 1;
    case '+':
    case '-':
        return 0;
    default:
        return -1;
    }
}
/*
* infixToPostfix: converts an expression in infix notation to
* to a postfix notation (Reverse-Polish Notation)
* ASSUME single-digit operands
* ASSUME ^*+-/ operators
* e.g., 22 + 31 --> 22 31 +
* e.g., ( 2 + 3 ) * 4 --> 2 3 + 4 *
*/
void infixToPostfix(char* infix, char* postfix)
{
    Stack* s = initialize();                // a stack to store the operators.
    char* token = malloc(strlen(infix)+1);  // a string to store the tokens.
    char c;                                 //first character in token.
    char temp[3];                           // to concatenate a character, space and null character to the postfix using strcat() function.
    token = strtok(infix," ");              // tokenizing the infix with space as a delimiter.
    Item x;                                 // temp. union to be pushed in the stack.
    while(token)                            // condition to end the loop when token == NULL.
    {
        c = token[0];                       // storing first character of the token string in c variable to be checked.
        if(!isOperator(c) || isNegative(token)) // if the c is not an operator then it's an operand ( add it to postfix ).
        {
            strcat(postfix,token);
            strcat(postfix," ");
        }
        else if(c == ')')                   // if c is ')' pop all the operators in the stack until a '(' is found.
        {
            while(!isEmpty(s) && top(s).cData!='(')
            {
                c = pop(s).cData;           // concatenating the popped operator to the postfix string.
                temp[0] = c;
                temp[1] = ' ';
                temp[2] = '\0';
                strcat(postfix,temp);
            }
            pop(s);                         // pop the ')' character in the stack without adding it to the postfix.
        }
        else if(isEmpty(s) || c == '(')     // if the stack is empty or c == ')' push it to the stack without checking.
        {
            x.cData = c;
            push(s,x);
        }
        else if(!isEmpty(s) && c!='(')      // the last option is that c is an operator.
        {
            if( precedence(top(s).cData) < precedence(c))   // if the next operator has higher precedence than that at the top of the stack.
            {
                // push it to the stack.
                x.cData = c;
                push(s,x);
            }
            else
            {
                char hold;                  // to save the popped operator from the stack to be added to the postfix.
                while(precedence(c) <= precedence(top(s).cData) && top(s).cData != '(' && !isEmpty(s))
                {
                    hold = pop(s).cData;
                    temp[0] = hold;
                    temp[1] = ' ';
                    temp[2] = '\0';
                    strcat(postfix,temp);
                }
                x.cData = c;                // if the next operator (operator A) has lower precedence than that at the top of the stack.
                push(s,x);                  // pop the operator at the top of the stack then push that operator (operator A)
            }
        }
        token = strtok(NULL," ");           // continue tokenizing.
    }
    while(!isEmpty(s))
    {
        c = pop(s).cData;                   // pop any operators left in the stack and add them to the postfix.
        temp[0] = c;
        temp[1] = ' ';
        temp[2] = '\0';
        strcat(postfix,temp);
    }
    free(s);                                // free the allocated memory used by the stack and the token string.
    free(token);
}
/*
* evaluatePostfix: Evaluates an expression in postfix notation
* (Reverse-Polish Notation)
*/
float evaluatePostfix(char* postfix)
{
    Stack* s = initialize();               // a stack to store the operands.
    Item x;                                // temp. union to be pushed in the stack.
    float oper1,oper2,value;
    char* token = malloc(strlen(postfix)+1);
    token = strtok(postfix," ");
    char c;
    while(token)
    {
        c = token[0];
        if(!isOperator(c) || isNegative(token)) // if the tokenized string is not an operator then it's an operand.
        {
            // push it to the stack.
            x.fData = atof(token);
            push(s,x);
        }

        else                                    // if it's an operator pop the last two items in the stack and evaluate that operator on them.
        {
            oper2 = pop(s).fData;
            oper1 = pop(s).fData;
            value = evaluateOperation(c,oper1,oper2);
            x.fData = value;
            push(s,x);                          // push the value of the previous evaluation into the stack.
        }
        token = strtok(NULL," ");               // continue tokenizing.
    }

    value = pop(s).fData;                       // pop the last value in the stack and it's the value of the postfix.
    free(s);
    free(token);
    return value;
}



void replaceNewLineBySpace(char *s)
{
    char *s1 = s;
    while((s1 = strstr(s1, "\n")) != NULL)
        *s1 = ' ';
}

int main()
{
    char infixExpr[256] = "";
    char postfixExpr[256] = "";
    printf("Enter an expression you want to evaluate or Ctrl+Z to exit: ");
    while(fgets(infixExpr, 255, stdin) != NULL)
    {
        replaceNewLineBySpace(infixExpr);
        infixToPostfix(infixExpr, postfixExpr);
        printf("Postfix : %s\n", postfixExpr);
        float result = evaluatePostfix(postfixExpr);
        printf("Result: %f\n\n", result);
        printf("Enter an expression you want to evaluate or Ctrl+Z to exit: ");
        strcpy(postfixExpr,"");
    }


    return 0;
}


// Ali Hassan Mekky 6850 group 3 section 2
// Seif El-Din Ayman Mohamed 6825 group 3 section 2
