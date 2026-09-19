/*
 * Lab Exercise — Arrays, Linked Lists, and Stacks
 *
 * Main Solution:
 * Time Complexity: O(n)
 * Space Complexity: O(n)
 *
 * The array is used to build the linked list.
 * Every linked-list value is pushed onto the stack in one traversal.
 * A second traversal compares each node with the popped stack value.
 *
 * Memory:
 * All dynamically allocated array, stack, and linked-list memory
 * is freed before the program exits.
 *
 * Leak Testing:
 * Tested using:
 * valgrind --leak-check=full ./palindrome_checker
 * Expected result: 0 bytes in use at exit, 0 errors.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;


typedef struct {
    int *data;
    int top;
    int capacity;
} Stack;

Stack *createStack(int capacity) {
    Stack *stack = malloc(sizeof(Stack));
    stack->data = malloc(capacity * sizeof(int));
    stack->top = -1;
    stack->capacity = capacity;

    return stack;
}

void push(Stack *stack, int value) {
    stack->data[++stack->top] = value;
}

int pop(Stack *stack) {
    return stack->data[stack->top--];
}

Node *createLinkedList(int arr[], int n) {
    Node *head = NULL;
    Node *tail = NULL;

    for (int i = 0; i < n; i++) {
        Node *newNode = malloc(sizeof(Node));

        newNode->data = arr[i];
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    return head;
}

void freeLinkedList(Node *head) {
    Node *temp;

    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int checkPalindrome(Node *head, int n) {
    Stack *stack = createStack(n);
    Node *current = head;

    /* First traversal: push every value onto the stack */
    while (current != NULL) {
        push(stack, current->data);
        current = current->next;
    }

    /* Second traversal: compare with values popped from stack */
    current = head;

    while (current != NULL) {
        if (current->data != pop(stack)) {
            free(stack->data);
            free(stack);
            return 0;
        }

        current = current->next;
    }

    free(stack->data);
    free(stack);

    return 1;
    // ban ban banana
}

int main(void) {
    int n;

    printf("Enter the size of the string numbers\n");
    scanf("%d", &n);

    if (n <= 0) {
        return 0;
    }

    int *arr = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        printf("Enter the %d number:  ",i+1); 
        scanf("%d", &arr[i]);
    }

    Node *head = createLinkedList(arr, n);

    if (checkPalindrome(head, n)) {
        printf("\n PALINDROME\n");
    } else {
        printf("\n NOT A PALINDROME\n");
    }

    free(arr);
    freeLinkedList(head);

    return 0;
}