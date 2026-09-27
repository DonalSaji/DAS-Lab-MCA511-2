#include<stdio.h>
#include<stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
    int head;
    
} Node;


// 1. Insert a new node at the end of the list
void insertAtEnd(Node** head, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    newNode->data = value;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

// 2. Delete the first node holding the given value
void deleteValue(Node** head, int value) {
    if (*head == NULL) {
        printf("List is empty. Nothing to delete.\n");
        return;
    }

    Node* temp = *head;
    Node* prev = NULL;

    if (temp->data == value) {
        *head = temp->next; 
        free(temp);        
        return;
    }

    while (temp != NULL && temp->data != value) {
        prev = temp;
        temp = temp->next;
    }

    
    if (temp == NULL) {
        printf("Value %d not found in the list.\n", value);
        return;
    }

    
    prev->next = temp->next;
    free(temp);
}

// 3. Display the list elements
void displayList(Node* head) {
    if (head == NULL) {
        printf("List is empty.\n");
        return;
    }

    Node* temp = head;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

// 4. Reverse the list in place (without creating new nodes)
void reverseList(Node** head) {
    Node* prev = NULL;
    Node* current = *head;
    Node* next = NULL;

    while (current != NULL) {
        next = current->next; 
        current->next = prev; 
        prev = current;       
        current = next;
    }
    *head = prev;
}


int main() {
    Node* head = NULL; // Initialize an empty list

    printf("--- Inserting Elements ---\n");
    insertAtEnd(&head, 10);
    insertAtEnd(&head, 20);
    insertAtEnd(&head, 30);
    insertAtEnd(&head, 40);
    displayList(head); 

    printf("\n--- Deleting 20 (Middle Node) ---\n");
    deleteValue(&head, 20);
    displayList(head); 

    printf("\n--- Deleting 10 (Head Node) ---\n");
    deleteValue(&head, 10);
    displayList(head); 

    printf("\n--- Reversing the List in Place ---\n");
    reverseList(&head);
    displayList(head); 
    
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}

