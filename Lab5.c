/*

 * CHRIST (Deemed to be University)

 * MCA511-2 - Data Structures and Algorithms

 * Lab 5: Topological Sort using Kahn's Algorithm

 *

 * Adjacency list + indegree array + queue

 */

 

#include <stdio.h>

#include <stdlib.h>

 

#define MAX_VERTICES 10000

#define MAX_EDGES 100000

 

typedef struct Node {

    int vertex;

    struct Node *next;

} Node;

 

Node *createNode(int vertex)

{

    Node *node = malloc(sizeof(Node));

 

    if (node == NULL) {

        fprintf(stderr, "Memory allocation failed\n");

        exit(EXIT_FAILURE);

    }

 

    node->vertex = vertex;

    node->next = NULL;

    return node;

}

 

void addEdge(Node *adj[], int source, int destination)

{

    Node *node = createNode(destination);

    node->next = adj[source];

    adj[source] = node;

}

 

void freeGraph(Node *adj[], int vertices)

{

    int i;

 

    for (i = 0; i < vertices; i++) {

        Node *current = adj[i];

 

        while (current != NULL) {

            Node *temp = current;

            current = current->next;

            free(temp);

        }

    }

}

 

void topologicalSort(Node *adj[], int indegree[], int vertices)

{

    int *queue = malloc((size_t)vertices * sizeof(int));

    int *order = malloc((size_t)vertices * sizeof(int));

    int front = 0;

    int rear = 0;

    int count = 0;

    int i;

 

    if (queue == NULL || order == NULL) {

        fprintf(stderr, "Memory allocation failed\n");

        free(queue);

        free(order);

        return;

    }

 

    /* Add all vertices that have no incoming edges */

    for (i = 0; i < vertices; i++) {

        if (indegree[i] == 0) {

            queue[rear++] = i;

        }

    }

 

    /* Process the queue */

    while (front < rear) {

        int current = queue[front++];

        Node *temp = adj[current];

 

        order[count++] = current;

 

        while (temp != NULL) {

            int neighbour = temp->vertex;

            indegree[neighbour]--;

 

            if (indegree[neighbour] == 0) {

                queue[rear++] = neighbour;

            }

 

            temp = temp->next;

        }

    }

 

    /* A cycle exists if some vertices were not processed */

    if (count != vertices) {

        printf("CYCLE DETECTED\n");

    }

    else {

        printf("Topological Order: ");

 

        for (i = 0; i < count; i++) {

            printf("%d", order[i]);

            if (i < count - 1) {

                printf(" ");

            }

        }

 

        printf("\n");

    }

 

    free(queue);

    free(order);

}

 

int main(void)

{

    int vertices;

    int edges;

    int i;

    int *indegree;

    Node **adj;

 

    if (scanf("%d %d", &vertices, &edges) != 2) {

        fprintf(stderr, "Invalid input\n");

        return EXIT_FAILURE;

    }

 

    if (vertices < 1 || vertices > MAX_VERTICES ||

        edges < 0 || edges > MAX_EDGES) {

        fprintf(stderr, "Input is outside the allowed constraints\n");

        return EXIT_FAILURE;

    }

 

    adj = calloc((size_t)vertices, sizeof(Node *));

    indegree = calloc((size_t)vertices, sizeof(int));

 

    if (adj == NULL || indegree == NULL) {

        fprintf(stderr, "Memory allocation failed\n");

        free(adj);

        free(indegree);

        return EXIT_FAILURE;

    }

 

    for (i = 0; i < edges; i++) {

        int source;

        int destination;

 

        if (scanf("%d %d", &source, &destination) != 2) {

            fprintf(stderr, "Invalid edge input\n");

            freeGraph(adj, vertices);

            free(adj);

            free(indegree);

            return EXIT_FAILURE;

        }

 

        if (source < 0 || source >= vertices ||

            destination < 0 || destination >= vertices) {

            fprintf(stderr, "Vertex number is out of range\n");

            freeGraph(adj, vertices);

            free(adj);

            free(indegree);

            return EXIT_FAILURE;

        }

 

        addEdge(adj, source, destination);

        indegree[destination]++;

    }

 

    topologicalSort(adj, indegree, vertices);

 

    freeGraph(adj, vertices);

    free(adj);

    free(indegree);

 

    return EXIT_SUCCESS;

}


