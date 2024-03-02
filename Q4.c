#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <dispatch/dispatch.h>

#define BUFFER_SIZE 5
#define NUMBERS 10

// Global array buffer to be shared by producer and consumers
int buffer[BUFFER_SIZE] = {0};

// Dispatch semaphores declarations
dispatch_semaphore_t empty;
dispatch_semaphore_t full;
dispatch_semaphore_t mutex;

// Producer function
void *producer(void *arg) {
    int *numbers = (int *)arg;

    for (int i = 0; i < NUMBERS; i++) {
        // Random delay
        usleep(rand() % 1000000);

        // Wait until there is space in the buffer
        dispatch_semaphore_wait(empty, DISPATCH_TIME_FOREVER);
        dispatch_semaphore_wait(mutex, DISPATCH_TIME_FOREVER);

        // Find an empty slot in the buffer
        int j = 0;
        while (buffer[j] != 0) {
            j++;
        }
        // Add number to buffer
        buffer[j] = numbers[i];
        printf("Produced %d\n", numbers[i]);

        dispatch_semaphore_signal(mutex);
        dispatch_semaphore_signal(full);
    }

    pthread_exit(NULL);
}

// Consumer function
void *consumer(void *arg) {
    (void)arg; // To suppress the unused parameter warning

    for (int i = 0; i < NUMBERS; i++) {
        // Random delay
        usleep(rand() % 1000000);

        // Wait until there are items in the buffer
        dispatch_semaphore_wait(full, DISPATCH_TIME_FOREVER);
        dispatch_semaphore_wait(mutex, DISPATCH_TIME_FOREVER);

        // Find a number to consume in the buffer
        int j = 0;
        while (buffer[j] == 0) {
            j++;
        }
        // Consume number from buffer
        printf("Consumed %d\n", buffer[j]);
        buffer[j] = 0;

        dispatch_semaphore_signal(mutex);
        dispatch_semaphore_signal(empty);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t producer_thread, consumer_thread;
    int numbers[NUMBERS];

    // Initialize dispatch semaphores
    empty = dispatch_semaphore_create(BUFFER_SIZE);
    full = dispatch_semaphore_create(0);
    mutex = dispatch_semaphore_create(1);

    // Prompt user for ten numbers
    printf("Enter ten numbers:\n");
    for (int i = 0; i < NUMBERS; i++) {
        scanf("%d", &numbers[i]);
    }

    // Create producer thread
    pthread_create(&producer_thread, NULL, producer, (void *)numbers);

    // Create consumer thread
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Join threads
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Print buffer contents
    printf("Buffer contents:\n");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    // Destroy dispatch semaphores
    dispatch_release(empty);
    dispatch_release(full);
    dispatch_release(mutex);

    return 0;
}