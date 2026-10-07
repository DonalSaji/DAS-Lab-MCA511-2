#include <stdio.h>
#include <stdlib.h>
#include "orderqueue.h"

/* Initialize the queue */
void initializeQueue(OrderQueue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
}

/* Check whether the queue is empty */
int isEmpty(OrderQueue *queue)
{
    return queue->front == NULL;
}

/* Add an order to the rear */
void enqueue(OrderQueue *queue, int order)
{
    Order *newOrder = malloc(sizeof(Order));

    if (newOrder == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    newOrder->orderId = order;
    newOrder->next = NULL;

    /* If queue is empty */
    if (isEmpty(queue)) {
        queue->front = newOrder;
        queue->rear = newOrder;
    }
    else {
        queue->rear->next = newOrder;
        queue->rear = newOrder;
    }
}

/* Remove and return the front order */
int dequeue(OrderQueue *queue)
{
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        return -1;
    }

    Order *temp = queue->front;
    int order = temp->orderId;

    queue->front = queue->front->next;

    /* If queue becomes empty */
    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);

    return order;
}

/* Return the front order without removing it */
int peek(OrderQueue *queue)
{
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        return -1;
    }

    return queue->front->orderId;
}

/* Free all remaining orders */
void freeQueue(OrderQueue *queue)
{
    while (!isEmpty(queue)) {
        dequeue(queue);
    }
}