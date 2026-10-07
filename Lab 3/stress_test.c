#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "orderqueue.h"

#define TOTAL_OPERATIONS 100000
#define MAX_ORDER_ID 1000000

int main(void)
{
    OrderQueue queue;

    initializeQueue(&queue);

    /* Expected FIFO values */
    int *expected = malloc(TOTAL_OPERATIONS * sizeof(int));

    if (expected == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    int frontIndex = 0;
    int rearIndex = 0;

    FILE *file = fopen("timing_results.csv", "w");

    if (file == NULL) {
        printf("Could not create timing file\n");
        free(expected);
        return 1;
    }

    fprintf(file, "Operation,Time_ns,Queue_Size\n");

    srand((unsigned int)time(NULL));

    /* Test empty queue */
    printf("Testing empty queue...\n");

    if (isEmpty(&queue))
        printf("Queue is initially empty: PASS\n");
    else
        printf("Queue is initially empty: FAIL\n");

    if (peek(&queue) == -1)
        printf("Empty peek test: PASS\n");
    else
        printf("Empty peek test: FAIL\n");

    if (dequeue(&queue) == -1)
        printf("Empty dequeue test: PASS\n");
    else
        printf("Empty dequeue test: FAIL\n");


    printf("\nRunning stress test...\n");

    for (int i = 0; i < TOTAL_OPERATIONS; i++) {

        /*
         * If queue is empty, we must enqueue.
         * Otherwise randomly choose enqueue or dequeue.
         */
        int operation;

        if (frontIndex == rearIndex) {
            operation = 0;
        }
        else {
            operation = rand() % 2;
        }

        struct timespec start;
        struct timespec end;

        clock_gettime(CLOCK_MONOTONIC, &start);

        if (operation == 0) {

            /* ENQUEUE */

            int orderId = rand() % MAX_ORDER_ID + 1;

            enqueue(&queue, orderId);

            expected[rearIndex] = orderId;
            rearIndex++;

        }
        else {

            /* DEQUEUE */

            int actual = dequeue(&queue);
            int expectedValue = expected[frontIndex];

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

        clock_gettime(CLOCK_MONOTONIC, &end);

        long timeNs =
            (end.tv_sec - start.tv_sec) * 1000000000L +
            (end.tv_nsec - start.tv_nsec);

        fprintf(
            file,
            "%s,%ld,%d\n",
            operation == 0 ? "ENQUEUE" : "DEQUEUE",
            timeNs,
            rearIndex - frontIndex
        );
    }


    /*
     * Test remaining elements using dequeue.
     * This also verifies FIFO ordering.
     */
    printf("\nChecking remaining queue...\n");

    while (!isEmpty(&queue)) {

        int actual = dequeue(&queue);
        int expectedValue = expected[frontIndex];

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

    if (frontIndex == rearIndex)
        printf("FIFO test: PASS\n");
    else
        printf("FIFO test: FAIL\n");


    /* Queue should be empty */
    if (isEmpty(&queue))
        printf("Final empty queue test: PASS\n");
    else
        printf("Final empty queue test: FAIL\n");


    fclose(file);
    free(expected);

    printf("\nStress test completed successfully.\n");
    printf("Timing results saved to timing_results.csv\n");

    return 0;
}