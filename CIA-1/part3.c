#include <stdio.h>
#include <string.h>

#define MAX 100

char stack[MAX];
int top = -1;

// Push
void push(char item) {
    if (top >= MAX - 1) {
        return; 
    }
    stack[++top] = item;
}

// Pop
char pop() {
    if (top == -1) {
        return '\0'; 
    }
    return stack[top--];
}

int isBalanced(char* expr) {
    for (int i = 0; expr[i] != '\0'; i++) {
        char current = expr[i];

        if (current == '(' || current == '{' || current == '[') {
            push(current);
        }
        
        else if (current == ')' || current == '}' || current == ']') {

            if (top == -1) {
                return 0;
            }

            char open = pop();

            if ((current == ')' && open != '(') ||
                (current == '}' && open != '{') ||
                (current == ']' && open != '[')) {
                return 0;
            }
        }
    }

    return (top == -1);
}

int main() {
    char expr[MAX];

    printf("Enter a string of brackets: ");
    scanf("%s", expr);

    if (isBalanced(expr)) {
        printf("The brackets are balanced.\n");
    } else {
        printf("The brackets are not balanced.\n");
    }

    return 0;
}
