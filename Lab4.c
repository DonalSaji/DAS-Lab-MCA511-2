/*
 * MCA - Data Structures and Algorithms Lab
 * Binary Search Tree (BST)
 *
 * Duplicate keys are ignored.
 * Height convention: empty tree = -1, leaf = 0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
} Node;

/* A1: Create a new node */
Node *createNode(int key)
{
    Node *newNode = malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

/* A2: Insert a key recursively */
Node *insert(Node *root, int key)
{
    if (root == NULL) {
        return createNode(key);
    }

    if (key < root->key) {
        root->left = insert(root->left, key);
    }
    else if (key > root->key) {
        root->right = insert(root->right, key);
    }
    /* Duplicate key: do nothing */

    return root;
}

/* A3: Search iteratively */
int search(Node *root, int key)
{
    while (root != NULL) {
        if (key == root->key) {
            return 1;
        }
        else if (key < root->key) {
            root = root->left;
        }
        else {
            root = root->right;
        }
    }

    return 0;
}

/* B1: Inorder = Left, Root, Right */
void inorder(Node *root)
{
    if (root == NULL) {
        return;
    }

    inorder(root->left);
    printf("%d ", root->key);
    inorder(root->right);
}

/* B1: Preorder = Root, Left, Right */
void preorder(Node *root)
{
    if (root == NULL) {
        return;
    }

    printf("%d ", root->key);
    preorder(root->left);
    preorder(root->right);
}

/* B1: Postorder = Left, Right, Root */
void postorder(Node *root)
{
    if (root == NULL) {
        return;
    }

    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->key);
}

/* B3: Height of tree */
int height(Node *root)
{
    int leftHeight;
    int rightHeight;

    if (root == NULL) {
        return -1;
    }

    leftHeight = height(root->left);
    rightHeight = height(root->right);

    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

/* B4: Count all nodes */
int countNodes(Node *root)
{
    if (root == NULL) {
        return 0;
    }

    return 1 + countNodes(root->left) + countNodes(root->right);
}

/* B4: Count leaf nodes */
int countLeaves(Node *root)
{
    if (root == NULL) {
        return 0;
    }

    if (root->left == NULL && root->right == NULL) {
        return 1;
    }

    return countLeaves(root->left) + countLeaves(root->right);
}

/* B5: Find minimum iteratively */
Node *findMin(Node *root)
{
    if (root == NULL) {
        return NULL;
    }

    while (root->left != NULL) {
        root = root->left;
    }

    return root;
}

/* B5: Find maximum iteratively */
Node *findMax(Node *root)
{
    if (root == NULL) {
        return NULL;
    }

    while (root->right != NULL) {
        root = root->right;
    }

    return root;
}

/* C: Delete a key (leaf, one child, or two children) */
Node *deleteNode(Node *root, int key)
{
    Node *temp;

    if (root == NULL) {
        return NULL;
    }

    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    }
    else {
        /* Case 1: Leaf node */
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }

        /* Case 2: Only right child */
        if (root->left == NULL) {
            temp = root->right;
            free(root);
            return temp;
        }

        /* Case 2: Only left child */
        if (root->right == NULL) {
            temp = root->left;
            free(root);
            return temp;
        }

        /* Case 3: Two children - use inorder successor */
        temp = findMin(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }

    return root;
}

/* C: Free all nodes using postorder */
void freeTree(Node *root)
{
    if (root == NULL) {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

/* D1: Validate the complete BST using allowed bounds */
int isBST(Node *root, long long min, long long max)
{
    if (root == NULL) {
        return 1;
    }

    if (root->key <= min || root->key >= max) {
        return 0;
    }

    return isBST(root->left, min, root->key) &&
           isBST(root->right, root->key, max);
}

/* Insert an array of keys */
Node *buildTree(const int keys[], int size)
{
    Node *root = NULL;
    int i;

    for (i = 0; i < size; i++) {
        root = insert(root, keys[i]);
    }

    return root;
}

int main(void)
{
    int keys[] = {50, 30, 70, 20, 40, 60, 80, 35};
    int size = sizeof(keys) / sizeof(keys[0]);
    Node *root = NULL;
    Node *minNode;
    Node *maxNode;
    int i;

    /* Part A: Build the tree */
    for (i = 0; i < size; i++) {
        root = insert(root, keys[i]);
    }

    printf("PART A AND B\n");
    printf("Inorder: ");
    inorder(root);
    printf("\nPreorder: ");
    preorder(root);
    printf("\nPostorder: ");
    postorder(root);

    printf("\nHeight: %d Nodes: %d Leaves: %d\n",
           height(root), countNodes(root), countLeaves(root));

    printf("Search 60: %s\n", search(root, 60) ? "found" : "not found");
    printf("Search 25: %s\n", search(root, 25) ? "found" : "not found");
    printf("Search 90: %s\n", search(root, 90) ? "found" : "not found");

    minNode = findMin(root);
    maxNode = findMax(root);

    if (minNode != NULL && maxNode != NULL) {
        printf("Minimum: %d Maximum: %d\n", minNode->key, maxNode->key);
    }

    printf("Valid BST: %s\n", isBST(root, LLONG_MIN, LLONG_MAX) ? "Yes" : "No");

    /* Part C: Test all deletion cases */
    printf("\nPART C - DELETION\n");

    printf("Delete leaf 35: ");
    root = deleteNode(root, 35);
    inorder(root);
    printf("\n");

    printf("Delete node 40 (one child): ");
    root = deleteNode(root, 40);
    inorder(root);
    printf("\n");

    printf("Delete node 30 (two children): ");
    root = deleteNode(root, 30);
    inorder(root);
    printf("\n");

    printf("Valid BST after deletions: %s\n",
           isBST(root, LLONG_MIN, LLONG_MAX) ? "Yes" : "No");

    freeTree(root);

    /* Part D2: Sorted insertion */
    {
        Node *sortedRoot = NULL;
        Node *balancedRoot = NULL;
        int sortedKeys[15];
        int balancedKeys[] = {8, 4, 12, 2, 6, 10, 14,
                              1, 3, 5, 7, 9, 11, 13, 15};

        for (i = 0; i < 15; i++) {
            sortedKeys[i] = i + 1;
        }

        sortedRoot = buildTree(sortedKeys, 15);
        balancedRoot = buildTree(balancedKeys, 15);

        printf("\nPART D2 - HEIGHT COMPARISON\n");
        printf("Sorted insertion height: %d\n", height(sortedRoot));
        printf("Balanced insertion height: %d\n", height(balancedRoot));

        freeTree(sortedRoot);
        freeTree(balancedRoot);
    }

    /* Part D3: Random insertion of 1000 values */
    {
        Node *randomRoot = NULL;
        int inserted = 0;

        srand((unsigned int)time(NULL));

        while (inserted < 1000) {
            int value = rand();

            /* Count only unique values */
            if (!search(randomRoot, value)) {
                randomRoot = insert(randomRoot, value);
                inserted++;
            }
        }

        printf("\nPART D3 - RANDOM TREE\n");
        printf("Random tree height: %d\n", height(randomRoot));
        printf("log2(1000) is approximately 9.97\n");

        freeTree(randomRoot);
    }

    return 0;
}
