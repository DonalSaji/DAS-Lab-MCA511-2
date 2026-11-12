#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int key;
    struct Node* left;
    struct Node* right;
} Node;

Node *new_node(int key) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

//Inserting elements
Node *insert(Node *root, int key) {
    if(!root) return new_node(key); // !root checks if the root is NULL, if it is, a new node is created and returned
    if(key < root->key) {
        root->left = insert(root->left, key); // Arrows used to access the left and right child nodes of the current node
    } else if(key > root->key) {
        root->right = insert(root->right, key); 
    }
    return root;
}
jhgj
jh