/*
 * Lab 2 - Searching and Hashing
 *
 * Time Complexity:
 * INSERT  : O(K + log S + S)
 * SEARCH  : O(K + log S)
 * RANK    : O(K + log S)
 * DELETE  : O(K + log S + S)
 *
 * K = number of departments in the bucket
 * S = number of roll numbers in the department
 *
 * Space Complexity: O(N + M + G)
 *
 * N = total roll numbers
 * M = number of buckets
 * G = number of department groups
 *
 * Rehashing:
 * Rehash when average number of department groups per bucket
 * reaches 5. This keeps collision chains reasonably small.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_CAPACITY 4
#define LOAD_FACTOR 5

/* Department group */
typedef struct Group {
    char dept[6];
    int *roll;
    int size;
    int capacity;
    struct Group *next;
} Group;

/* Hash table */
typedef struct {
    int M;
    int groups;
    Group **bucket;
} HashTable;


/* Create hash table */
HashTable *createTable(int M)
{
    HashTable *ht = malloc(sizeof(HashTable));

    ht->M = M;
    ht->groups = 0;
    ht->bucket = calloc(M, sizeof(Group *));

    return ht;
}


/* Polynomial rolling hash */
int hash(char dept[], int M)
{
    int h = 0;

    for (int i = 0; dept[i] != '\0'; i++)
        h = (h * 31 + dept[i]) % M;

    return h;
}


/* Find department group */
Group *findGroup(HashTable *ht, char dept[])
{
    int index = hash(dept, ht->M);
    Group *current = ht->bucket[index];

    while (current != NULL) {
        if (strcmp(current->dept, dept) == 0)
            return current;

        current = current->next;
    }

    return NULL;
}


/* Create department group */
Group *createGroup(HashTable *ht, char dept[])
{
    int index = hash(dept, ht->M);

    Group *group = malloc(sizeof(Group));

    strcpy(group->dept, dept);
    group->size = 0;
    group->capacity = START_CAPACITY;
    group->roll = malloc(START_CAPACITY * sizeof(int));

    group->next = ht->bucket[index];
    ht->bucket[index] = group;

    ht->groups++;

    return group;
}


/* Find insertion position using binary search */
int binarySearch(Group *group, int rollNo)
{
    int low = 0;
    int high = group->size - 1;

    while (low <= high) {
        int mid = (low + high) / 2;

        if (group->roll[mid] == rollNo)
            return mid;

        if (group->roll[mid] < rollNo)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return low;
}


/* INSERT */
void insert(HashTable *ht, char dept[], int rollNo)
{
    Group *group = findGroup(ht, dept);

    if (group == NULL)
        group = createGroup(ht, dept);

    int position = binarySearch(group, rollNo);

    /* Ignore duplicate */
    if (position < group->size &&
        group->roll[position] == rollNo)
        return;

    /* Increase array size */
    if (group->size == group->capacity) {
        group->capacity *= 2;
        group->roll = realloc(
            group->roll,
            group->capacity * sizeof(int)
        );
    }

    /* Shift elements right */
    for (int i = group->size; i > position; i--)
        group->roll[i] = group->roll[i - 1];

    group->roll[position] = rollNo;
    group->size++;
}


/* SEARCH */
int search(HashTable *ht, char dept[], int rollNo)
{
    Group *group = findGroup(ht, dept);

    if (group == NULL)
        return 0;

    int position = binarySearch(group, rollNo);

    if (position < group->size &&
        group->roll[position] == rollNo)
        return 1;

    return 0;
}


/* RANK */
int rank(HashTable *ht, char dept[], int rollNo)
{
    Group *group = findGroup(ht, dept);

    if (group == NULL)
        return -1;

    int low = 0;
    int high = group->size - 1;

    while (low <= high) {
        int mid = (low + high) / 2;

        if (group->roll[mid] < rollNo)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return low;
}


/* DELETE */
void deleteRecord(HashTable *ht, char dept[], int rollNo)
{
    int index = hash(dept, ht->M);

    Group *current = ht->bucket[index];
    Group *previous = NULL;

    while (current != NULL) {

        if (strcmp(current->dept, dept) == 0) {

            int position = binarySearch(current, rollNo);

            if (position >= current->size ||
                current->roll[position] != rollNo)
                return;

            /* Shift elements left */
            for (int i = position; i < current->size - 1; i++)
                current->roll[i] = current->roll[i + 1];

            current->size--;

            /* Remove empty department */
            if (current->size == 0) {

                if (previous == NULL)
                    ht->bucket[index] = current->next;
                else
                    previous->next = current->next;

                free(current->roll);
                free(current);

                ht->groups--;
            }

            return;
        }

        previous = current;
        current = current->next;
    }
}


/* Check prime number */
int isPrime(int n)
{
    if (n < 2)
        return 0;

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return 0;
    }

    return 1;
}


/* Find next prime */
int nextPrime(int n)
{
    n *= 2;

    while (!isPrime(n))
        n++;

    return n;
}


/* Free hash table */
void freeTable(HashTable *ht)
{
    for (int i = 0; i < ht->M; i++) {

        Group *current = ht->bucket[i];

        while (current != NULL) {

            Group *temp = current;

            current = current->next;

            free(temp->roll);
            free(temp);
        }
    }

    free(ht->bucket);
    free(ht);
}


/* Rehash */
void rehash(HashTable **ht)
{
    HashTable *old = *ht;

    int newM = nextPrime(old->M);

    HashTable *newTable = createTable(newM);

    for (int i = 0; i < old->M; i++) {

        Group *group = old->bucket[i];

        while (group != NULL) {

            for (int j = 0; j < group->size; j++)
                insert(newTable, group->dept, group->roll[j]);

            group = group->next;
        }
    }

    freeTable(old);

    *ht = newTable;
}


/* Main */
int main()
{
    int M, Q;

    scanf("%d %d", &M, &Q);

    HashTable *ht = createTable(M);

    char operation[10];
    char dept[6];
    int rollNo;

    for (int i = 0; i < Q; i++) {

        scanf("%s %s %d", operation, dept, &rollNo);

        if (strcmp(operation, "INSERT") == 0) {

            insert(ht, dept, rollNo);

            /* Rehash when load factor reaches 5 */
            if ((double)ht->groups / ht->M >= LOAD_FACTOR)
                rehash(&ht);
        }

        else if (strcmp(operation, "SEARCH") == 0) {

            if (search(ht, dept, rollNo))
                printf("FOUND\n");
            else
                printf("NOT FOUND\n");
        }

        else if (strcmp(operation, "RANK") == 0) {

            printf("%d\n", rank(ht, dept, rollNo));
        }

        else if (strcmp(operation, "DELETE") == 0) {

            deleteRecord(ht, dept, rollNo);
        }
    }

    freeTable(ht);

    return 0;
}
