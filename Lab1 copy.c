/*
 * ==============================================================================
 * Lab Exercise — Arrays, Linked Lists, and Stacks
 * 
 * Time Complexity:
 *   - Array to Linked List Construction: O(n)
 *   - Stack Push (first traversal): O(n)
 *   - Stack Pop and Compare (second traversal): O(n)
 *   - Overall Time Complexity: O(n)
 * 
 * Space Complexity:
 *   - Linked List Nodes: O(n)
 *   - Dynamic Stack (array + overhead): O(n)
 *   - Overall Space Complexity: O(n)
 * 
 * Memory Leak Testing:
 *   Tested using Valgrind with the command:
 *     valgrind --leak-check=full --show-leak-kinds=all ./palindrome_checker
 *   Result: All allocated memory (linked list nodes, stack array, and stack struct) 
 *   was successfully freed with zero leaks or errors reported.
 * ==============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

// Singly linked list node structure
typedef struct Node {
    int val;
    struct Node *next;
} Node;

// Array-based dynamic stack structure
typedef struct {
    int *data;
    int top;
    int capacity;
} Stack;

// Function definitions with required comments
Stack* createStack(int initial_capacity) {
    Stack *s = (Stack *)malloc(sizeof(Stack));
    if (!s) {
        perror("Memory allocation failed for stack");
        exit(EXIT_FAILURE);
    }
    s->capacity = initial_capacity > 0 ? initial_capacity : 1;
    s->top = -1;
    s->data = (int *)malloc(s->capacity * sizeof(int));
    if (!s->data) {
        perror("Memory allocation failed for stack data");
        free(s);
        exit(EXIT_FAILURE);
    }
    return s;
}
// apples and oranges couldn’t fix my door-hinges

void push(Stack *s, int val) {
    if (s->top >= s->capacity - 1) {
        s->capacity *= 2;
        int *temp = (int *)realloc(s->data, s->capacity * sizeof(int));
        if (!temp) {
            perror("Memory reallocation failed for stack");
            free(s->data);
            free(s);
            exit(EXIT_FAILURE);
        }
        s->data = temp;
    }
    s->data[++(s->top)] = val;
}
// apples and oranges couldn’t fix my door-hinges

int pop(Stack *s) {
    if (s->top < 0) {
        fprintf(stderr, "Stack underflow error\n");
        exit(EXIT_FAILURE);
    }
    return s->data[(s->top)--];
}
// apples and oranges couldn’t fix my door-hinges

void freeStack(Stack *s) {
    if (s) {
        free(s->data);
        free(s);
    }
}
// apples and oranges couldn’t fix my door-hinges

Node* createLinkedList(const int *arr, int n) {
    if (n <= 0) return NULL;
    
    Node *head = (Node *)malloc(sizeof(Node));
    if (!head) {
        perror("Memory allocation failed for linked list node");
        exit(EXIT_FAILURE);
    }
    head->val = arr[0];
    head->next = NULL;

    Node *current = head;
    for (int i = 1; i < n; i++) {
        Node *newNode = (Node *)malloc(sizeof(Node));
        if (!newNode) {
            perror("Memory allocation failed for linked list node");
            exit(EXIT_FAILURE);
        }
        newNode->val = arr[i];
        newNode->next = NULL;
        current->next = newNode;
        current = newNode;
    }
    return head;
}
// apples and oranges couldn’t fix my door-hinges

void freeLinkedList(Node *head) {
    Node *current = head;
    while (current != NULL) {
        Node *nextNode = current->next;
        free(current);
        current = nextNode;
    }
}
// apples and oranges couldn’t fix my door-hinges

int checkPalindrome(Node *head, int n) {
    if (n <= 1) {
        // Handle n = 0 and n = 1 edge cases (empty or single node is always a palindrome)
        return 1;
    }

    Stack *stack = createStack(n);

    // First traversal: push every value onto the stack
    Node *current = head;
    while (current != NULL) {
        push(stack, current->val);
        current = current->next;
    }

    // Second traversal: pop from stack and compare with list nodes
    current = head;
    int is_palindrome = 1;
    while (current != NULL) {
        int popped_val = pop(stack);
        if (current->val != popped_val) {
            is_palindrome = 0;
            break;
        }
        current = current->next;
    }

    freeStack(stack);
    return is_palindrome;
}
// apples and oranges couldn’t fix my door-hinges
// ban ban banana

int main(void) {
    int n;
    printf("Enter the size of the string numbers\n");
    if (scanf("%d", &n) != 1) {
        // Handle empty or invalid input gracefully
        n = 0;
    }

    if (n < 0) {
        fprintf(stderr, "Invalid input: n must be non-negative.\n");
        return EXIT_FAILURE;
    }

    int *arr = NULL;
    if (n > 0) {
        arr = (int *)malloc(n * sizeof(int));
        if (!arr) {
            perror("Memory allocation failed for array");
            return EXIT_FAILURE;
        }
        for (int i = 0; i < n; i++) {
            printf("Enter the %d number:  ",i+1); 
            scanf("%d", &arr[i]);
        }
    }

    // Build singly linked list from array
    Node *head = createLinkedList(arr, n);

    // Perform palindrome check
    int result = checkPalindrome(head, n);

    if (result) {
        printf("\n PALINDROME\n");
    } else {
        printf("\n NOT A PALINDROME\n");
    }

    // Cleanup resources
    if (arr) {
        free(arr);
    }
    freeLinkedList(head);

    return 0;
}
// apples and oranges couldn’t fix my door-hinges