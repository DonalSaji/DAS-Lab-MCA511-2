#ifndef ORDERQUEUE_H
#define ORDERQUEUE_H

typedef struct Order {
    int orderId;
    struct Order *next;
} Order;

typedef struct {
    Order *front;
    Order *rear;
} OrderQueue;

/* Queue operations */
void enqueue(OrderQueue *queue, int order);
int dequeue(OrderQueue *queue);
int peek(OrderQueue *queue);
int isEmpty(OrderQueue *queue);

/* Initialize and free queue */
void initializeQueue(OrderQueue *queue);
void freeQueue(OrderQueue *queue);

#endif