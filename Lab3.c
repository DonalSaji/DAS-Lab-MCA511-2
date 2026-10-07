/*
 * MCA511-2 - Data Structures and Algorithms
 * Lab 3 - Queue Implementation
 *
 * Queue Type: Linked List Queue
 *
 * Operations:
 * 1. enqueue()  - Add order to rear
 * 2. dequeue()  - Remove order from front
 * 3. peek()     - View front order
 * 4. isEmpty()  - Check whether queue is empty
 *
 * Time Complexity:
 * enqueue()  : O(1) worst case
 * dequeue()  : O(1) worst case
 * peek()     : O(1)
 * isEmpty()  : O(1)
 *
 * Space Complexity:
 * O(n), where n is the current number of orders.
 *
 * Memory:
 * Each order is dynamically allocated using malloc().
 * Removed orders are immediately freed using free().
 *
 * The program also performs a stress test with random
 * enqueue and dequeue operations and checks FIFO ordering.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_OPERATIONS 100000
#define MAX_ORDER_ID 1000000

   //Queue Node
typedef struct Order {
    int orderId;
    struct Order *next;
} Order;

   //Queue Structure
typedef struct {
    Order *front;
    Order *rear;
} OrderQueue;

   //Initialize Queue
void initializeQueue(OrderQueue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
}

   //Check if Queue is Empty
int isEmpty(OrderQueue *queue)
{
    return queue->front == NULL;
}

   //Enqueue
   //Add order to the rear
void enqueue(OrderQueue *queue, int orderId)
{
    Order *newOrder = malloc(sizeof(Order));
    if (newOrder == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newOrder->orderId = orderId;
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

   //Dequeue
   //Remove order from the front
int dequeue(OrderQueue *queue)
{
    Order *temp;
    int orderId;
    if (isEmpty(queue)) {
        return -1;
    }
    temp = queue->front;
    orderId = temp->orderId;
    queue->front = queue->front->next;
    /* Queue became empty */
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    /* Release removed node */
    free(temp);
    return orderId;
}

   //Peek
   //View front order
int peek(OrderQueue *queue)
{
    if (isEmpty(queue)) {
        return -1;
    }

    return queue->front->orderId;
}

   //Free Entire Queue
void freeQueue(OrderQueue *queue)
{
    while (!isEmpty(queue)) {
        dequeue(queue);
    }
}


   //Main
int main(void)
{
    OrderQueue queue;

    int *expected;
    int frontIndex = 0;
    int rearIndex = 0;

    FILE *file;

    initializeQueue(&queue);

        //Allocate memory for FIFO verification
    expected = malloc(TOTAL_OPERATIONS * sizeof(int));

    if (expected == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

       //Create timing output file
    file = fopen("timing_results.csv", "w");

    if (file == NULL) {
        printf("Could not create timing file\n");
        free(expected);
        return 1;
    }

    fprintf(file, "Operation,Time_ns,Queue_Size\n");

       //Random number initialization
    srand((unsigned int)time(NULL));

       //Test Empty Queue
    printf("Testing empty queue...\n");

    if (isEmpty(&queue)) {
        printf("isEmpty test: PASS\n");
    }
    else {
        printf("isEmpty test: FAIL\n");
    }

    if (peek(&queue) == -1) {
        printf("Empty peek test: PASS\n");
    }
    else {
        printf("Empty peek test: FAIL\n");
    }


    if (dequeue(&queue) == -1) {
        printf("Empty dequeue test: PASS\n");
    }
    else {
        printf("Empty dequeue test: FAIL\n");
    }

       //Stress Test
    printf("\nRunning stress test...\n");


    for (int i = 0; i < TOTAL_OPERATIONS; i++) {
        int operation;
        struct timespec start;
        struct timespec end;
        long timeNs;


        /*
         * If queue is empty,
         * we must enqueue.
         *
         * Otherwise randomly choose:
         * 0 = enqueue
         * 1 = dequeue
         */

        if (frontIndex == rearIndex) {
            operation = 0;
        }
        else {
            operation = rand() % 2;
        }


        /* Start timing */
        clock_gettime(CLOCK_MONOTONIC, &start);

           //ENQUEUE
        if (operation == 0) {
            int orderId;
            orderId = rand() % MAX_ORDER_ID + 1;
            enqueue(&queue, orderId);

            /* Store the expected FIFO order. */
            expected[rearIndex] = orderId;
            rearIndex++;
        }

           //DEQUEUE
        else {

            int actual;
            int expectedValue;

            actual = dequeue(&queue);
            expectedValue = expected[frontIndex];

            /* Check FIFO ordering. */
            if (actual != expectedValue) {
                printf("\nFIFO ERROR!\n");
                printf("Expected: %d\n", expectedValue);
                printf("Received: %d\n", actual);
                fclose(file);
                free(expected);
                freeQueue(&queue);
                return 1;
            }
            frontIndex++;
        }

        /* End timing */
        clock_gettime(CLOCK_MONOTONIC, &end);

        /* Calculate operation time in nanoseconds.*/
        timeNs =
            (end.tv_sec - start.tv_sec) * 1000000000L
            +
            (end.tv_nsec - start.tv_nsec);

        /* Store timing result. */
        fprintf(
            file,
            "%s,%ld,%d\n",
            operation == 0 ? "ENQUEUE" : "DEQUEUE",
            timeNs,
            rearIndex - frontIndex
        );
    }

       //Check Remaining Orders
    printf("\nChecking remaining queue...\n");

    while (!isEmpty(&queue)) {

        int actual;
        int expectedValue;

        actual = dequeue(&queue);
        expectedValue = expected[frontIndex];

        if (actual != expectedValue) {

            printf("FIFO ERROR!\n");
            printf("Expected: %d\n", expectedValue);
            printf("Received: %d\n", actual);
            fclose(file);
            free(expected);
            freeQueue(&queue);
            return 1;
        }

        frontIndex++;
    }

       //FIFO Test Result
    if (frontIndex == rearIndex) {
        printf("FIFO ordering test: PASS\n");
    }
    else {
        printf("FIFO ordering test: FAIL\n");
    }

       //Final Empty Queue Test
    if (isEmpty(&queue)) {
        printf("Final empty queue test: PASS\n");
    }
    else {
        printf("Final empty queue test: FAIL\n");
    }

       //Cleanup
    fclose(file);

    free(expected);

    freeQueue(&queue);
    printf("\nStress test completed successfully.\n");
    printf("Timing results saved to timing_results.csv\n");


    return 0;
}